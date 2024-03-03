#pragma once
#include "Component.h"
namespace dae
{
	class RotationComponent final: public Component
	{
	public:
		RotationComponent(GameObject* pParent, float rotationSpeed);
		~RotationComponent()  = default;
		void Update() override;


		RotationComponent(const RotationComponent& other) = delete;
		RotationComponent(RotationComponent&& other) noexcept = delete;
		RotationComponent& operator=(const RotationComponent& other) = delete;
		RotationComponent& operator=(RotationComponent&& other) noexcept = delete;
	private:
		float m_RotationSpeed{};
	};
}

