#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Scene0.h"
#include <MMath.h>
#include "Debug.h"
#include "Body.h"
#include <Quaternion.h>
#include <QMath.h>

Scene0::Scene0() :sphere{nullptr}, shader{nullptr}, mesh{nullptr},
drawInWireMode{false} {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene0::~Scene0() {
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);
}

bool Scene0::OnCreate() {
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);
	sphere = new Body();
	sphere->OnCreate();

	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 5.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	mario = std::make_shared<Actor>(nullptr);
	if (!mario) {return false;}
	mario->AddComponent<MeshComponent>(nullptr, "meshes/Mario.obj");
	mario->AddComponent<ShaderComponent>(nullptr,"shaders/textureVert.glsl","shaders/textureFrag.glsl");
	mario->AddComponent<TransformComponent>(nullptr, Quaternion(), Vec3());
	mario->AddComponent<MaterialComponent>(nullptr, "textures/mario_main.png");
	mario->OnCreate();
	mario->ListComponents();
	modelMatrix.loadIdentity();

	hammer = std::make_shared<Actor>(mario);
	if (!hammer) { return false; }
	hammer->AddComponent<MeshComponent>(nullptr, "meshes/Hammer.obj");
	hammer->AddComponent<ShaderComponent>(nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	hammer->AddComponent<TransformComponent>(nullptr, QMath::angleAxisRotation(-90.0f, Vec3(1.0f, 0.0f, 0.0f)), Vec3(1.1f, 0.0f, 0.0f));
	hammer->AddComponent<MaterialComponent>(nullptr, "textures/hammer_BaseColor.png");
	hammer->OnCreate();
	hammer->ListComponents();
    
	return true;
}

void Scene0::OnDestroy() {
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);
	sphere->OnDestroy();
	delete sphere;

	mario->RemoveAllComponents();
	mario->OnDestroy();
}

void Scene0::HandleEvents(const SDL_Event &sdlEvent) {
	switch( sdlEvent.type ) {
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

void Scene0::Update(const float deltaTime) {
	static float angle = 0.0f;
	angle += 20 * deltaTime;
	Quaternion rotation = QMath::angleAxisRotation(angle, Vec3(0.0f, 1.0f, 0.0f));
	mario->GetComponent<TransformComponent>()->SetOrientation(rotation);
}

void Scene0::Render() const {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(drawInWireMode){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	Ref<ShaderComponent> sc = mario->GetComponent<ShaderComponent>();
	Ref<MeshComponent> mc = mario->GetComponent<MeshComponent>();
	Ref<TransformComponent> tc = mario->GetComponent<TransformComponent>();
	Ref<MaterialComponent> matc = mario->GetComponent<MaterialComponent>();
	
	
	glUseProgram(sc->GetProgram());
	glUniformMatrix4fv(sc->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(sc->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sc->GetUniformID("modelMatrix"), 1, GL_FALSE, tc->getModelmatrix());
    glBindTexture(GL_TEXTURE_2D, matc->getMaterialID());
	mc->Render(GL_TRIANGLES);



	glUniformMatrix4fv(sc->GetUniformID("modelMatrix"), 1, GL_FALSE,tc->getModelmatrix() *  hammer->GetComponent<TransformComponent>()->getModelmatrix());
	glBindTexture(GL_TEXTURE_2D, hammer->GetComponent<MaterialComponent>()->getMaterialID());
	hammer->GetComponent<MeshComponent>()->Render();
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}



	
