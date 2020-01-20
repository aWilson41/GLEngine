#pragma once
#include "Camera2D.h"

// Lerped version of Camera2D
class Camera2DSmooth : public Camera2D
{
public:
	void setT(GLfloat t) { this->t = t; }

	GLfloat getT() const { return t; }

public:
	// Updates the view without the lerping function
	// Useful for initialization
	void forceUpdateView() { Camera2D::updateView(); }

	void updateView() override
	{
		desiredView = MathHelp::scale(-scale) * MathHelp::translate(-shift);
		view = MathHelp::lerp(view, desiredView, t);
		invView = glm::inverse(view);
	}

protected:
	glm::mat4 desiredView = glm::mat4(1.0f);
	GLfloat t = 0.0005f;
};