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

		void Translate(const glm::vec3& delta);
		void SetPosition(const glm::vec3& position);
		
		void Rotate(const glm::vec3& delta);
		void SetRotation(const glm::vec3& rotation);
		
		void SetScale(const glm::vec3& scale);


		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetRotation() const { return m_Rotation; }
		const glm::vec3& GetScale() const { return m_Scale; }

		glm::mat4& GetTransformationMatrix();

	private:
		void UpdateTransformationMatrix();
		glm::mat4 m_TransformationMatrix;

		glm::vec3 m_Position{};
		glm::vec3 m_Rotation{};
		glm::vec3 m_Scale{ 1,1,1 };
		bool m_IsDirty = true;
	};
}
