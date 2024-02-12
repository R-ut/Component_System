#pragma once
#include "Component.h"
#include <Vector.h>
#include <Quaternion.h>
#include <Matrix.h>
#include <MMath.h>
using namespace MATH;


class TransformComponent : public Component {
	TransformComponent(const TransformComponent&) = delete;
	TransformComponent(TransformComponent&&) = delete;
	TransformComponent& operator=(const TransformComponent&) = delete;
	TransformComponent& operator=(const TransformComponent&&) = delete;
private:
	Quaternion orientation;
	Vec3 position;
	Vec3 scale;
public:
	TransformComponent(Component* parent_, Quaternion orientation_, Vec3 position_, Vec3 scale_ = Vec3(1.0f, 1.0f, 1.0f));
	~TransformComponent();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const;
	Matrix4 getModelmatrix() const {
		return MMath::translate(position) * MMath::scale(scale) * MMath::toMatrix4(orientation);
	}

	Quaternion getOrientation() const { return orientation; }
	void SetOrientation(Quaternion orientation_) { orientation = orientation_; }
};
