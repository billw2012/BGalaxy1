
#if !defined(__UGLY_UGLY_GRAPHICS_VIEW_H__)
#define __UGLY_UGLY_GRAPHICS_VIEW_H__

#include <QGraphicsView>
#include <QWheelEvent>

class UglyGraphicsView : public QGraphicsView
{
	Q_OBJECT ;

private:

public:
	UglyGraphicsView(QWidget* parent);

protected:
	virtual void	mouseMoveEvent ( QMouseEvent * event );
	virtual void	mousePressEvent ( QMouseEvent * event );
	virtual void	mouseReleaseEvent ( QMouseEvent * event );
	virtual void	wheelEvent ( QWheelEvent * event );

signals:
	void mouseMoveSig ( QMouseEvent * event );
	void mousePressSig ( QMouseEvent * event );
	void mouseReleaseSig ( QMouseEvent * event );
	void wheelSig ( QWheelEvent * event );

private:
	QPoint _lastMousePos;
};

#endif // __UGLY_UGLY_GRAPHICS_VIEW_H__