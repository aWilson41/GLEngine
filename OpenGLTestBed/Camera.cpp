#include "Camera.h"

Camera::Camera()
{
	// The Projection Identity
	proj[0][0] = 1.0f; proj[0][1] = 0.0f; proj[0][2] = 0.0f; proj[0][3] = 0.0f;

	proj[1][0] = 0.0f; proj[1][1] = 1.0f; proj[1][2] = 0.0f; proj[1][3] = 0.0f;

	proj[2][0] = 0.0f; proj[2][1] = 0.0f; proj[2][2] = 1.0f; proj[2][3] = 0.0f;

	proj[3][0] = 0.0f; proj[3][1] = 0.0f; proj[3][2] = 1.0f; proj[3][3] = 0.0f;

    view = glm::mat4();
    //dir = float3(0.0f, 0.0f, 1.0f);
}

// Sets Perspective Projection
void Camera::SetPerspective(float fov, float aspectRatio, float nearZ, float farZ)
{
   // Set camera specifications
	Camera::fov = fov;
	Camera::nearZ = nearZ;
	Camera::farZ = farZ;
	Camera::aspectRatio = aspectRatio;

    proj = glm::perspective(fov, aspectRatio, nearZ, farZ);
}

void Camera::LookAt(glm::vec3 pos)
{
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    view = glm::lookAt(eye, pos, up);
}

void Camera::SetCameraView(glm::mat4 dM)
{
   view = dM * view;
   //dir = MathHelper::ToFloat3(dM * MathHelper::ToFloat4w0(dir));
}