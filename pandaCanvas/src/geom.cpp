#include "pandaCanvas/geom.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

using namespace Geom;

/*
	Geom::Rect implementation
*/

void Rect::set(const glm::vec2 * vertices)
{
	for (int i = 0; i < 4; i++)
		m_vertices[i] = vertices[i];
}

Geom::Rect operator*(const glm::mat4 & matrix, const Geom::Rect & rect)
{
	glm::vec2 vertices[4];
	for (int i = 0; i < 4; i++)
		vertices[i] = matrix * glm::vec4(rect.m_vertices[i], 0.f, 1.f);

	Rect result;
	result.set(vertices);
	return result;
}

/*
	Geom::Bounds implementation
*/

Bounds Bounds::fromOffsetAndSize(glm::vec2 offset, glm::vec2 size)
{
	return { offset, offset + size };
}

Bounds::Bounds() :
	m_min(), m_max()/*, m_size(), m_invSize()*/
{

}

Bounds::Bounds(const glm::vec2 & min, const glm::vec2 & max) :
	m_min(min), m_max(max)/*, m_size(max - min), m_invSize(1.0f / m_size)*/
{

}

Bounds::Bounds(const Rect & rect) :
	m_min(rect.m_vertices[0]), m_max(rect.m_vertices[0])
{
	for (int i = 1; i < 4; i++)
	{
		const glm::vec2 & vertex = rect.m_vertices[i];
		m_min = glm::min(vertex, m_min);
		m_max = glm::max(vertex, m_max);
	}
}

const glm::vec2 & Bounds::min() const
{
	return m_min;
}

const glm::vec2 & Bounds::max() const
{
	return m_max;
}

glm::vec2 Bounds::offset() const
{
	return m_min;
}

glm::vec2 Bounds::size() const
{
	return m_max - m_min;
}

glm::vec2 Bounds::inverseSize() const
{
	return 1.0f / size();
}

Rect Bounds::asRect() const
{
	glm::vec2 vt[4]
	{
		{ m_min.x, m_min.y },
		{ m_max.x, m_min.y },
		{ m_max.x, m_max.y },
		{ m_min.x, m_max.y }
	};

	Rect result;
	result.set(vt);
	return result;
}

glm::mat4 Bounds::createOrthoMatrix() const
{
	return glm::ortho(m_min.x, m_max.x, m_min.y, m_max.y);
}

/*
	Geom::Rotation implementation
*/

Rotation::Rotation(float sin, float cos) :
	m_sin(sin), m_cos(cos)
{

}

Rotation::Rotation(float radians) :
	Rotation(std::sin(radians), std::cos(radians))
{

}

glm::mat4 Rotation::asMatrix() const
{
	glm::mat4 m (1.0f);

	m[0][0] = m_cos;
	m[0][1] = m_sin;

	m[1][0] = -m_sin;
	m[1][1] = m_cos;

	return m;
}

/*
	Geom::MatrixTransform implementation
*/

glm::vec2 MatrixTransform::project(glm::vec2 point, const glm::mat4 & matrix)
{
	return matrix * glm::vec4(point, 0.f, 1.f);
}

glm::vec2 MatrixTransform::unProject(glm::vec2 point, const glm::mat4 & inverseMatrix)
{
	return inverseMatrix * glm::vec4(point, 0.f, 1.f);
}

void MatrixTransform::update()
{
	m_matrix = calculateMatrix();
	m_inverseMatrix = glm::inverse(m_matrix);
}

glm::mat4 MatrixTransform::getMatrix() const
{
	return m_matrix;
}

glm::mat4 MatrixTransform::getInverseMatrix() const
{
	return m_inverseMatrix;
}

glm::vec2 MatrixTransform::project(const glm::vec2 & point) const
{
	return project(point, getMatrix());
}

glm::vec2 MatrixTransform::unProject(const glm::vec2 & point) const
{
	return unProject(point, getInverseMatrix());
}

/*
	Geom::Transform implementation
*/

Transform::Transform() :
	m_pos(1.f), m_rot(1.f)
{
	update();
}

Transform::Transform(const glm::vec2 & pos, const Rotation & rot)
{
	set(pos, rot);
}

void Transform::setIdentity()
{
	m_pos = glm::identity<glm::mat4>();
	m_rot = glm::identity<glm::mat4>();
}

void Transform::set(const glm::vec2 & pos, const Rotation & rot)
{
	m_pos = glm::identity<glm::mat4>();
	m_pos[3][0] = pos.x;
	m_pos[3][1] = pos.y;

	m_rot = rot.asMatrix();

	update();
}

//void Transform::setPosition(const glm::vec2 & pos)
//{
//	m_pos = glm::identity<glm::mat4>();
//	m_pos[3][0] = pos.x;
//	m_pos[3][1] = pos.y;
//	update();
//}
//
//void Transform::setRotation(const Rotation & rot)
//{
//	m_rot = rot.asMatrix();
//	update();
//}

glm::mat4 Transform::calculateMatrix() const
{
	return m_pos * m_rot;
}

/*
	Geom::Camera implementation
*/

glm::vec2 Camera::project(glm::vec2 point, const glm::mat4 & matrix, const Geom::Bounds & viewport)
{
	/* transform the point according to the matrix */
	point = MatrixTransform::project(point, matrix);

	/* map the point to normalized space (0.0, 1.0) */
	point = (point * 0.5f) + 0.5f;

	/* transform the point from normalized space to viewport space */
	point = (point * viewport.size()) + viewport.offset();

	return point;
}

