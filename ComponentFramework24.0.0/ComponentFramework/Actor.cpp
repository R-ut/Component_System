#include "Actor.h"

Actor::Actor(Ref<Component> parent_) :Component(parent_) {}

Actor::~Actor() {}

bool Actor::OnCreate() {
	for (Ref<Component> component : components) {
		bool status = component->OnCreate();
		if (status == false) {
			return false;
		}

	}
	return true;
}

void Actor::OnDestroy() {
	RemoveAllComponents();
}

void Actor::Update(const float deltaTime) {
}

void Actor::Render() const {
}

Matrix4 Actor::GetModelMatrix() const
{
	Matrix4 modelMatrix;
	Ref<TransformComponent> tc = GetComponent<TransformComponent>();
	if (tc.get() != nullptr) {
		modelMatrix = tc->getModelmatrix();
		return modelMatrix;
	}
	else {
		modelMatrix.loadIdentity();
	}
	if (parent) {
		modelMatrix = std::dynamic_pointer_cast<Actor>(parent)->GetModelMatrix() * modelMatrix;
	}
	return modelMatrix;
}

void Actor::ListComponents() const {
	/// typeid is a RTTI operator
	std::cout << typeid(*this).name() << " contain the following components:\n";
	for (Ref<Component> component : components) {
		std::cout << typeid(*component).name() << std::endl;
	}
}


void Actor::RemoveAllComponents() {
	components.clear();
}

