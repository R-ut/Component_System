#pragma once
#include "Component.h"
#include <glew.h>
class MaterialComponent : public Component {
private:
	GLuint textureID;
	const char* filename;
public:
	MaterialComponent(Component* parent_,const char* filename_);
	virtual ~MaterialComponent();
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render()const;
	GLuint getMaterialID() { return textureID; }
};
