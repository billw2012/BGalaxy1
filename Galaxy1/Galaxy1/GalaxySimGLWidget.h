#ifndef GalaxySimGLWidget_h__
#define GalaxySimGLWidget_h__

#include <QtOpenGL/QGLWidget>

#include <QWheelEvent>
#include <QMouseEvent>

#include "GalaxySim.h"

#include "GalaxyGizmo.h"

#include "Math/matrix4.hpp"
#include "Math/transformation.hpp"
#include "Math/plane.hpp"
#include "Math/intersection.hpp"

struct OpenGLCamera
{
	OpenGLCamera()
		: alt(0.0f), az(0.0f), distance(100000) {}

	math::Matrix4f get_matrix() const
	{
		math::Matrix4f rmat;
		rmat = math::translate(center);
		rmat = rmat * math::rotatey(az);
		rmat = rmat * math::rotatex(alt);
		rmat = rmat * math::translate(0.0f, 0.0f, distance);
		return rmat;
	}

	math::Vector3f center;
	float alt, az, distance;
};

class GalaxySimGLWidget : public QGLWidget
{
	Q_OBJECT        // must include this if you use Qt signals/slots

public:
	std::vector<math::Vector3f> _testH;

	GalaxySimGLWidget(QWidget *parent)
		: QGLWidget(parent),
		_playbackMode(false)
	{
		setAutoBufferSwap(true);
		setMouseTracking(true);
	}

	void set_sim(GalaxySim* sim) { _sim = sim; }

	void set_points(const std::vector< pf_Vector3 >& data)
	{
		_currentData = data;
	}

	void set_playback_mode(bool playbackMode)
	{
		_playbackMode = playbackMode;
	}

protected:

	void initializeGL()
	{
		// Set up the rendering context, define display lists etc.:
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glEnable(GL_DEPTH_TEST);
	}

	void resizeGL(int w, int h)
	{
		// setup viewport, projection etc.:
		glViewport(0, 0, (GLint)w, (GLint)h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		float ratio = (float)width() / (float)height();
		//glOrtho(-10000, 10000, -10000, 10000, -10000, 10000);
		gluPerspective(60.0f, ratio, 1.0f, 1000000.0f);
	}

	void paintGL()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		math::Matrix4f cameraInv = _cameraMat.get_matrix().inverse().transpose();
		glLoadMatrixf(&(cameraInv.m11));
		//glDisable(GL_LIGHTING);
		//glEnable(GL_DEPTH_TEST);
		glBegin(GL_POINTS);
		{
			const std::vector< pf_Vector3 >* pData;
			if(_playbackMode)
				pData = &_currentData;
			else
			{
				_sim->lock_data();
				pData = &_sim->get_data();
			}
			glColor3f(1.0f, 1.0f, 1.0f);
			
			for(size_t idx = 0; idx < pData->size(); ++idx)
			{
				glVertex3d((*pData)[idx].x, (*pData)[idx].y, (*pData)[idx].z);
			}
			if(!_playbackMode)
				_sim->unlock_data();
		}
		glEnd();
		//glBegin(GL_LINE_STRIP);
		//{
		//	for(size_t idx = 0; idx < _testH.size(); ++idx)
		//	{
		//		glVertex3f(_testH[idx].x, _testH[idx].y, _testH[idx].z);
		//	}
		//}
		//glEnd();
		_gizmo.draw();
	}

	virtual void mouseMoveEvent ( QMouseEvent * event )
	{
		if(_gizmo.mouseMoveEvent(event->x(), height() - event->y(), event->buttons()))
			return ;
		using namespace math;
		QPointF pDiff = event->posF() - _lastMousePt;
		if(event->buttons() == Qt::LeftButton)
		{
			float newAlt = _cameraMat.alt + pDiff.y() * 0.5f;
			_cameraMat.alt = std::min<float>(180.0f, std::max<float>(-180.0f, newAlt));
			float newAz = _cameraMat.az + pDiff.x() * 0.5f;
			_cameraMat.az = newAz;
		}
		else if(event->buttons() == Qt::RightButton)
		{
			Vector3f plast0 = project_pt(Vector3f(event->posF().x(), _lastMousePt.y(), 0.0));
			Vector3f plast1 = project_pt(Vector3f(event->posF().x(), _lastMousePt.y(), 1.0));
			Vector3f pcurr0 = project_pt(Vector3f(_lastMousePt.x(),  event->posF().y(),  0.0));
			Vector3f pcurr1 = project_pt(Vector3f(_lastMousePt.x(),  event->posF().y(),  1.0));
			Vector3f cameraPos(_cameraMat.get_matrix() * Vector4f::WAxis);
			Planef cameraPlane((cameraPos - _cameraMat.center).normal(), _cameraMat.center);
			IntersectionPairf isectlast = intersects(math::Rayf(plast0, plast1), cameraPlane);
			IntersectionPairf isectcurr = intersects(math::Rayf(pcurr0, pcurr1), cameraPlane);
			_cameraMat.center += (isectcurr.point - isectlast.point);
		}
		_lastMousePt = event->posF();
	}

	virtual void mousePressEvent ( QMouseEvent * event )
	{
		if(_gizmo.mousePressEvent(event->x(), height() - event->y(), event->buttons()))
			return ;
		_lastMousePt = event->posF();
	}

	virtual void mouseReleaseEvent ( QMouseEvent * event )
	{
		if(_gizmo.mouseReleaseEvent(event->x(), height() - event->y(), event->buttons()))
			return ;
		_lastMousePt = event->posF();
	}

	virtual void wheelEvent ( QWheelEvent * event )
	{
		_cameraMat.distance = std::max<float>(100.0f, _cameraMat.distance + (float)event->delta() * (_cameraMat.distance / 1000.0f));
	}

private:
	GalaxySim* _sim;
	OpenGLCamera _cameraMat;
	QPointF _lastMousePt;
	GalaxyGizmo _gizmo;
	std::vector< pf_Vector3 > _currentData;
	bool _playbackMode;
};

#endif // GalaxySimGLWidget_h__
