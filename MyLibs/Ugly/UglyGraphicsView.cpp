
#include "UglyGraphicsView.h"

UglyGraphicsView::UglyGraphicsView( QWidget* parent )
	: QGraphicsView(parent)
{
	setTransformationAnchor(QGraphicsView::NoAnchor);
	setMouseTracking(true);
}

void UglyGraphicsView::mouseMoveEvent ( QMouseEvent * event )
{
	if(event->buttons() == Qt::MidButton)
	{
		QPointF diff = this->mapToScene(event->pos()) - this->mapToScene(_lastMousePos);
		this->translate(diff.x(), diff.y());
	}

	_lastMousePos = event->pos();
	QGraphicsView::mouseMoveEvent(event);

	emit mouseMoveSig(event);
}

void UglyGraphicsView::mousePressEvent ( QMouseEvent * event )
{
	QGraphicsView::mousePressEvent(event);

	emit mousePressSig(event);
}

void UglyGraphicsView::mouseReleaseEvent ( QMouseEvent * event )
{
	QGraphicsView::mouseReleaseEvent(event);

	emit mouseReleaseSig(event);
}

void UglyGraphicsView::wheelEvent ( QWheelEvent * event )
{
	// zoom transform:
	// zoom scene, transform by difference at zoom point
	
	QPointF zoomPt = this->mapToScene(event->pos());
	float currScale = this->transform().toAffine().m11();
	if(event->delta() < 0 && currScale > 0.2)
		this->scale(0.9, 0.9);
	else if(event->delta() > 0 && currScale < 10)
		this->scale(1.1, 1.1);
	QPointF newZoomPt = this->mapToScene(event->pos());
	QPointF diff = newZoomPt - zoomPt;
	this->translate(diff.x(), diff.y());

	emit wheelSig(event);
	//QGraphicsView::wheelEvent(event);
}
