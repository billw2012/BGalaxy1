#include "galaxy1.h"

#include "Ugly/QtUtils.h"

#include <Windows.h>

#include <QFileDialog>
#include <QMessageBox>

#if defined(KERNEL_DEBUGGING)
#define REMOVE_WITHOUT_KERNEL_DEBUGGING(x) x
#else
#define REMOVE_WITHOUT_KERNEL_DEBUGGING(x)
#endif

using namespace opencl;

//#define UNIVERSE_SIM

#if defined(UNIVERSE_SIM)
#	define ITERATION_DURATION 0.0005
#else
#	define ITERATION_DURATION 500
#endif

#define IMAGE_WIDTH		1024
#define IMAGE_HEIGHT	1024

Galaxy1::Galaxy1(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags),
	_terminateSim(FALSE),
	_record(false)
{
	ui.setupUi(this);
	debugStream = new QDebugStream(std::cout, ui.consoleWidget);

	using namespace opencl;

	if(!_sim.init())
	{
		std::cout << "Could not initialize sim!" << std::endl;
	}

	init_writing("../Data/Results/output.avi");

	ui.glWidget->set_sim(&_sim);

	asserted_connect(&_sim, SIGNAL(new_data_available()), this, SLOT(new_data_available()));
	asserted_connect(ui.processButton, SIGNAL(clicked(bool)), this, SLOT(processButton_clicked(bool)));
	asserted_connect(ui.newGalaxyAddButton, SIGNAL(clicked(bool)), this, SLOT(newGalaxyAddButton_clicked(bool)));
	asserted_connect(ui.resetButton, SIGNAL(clicked(bool)), this, SLOT(resetButton_clicked(bool)));
	asserted_connect(ui.playbackModeButton, SIGNAL(clicked(bool)), this, SLOT(playbackModeButton_clicked(bool)));
	asserted_connect(ui.playbackSlider, SIGNAL(sliderMoved(int)), this, SLOT(playbackSlider_sliderMoved(int)));

	asserted_connect(ui.saveButton, SIGNAL(clicked(bool)), this, SLOT(saveButton_clicked(bool)));
	asserted_connect(ui.loadButton, SIGNAL(clicked(bool)), this, SLOT(loadButton_clicked(bool)));
	asserted_connect(ui.recordButton, SIGNAL(clicked(bool)), this, SLOT(recordButton_clicked(bool)));

	_idleTimer = new QTimer();
	_idleTimer->setSingleShot(false);
	_idleTimer->start(0);

	asserted_connect(_idleTimer, SIGNAL(timeout()), this, SLOT(draw_frame()));
}

void Galaxy1::idle_processing()
{
	// try this for vista, it will fail on XP
	if (::SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED) == NULL)
	{
		// try XP variant as well just to make sure 
		::SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED);
	}  // if

	while(!_terminateSim.is_set())
		_sim.iterate(ITERATION_DURATION);

	::SetThreadExecutionState(ES_CONTINUOUS);
}

void Galaxy1::processButton_clicked(bool checked)
{
	if(checked)
	{
		executeThread.join();
		_terminateSim.reset();
		executeThread = boost::thread(std::bind(&Galaxy1::idle_processing, this));
		ui.newGalaxyAddButton->setEnabled(false);
		ui.resetButton->setEnabled(false);
		ui.playbackModeButton->setEnabled(false);
	}
	else
	{
		_terminateSim.set();
		executeThread.join();
		ui.newGalaxyAddButton->setEnabled(true);
		ui.resetButton->setEnabled(true);
		ui.playbackModeButton->setEnabled(true);
	}
}

