#pragma once
#include "MathHelper.h"
#include <GL\glew.h>

class Camera
{
private:
	// Camera View
    glm::mat4 view;
	// Camera Projection
	glm::mat4 proj;
    // Camera position
    glm::vec3 eye;

   // Direction
   //float3 dir;
   
	float fov;
	float width;
	float height;
	float aspectRatio;
	float nearZ;
	float farZ;

public:
    GLuint viewMatID;
    GLuint projMatID;

public:
	Camera();

	// Sets the Camera's Projection
	void SetPerspective(float fov, float aspectRatio, float nearZ, float farZ);
    void SetCamPos(glm::vec3 eye) { Camera::eye = eye; }
    void LookAt(glm::vec3 pos);
    void SetCameraView(glm::mat4 dM); // Delta transform

	float GetFOV() { return fov; }
	float GetAspectRatio() { return aspectRatio; }
	float GetNearZ() { return nearZ; }
	float GetFarZ() { return farZ; }

	glm::mat4 GetProj() { return proj; }
	glm::mat4 GetView() { return view; }

    //float3 GetViewDir() { return dir; }
};