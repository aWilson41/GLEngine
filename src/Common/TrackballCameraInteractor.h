#pragma once
#include <memory>
#include "WindowInteractor.h"

class TrackballCamera;

// Maps mouse values to camera
class TrackballCameraInteractor : public WindowInteractor
{
public:
	virtual ~TrackballCameraInteractor() override = default;

public:
	void setCamera(std::shared_ptr<TrackballCamera> cam) { this->cam = cam; }

	void init(glm::vec2 pos, int windowWidth, int windowHeight) override;

	virtual void mouseMove(glm::vec2 pos) override;
	virtual void mouseScroll(GLfloat ds) override;
	virtual void windowResize(int width, int height) override;

protected:
	std::shared_ptr<TrackballCamera> cam = nullptr;
	glm::vec2 screenSize = glm::vec2(1.0f);
};