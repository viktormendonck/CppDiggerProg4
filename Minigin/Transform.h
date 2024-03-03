#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Component.h"


namespace dae
{
	class Transform final
	{
	public:
	
		Transform() = default;

		void Translate(const glm::vec2& delta);
		void SetPosition(const glm::vec2& position);
		
		void Rotate(const float delta);
		void SetRotation(const float rotation);
		
		void SetScale(const glm::vec2& scale);


		const glm::vec2 GetPosition();
		const float GetRotation();
		const glm::vec2 GetScale();

		glm::mat4& GetTransformationMatrix();

	private:
		void UpdateTransformationMatrix();
		glm::mat4 m_TransformationMatrix{};

		glm::vec2 m_Position{};
		float m_Rotation{};
		glm::vec2 m_Scale{ 1,1};
		bool m_IsDirty = true;
	};
}
