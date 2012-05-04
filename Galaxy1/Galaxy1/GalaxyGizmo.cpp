
#define _USE_MATH_DEFINES


#include "GalaxyGizmo.h"
#include <Windows.h>
#include <gl/GL.h>
#include <vector>
#include "Math/plane.hpp"
#include "Math/transformation.hpp"

using namespace math;

#define TRANSLATE_AXIS_LEN 100.0f
#define ROTATE_SEGMENT_INNER_RADIUS 30.0f
#define ROTATE_SEGMENT_OUTER_RADIUS 40.0f

//////////////////////////////////////////////////////////////////////////

std::vector<Vector3d> generate_arc(float startAngleRads, float endAngleRads, float radius, int segments, int axis)
{
	float segmentAngle = (endAngleRads - startAngleRads) / segments;
	std::vector<Vector3d> arcPts;
	for(size_t idx = 0; idx <= segments; ++idx)
	{
		float currAngle = startAngleRads + segmentAngle * idx;
		float x = ::cos(currAngle) * radius;
		float y = ::sin(currAngle) * radius;
		switch(axis)
		{
		case 0:
			arcPts.push_back(Vector3d(0.0f, x, y));
			break;
		case 1:
			arcPts.push_back(Vector3d(x, 0.0f, y));
			break;
		case 2:
			arcPts.push_back(Vector3d(x, y, 0.0f));
			break;
		};
	}
	return arcPts;
}

void gl_draw_arc(float startAngleRads, float endAngleRads, float radius, int segments, int axis)
{
	std::vector<Vector3d> pts = generate_arc(startAngleRads, endAngleRads, radius, segments, axis);

	glBegin(GL_LINE_STRIP);
	for(size_t idx = 0; idx <= segments; ++idx)
	{
		glVertex3f(pts[idx].x, pts[idx].y, pts[idx].z);
	}
	glEnd();
}

void gl_filled_draw_arc(float startAngleRads, float endAngleRads, float innerRadius, float outerRadius, int segments, int axis)
{
	std::vector<Vector3d> innerpts = generate_arc(startAngleRads, endAngleRads, innerRadius, segments, axis);
	std::vector<Vector3d> outerpts = generate_arc(startAngleRads, endAngleRads, outerRadius, segments, axis);

	glBegin(GL_TRIANGLE_STRIP);
	for(size_t idx = 0; idx <= segments; ++idx)
	{
		glVertex3f(innerpts[idx].x, innerpts[idx].y, innerpts[idx].z);
		glVertex3f(outerpts[idx].x, outerpts[idx].y, outerpts[idx].z);
	}
	glEnd();
}

math::Vector3f project_pt( const math::Vector3f& screenPos )
{
	GLdouble modelViewMatrix[16], projectionMatrix[16];
	GLint viewport[4];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
	double px, py, pz;
	gluUnProject(screenPos.x, screenPos.y, screenPos.z, modelViewMatrix, projectionMatrix, viewport, &px, &py, &pz);
	return math::Vector3f(px, py, pz);
}

math::Rayf get_ray(const math::Vector2f& screenPt)
{
	math::Vector3f pt0 = project_pt(math::Vector3f(screenPt.x, screenPt.y, 0.0f));
	math::Vector3f pt1 = project_pt(math::Vector3f(screenPt.x, screenPt.y, 1.0f));

	return math::Rayf(pt0, (pt1 - pt0).normal());
}

//////////////////////////////////////////////////////////////////////////

AxisSubGizmo::AxisSubGizmo(GalaxyGizmo* owner, int axis)
	: _owner(owner), 
	_axis(axis)
{

}

void AxisSubGizmo::draw(bool highlight)
{
	glBegin(GL_LINES);
	{
		math::Vector3f col = get_axis_color(highlight);
		math::Vector3f pt = get_axis_pt();
		glColor3f(col.x, col.y, col.z);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(pt.x, pt.y, pt.z);
	}
	glEnd();
}

math::Vector3f AxisSubGizmo::get_axis_pt() const
{
	switch(_axis)
	{
	case 0:
		return math::Vector3f(TRANSLATE_AXIS_LEN, 0.0f, 0.0f);
	case 1:
		return math::Vector3f(0.0f, TRANSLATE_AXIS_LEN, 0.0f);
	}
	return math::Vector3f(0.0f, 0.0f, TRANSLATE_AXIS_LEN);
}

