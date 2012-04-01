#include "galaxy1.h"

#include "Ugly/QtUtils.h"

#include <Windows.h>


#if defined(KERNEL_DEBUGGING)
#define REMOVE_WITHOUT_KERNEL_DEBUGGING(x) x
#else
#define REMOVE_WITHOUT_KERNEL_DEBUGGING(x)
#endif

using namespace opencl;

//#define UNIVERSE_SIM

#define AU_PER_LIGHTYEAR			63239.6717
#define SOLAR_MASS_IN_EARTH_MASS	332918.215

#if defined(UNIVERSE_SIM)
#	define ITERATION_DURATION 1
#else
#	define ITERATION_DURATION 5000
#endif

void Galaxy1::idle_processing()
{
	// try this for vista, it will fail on XP
	if (::SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED) == NULL)
	{
		// try XP variant as well just to make sure 
		::SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED);
	}  // if

	while(!_terminateSim.is_set())
		sim.iterate(ITERATION_DURATION);

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
	}
	else
	{
		_terminateSim.set();
		executeThread.join();
		ui.newGalaxyAddButton->setEnabled(true);
		ui.resetButton->setEnabled(true);
	}
}

void Galaxy1::newGalaxyAddButton_clicked(bool)
{
	//sim.initialize_bodies(0.1 * SOLAR_MASS_IN_EARTH_MASS, 2 * SOLAR_MASS_IN_EARTH_MASS, -1 * AU_PER_LIGHTYEAR, 1 * AU_PER_LIGHTYEAR);
#if defined(UNIVERSE_SIM)
	sim.initialize_universe(0.1 * SOLAR_MASS_IN_EARTH_MASS, 10 * SOLAR_MASS_IN_EARTH_MASS, 5000 * AU_PER_LIGHTYEAR, 50000000);
#else
	sim.initialize_galaxy(math::Vector3d(
		ui.newGalaxyXPositionSpinBox->value() * AU_PER_LIGHTYEAR, 
		ui.newGalaxyYPositionSpinBox->value() * AU_PER_LIGHTYEAR, 
		ui.newGalaxyZPositionSpinBox->value() * AU_PER_LIGHTYEAR),
		math::Vector3d(
		ui.newGalaxyXVelocitySpinBox->value() * AU_PER_LIGHTYEAR, 
		ui.newGalaxyYVelocitySpinBox->value() * AU_PER_LIGHTYEAR, 
		ui.newGalaxyZVelocitySpinBox->value() * AU_PER_LIGHTYEAR), 
		ui.newGalaxyBodyCountSpinBox->value() * 1000, 
		0.1 * SOLAR_MASS_IN_EARTH_MASS, 10 * SOLAR_MASS_IN_EARTH_MASS, 
		ui.newGalaxySizeSpinBox->value() * AU_PER_LIGHTYEAR);
#endif
}

void Galaxy1::resetButton_clicked(bool)
{
	sim.reset();
}

void Galaxy1::new_image_available(QImage lastImage)
{
	lastImageItem->setPixmap(QPixmap::fromImage(lastImage));
}

Galaxy1::Galaxy1(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags),
	_terminateSim(FALSE)
{
	ui.setupUi(this);
	debugStream = new QDebugStream(std::cout, ui.consoleWidget);
	
	using namespace opencl;

	if(!sim.init())
	{
		std::cout << "Could not initialize sim!" << std::endl;
		return ;
	}

	sim.init_writing("../Data/Results/output.avi");

	scene = new QGraphicsScene();
	lastImageItem = new QGraphicsPixmapItem();
	scene->addItem(lastImageItem);
	ui.graphicsView->setScene(scene);

	asserted_connect(&sim, SIGNAL(new_image_available(QImage)), this, SLOT(new_image_available(QImage)), Qt::QueuedConnection);
	asserted_connect(ui.processButton, SIGNAL(clicked(bool)), this, SLOT(processButton_clicked(bool)));
	asserted_connect(ui.newGalaxyAddButton, SIGNAL(clicked(bool)), this, SLOT(newGalaxyAddButton_clicked(bool)));
	asserted_connect(ui.resetButton, SIGNAL(clicked(bool)), this, SLOT(resetButton_clicked(bool)));
}

Galaxy1::~Galaxy1()
{
	_terminateSim.set();
	executeThread.join();
	// set state back to normal
}
