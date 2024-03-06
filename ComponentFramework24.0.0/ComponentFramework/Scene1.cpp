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
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f,14.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	board = new Actor(nullptr);
	if (!board) { return false;}
	board->AddComponent<MeshComponent>(nullptr, "meshes/Plane.obj");
	board->AddComponent<ShaderComponent>(nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	board->AddComponent<TransformComponent>(nullptr, QMath::angleAxisRotation(-0.0f, Vec3(1.0f, 0.0f, 0.0f)), Vec3(0,0,0));//-7.8f works the best
	board->AddComponent<MaterialComponent>(nullptr, "textures/8x8_checkered_board.png");
	board->OnCreate();
	board->ListComponents();
	modelMatrix.loadIdentity();
	for (int i = 0; i < 12; i++) {
		whiteCheckers.push_back(new Actor(board));
	}


	TransformComponent* btc = board->GetComponent<TransformComponent>();


	float i = 0.5f;
	float j = 0;
	for (auto checker : whiteCheckers) {
		if (!checker) { return false; }
		checker->AddComponent<MeshComponent>(nullptr, "meshes/CheckerPiece.obj");
		checker->AddComponent<ShaderComponent>(nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
		checker->AddComponent<TransformComponent>(btc,btc->getOrientation(), btc->getPosition() + Vec3(-4.3,4.3,0) + Vec3(i * 2.48, j * -1.2,0 ), Vec3(0.1f, 0.1f, 0.1f));
		checker->AddComponent<MaterialComponent>(nullptr, "textures/whiteCheckerPiece.png");
		checker->OnCreate();
		checker->ListComponents();
		//-2.5,2.55,0
		//-0.5,1.9,0
		i++;
		std::cout << i << std::endl;
		if (int(i) % 4 == 0 && i != 0) {
			j++;
			if (int(j) % 2 != 0 && i != 0) {
				i = 0;
			}
			else { i = 0.5f; }
		}
	}



	for (int i = 0; i < 12; i++) {
		blackCheckers.push_back(new Actor(board));
	}
	 i = 0;
	 j = 5;
	for (auto checker : blackCheckers) {
		if (!checker) { return false; }
		checker->AddComponent<MeshComponent>(nullptr, "meshes/CheckerPiece.obj");
		checker->AddComponent<ShaderComponent>(nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
		checker->AddComponent<TransformComponent>(btc, Quaternion(), btc->getPosition() + Vec3(-4.3, 4.3, 0) + Vec3(i * 2.48, j * -1.2, 0), Vec3(0.1f, 0.1f, 0.1f));
		checker->AddComponent<MaterialComponent>(nullptr, "textures/blackCheckerPiece.png");
		checker->OnCreate();
		checker->ListComponents();
		i++;
		if (int(i) % 4 == 0 && i != 0) {
			j++;
			if (int(j) % 2 != 0 && i != 0) {
				i = 0;
			}
			else { i = 0.5f; }
		}
	}
	//create 2 vectors one for red checker and other for black
	// each vector will have 12 pieces in total ,  1.2 is the difference between two consecutive black squares
	// u will need to use double for loop one to check the lines and other to check the columns, 
	// each line will have 4 pieces 
	//once u get one checkered piece just make the top left one a parent and all of them to follow it through a vector. 
	return true;
}

void Scene1::OnDestroy() {
	Debug::Info("Deleting assets Scene1: ", __FILE__, __LINE__);
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
	static float angle_1;
	static float angle_2;
	angle_1 += -45.0f * deltaTime;
	angle_2 += 60.0f * deltaTime;
	Quaternion q = board->GetComponent<TransformComponent>()->getOrientation();
	Quaternion rotate_1 = QMath::angleAxisRotation(angle_1, Vec3(1.0f, 0.0f, 1.0f));
	Quaternion rotate_2 = QMath::angleAxisRotation(angle_2, Vec3(0.0f, 1.0f, 1.0f));

	// Adding slerp and making the movement smoother
	float slerpValue = 0.5f;
	Quaternion slerp = QMath::slerp(rotate_1, rotate_2, slerpValue);
	board->GetComponent<TransformComponent>()->SetOrientation(slerp);
}

void Scene1::Render() const {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	/// Set the background color then clear the screen
	glClearColor(0.19f,	0.56f,	0.53f, 0.0f);
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
	glBindTexture(GL_TEXTURE_2D, 0);

	for (auto checker : whiteCheckers) {
		glUniformMatrix4fv(checker->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE,  tc->getModelmatrix()* checker->GetComponent<TransformComponent>()->getModelmatrix());
		glBindTexture(GL_TEXTURE_2D, checker->GetComponent<MaterialComponent>()->getMaterialID());
		checker->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	for (auto checker : blackCheckers) {
		glUniformMatrix4fv(checker->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, tc->getModelmatrix() * checker->GetComponent<TransformComponent>()->getModelmatrix());
		glBindTexture(GL_TEXTURE_2D, checker->GetComponent<MaterialComponent>()->getMaterialID());
		checker->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
		glUseProgram(0);
}




