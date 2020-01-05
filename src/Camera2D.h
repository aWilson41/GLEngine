#pragma once
#include "Camera.h"

// A camera that only supports panning
class Camera2D : public Camera
{
public:
	Camera2D() { reset(); }

public:
	GLfloat getShiftSpeed() const { return shiftSpeed; }
	glm::vec2 getPos() const { return shift; }
	glm::vec2 getScale() const { return scale; }

	// Default is 0.025f
	void setShiftSpeed(GLfloat speed) { shiftSpeed = speed; }
	// Default is 0.5f
	void setZoomSpeed(GLfloat speed) { zoomSpeed = speed; }
	void setPos(glm::vec2 shift) { setPos(shift.x, shift.y); }
	void setPos(GLfloat x, GLfloat y) { shift = glm::vec3(x, y, 0.0f); }
	void setScale(GLfloat s) { setScale(s, s); }
	void setScale(glm::vec2 scale) { setScale(scale.x, scale.y); }
	void setScale(GLfloat x, GLfloat y) { scale = glm::vec3(x, y, 1.0f); }

	void initCamera2D(GLfloat focusX, GLfloat focusY)
	{
		shift = glm::vec3(focusX, focusY, 0.0f);
		initCamera(-1.0f, 1.0f, 1.0f, -1.0f, 0.001f, 100.0f);
	}
	void initCamera2D(GLfloat focusX, GLfloat focusY, GLfloat width, GLfloat height)
	{
		shift = glm::vec3(focusX, focusY, 0.0f);
		GLfloat scaleX = width * aspectRatio / 1920.0f;
		GLfloat scaleY = height / 1080.0f;
		initCamera(-scaleX * 0.5f, scaleX * 0.5f, scaleY * 0.5f, -scaleY * 0.5f, 0.01f, 100.0f);
	}

	// Updates only the view
	void updateView() override
	{
		view = MathHelp::scale(1.0f / (scale * scale)) * MathHelp::translate(shift);
		invView = glm::inverse(view);
	}

	// Differential transform for panning
	void pan(glm::vec2 diff)
	{
		// Flip screen coord y
		diff.x *= aspectRatio;
		shift += glm::vec3(diff, 0.0f);
		updateView();
	}
	// Differential transform for zooming
	void zoom(GLfloat diff)
	{
		GLfloat s = diff;
		if (diff < 0.0f)
			s = -diff;
		scale += s * zoomSpeed;
		updateView();
	}

	// Resets to defaults
	void reset() override { initCamera2D(0.0f, 0.0f, 2.0f, 2.0f); }

protected:
	GLfloat shiftSpeed = 0.025f;
	GLfloat zoomSpeed = 0.05f;
	glm::vec3 scale = glm::vec3(1.0f);
	glm::vec3 shift = glm::vec3(0.0f); // Focal pt
};