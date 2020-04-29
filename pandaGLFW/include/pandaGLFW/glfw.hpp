#ifndef PANDAGLFW_GLFW_H_
#define PANDAGLFW_GLFW_H_

namespace GLFW
{

enum class Key : int
{
	SPACE = 32,
	RIGHT = 262,
	LEFT = 263,
	DOWN = 264,
	UP = 265
};

enum class MouseButton : int
{
	LEFT = 0,
	RIGHT = 1,
	MIDDLE = 2
};

enum class Action : int
{
	RELEASE = 0,
	PRESS = 1,
	REPEAT = 2
};

enum class Modifiers : int
{

};

void init();

double getTime();
void setTime(double time);

}

#endif