math::Vector3f AxisSubGizmo::get_axis_color(bool highlight) const
{
	if(highlight)
		return math::Vector3f(1.0f, 1.0f, 0.0f);
	switch(_axis)
	{
	case 0:
		return math::Vector3f(1.0f, 0.0f, 0.0f);
	case 1:
		return math::Vector3f(0.0f, 1.0f, 0.0f);
	}
	return math::Vector3f(0.0f, 0.0f, 1.0f);
}

math::IntersectionPairf AxisSubGizmo::intersects(const math::Rayf& ray)
{
	math::Vector3f gizmoCenter = get_translate(_owner->_renderTransform);
	math::Vector3f gizmoAxisEndPt(_owner->_renderTransform * (math::Vector4f(get_axis_pt()) + math::Vector4f::WAxis));
	math::Rayf axisRay(gizmoCenter, gizmoAxisEndPt - gizmoCenter);
	std::pair<float, float> closeUs = ray.distance(axisRay);
	math::Vector3f rayPt = ray.origin + closeUs.first * ray.direction;
	math::Vector3f axisRayPt = axisRay.origin + math::clamp(closeUs.second, 0.0f, 1.0f) * axisRay.direction;
	float distance = (rayPt - axisRayPt).length();
	return math::IntersectionPairf(distance/_owner->_gizmoScale < 5, math::Vector3f::Zero, closeUs.first);
}

bool AxisSubGizmo::mousePressEvent(int x, int y, Qt::MouseButtons buttons)
{
	return false;
}

bool AxisSubGizmo::mouseReleaseEvent(int x, int y, Qt::MouseButtons buttons)
{
	return false;
}

bool AxisSubGizmo::mouseMoveEvent(int x, int y, Qt::MouseButtons buttons)
{
	return false;
}

RotationSubGizmo::RotationSubGizmo(GalaxyGizmo* owner, int axis)
	: _owner(owner),
	_axis(axis)
{

}

void RotationSubGizmo::draw(bool highlight)
{
	switch(_axis)
	{
	case 0:
		if(highlight)
			glColor3f(1.0f, 1.0f, 0.0f);
		else
			glColor3f(1.0f, 0.0f, 0.0f);
		gl_filled_draw_arc(0.0f, M_PI_2, ROTATE_SEGMENT_INNER_RADIUS, ROTATE_SEGMENT_OUTER_RADIUS, 20, 0);
		break;
	case 1:
		if(highlight)
			glColor3f(1.0f, 1.0f, 0.0f);
		else
			glColor3f(0.0f, 1.0f, 0.0f);
		gl_filled_draw_arc(0.0f, M_PI_2, ROTATE_SEGMENT_INNER_RADIUS, ROTATE_SEGMENT_OUTER_RADIUS, 20, 1);
		break;
	case 2:
		if(highlight)
			glColor3f(1.0f, 1.0f, 0.0f);
		else
			glColor3f(0.0f, 0.0f, 1.0f);
		gl_filled_draw_arc(0.0f, M_PI_2, ROTATE_SEGMENT_INNER_RADIUS, ROTATE_SEGMENT_OUTER_RADIUS, 20, 2);
		break;
	};
}

math::IntersectionPairf RotationSubGizmo::intersects(const math::Rayf& ray)
{
	return math::IntersectionPairf();
}

bool RotationSubGizmo::mousePressEvent(int x, int y, Qt::MouseButtons buttons)
{
	return false;
}

bool RotationSubGizmo::mouseReleaseEvent(int x, int y, Qt::MouseButtons buttons)
{
	return false;
}

bool RotationSubGizmo::mouseMoveEvent(int x, int y, Qt::MouseButtons buttons)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
// GalaxyGizmo
//
GalaxyGizmo::GalaxyGizmo()
{
	_subGizmos.push_back(SubGizmoPtr(new AxisSubGizmo(this, 0)));
	_subGizmos.push_back(SubGizmoPtr(new AxisSubGizmo(this, 1)));
	_subGizmos.push_back(SubGizmoPtr(new AxisSubGizmo(this, 2)));
	_subGizmos.push_back(SubGizmoPtr(new RotationSubGizmo(this, 0)));
	_subGizmos.push_back(SubGizmoPtr(new RotationSubGizmo(this, 1)));
	_subGizmos.push_back(SubGizmoPtr(new RotationSubGizmo(this, 2)));
}
void GalaxyGizmo::reset()
{

}