glm::vec2 Camera::unProject(glm::vec2 point, const glm::mat4 & inverseMatrix, const Geom::Bounds & viewport)
{
	/* transform the point from viewport space to normalized space */
	point = (point - viewport.offset()) * viewport.inverseSize();

	/* map the point from normalized space to NDC space (-1.0, 1.0) */
	point = (point - 0.5f) * 2.0f;

	/* inverse transform the point according to the matrix  */
	point = MatrixTransform::unProject(point, inverseMatrix);

	return point;
}

Rect Camera::getVisibleArea() const
{
	Bounds viewportBounds ({ -1.f, -1.f }, { 1.f, 1.f });
	Rect viewportArea = viewportBounds.asRect();
	Rect cameraArea = getInverseMatrix() * viewportArea;
	return cameraArea;
}

glm::vec2 Camera::project(glm::vec2 point, const Geom::Bounds & viewport) const
{
	return project(point, getMatrix(), viewport);
}

glm::vec2 Camera::unProject(glm::vec2 point, const Geom::Bounds & viewport) const
{
	return unProject(point, getInverseMatrix(), viewport);
}

/*
	Geom::TransformCamera implementation
*/

TransformCamera::TransformCamera() :
	m_transform()
{

}

void TransformCamera::setTransform(const Transform & transform)
{
	m_transform = transform;
	update();
}

const Transform & TransformCamera::getTransform() const
{
	return m_transform;
}

glm::mat4 TransformCamera::calculateMatrix() const
{
	return glm::inverse(m_transform.getMatrix());
}

/*
	Geom::ViewportCamera implementation
*/

//ViewportCamera::ViewportCamera(const glm::vec2 & viewportSize) :
//	m_viewportSize(viewportSize)
//{
//
//}
//
//void ViewportCamera::setViewportSize(const glm::vec2 & size)
//{
//	m_viewportSize = size;
//}
//
//glm::vec2 ViewportCamera::getViewportSize() const
//{
//	return m_viewportSize;
//}
//
//glm::vec2 ViewportCamera::project(const glm::vec2 & point) const
//{
//	Geom::Bounds viewportBounds (glm::vec2(0.0f), m_viewportSize);
//	return Camera::project(point, getMatrix(), viewportBounds);
//}
//
//glm::vec2 ViewportCamera::unProject(const glm::vec2 & point) const
//{
//	Geom::Bounds viewportBounds (glm::vec2(0.0f), m_viewportSize);
//	return Camera::unProject(point, getInverseMatrix(), viewportBounds);
//}

/*
	Geom::Scaling implementation
*/

glm::mat4 Scaling::createOrthoMatrix(const glm::vec2 & viewportSize) const
{
	return getBounds(viewportSize).createOrthoMatrix();
}

//glm::vec2 Scaling::scalePoint(const glm::vec2 & point, const glm::vec2 & viewportSize, bool normalizedOutput) const
//{
//	const Geom::Bounds bounds = getBounds(viewportSize);
//	glm::vec2 norm = bounds.normalize(point);
//	return normalizedOutput ? norm : norm * viewportSize;
//}
//
//glm::vec2 Scaling::unscalePoint(const glm::vec2 & point, const glm::vec2 & viewportSize, bool normalizedInput) const
//{
//	glm::vec2 norm = normalizedInput ? point : point / viewportSize;
//	const Geom::Bounds bounds = getBounds(viewportSize);
//	return bounds.denormalize(norm);
//}

StretchScaling::StretchScaling(const glm::vec2 & targetSize) :
	SizedScaling(targetSize)
{

}

Bounds StretchScaling::getBounds(const glm::vec2 & viewportSize) const
{
	glm::vec2 halfSize = m_targetSize / 2.0f;
	return { { -halfSize.x, -halfSize.y }, { halfSize.x, halfSize.y } };
}

FitScaling::FitScaling(const glm::vec2 & targetSize) :
	SizedScaling(targetSize)
{

}

Bounds FitScaling::getBounds(const glm::vec2 & viewportSize) const
{
	float V = viewportSize.x / viewportSize.y;

	float target_width = m_targetSize.x;
	float target_height = m_targetSize.y;
	float A = target_width / target_height;

	float half_target_width = target_width * 0.5f;
	float half_target_height = target_height * 0.5f;

	/* orthographic projection dimensions */
	float left, right, bottom, top;

	if (V >= A)
	{
		/* wide viewport, use full height */
		bottom = -half_target_height;
		top = half_target_height;

		/* scale width accordingly */
		float VA_ratio = V / A;
		left = -VA_ratio * half_target_width;
		right = VA_ratio * half_target_width;

	}
	else
	{
		/* tall viewport, use full width */
		left = -half_target_width;
		right = half_target_width;

		/* scale height accordingly */
		float AV_ratio = A / V;
		bottom = -AV_ratio * half_target_height;
		top = AV_ratio * half_target_height;
	}

	return { { left, bottom }, { right, top } };
}

/*
	Geom::ScaledCamera implementation
*/

ScaledCamera::ScaledCamera(const glm::vec2 & viewportSize, const std::shared_ptr<Scaling> & scaling) :
	m_viewportSize(viewportSize), m_scaling(scaling)
{
	update();
}

void ScaledCamera::setViewportSize(const glm::vec2 & size)
{
	m_viewportSize = size;
	update();
}

void ScaledCamera::setScaling(const std::shared_ptr<Scaling> & scaling)
{
	m_scaling = scaling;
	update();
}

glm::mat4 ScaledCamera::calculateMatrix() const
{
	if (m_scaling) 
		return m_scaling->createOrthoMatrix(m_viewportSize);
	else 
		return glm::mat4(1.f);
}