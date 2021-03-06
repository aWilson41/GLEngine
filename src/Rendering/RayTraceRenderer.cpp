//#include "RayTraceRenderer.h"
//#include "Camera.h"
//#include "Framebuffer.h"
//#include "ShaderProgram.h"
//#include "Shaders.h"
//
//static int nextPowerOfTwo(int x) {
//	x--;
//	x |= x >> 1; // handle 2 bit numbers
//	x |= x >> 2; // handle 4 bit numbers
//	x |= x >> 4; // handle 8 bit numbers
//	x |= x >> 8; // handle 16 bit numbers
//	x |= x >> 16; // handle 32 bit numbers
//	x++;
//	return x;
//}
//
//RayTraceRenderer::RayTraceRenderer()
//{
//	shaderGroup = "Raytrace";
//
//	// Load the compute shader
//	std::string shaderDir = "Shaders/" + shaderGroup + '/';
//	std::vector<std::string> shaderPaths;
//	shaderPaths.push_back(shaderDir + "raytracer.glsl");
//	shaderPaths.push_back(shaderDir + "random.glsl");
//	shaderProgram = Shaders::loadComputeShader("main", shaderPaths);
//	quadShaderProgram = Shaders::loadVSFSShader("quad", shaderDir + "fullScreenQuadVS.glsl", shaderDir + "fullScreenQuadFS.glsl");
//
//	glGenVertexArrays(1, &emptyVaoID);
//
//	start = std::chrono::system_clock::now();
//}
//
//void RayTraceRenderer::render()
//{
//	framebuffer->bind();
//	framebuffer->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	GLuint shaderID = shaderProgram->getProgramID();
//	glUseProgram(shaderID);
//
//
//	bindCameraUniforms(shaderID);
//	bindSceneUniforms(shaderID);
//	bindRenderUniforms(shaderID);
//
//
//	// Bind the framebuffer
//	glBindImageTexture(0, colorTexID, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
//
//	// Do the render
//	static GLint workGroupSize[3] = { -1, -1, -1 };
//	glGetProgramiv(shaderID, GL_COMPUTE_WORK_GROUP_SIZE, workGroupSize);
//	const glm::ivec2 fboDim = getFramebufferDim();
//	int numGroupsX = nextPowerOfTwo(fboDim.x) / workGroupSize[0];
//	int numGroupsY = nextPowerOfTwo(fboDim.y) / workGroupSize[1];
//	glDispatchCompute(numGroupsX, numGroupsY, 1);
//
//	// Block until the image is written
//	glBindImageTexture(0, 0, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
//	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
//	glUseProgram(0);
//
//
//	// Present the actual image
//	glUseProgram(quadShaderProgram->getProgramID());
//	glBindVertexArray(0);
//	glBindTexture(GL_TEXTURE_2D, colorTexID);
//	
//	// Then render the quad
//	glBindVertexArray(emptyVaoID);
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//	glBindVertexArray(0);
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//	glBindVertexArray(0);
//	glUseProgram(0);
//	frameCount++;
//
//	framebuffer->unbind();
//}
//
//void RayTraceRenderer::bindCameraUniforms(GLuint shaderID)
//{
//	// Bind camera uniforms
//	GLuint eyePosLocation = glGetUniformLocation(shaderID, "eye");
//	GLuint ray00Location = glGetUniformLocation(shaderID, "ray00");
//	GLuint ray01Location = glGetUniformLocation(shaderID, "ray01");
//	GLuint ray10Location = glGetUniformLocation(shaderID, "ray10");
//	GLuint ray11Location = glGetUniformLocation(shaderID, "ray11");
//	if (eyePosLocation == -1 ||
//		ray00Location == -1 || ray01Location == -1 ||
//		ray10Location == -1 || ray11Location == -1)
//	{
//		printf("Failed to find a camera uniform\n");
//		return;
//	}
//
//	glm::vec3 eyePos = cam->getViewPos();
//	glUniform3f(eyePosLocation, eyePos.x, eyePos.y, eyePos.z);
//	geom3d::Ray eyeRay = cam->getEyeRay(-1.0f, -1.0f);
//	glUniform3f(ray00Location, eyeRay.dir.x, eyeRay.dir.y, eyeRay.dir.z);
//	eyeRay = cam->getEyeRay(-1.0f, 1.0f);
//	glUniform3f(ray01Location, eyeRay.dir.x, eyeRay.dir.y, eyeRay.dir.z);
//	eyeRay = cam->getEyeRay(1.0f, -1.0f);
//	glUniform3f(ray10Location, eyeRay.dir.x, eyeRay.dir.y, eyeRay.dir.z);
//	eyeRay = cam->getEyeRay(1.0f, 1.0f);
//	glUniform3f(ray11Location, eyeRay.dir.x, eyeRay.dir.y, eyeRay.dir.z);
//	// Save one of the rays to determine if view changes
//	currEyeRayDir = eyeRay.dir;
//}
//void RayTraceRenderer::bindSceneUniforms(GLuint shaderID)
//{
//	// Bind the scene uniforms
//	GLuint lightDirLocation = glGetUniformLocation(shaderID, "lightDir");
//	if (lightDirLocation == -1)
//	{
//		printf("Failed to find a scene uniform\n");
//		return;
//	}
//	glUniform3f(lightDirLocation, lightDir.x, lightDir.y, lightDir.z);
//}
//void RayTraceRenderer::bindRenderUniforms(GLuint shaderID)
//{
//	// Bind the renderer uniforms
//	GLuint timeLocation = glGetUniformLocation(shaderID, "time");
//	GLuint blendRatioLocation = glGetUniformLocation(shaderID, "blendRatio");
//	GLuint numBouncesLocation = glGetUniformLocation(shaderID, "numBounces");
//	GLuint bgColorLocation = glGetUniformLocation(shaderID, "bgColor");
//	if (timeLocation == -1 || blendRatioLocation == -1 ||
//		numBouncesLocation == -1 || bgColorLocation == -1)
//	{
//		printf("Failed to find a renderer uniform\n");
//		return;
//	}
//	glUniform1f(timeLocation, static_cast<GLfloat>((std::chrono::system_clock::now() - start).count()));
//	// If the camera direciton has changed flush the render
//	if (prevEyeRayDir.x != currEyeRayDir.x || prevEyeRayDir.y != currEyeRayDir.y || prevEyeRayDir.z != currEyeRayDir.z)
//	{
//		glUniform1f(blendRatioLocation, 0.0f);
//		frameCount = 0;
//	}
//	else
//		glUniform1f(blendRatioLocation, static_cast<float>(frameCount) / (frameCount + 1));
//	prevEyeRayDir = currEyeRayDir;
//	glUniform1i(numBouncesLocation, 7);
//	glUniform3f(bgColorLocation, clearColor[0], clearColor[1], clearColor[2]);
//}