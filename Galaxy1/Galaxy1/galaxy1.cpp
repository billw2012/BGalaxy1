#include "galaxy1.h"

#include "Ugly/QtUtils.h"


#if defined(KERNEL_DEBUGGING)
#define REMOVE_WITHOUT_KERNEL_DEBUGGING(x) x
#else
#define REMOVE_WITHOUT_KERNEL_DEBUGGING(x)
#endif

using namespace opencl;

void Galaxy1::idle_processing()
{
	while(!terminateSim)
		sim.iterate(1);
}

void Galaxy1::processButton_clicked(bool checked)
{
	if(checked)
	{
		executeThread.join();
		terminateSim = false;
		executeThread = boost::thread(std::bind(&Galaxy1::idle_processing, this));
	}
	else
	{
		terminateSim = true;
	}
}

void Galaxy1::new_image_available(QImage lastImage)
{
	lastImageItem->setPixmap(QPixmap::fromImage(lastImage));
}

#define AU_PER_LIGHTYEAR			63239.6717
#define SOLAR_MASS_IN_EARTH_MASS	332918.215
Galaxy1::Galaxy1(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	debugStream = new QDebugStream(std::cout, ui.consoleWidget);
	
	using namespace opencl;

	if(!sim.init(3000))
	{
		std::cout << "Could not initialize sim!" << std::endl;
		return ;
	}

	sim.init_writing("Data/Results/output.avi");
	//sim.initialize_bodies(0.1 * SOLAR_MASS_IN_EARTH_MASS, 2 * SOLAR_MASS_IN_EARTH_MASS, -1 * AU_PER_LIGHTYEAR, 1 * AU_PER_LIGHTYEAR);
	//sim.initialize_universe(0.1 * SOLAR_MASS_IN_EARTH_MASS, 2 * SOLAR_MASS_IN_EARTH_MASS, 10 * AU_PER_LIGHTYEAR, 1);
	sim.initialize_galaxy(0.1 * SOLAR_MASS_IN_EARTH_MASS, 2 * SOLAR_MASS_IN_EARTH_MASS, 0.2 * AU_PER_LIGHTYEAR);

	scene = new QGraphicsScene();
	lastImageItem = new QGraphicsPixmapItem();
	scene->addItem(lastImageItem);
	ui.graphicsView->setScene(scene);
	asserted_connect(&sim, SIGNAL(new_image_available(QImage)), this, SLOT(new_image_available(QImage)), Qt::QueuedConnection);


	asserted_connect(ui.processButton, SIGNAL(clicked(bool)), this, SLOT(processButton_clicked(bool)));
}

Galaxy1::~Galaxy1()
{
	terminateSim = true;
	executeThread.join();
}
