#include "RenderWindow.h"
#include "Framebuffer.h"
#include "Renderer.h"
#include "WindowInteractor.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

//#define FULLSCREEN

RenderWindow::RenderWindow(std::string windowName, int x, int y, int width, int height, bool fullscreen)
{
	glfwSetErrorCallback(glfwErrorCallback);

	if (!glfwInit())
		throw std::runtime_error("Failed to initialise GLFW");

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Deprecated functionality removed
	//glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

	createWindow(windowName, x, y, width, height, fullscreen);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS); // The default
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
}
RenderWindow::RenderWindow(std::string windowName, int width, int height, bool fullscreen)
{
	glfwSetErrorCallback(glfwErrorCallback);

	if (!glfwInit())
		throw std::runtime_error("Failed to initialise GLFW");

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Deprecated functionality removed
														 //glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

	const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int windowWidth = static_cast<int>(vidMode->width * 0.75);
	int windowHeight = static_cast<int>(vidMode->height * 0.75);
	int x = static_cast<int>((windowWidth - width) * 0.5);
	int y = static_cast<int>((windowHeight - height) * 0.5);
	createWindow(windowName, x, y, width, height, fullscreen);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS); // The default
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
}

int RenderWindow::getWindowWidth() const
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return width;
}
int RenderWindow::getWindowHeight() const
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return height;
}
int RenderWindow::getFramebufferWidth() const
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	return width;
}
int RenderWindow::getFramebufferHeight() const
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	return height;
}

void RenderWindow::setRenderer(std::shared_ptr<Renderer> ren)
{
	this->ren = ren;
	// Get the default framebuffers new size
	int frameBufferWidth, frameBufferHeight;
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	// Initialize renderer to the same size framebuffer as the windows default
	ren->resizeFramebuffer(frameBufferWidth, frameBufferHeight);
}
void RenderWindow::setWindowName(std::string name)
{
	windowName = name;
	glfwSetWindowTitle(window, windowName.c_str());
}
void RenderWindow::setInteractor(std::shared_ptr<WindowInteractor> interactor)
{
	this->interactor = interactor;
	// Initialize interactor with mouse position
	double posX, posY;
	glfwGetCursorPos(window, &posX, &posY);
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	interactor->init(glm::vec2(posX, posY), width, height);
}


void RenderWindow::start()
{
	// Update loop
	while (isActive())
	{
		render();
	}
}
void RenderWindow::stop() { glfwTerminate(); }

void RenderWindow::render()
{
	if (window == nullptr || ren == nullptr)
		return;

	ren->render();

	// Blit the results to the default fbo
	const glm::ivec2 fboDim = ren->getFramebufferDim();
	ren->getColorOutput()->bindRead();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, fboDim.x, fboDim.y, 0, 0, fboDim.x, fboDim.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	ren->getDepthOutput()->bindRead();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, fboDim.x, fboDim.y, 0, 0, fboDim.x, fboDim.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool RenderWindow::isActive() { return !glfwWindowShouldClose(window); }


void RenderWindow::createWindow(std::string windowName, int x, int y, int windowWidth, int windowHeight, bool fullscreen)
{
	const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if (windowWidth == -1)
		windowWidth = static_cast<int>(vidMode->width * 0.75);
	if (windowHeight == -1)
		windowHeight = static_cast<int>(vidMode->height * 0.75);
	if (x == -1)
		x = static_cast<int>(vidMode->width * 0.25 * 0.5);
	if (y == -1)
		y = static_cast<int>(vidMode->height * 0.25 * 0.5);
	if (fullscreen)
		window = glfwCreateWindow(static_cast<int>(vidMode->width), static_cast<int>(vidMode->height), windowName.c_str(), glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create window with GLFW");
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, x, y);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetWindowSizeCallback(window, glfwWindowResize);
	glfwSetCursorPosCallback(window, glfwMouseMove);
	glfwSetMouseButtonCallback(window, glfwMouseButton);
	glfwSetScrollCallback(window, glfwScroll);
	glfwSetKeyCallback(window, glfwKeyEvent);
	glfwSetWindowUserPointer(window, this);
	glfwSwapInterval(0); // Turn off vsync

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to initialize GLEW");
	}
}

void RenderWindow::glfwWindowResize(GLFWwindow* window, int width, int height)
{
	RenderWindow* renWin = static_cast<RenderWindow*>(glfwGetWindowUserPointer(window));
	renWin->glfwWindowResize(width, height);
}
void RenderWindow::glfwWindowResize(int width, int height)
{
	if (interactor == nullptr)
	{
		printf("Error: Render Window missing Interactor\n");
		return;
	}
	interactor->windowResize(width, height);
	// Get the default framebuffers new size
	int frameBufferWidth, frameBufferHeight;
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	// Make sure the renderer is updated, in case it uses a different framebuffer
	ren->resizeFramebuffer(frameBufferWidth, frameBufferHeight);
}

void RenderWindow::glfwMouseMove(GLFWwindow* window, double posX, double posY)
{
	RenderWindow* renWin = static_cast<RenderWindow*>(glfwGetWindowUserPointer(window));
	if (renWin->getInteractor() == nullptr)
	{
		printf("Error: Render Window missing Interactor\n");
		return;
	}
	renWin->getInteractor()->mouseMove(glm::vec2(posX, posY));
}

void RenderWindow::glfwMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	RenderWindow* renWin = static_cast<RenderWindow*>(glfwGetWindowUserPointer(window));
	if (renWin->getInteractor() == nullptr)
	{
		printf("Error: Render Window missing Interactor\n");
		return;
	}
	if (action == GLFW_PRESS)
		renWin->getInteractor()->mouseDown(button);
	else if (action == GLFW_RELEASE)
		renWin->getInteractor()->mouseUp(button);
}

void RenderWindow::glfwScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	RenderWindow* renWin = static_cast<RenderWindow*>(glfwGetWindowUserPointer(window));
	if (renWin->getInteractor() == nullptr)
	{
		printf("Error: Render Window missing Interactor\n");
		return;
	}
	renWin->getInteractor()->mouseScroll(static_cast<GLfloat>(yoffset));
}

void RenderWindow::glfwKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	RenderWindow* renWin = static_cast<RenderWindow*>(glfwGetWindowUserPointer(window));
	if (renWin->getInteractor() == nullptr)
	{
		printf("Error: Render Window missing Interactor\n");
		return;
	}
	if (action == GLFW_PRESS)
		renWin->getInteractor()->keyDown(key);
	else if (action == GLFW_RELEASE)
		renWin->getInteractor()->keyUp(key);
}