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
	for (int i = 0; i < 12; i++) {
		redCheckers.push_back(new Actor(board));
	}





	float i = 0.5f;
	float j = 0;
	for (auto checker : redCheckers) {
		if (!checker) { return false; }
		checker->AddComponent<MeshComponent>(nullptr, "meshes/CheckerPiece.obj");
		checker->AddComponent<ShaderComponent>(nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
		checker->AddComponent<TransformComponent>(board, Quaternion(), Vec3(-4.3f + (i * 1.25 *2), 4.3f - (j * 1.25), 0), Vec3(0.1f, 0.1f, 0.1f));
		checker->AddComponent<MaterialComponent>(nullptr, "textures/redCheckerPiece.png");
		checker->OnCreate();
		checker->ListComponents();
		i++;
		if (int(i) % 4 == 0 && i != 0) {
			j++;
			if (int(j) % 2 != 0 && i != 0) {
				i = 0;
			}
			else { i = 0.5f; }
			//-3.1f + (i * 1.25 *2) works for the first row
			// -3.1 , -0.6, 1.9,4.4
			// -4.3, -1.8,0.7,3.2
			//-4.3f + (i * 1.25 *2) works for the second row
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
		checker->AddComponent<TransformComponent>(board, Quaternion(), Vec3(-4.3f + (i * 1.25 * 2), 4.3f - (j * 1.25), 0), Vec3(0.1f, 0.1f, 0.1f));
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
	/*checker = new Actor(board);
	if (!checker) { return false; }
	checker->AddComponent<MeshComponent>(nullptr, "meshes/CheckerPiece.obj");
	checker->AddComponent<ShaderComponent>(nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	checker->AddComponent<TransformComponent>(nullptr, Quaternion(), Vec3(-3.1f,4.3f,0),Vec3(0.1f,0.1f,0.1f));
	checker->AddComponent<MaterialComponent>(nullptr, "textures/redCheckerPiece.png");
	checker->OnCreate();
	checker->ListComponents();*/
	//create 2 vectors one for red checker and other for black
	// each vector will have 12 pieces in total ,  1.2 is the difference between two consecutive black squares
	// u will need to use double for loop one to check the lines and other to check the columns, 
	// each line will have 4 pieces 
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
	static float angle = 0.0f;
	angle += 20 * deltaTime;
	Quaternion rotation = QMath::angleAxisRotation(angle, Vec3(0.0f, 1.0f, 0.0f));
	board->GetComponent<TransformComponent>()->SetOrientation(rotation);
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


	for (auto checker : redCheckers) {
		glUniformMatrix4fv(sc->GetUniformID("modelMatrix"), 1, GL_FALSE, tc->getModelmatrix() * checker->GetComponent<TransformComponent>()->getModelmatrix());
		glBindTexture(GL_TEXTURE_2D, checker->GetComponent<MaterialComponent>()->getMaterialID());
		checker->GetComponent<MeshComponent>()->Render();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	for (auto checker : blackCheckers) {
		glUniformMatrix4fv(sc->GetUniformID("modelMatrix"), 1, GL_FALSE, tc->getModelmatrix() * checker->GetComponent<TransformComponent>()->getModelmatrix());
		glBindTexture(GL_TEXTURE_2D, checker->GetComponent<MaterialComponent>()->getMaterialID());
		checker->GetComponent<MeshComponent>()->Render();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
		glUseProgram(0);
}




