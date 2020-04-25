#pragma once
#include "Camera.h"

// Camera whose view is controlled by a lookat function
// Specify an eye position and focal pt along with an up direction
class CameraLookAt : public Camera
{
public:
	// Makes sure to update with default on creation
	CameraLookAt() { reset(); }

public:
	glm::vec3 getEyePos() const { return eyePos; }
	glm::vec3 getFocalPt() const { return focalPt; }
	glm::vec3 getUp() const { return up; }
	//glm::vec3 getLookDir() const { return glm::normalize(focalPt - eyePos); }

	void setEyePos(glm::vec3 eyePos) { this->eyePos = eyePos; }
	void setEyePos(GLfloat x, GLfloat y, GLfloat z) { setEyePos(glm::vec3(x, y, z)); }
	void setFocalPt(glm::vec3 focalPt) { this->focalPt = focalPt; }
	void setFocalPt(GLfloat x, GLfloat y, GLfloat z) { setFocalPt(glm::vec3(x, y, z)); }
	void setUp(glm::vec3 up) { this->up = up; }
	void setUp(GLfloat x, GLfloat y, GLfloat z) { setUp(glm::vec3(x, y, z)); }

	void initCameraLookAt(
		GLfloat fov, GLfloat aspectRatio, GLfloat nearZ, GLfloat farZ,
		glm::vec3 eyePos, glm::vec3 focalPt, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
	{
		setEyePos(eyePos);
		setFocalPt(focalPt);
		setUp(up);
		initCamera(fov, aspectRatio, nearZ, farZ);
	}
	void initCameraLookAt(
		GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearZ, GLfloat farZ,
		glm::vec3 eyePos, glm::vec3 focalPt, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
	{
		setEyePos(eyePos);
		setFocalPt(focalPt);
		setUp(up);
		initCamera(left, right, top, bottom, nearZ, farZ);
	}

	void updateProj()
	{
		if (ortho)
		{
			const GLfloat dist = glm::distance(eyePos, focalPt);
			orthoBounds[0] = -dist;
			orthoBounds[1] = dist;
			orthoBounds[2] = dist;
			orthoBounds[3] = -dist;
		}
		Camera::updateProj();
	}

	void updateView() override
	{
		view = glm::lookAt(eyePos, focalPt, up);
		invView = glm::inverse(view);
	}

	void reset() override
	{
		if (ortho)
			initCameraLookAt(-1.0f, 1.0f, 1.0f, -1.0f, 0.0001f, 1000.0f, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f));
		else
			initCameraLookAt(45.0f, 16.0f / 9.0f, 0.0001f, 1000.0f, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f));
	}

protected:
	// Eye position
	glm::vec3 eyePos = glm::vec3(1.0f);
	// Focal point
	glm::vec3 focalPt = glm::vec3(0.0f);
	// Up
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
};