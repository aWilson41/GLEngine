#pragma once
#include "MathHelper.h"
#include "PropertyMap.h"
#include <string>

class Renderer;
class ShaderProgram;

// Contains property maps that form a bitset combined of render, scene, and object properties
class ShaderProperties
{
public:
	PropertyMap<32>* getObjectProperties() { return &objectProperties; }
	PropertyMap<16>* getSceneProperties() { return &sceneProperties; }
	PropertyMap<16>* getRenderProperties() { return &renderProperties; }
	unsigned long long getKey() const { return keyLong; }
	std::string getFullBitString() const { return key.to_string(); }
	std::string getFullPropertyString();

	void setObjectProperties(PropertyMap<32> objectProperties) { ShaderProperties::objectProperties = objectProperties; }
	void setSceneProperties(PropertyMap<16> sceneProperties) { ShaderProperties::sceneProperties = sceneProperties; }
	void setRenderProperties(PropertyMap<16> renderProperties) { ShaderProperties::renderProperties = renderProperties; }

	// Updates the property combining all the properties to produce a unique 64bit value
	unsigned long long update();

protected:
	PropertyMap<32> objectProperties;
	PropertyMap<16> sceneProperties;
	PropertyMap<16> renderProperties;

	std::bitset<64> key;
	unsigned long long keyLong = 0;
};

class AbstractMapper
{
public:
	AbstractMapper() { objectProperties = properties.getObjectProperties(); }

public:
	virtual void update() = 0;

	// Binds the shader program
	bool use(Renderer* ren);
	virtual bool useShader(std::string shaderGroup) = 0;
	virtual void draw(Renderer* ren) const = 0;

protected:
	PropertyMap<32>* objectProperties = nullptr;
	ShaderProperties properties;
};