#include "pandaCanvas/canvas.hpp"

#include <iostream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <pandaGLFW/gl.hpp>
#include <pandaGLFW/window.hpp>

#include "pandaCanvas/geom.hpp"

#include "host_engine.h"

class Canvas::Listener : public Window::Event::Listener
{
public:
	Listener(Canvas * canvas) :
		m_canvas(canvas)
	{

	}

	void onWindowResize(int width, int height) override
	{
		m_canvas->recalculateScaling();
	}

private:
	Canvas * m_canvas;
};

Canvas::Canvas(const Geom::Scaling * const & scaling) :
	m_state({ glm::mat4(1.f), glm::vec4(1.f) }),
	m_hostState({ glm::mat4 { 1.f }, nullptr, nullptr })
{
	Window::checkInit();
	m_engine = new HostEngine();

	recalculateScaling();

	m_listener = new Listener(this);
	Window::Event::addListener(m_listener);
}

Canvas::~Canvas()
{
	Window::Event::removeListener(m_listener);
	delete m_listener;

	delete m_engine;
}

//void Canvas::setScaling(const Geom::Scaling * const & scaling)
//{
//	m_scaling = scaling;
//	recalculateScaling();
//}
//
//void Canvas::recalculateScaling()
//{
//	const glm::vec2 viewportSize = Window::Graphics::getSize();
//	m_scalingBounds = m_scaling->getBounds(viewportSize);
//	setHostMatrix (m_scalingBounds.createOrthoMatrix());
//	recalculateVisibleArea();
//}
//
//const Geom::Bounds & Canvas::getScalingBounds() const
//{
//	return m_scalingBounds;
//}


/*

	Client-state management

*/

void Canvas::setMatrix(const glm::mat4 & matrix)
{
	m_state.matrix = matrix;
	recalculateVisibleArea();
}

void Canvas::recalculateVisibleArea()
{
	static const glm::vec2 vt[4] { { -1.f, -1.f }, { 1.f, -1.f }, { 1.f, 1.f }, { -1.f, 1.f } };
	m_visibleArea.set(vt);
	m_visibleArea = glm::inverse(m_state.matrix) * m_visibleArea;
}

const Geom::Rect & Canvas::getVisibleArea() const
{
	return m_visibleArea;
}

glm::vec4 Canvas::getFillColor() const
{
	return m_state.color;
}

void Canvas::setFillColor(const glm::vec4 & color)
{
	m_state.color = color;
}

/*

	Host-state management

*/

void Canvas::setHostMatrix(const glm::mat4 & matrix)
{
	if (m_vertexCount != -1)
		throw std::logic_error("Call Canvas::end() before modifying host state!");

	m_hostState.matrix = matrix;
}

/*

	Drawing functions

*/

void Canvas::begin(const GLU::Texture * texture = nullptr, const glm::vec4 * scissorRect = nullptr)
{
	// std::cout << "begin: " << m_vertexCount << std::endl;

	if (m_vertexCount != -1)
		throw std::logic_error("Call Canvas::end() before calling Canvas::begin() again!");

	m_hostState.texture = texture;
	m_hostState.scissorRect = scissorRect;

	m_vertexCount = 0;
}

void Canvas::end()
{
	// std::cout << "end" << std::endl;

	if (m_vertexCount == -1)
		throw std::logic_error("Call Canvas::begin() before calling Canvas::end()!");

	if (m_vertexCount > 0)
	{
		m_engine->upload(m_vertexData);
		m_engine->draw(0, m_vertexCount, GL::RenderMode::TRIANGLES, m_hostState.matrix, m_hostState.texture, m_hostState.scissorRect);
	}

	m_vertexData.clear();
	m_vertexCount = -1;
}

void Canvas::vertex(glm::vec2 pos, glm::vec2 tex)
{
	if (m_vertexCount == -1)
		throw std::logic_error("Call Canvas::begin() before drawing!");

	m_vertexData.reserve(m_vertexData.size() + 8);

	/* transform position */
	pos = m_state.matrix * glm::vec4(pos.x, pos.y, 0.f, 1.f);

	/* emit position */
	m_vertexData.push_back(pos.x);
	m_vertexData.push_back(pos.y);

	/* emit texture coords */
	m_vertexData.push_back(tex.x);
	m_vertexData.push_back(tex.y);

	/* emit color */
	{
		const glm::vec4 & color = m_state.color;
		m_vertexData.push_back(color.x);
		m_vertexData.push_back(color.y);
		m_vertexData.push_back(color.z);
		m_vertexData.push_back(color.w);
	}

	m_vertexCount++;
}

void Canvas::triangle(const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3, const glm::vec2 & t1, const glm::vec2 & t2, const glm::vec2 & t3)
{
	vertex(p1, t1);
	vertex(p2, t2);
	vertex(p3, t3);
}

void Canvas::quad(const glm::vec2 & v1, const glm::vec2 & v2, const glm::vec2 & v3, const glm::vec2 & v4)
{
	triangle(v1, v2, v3);
	triangle(v3, v4, v1);
}

void Canvas::rect(const glm::vec2 & p1, const glm::vec2 & p2, RectMode rectMode, const glm::vec2 & tmin, const glm::vec2 & tmax)
{
	using RM = RectMode;
	switch (rectMode)
	{
		case RM::CORNERS:
		{
			quad({ p1.x, p1.y }, { p2.x, p1.y }, { p2.x, p2.y }, { p1.x, p2.y });
			break;
		}

		case RM::CORNER:
		{
			rect({ p1.x, p1.y }, { p1.x + p2.x, p1.y + p2.y }, RM::CORNERS);
			break;
		}

		case RM::CENTER:
		{
			rect(p1 - (p2 / 2.0f), p2, RM::CORNERS);
			break;
		}
	}
}

void Canvas::circle(const glm::vec2 & c, float r, int num_segments)
{
	float theta = (2.f * 3.1415926f) / float(num_segments);
	float tangetial_factor = tanf(theta);//calculate the tangential factor 
	float radial_factor = cosf(theta);//calculate the radial factor 

	glm::vec2 v1 (r, 0); //we start at angle = 0
	for (int ii = 0; ii < num_segments; ii++)
	{
		//calculate the tangential vector 
		//remember, the radial vector is (x, y) 
		//to get the tangential vector we flip those coordinates and negate one of them 
		glm::vec2 t (-v1.y, v1.x);

		//add the tangential vector 
		glm::vec2 v2 = v1 + (t * tangetial_factor);

		//correct using the radial factor 
		v2 *= radial_factor;

		triangle(c, v1, v2);
		v1 = v2;
	}
}

void Canvas::line(const glm::vec2 & p1, const glm::vec2 & p2, float radius)
{
	glm::vec2 dir = glm::normalize(p2 - p1); /* calculate the direction of line AB */
	glm::vec2 norm { -dir.y, dir.x }; /* calculate the normal of line AB */
	glm::vec2 ext = norm * radius; /* calculate the extrusion of line AB */
	quad(p1 + ext, p2 + ext, p2 - ext, p1 - ext);
}