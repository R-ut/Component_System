#ifndef SCENE2_H
#define SCENE2_H
#include "Scene.h"
#include <Vector.h>
#include <Matrix.h>
#include "Actor.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "ShaderComponent.h"
#include "UBO_Padding.h"
#include "CameraActor.h"
#include "AssetManager.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Body;
class Mesh;
class Shader;
class CameraActor;
class Actor;
class LightActor;
class AssetManager;

class Scene2 : public Scene {
private:
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 modelMatrix;
	bool drawInWireMode;
	Ref<CameraActor> camera;
	/*Ref<LightActor> light;*/
	AssetManager assetManager;
	Ref<Shader> shader;
	Ref<Mesh> mesh;
	Ref<Actor> board;
	Ref<Actor> checker;
	std::unordered_map<std::string, Ref<Actor>> whiteChessPieces;
	std::unordered_map<std::string, Ref<Actor>> blackChessPieces;
	std::vector<Ref<Actor>> actors;
	Ref<MeshComponent> Bishop = assetManager.GetComponent<MeshComponent>("Bishop");
	Ref<MeshComponent> King = assetManager.GetComponent<MeshComponent>("King");
	Ref<MeshComponent> Knight = assetManager.GetComponent<MeshComponent>("Knight");
	Ref<MeshComponent> Rook = assetManager.GetComponent<MeshComponent>("Rook");
	Ref<MeshComponent>Queen = assetManager.GetComponent<MeshComponent>("Queen");
	Ref<MeshComponent> Pawn = assetManager.GetComponent<MeshComponent>("Pawn");

public:
	explicit Scene2();
	virtual ~Scene2();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
	int Pick(int x, int y);
};


#endif // Scene2_H