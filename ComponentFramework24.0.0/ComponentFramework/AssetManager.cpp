#include "AssetManager.h"
#include <iostream>
#include "ShaderComponent.h"
AssetManager::AssetManager() {
	AddComponent<ShaderComponent>("TextureShader", nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
}
AssetManager::~AssetManager() {
}

bool AssetManager::OnCreate() {
	for (std::pair<const char*, Ref<Component>> c : componentCatalogue) {
		if (c.second->OnCreate() == false) {
			return false;
		}
	}
	return true;
}
void AssetManager::RemoveAllComponents() {
	componentCatalogue.clear();
}

void AssetManager::ListAllComponents() {
	for(auto c : componentCatalogue) {
		std::cout << c.first << "; " << typeid(*(c.second.get())).name() << std::endl;
	}
}