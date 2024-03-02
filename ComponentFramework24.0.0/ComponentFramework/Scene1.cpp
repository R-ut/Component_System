#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Scene1.h"
#include <MMath.h>
#include "Debug.h"
#include "Body.h"
#include <Quaternion.h>
#include <QMath.h>

Scene1::Scene1() : shader{ nullptr }, mesh{ nullptr },
drawInWireMode{ false } {
	Debug::Info("Created Scene1: ", __FILE__, __LINE__);
}

Scene1::~Scene1() {
	Debug::Info("Deleted Scene1: ", __FILE__, __LINE__);
}

bool Scene1::OnCreate() {
	Debug::Info("Loading assets Scene1: ", __FILE__, __LINE__);

	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 5.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	board = new Actor(nullptr);
	if (!board) { return false; }
	board->AddComponent<MeshComponent>(nullptr, "meshes/Plane.obj");
	board->AddComponent<ShaderComponent>(nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	board->AddComponent<TransformComponent>(nullptr, Quaternion(), Vec3(0,0,-8.0f));//-7.8f works the best
	board->AddComponent<MaterialComponent>(nullptr, "textures/8x8_checkered_board.png");
	board->OnCreate();
	board->ListComponents();
	modelMatrix.loadIdentity();

	checker = new Actor(board);
	if (!checker) { return false; }
	checker->AddComponent<MeshComponent>(nullptr, "meshes/CheckerPiece.obj");
	checker->AddComponent<ShaderComponent>(nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	checker->AddComponent<TransformComponent>(nullptr, Quaternion(), Vec3(-3.1f,4.3f,0),Vec3(0.1f,0.1f,0.1f));
	checker->AddComponent<MaterialComponent>(nullptr, "textures/redCheckerPiece.png");
	checker->OnCreate();
	checker->ListComponents();
	//once u get one checkered piece just make the top left one a parent and all of them to follow it through a vector. 
	return true;
}

void Scene1::OnDestroy() {
	Debug::Info("Deleting assets Scene1: ", __FILE__, __LINE__);
}

void Scene1::HandleEvents(const SDL_Event& sdlEvent) {
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_W:
			drawInWireMode = !drawInWireMode;
			break;
		}
		break;

	case SDL_MOUSEMOTION:
		break;

	case SDL_MOUSEBUTTONDOWN:
		break;

	case SDL_MOUSEBUTTONUP:
		break;

	default:
		break;
	}
}

void Scene1::Update(const float deltaTime) {
	/*static float angle = 0.0f;
	angle += 20 * deltaTime;
	Quaternion rotation = QMath::angleAxisRotation(angle, Vec3(0.0f, 1.0f, 0.0f));
	board->GetComponent<TransformComponent>()->SetOrientation(rotation);*/
}

void Scene1::Render() const {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (drawInWireMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	ShaderComponent* sc = board->GetComponent<ShaderComponent>();
	MeshComponent* mc = board->GetComponent<MeshComponent>();
	TransformComponent* tc = board->GetComponent<TransformComponent>();
	MaterialComponent* matc = board->GetComponent<MaterialComponent>();


	glUseProgram(sc->GetProgram());
	glUniformMatrix4fv(sc->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(sc->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sc->GetUniformID("modelMatrix"), 1, GL_FALSE, tc->getModelmatrix());
	glBindTexture(GL_TEXTURE_2D, matc->getMaterialID());
	mc->Render(GL_TRIANGLES);



	glUniformMatrix4fv(sc->GetUniformID("modelMatrix"), 1, GL_FALSE, tc->getModelmatrix() * checker->GetComponent<TransformComponent>()->getModelmatrix());
	glBindTexture(GL_TEXTURE_2D, checker->GetComponent<MaterialComponent>()->getMaterialID());
	checker->GetComponent<MeshComponent>()->Render();
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}




