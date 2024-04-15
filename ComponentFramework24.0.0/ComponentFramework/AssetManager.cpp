#include "AssetManager.h"
#include <iostream>
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "MeshComponent.h"
AssetManager::AssetManager() {
	AddComponent<ShaderComponent>("TextureShader", nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	AddComponent<MeshComponent>("CheckerMesh", nullptr, "meshes/CheckerPiece.obj");
	AddComponent<MeshComponent>("BoardMesh", nullptr, "meshes/Plane.obj");
	AddComponent<MaterialComponent>("BoardMaterial", nullptr, "textures/8x8_checkered_board.png");
	AddComponent<MaterialComponent>("WhiteCheckerMaterial", nullptr, "textures/whiteCheckerPiece.png");
	AddComponent<MaterialComponent>("BlackCheckerMaterial", nullptr, "textures/blackCheckerPiece.png");
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