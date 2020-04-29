#ifndef PANDACANVAS_CANVAS_H_
#define PANDACANVAS_CANVAS_H_

#include <memory>
#include <stack>
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "pandaCanvas/geom.hpp"

namespace GL
{
	enum class RenderMode;
}

namespace GLU
{
	class Texture;
}

class HostEngine;

class Canvas
{
public:
	enum class RectMode
	{
		CORNER, CORNERS, CENTER
	};

	Canvas( const Geom::Scaling * const & scaling );
	~Canvas();

	void setScaling( const Geom::Scaling * const & scaling );
	const Geom::Bounds & getScalingBounds() const;
	const Geom::Rect & getVisibleArea() const;

	/*
		Client-state management
	*/

	void setMatrix( const glm::mat4 & matrix );

	glm::vec4 getFillColor() const;
	void setFillColor( const glm::vec4 & color );

	/*
		Drawing functions
	*/

	void begin(const GLU::Texture * texture = nullptr, const glm::vec4 * scissorRect = nullptr);
	void end();

	void vertex( glm::vec2 pos, glm::vec2 tex );
	void triangle( const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3, const glm::vec2 & t1 = {}, const glm::vec2 & t2 = {}, const glm::vec2 & t3 = {} );
	void quad( const glm::vec2 & v1, const glm::vec2 & v2, const glm::vec2 & v3, const glm::vec2 & v4 );

	void rect( const glm::vec2 & p1, const glm::vec2 & p2, RectMode rectMode = RectMode::CORNER, const glm::vec2 & tmin = {}, const glm::vec2 & tmax = {});
	void circle( const glm::vec2 & c, float r, int num_segments );

	void line( const glm::vec2 & p1, const glm::vec2 & p2, float radius );

private:
	class Listener;

	void recalculateScaling();
	void recalculateVisibleArea();
	void setHostMatrix(const glm::mat4 & matrix);

	/* listener that automatically resizes the canvas */
	class Listener * m_listener;

	/* the visible area of the canvas */
	Geom::Rect m_visibleArea;

	struct State
	{
		glm::mat4 matrix;
		glm::vec4 color;
	} m_state;

	struct HostState
	{
		glm::mat4 matrix;
		const GLU::Texture * texture;
		const glm::vec4 * scissorRect;
	} m_hostState;

	HostEngine * m_engine;
	std::vector<float> m_vertexData;
	int m_vertexCount = -1;

	friend class Listener;
};

#endif