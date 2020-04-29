#ifndef PANDAGLFW_WINDOW_H_
#define PANDAGLFW_WINDOW_H_

#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct GLFWwindow;

namespace GLFW
{

	enum class Action;
	enum class Key;
	enum class Modifier;
	enum class MouseButton;

}

namespace Window
{
	namespace Graphics
	{
		namespace Event
		{
			class Listener
			{
			public:
				virtual void onWindowRedraw() {}
				virtual void onWindowResize(int width, int height) {}
				virtual void onFramebufferResize(int width, int height) {}
				virtual void onWindowContentScaleChange(float xscale, float yscale) {}
			};

			void setupCallbacks();
			void addListener(Listener * listener);
			void removeListener(Listener * listener);

			void dispatchFramebufferResize();
			void dispatchWindowRedraw();
		}

		enum Option
		{
			CLEAR_ON_REDRAW
		};

		void setOptions(int options);

		void setDirty(bool dirty = true);
		bool isDirty();

		glm::ivec2 getSize();
		glm::vec2 getScale();

		void setViewport(int x, int y, int width, int height);

		void setClearColor(float red, float green, float blue, float alpha);
		void clear();
	}

	namespace Input
	{
		class Listener
		{
		public:
			virtual void onKeyEvent(GLFW::Key key, GLFW::Action action, GLFW::Modifier mods) {}
			virtual void onMouseButtonEvent(GLFW::MouseButton button, GLFW::Action action, GLFW::Modifier mods) {}
		};

		void setupCallbacks();
		void setListener(Listener * const & listener);

		glm::vec2 getCursorPos();
		bool isKeyPressed(GLFW::Key key);
	}

	namespace RunLoop
	{
		void start(bool constantRedraw = false);
		void interrupt();
	}

	void init(int width, int height, const char * title);
	void destroy() noexcept;

	bool isInit() noexcept;
	void checkInit();

	GLFWwindow * getHandle();
}

namespace Viewport
{

}

#endif