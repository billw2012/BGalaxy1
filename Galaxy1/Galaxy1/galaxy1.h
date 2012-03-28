#ifndef GALAXY1_H
#define GALAXY1_H

#include <QtGui/QMainWindow>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include "ui_galaxy1.h"

#include "Ugly/QDebugStream.h"
#include <boost/thread.hpp>

#include "GalaxySim.h"

class Galaxy1 : public QMainWindow
{
	Q_OBJECT

public:
	Galaxy1(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Galaxy1();

public slots:
	void idle_processing();
	void new_image_available(QImage);

	void processButton_clicked(bool);

private:
	Ui::Galaxy1Class ui;
	QDebugStream* debugStream;
	//QTimer* idleTimer;
	bool terminateSim;
	boost::thread executeThread;
	GalaxySim sim;
	QGraphicsScene* scene;
	QGraphicsPixmapItem* lastImageItem;
	
};

#endif // GALAXY1_H
