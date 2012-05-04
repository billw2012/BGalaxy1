#ifndef GalaxyGizmo_h__
#define GalaxyGizmo_h__

#include <QtOpenGL/QGLWidget>
#include <gl/GLU.h>
#include "Math/vector3.hpp"
#include "Math/vector4.hpp"
#include "Math/matrix4.hpp"
#include "Math/intersection.hpp"
#include <QMouseEvent>
#include <memory>

struct SubGizmo
{
	virtual void draw(bool highlight) = 0;
	virtual math::IntersectionPairf intersects(const math::Rayf& ray) = 0;
	virtual bool mousePressEvent(int x, int y, Qt::MouseButtons buttons) = 0;
	virtual bool mouseReleaseEvent(int x, int y, Qt::MouseButtons buttons) = 0;
	virtual bool mouseMoveEvent(int x, int y, Qt::MouseButtons buttons) = 0;
};

typedef std::shared_ptr<SubGizmo> SubGizmoPtr;

struct GalaxyGizmo;

struct AxisSubGizmo : public SubGizmo
{
	AxisSubGizmo(GalaxyGizmo* owner, int axis);
	virtual void draw(bool highlight);
	virtual math::IntersectionPairf intersects(const math::Rayf& ray);
	virtual bool mousePressEvent(int x, int y, Qt::MouseButtons buttons);
	virtual bool mouseReleaseEvent(int x, int y, Qt::MouseButtons buttons);
	virtual bool mouseMoveEvent(int x, int y, Qt::MouseButtons buttons);
private:
	math::Vector3f get_axis_pt() const;
	math::Vector3f get_axis_color(bool highlight) const;
private:
	int _axis;
	GalaxyGizmo* _owner;
};

struct RotationSubGizmo : public SubGizmo
{
	RotationSubGizmo(GalaxyGizmo* owner, int axis);
	virtual void draw(bool highlight);
	virtual math::IntersectionPairf intersects(const math::Rayf& ray);
	virtual bool mousePressEvent(int x, int y, Qt::MouseButtons buttons);
	virtual bool mouseReleaseEvent(int x, int y, Qt::MouseButtons buttons);
	virtual bool mouseMoveEvent(int x, int y, Qt::MouseButtons buttons);
private:
	int _axis;
	GalaxyGizmo* _owner;
};

struct GalaxyGizmo
{
	friend struct AxisSubGizmo;
	friend struct RotationSubGizmo;

	GalaxyGizmo();

	void reset();
	void draw();
	bool mousePressEvent(int x, int y, Qt::MouseButtons buttons);
	bool mouseReleaseEvent(int x, int y, Qt::MouseButtons buttons);
	bool mouseMoveEvent(int x, int y, Qt::MouseButtons buttons);

private:
	SubGizmoPtr get_nearest_intersect(int x, int y);

private:
	math::Matrix4f _transform, _renderTransform/*, _finalTransform*/;
	math::Vector3f _velocity;
	float _radius;
	float _density;
	float _gizmoScale;
	std::vector<SubGizmoPtr> _subGizmos;
	SubGizmoPtr _activeGizmo, _highlightGizmo;
};

math::Vector3f project_pt(const math::Vector3f& screenPos);

math::Rayf get_ray(const math::Vector2f& screenPt);


#endif // GalaxyGizmo_h__
