#pragma once

#include <glm/vec2.hpp>

class Screen
{
public:
	virtual glm::vec2 getSize() const = 0;
};

class VirtualScreen: public Screen
{
	void setParent( Screen * parent );
	glm::vec2 getRealCoords( const glm::vec2 & virtualCoords );
	glm::vec2 getVirtualCoords( const glm::vec2 & realCoords );
};