void Galaxy1::newGalaxyAddButton_clicked(bool)
{
	//sim.initialize_bodies(0.1 * SOLAR_MASS_IN_EARTH_MASS, 2 * SOLAR_MASS_IN_EARTH_MASS, -1 * AU_PER_LIGHTYEAR, 1 * AU_PER_LIGHTYEAR);
#if defined(UNIVERSE_SIM)
	_sim.initialize_universe(0.1 * SOLAR_MASS_IN_EARTH_MASS, 10 * SOLAR_MASS_IN_EARTH_MASS, 5000 * AU_PER_LIGHTYEAR, 50000000);
#else
	_sim.initialize_galaxy(pf_Vector3(
		ui.newGalaxyXPositionSpinBox->value(), 
		ui.newGalaxyYPositionSpinBox->value(), 
		ui.newGalaxyZPositionSpinBox->value()),
		pf_Vector3(
		ui.newGalaxyXVelocitySpinBox->value(), 
		ui.newGalaxyYVelocitySpinBox->value(), 
		ui.newGalaxyZVelocitySpinBox->value()), 
		ui.newGalaxyBodyCountSpinBox->value() * 1000, 
		0.1, 10, ui.newGalaxySizeSpinBox->value());
#endif
	_serializer.set_error_margin(1);
	//_serializer.set_error_margin(ui.newGalaxySizeSpinBox->value() * 0.000001);
}

void Galaxy1::resetButton_clicked(bool)
{
	_sim.reset();
}

#define FRAME_MULTIPLIER 100
void Galaxy1::playbackModeButton_clicked(bool checked)
{
	if(checked)
	{
		ui.processButton->setEnabled(false);
		update_playback_slider();
		ui.glWidget->set_playback_mode(true);
		ui.playbackSlider->setSliderPosition(0);
		ui.playbackSlider->setEnabled(true);
		set_frame(0);
	}
	else
	{
		ui.processButton->setEnabled(true);
		ui.glWidget->set_playback_mode(false);
		ui.playbackSlider->setEnabled(false);
	}
	
}

void Galaxy1::playbackSlider_sliderMoved(int frame)
{
	set_frame(frame / (float)FRAME_MULTIPLIER);
}

void Galaxy1::new_data_available()
{
	if(_record)
	{
		_sim.lock_data();
		_serializer.add_data(_sim.get_data());
		_sim.unlock_data();
	}
	//QImage output = ui.glWidget->grabFrameBuffer().
	//	scaled(IMAGE_WIDTH, IMAGE_HEIGHT, Qt::KeepAspectRatio).
	//	convertToFormat(QImage::Format_RGB888);
	//if(_video.isOpened())
	//{
	//	cv::Mat img(IMAGE_WIDTH, IMAGE_HEIGHT, CV_8UC3);
	//	size_t outputSize = output.byteCount();
	//	size_t imgSize = img.total() * img.elemSize();
	//	//assert(outputSize == imgSize);
	//	memcpy_s(img.ptr(), imgSize, output.bits(), outputSize);
	//	_video << img;
	//}
}

void Galaxy1::draw_frame() const
{
	ui.glWidget->repaint();
}

void Galaxy1::loadButton_clicked(bool)
{
	QString	fileName = QFileDialog::getOpenFileName( this, "Load Simulation", QString(), QString("Sim Files (*.gxy)"));
	if(!fileName.isEmpty())
	{
		if(!_serializer.load(fileName.toAscii().data()))
		{
			QMessageBox::critical(this, "Load Error!", "Could not load specified file!");
		}
		update_playback_slider();
	}
}

void Galaxy1::saveButton_clicked(bool)
{
	QString	fileName = QFileDialog::getSaveFileName(this, "Save Simulation", QString(), QString("Sim Files (*.gxy)"));
	if(!fileName.isEmpty())
	{
		if(!_serializer.save(fileName.toAscii().data()))
		{
			QMessageBox::critical(this, "Save Error!", "Could not save to specified file!");
		}
	}
}

void Galaxy1::recordButton_clicked(bool checked)
{
	_record = checked;
}

Galaxy1::~Galaxy1()
{
	_terminateSim.set();
	executeThread.join();
	// set state back to normal
}

void Galaxy1::init_writing( const std::string& outputFile )
{
	// CV_FOURCC('x','v','i','d')
	_video.open(outputFile, CV_FOURCC('m','p','e','g'), 30.0, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));
}

void Galaxy1::set_frame(float f)
{
	ui.glWidget->set_points(_serializer.interpolate(f));
}

void Galaxy1::update_playback_slider()
{
	ui.playbackSlider->setMinimum(0);
	ui.playbackSlider->setMaximum(_serializer.get_frame_count() * FRAME_MULTIPLIER);
	ui.playbackSlider->setSliderPosition(0);
}
