#include <pandaGLFW/window.hpp>

#include <vector>

#include <GLFW/glfw3.h>

using namespace Window;

static thread_local std::vector< Event::Listener * > g_listeners;
static thread_local bool g_isDirty = true, g_clearOnRedraw = true;

/*
	Native GLFW event callbacks used for the WindowListener class
*/

static void onWindowResizeCallback( GLFWwindow * native, int width, int height )
{
	for ( Event::Listener * const & listener : g_listeners )
		listener->onWindowResize( width, height );
}

static void onFramebufferResizeCallback( GLFWwindow * native, int width, int height )
{
	Window::Graphics::setViewport( 0, 0, width, height );
	for ( Event::Listener * const & listener : g_listeners )
		listener->onFramebufferResize( width, height );
}

static void onWindowContentScaleChangeCallback( GLFWwindow * native, float xscale, float yscale )
{
	for ( Event::Listener * const & listener : g_listeners )
		listener->onWindowContentScaleChange( xscale, yscale );
}

static void onWindowRefreshCallback( GLFWwindow * native )
{
	for ( Event::Listener * const & listener : g_listeners )
		listener->onWindowRedraw();

	glfwSwapBuffers( native );
	Event::setDirty( false ); 
}

/*
	Window::Event function implementations
*/

void Event::setupCallbacks()
{
	GLFWwindow * handle = Window::getHandle();
	glfwSetWindowSizeCallback( handle, onWindowResizeCallback );
	glfwSetFramebufferSizeCallback( handle, onFramebufferResizeCallback );
	glfwSetWindowContentScaleCallback( handle, onWindowContentScaleChangeCallback );
	glfwSetWindowRefreshCallback( handle, onWindowRefreshCallback );
}

void Event::addListener( Listener * listener )
{
	g_listeners.push_back( listener );
}

void Event::removeListener( Listener * listener )
{
	auto it = std::find( g_listeners.begin(), g_listeners.end(), listener );
	if ( it == g_listeners.end() ) return;
	g_listeners.erase( it );
}

void Event::setDirty( bool dirty )
{
	g_isDirty = dirty;
	Window::RunLoop::interrupt();
}

bool Event::isDirty()
{
	return g_isDirty;
}

void Event::setClearOnRedraw( bool clearOnRedraw )
{
	g_clearOnRedraw = clearOnRedraw;
}

void Event::dispatchWindowRedraw()
{
	if ( g_clearOnRedraw ) Window::Graphics::clear();
	for ( Event::Listener * const & listener : g_listeners )
		listener->onWindowRedraw();
}