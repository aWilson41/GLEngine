#pragma once
#include "CameraLookAt.h"

// A camera whose eye position is fixed on a sphere around
// a focal point (phi, theta, rho/radius)
// Also supports panning
// TODO: Fix panning so pixelwise shift is consistent with on screen shift when shiftSpeed=1
// TODO: Initialization methods for ortho camera
class TrackballCamera : public CameraLookAt
{
public:
	// Makes sure to update with default on creation
	TrackballCamera() { reset(); }
	TrackballCamera(GLfloat phi, GLfloat theta, GLfloat rho)
	{
		initTrackballCamera(phi, theta, rho);
	}

	virtual ~TrackballCamera() override = default;

public:
	GLfloat getPhi() const { return phi; }
	GLfloat getTheta() const { return theta; }
	GLfloat getRho() const { return rho; }
	GLfloat getRotateSpeed() const { return rotateSpeed; }
	GLfloat getShiftSpeed() const { return shiftSpeed; }

	void setPhi(GLfloat phi) { this->phi = phi; }
	void setTheta(GLfloat theta) { this->theta = theta; }
	void setRho(GLfloat rho) { this->rho = rho; }
	// Default is 4.0f
	void setRotateSpeed(GLfloat speed) { rotateSpeed = speed; }
	// Default is 0.025f
	void setShiftSpeed(GLfloat speed) { shiftSpeed = speed; }
	// Default is 0.5f
	void setZoomSpeed(GLfloat speed) { zoomSpeed = speed; }

	void initTrackballCamera(GLfloat phi, GLfloat theta, GLfloat rho,
		GLfloat fov, GLfloat aspectRatio, GLfloat nearZ, GLfloat farZ)
	{
		this->phi = phi;
		this->theta = theta;
		this->rho = rho;
		initCameraLookAt(fov, aspectRatio, nearZ, farZ, eyePos, focalPt);
	}
	void initTrackballCamera(GLfloat phi, GLfloat theta, GLfloat rho)
	{
		this->phi = phi;
		this->theta = theta;
		this->rho = rho;
		initCameraLookAt(fov, aspectRatio, nearZ, farZ, eyePos, focalPt);
	}
	void initTrackballCamera(GLfloat phi, GLfloat theta, GLfloat rho, GLfloat fov, GLfloat nearZ, GLfloat farZ)
	{
		this->phi = phi;
		this->theta = theta;
		this->rho = rho;
		initCameraLookAt(fov, aspectRatio, nearZ, farZ, eyePos, focalPt);
	}
	void initTrackballCamera(GLfloat fov, GLfloat aspectRatio, GLfloat nearZ, GLfloat farZ)
	{
		initCameraLookAt(fov, aspectRatio, nearZ, farZ, eyePos, focalPt);
	}
	void initTrackballCamera(GLfloat phi, GLfloat theta, GLfloat rho,
		GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearZ, GLfloat farZ)
	{
		this->phi = phi;
		this->theta = theta;
		this->rho = rho;
		initCameraLookAt(left, right, top, bottom, nearZ, farZ, eyePos, focalPt, up);
	}

	void updateView() override
	{
		// Exponentially scale rho
		GLfloat r = std::pow(1.2f, rho) * 0.01f;
		// Calc eye position on unit sphere at origin
		glm::vec3 eyePos = glm::vec3(
			r * sin(phi) * cos(theta),
			r * cos(phi),
			r * sin(phi) * sin(theta));

		setEyePos(eyePos + focalPt);
		setFocalPt(focalPt);
		CameraLookAt::updateView();
	}

	// Input is a change in position like (prevMousePos - currentPos)
	void rotate(glm::vec2 diff)
	{
		theta += diff.x * rotateSpeed;
		phi -= diff.y * rotateSpeed;

		phi = glm::clamp(phi, 0.01f, 3.14f);
		update();
	}
	// Maps 2d mouse coordinates to shifts
	void pan(glm::vec2 diff)
	{
		// Up and right are the directions to pan when camera is facing forward
		glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::mat3 invView = glm::inverse(view);
		glm::vec3 dy = invView * up;
		glm::vec3 dx = invView * right;

		// Scale shift with scale so shift is relative to how far you are zoomed out
		GLfloat r = std::pow(1.2f, rho);
		focalPt += (dy * diff.y - dx * diff.x) * r * shiftSpeed;
		update();
	}
	void zoom(GLfloat diff)
	{
		rho += diff * zoomSpeed;
		update();
	}

	// Resets to defaults
	void reset() override
	{
		if (ortho)
			initTrackballCamera(1.4f, 1.57f, 35.0f, -1.0f, 1.0f, 1.0f, -1.0f, 0.0001f, 1000.0f);
		else
			initTrackballCamera(1.4f, 1.57f, 35.0f, 45.0f, 16.0f / 9.0f, 0.0001f, 1000.0f);
	}

protected:
	// Sphere parameters
	GLfloat phi = 1.4f;
	GLfloat theta = 1.57f;
	GLfloat rho = 35.0f;

	GLfloat rotateSpeed = 4.0f;
	GLfloat shiftSpeed = 0.025f;
	GLfloat zoomSpeed = 0.5f;
};