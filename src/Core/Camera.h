#pragma once
#include "MathHelper.h"
#include "Geometry3D.h"

// Supports perspective and ortho cameras
// Offers no control over view, subclass for that
// All cameras should maintain an inverse view
class Camera
{
public:
	// Makes sure to update with default on creation
	Camera() { reset(); }

public:
	GLfloat getFov() { return fov; }
	GLfloat getAspectRatio() { return aspectRatio; }
	GLfloat getNearZ() { return nearZ; }
	GLfloat getFarZ() { return farZ; }
	glm::vec3 getLookDir() const { return glm::vec3(view[2][0], view[2][1], view[2][2]); }
	glm::vec3 getViewPos() const { return glm::vec3(invView[3][0], invView[3][1], invView[3][2]); }
	geom3d::Ray getEyeRay(glm::vec2 ndc) const
	{
		glm::vec4 worldPos = glm::inverse(proj * view) * glm::vec4(ndc, 0.0f, 1.0f);
		glm::vec3 viewPos = getViewPos(); // Eye Position
		return geom3d::Ray(viewPos, glm::normalize(glm::vec3(worldPos) / worldPos.w - viewPos));
	}
	geom3d::Ray getEyeRay(GLfloat x, GLfloat y) const { return getEyeRay(glm::vec2(x, y)); }

	void setPerspective(GLfloat fov, GLfloat aspectRatio, GLfloat nearZ, GLfloat farZ)
	{
		this->fov = fov;
		this->aspectRatio = aspectRatio;
		this->nearZ = nearZ;
		this->farZ = farZ;
		this->ortho = false;
	}
	void setOrtho(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearZ, GLfloat farZ)
	{
		orthoBounds[0] = left;
		orthoBounds[1] = right;
		orthoBounds[2] = bottom;
		orthoBounds[3] = top;
		this->nearZ = nearZ;
		this->farZ = farZ;
		this->ortho = true;
	}

	// Maps all parameters of Camera
	void initCamera(GLfloat fov, GLfloat aspectRatio, GLfloat nearZ, GLfloat farZ)
	{
		setPerspective(fov, aspectRatio, nearZ, farZ);
		update();
	}
	void initCamera(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearZ, GLfloat farZ)
	{
		setOrtho(left, right, top, bottom, nearZ, farZ);
		update();
	}

	// Updates projection then updates view
	virtual void update()
	{
		updateProj();
		updateView();
	}
	virtual void updateProj()
	{
		if (ortho)
			proj = glm::ortho(orthoBounds[0], orthoBounds[1], orthoBounds[3], orthoBounds[2], nearZ, farZ);
		else
			proj = glm::perspective(fov, aspectRatio, nearZ, farZ);
	}
	virtual void updateView()
	{
		view = I4;
		invView = glm::inverse(view);
	}

	// Resets the camera to perspective defaults
	virtual void reset() { initCamera(45.0f, 16.0f / 9.0f, 0.0001f, 1000.0f); }

public:
	// Camera view matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 invView = glm::mat4(1.0f);
	// Camera projection matrix
	glm::mat4 proj = glm::mat4(1.0f);

protected:
	// Persp camera specs
	GLfloat fov = 45.0f;
	GLfloat aspectRatio = 16.0f / 9.0f;
	GLfloat nearZ = 0.0001f;
	GLfloat farZ = 1000.0f;

	// Ortho camera specs
	GLfloat orthoBounds[4] = { -1.0f, 1.0f, -1.0f, 1.0f };
	bool ortho = false;
};