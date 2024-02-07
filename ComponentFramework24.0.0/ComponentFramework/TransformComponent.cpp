#include "TransformComponent.h"
TransformComponent::TransformComponent(Component* parent_ ,Quaternion orientation_, Vec3 position_,
	Vec3 scale_ = Vec3(1.0f,1.0f,1.0f))
	:Component(parent_),orientation(orientation_), position(position_),scale(scale_) {
	/*position = Vec3(0.0f, 0.0f, 0.0f);
	orientation = Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f));*/
}

TransformComponent::~TransformComponent() {}

bool TransformComponent::OnCreate() {
	isCreated = true;
	return true;
}

void TransformComponent::OnDestroy() {}

void TransformComponent::Update(const float deltaTime) {}

void TransformComponent::Render() const {}