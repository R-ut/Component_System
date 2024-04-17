#include "AssetManager.h"
#include <iostream>
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
AssetManager::AssetManager() {
	AddComponent<ShaderComponent>("TextureShader", nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	AddComponent<ShaderComponent>("ColorPickingShader", nullptr, "shaders/colorPickingVert.glsl", "shaders/colorPickingFrag.glsl");
	AddComponent<MeshComponent>("BoardMesh", nullptr, "meshes/Plane.obj");
	AddComponent<MeshComponent>("Bishop", nullptr, "meshes/Bishop.obj");
	AddComponent<MeshComponent>("Knight", nullptr, "meshes/Knight.obj");
	AddComponent<MeshComponent>("Rook", nullptr, "meshes/Rook.obj");
	AddComponent<MeshComponent>("Queen", nullptr, "meshes/Queen.obj");
	AddComponent<MeshComponent>("King", nullptr, "meshes/King.obj");
	AddComponent<MeshComponent>("Pawn", nullptr, "meshes/Pawn.obj");
	AddComponent<MaterialComponent>("BoardMaterial", nullptr, "textures/8x8_checkered_board.png");
	AddComponent<MaterialComponent>("WhiteCheckerMaterial", nullptr, "textures/White Chess Base Colour.png");
	AddComponent<MaterialComponent>("BlackCheckerMaterial", nullptr, "textures/Black Chess Base Colour.png.png");
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