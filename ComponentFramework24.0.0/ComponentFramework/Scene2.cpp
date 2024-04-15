#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Scene2.h"
#include <MMath.h>
#include "Debug.h"
#include "Body.h"
#include <Quaternion.h>
#include <QMath.h>
//Add a camera class, make it the parent of the scene and child everything below it 
Scene2::Scene2() : camera{nullptr}, shader { nullptr }, mesh{ nullptr },
drawInWireMode{ false } {
	Debug::Info("Created Scene2: ", __FILE__, __LINE__);
}

Scene2::~Scene2() {
	Debug::Info("Deleted Scene2: ", __FILE__, __LINE__);
}

bool Scene2::OnCreate() {
	Debug::Info("Loading assets Scene2: ", __FILE__, __LINE__);

	modelMatrix.loadIdentity();
	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 12.0f,8.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	board = std::make_shared<Actor>(nullptr);
	assetManager.OnCreate();
	
	if (!board) { return false;}
	board->AddComponent<TransformComponent>(nullptr, QMath::angleAxisRotation(-0.0f, Vec3(1.0f, 0.0f, 0.0f)), Vec3(0,0,0));//-7.8f works the best
	board->OnCreate();
	board->ListComponents();

	for (int i = 0; i < 16; i++) {
		whiteCheckers.push_back(std::make_shared<Actor>(board));
	}

	Ref<TransformComponent> btc = board->GetComponent<TransformComponent>();


	float i = 0.5f;
	float j = 0;
	for (auto checker : whiteCheckers) {
		if (!checker) { return false; }
		checker->AddComponent<TransformComponent>(btc,btc->getOrientation() * QMath::angleAxisRotation(90.0f, Vec3(1.0f, 0.0f, 0.0f)), btc->getPosition() + Vec3(-4.95,4.3,0) + Vec3(i * 1.24, j * -1.2,0 ), Vec3(0.1f, 0.1f, 0.1f));
		checker->OnCreate();
		checker->ListComponents();
		//-2.5,2.55,0
		//-0.5,1.9,0
		i++;
		if (int(i) % 8 == 0 && i != 0) {
			j++;
			i = 0.5f;
		}
	}



	for (int i = 0; i < 16; i++) {
		blackCheckers.push_back(std::make_shared<Actor>(board));
	}
	 i = 0;
	 j = 5;
	for (auto checker : blackCheckers) {
		if (!checker) { return false; }
		checker->AddComponent<TransformComponent>(nullptr, Quaternion(), btc->getPosition() + Vec3(-4.3, 2.88, 0) + Vec3(i * 1.24, j * -1.2, 0), Vec3(0.1f, 0.1f, 0.1f));
		checker->OnCreate();
		checker->ListComponents();
		i++;
		if (int(i) % 8 == 0 && i != 0) {
			j++;
			i = 0;
		}
	}
	//create 2 vectors one for red checker and other for black
	// each vector will have 12 pieces in total ,  1.2 is the difference between two consecutive black squares
	// u will need to use double for loop one to check the lines and other to check the columns, 
	// each line will have 4 pieces 
	//once u get one checkered piece just make the top left one a parent and all of them to follow it through a vector. 
	return true;
}

void Scene2::OnDestroy() {
	Debug::Info("Deleting assets Scene2: ", __FILE__, __LINE__);
	board->RemoveAllComponents();
	board->OnDestroy();
	for (auto checker : whiteCheckers) {
		checker->RemoveAllComponents();
		checker->OnDestroy();
	}
	for (auto checker :blackCheckers) {
		checker->RemoveAllComponents();
		checker->OnDestroy();
	}
}

void Scene2::HandleEvents(const SDL_Event& sdlEvent) {
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

void Scene2::Update(const float deltaTime) {
	//static float angle_1;
	//static float angle_2;
	//angle_1 += -45.0f * deltaTime;
	//angle_2 += 60.0f * deltaTime;
	//Quaternion q = board->GetComponent<TransformComponent>()->getOrientation();
	//Quaternion rotate_1 = QMath::angleAxisRotation(angle_1, Vec3(1.0f, 0.0f, 1.0f));
	//Quaternion rotate_2 = QMath::angleAxisRotation(angle_2, Vec3(0.0f, 1.0f, 1.0f));

	//// Adding slerp and making the movement smoother
	//float slerpValue = 0.5f;
	//Quaternion slerp = QMath::slerp(rotate_1, rotate_2, slerpValue);
	//board->GetComponent<TransformComponent>()->SetOrientation(slerp);
}

void Scene2::Render() const {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	/// Set the background color then clear the screen
	glClearColor(0.19f, 0.56f, 0.53f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (drawInWireMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	Ref<ShaderComponent> sc = assetManager.GetComponent<ShaderComponent>("TextureShader");
	Ref<MeshComponent> bmc = assetManager.GetComponent<MeshComponent>("BoardMesh");
	Ref<MaterialComponent> bmac = assetManager.GetComponent<MaterialComponent>("BoardMaterial");
	Ref<TransformComponent> btc = board->GetComponent<TransformComponent>();
	Ref<MaterialComponent> cwmc = assetManager.GetComponent<MaterialComponent>("WhiteCheckerMaterial");
	Ref<MaterialComponent> cbmc = assetManager.GetComponent<MaterialComponent>("BlackCheckerMaterial");
	Ref<MeshComponent> Bishop = assetManager.GetComponent<MeshComponent>("Bishop");
	Ref<MeshComponent> King = assetManager.GetComponent<MeshComponent>("King");
	Ref<MeshComponent> Knight = assetManager.GetComponent<MeshComponent>("Knight");
	Ref<MeshComponent> Rook = assetManager.GetComponent<MeshComponent>("Rook");
	Ref<MeshComponent>Queen = assetManager.GetComponent<MeshComponent>("Queen");
	Ref<MeshComponent> Pawn = assetManager.GetComponent<MeshComponent>("Pawn");


	glUseProgram(sc->GetProgram());
	glUniformMatrix4fv(sc->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(sc->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sc->GetUniformID("modelMatrix"), 1, GL_FALSE, btc->getModelmatrix());
	glBindTexture(GL_TEXTURE_2D, bmac->getMaterialID());
	bmc->Render(GL_TRIANGLES);
	glBindTexture(GL_TEXTURE_2D, 0);

	for (const auto& checker : whiteCheckers) {
			// Assume 'checker' is a specific piece instance with a TransformComponent
			glUniformMatrix4fv(sc->GetUniformID("modelMatrix"), 1, GL_FALSE, (btc->getModelmatrix() * checker->GetComponent<TransformComponent>()->getModelmatrix()));
			glBindTexture(GL_TEXTURE_2D, cwmc->getMaterialID());
			Bishop->Render(GL_TRIANGLES);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		for (auto checker : blackCheckers) {
			glUniformMatrix4fv(sc->GetUniformID("modelMatrix"), 1, GL_FALSE, btc->getModelmatrix() * checker->GetComponent<TransformComponent>()->getModelmatrix());
			glBindTexture(GL_TEXTURE_2D, cbmc->getMaterialID());
			//cmc->Render(GL_TRIANGLES);
			glBindTexture(GL_TEXTURE_2D, 0);

		}
		glUseProgram(0);
	}