void GalaxyGizmo::draw()
{
	Matrix4f cameraMatrix;
	glGetFloatv(GL_MODELVIEW_MATRIX, &cameraMatrix.m11);
	cameraMatrix = cameraMatrix.transpose();

	Vector3f gizmoCenter = get_translate(_transform);
	Vector3f cameraCenter = get_translate(cameraMatrix);
	Vector3f gizmoCameraVec = (cameraCenter - gizmoCenter).normal();
	Planef gizmoPlane(gizmoCameraVec, gizmoCenter);
	float gizmoDistance = intersects(cameraCenter, gizmoPlane).t;
	if(gizmoDistance > 0.0f)
		return ;
	_gizmoScale = -gizmoDistance * 0.002f;
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_CULL_FACE);
	glPushMatrix();
	{
		Matrix4f scaleMat = math::scale(_gizmoScale, _gizmoScale, _gizmoScale);
		_renderTransform = _transform * scaleMat;
		Matrix4f renderTransformTranspose = _renderTransform.transpose();
		glMultMatrixf(&(renderTransformTranspose.m11));
		//glGetFloatv(GL_MODELVIEW_MATRIX, &_finalTransform.m11);
		//_finalTransform = _finalTransform.transpose();
		//glScalef(_gizmoScale, _gizmoScale, _gizmoScale);
		for(size_t idx = 0; idx < _subGizmos.size(); ++idx)
			_subGizmos[idx]->draw(_subGizmos[idx] == _highlightGizmo);
		//glBegin(GL_LINES);
		//{
		//	glColor3f(1.0f, 0.0f, 0.0f);
		//	glVertex3f(0.0f, 0.0f, 0.0f);
		//	glVertex3f(TRANSLATE_AXIS_LEN, 0.0f, 0.0f);
		//	glColor3f(0.0f, 1.0f, 0.0f);
		//	glVertex3f(0.0f, 0.0f, 0.0f);
		//	glVertex3f(0.0f, TRANSLATE_AXIS_LEN, 0.0f);
		//	glColor3f(0.0f, 0.0f, 1.0f);
		//	glVertex3f(0.0f, 0.0f, 0.0f);
		//	glVertex3f(0.0f, 0.0f, TRANSLATE_AXIS_LEN);
		//}
		//glEnd();

		//glColor3f(1.0f, 0.0f, 0.0f);
		//gl_filled_draw_arc(0.0f, M_PI_2, ROTATE_SEGMENT_INNER_RADIUS, ROTATE_SEGMENT_OUTER_RADIUS, 20, 0);
		//glColor3f(0.0f, 1.0f, 0.0f);
		//gl_filled_draw_arc(0.0f, M_PI_2, ROTATE_SEGMENT_INNER_RADIUS, ROTATE_SEGMENT_OUTER_RADIUS, 20, 1);
		//glColor3f(0.0f, 0.0f, 1.0f);
		//gl_filled_draw_arc(0.0f, M_PI_2, ROTATE_SEGMENT_INNER_RADIUS, ROTATE_SEGMENT_OUTER_RADIUS, 20, 2);
	}
	glPopMatrix();
	glPopAttrib();
}

bool GalaxyGizmo::mousePressEvent(int x, int y, Qt::MouseButtons buttons)
{
	if(_activeGizmo != nullptr)
	{
		if(!_activeGizmo->mousePressEvent(x, y, buttons))
			_activeGizmo = nullptr;
		return true;
	}
	else
	{

	}
	return false;
}

bool GalaxyGizmo::mouseReleaseEvent(int x, int y, Qt::MouseButtons buttons)
{
	if(_activeGizmo != nullptr)
	{
		if(!_activeGizmo->mouseReleaseEvent(x, y, buttons))
			_activeGizmo = nullptr;
		return true;
	}
	else
	{

	}
	return false;
}

bool GalaxyGizmo::mouseMoveEvent(int x, int y, Qt::MouseButtons buttons)
{
	if(_activeGizmo != nullptr)
	{
		if(!_activeGizmo->mouseMoveEvent(x, y, buttons))
			_activeGizmo = nullptr;
		return true;
	}
	else
	{
		_highlightGizmo = get_nearest_intersect(x, y);
	}
	return false;
}

SubGizmoPtr GalaxyGizmo::get_nearest_intersect(int x, int y)
{
	math::Rayf mouseRay = get_ray(math::Vector2f(x, y));

	float minDist = std::numeric_limits<float>::max();
	int minIdx = -1;
	for(size_t idx = 0; idx < _subGizmos.size(); ++idx)
	{
		math::IntersectionPairf isect = _subGizmos[idx]->intersects(mouseRay);
		if(isect.occured && isect.t < minDist)
		{
			minDist = isect.t;
			minIdx = idx;
		}
	}
	if(minIdx != -1)
		return _subGizmos[minIdx];
	return SubGizmoPtr();
}