#ifndef GALAXY1_H
#define GALAXY1_H

#include <QtGui/QMainWindow>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QTimer>
#include "ui_galaxy1.h"

#include "Ugly/QDebugStream.h"
#include <boost/thread.hpp>

#include "GalaxySim.h"

#include "Utils/WaitHandle.h"

#include "GalaxySerializer.h"

class Galaxy1 : public QMainWindow
{
	Q_OBJECT

public:
	Galaxy1(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Galaxy1();

private slots:
	void idle_processing();
	void new_data_available();
	void processButton_clicked(bool);

	void newGalaxyAddButton_clicked(bool);
	void resetButton_clicked(bool);

	void playbackModeButton_clicked(bool);

	void update_playback_slider();

	void playbackSlider_sliderMoved(int);

	void draw_frame() const;

	void loadButton_clicked(bool);
	void saveButton_clicked(bool);

	void recordButton_clicked(bool);

private:
	void init_writing(const std::string& outputFile);
	void set_frame(float f);

private:
	Ui::Galaxy1Class ui;
	QDebugStream* debugStream;
	QTimer* _idleTimer;
	boost::thread executeThread;
	GalaxySim _sim;
	GalaxySerializer _serializer;
	//QGraphicsScene* scene;
	//QGraphicsPixmapItem* lastImageItem;
	WaitHandle _terminateSim;
	cv::VideoWriter _video;
	bool _record;
};

#endif // GALAXY1_H
