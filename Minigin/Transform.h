#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Component.h"


namespace dae
{
	class Transform final
	{
	public:
	
		Transform(GameObject* GameObject);

		void Translate(const glm::vec2& delta);
		void Rotate(const float delta);
		
		void SetLocalPosition(const glm::vec2& position);
		void SetLocalRotation(const float rotation);
		void SetLocalScale(const glm::vec2& scale);

		const glm::vec2 GetLocalPosition() const { return m_LocalPosition; }
		const float GetLocalRotation() const { return m_LocalRotation; }
		const glm::vec2 GetLocalScale() const { return m_LocalScale; }
		
		const glm::vec2 GetWorldPosition();
		const float GetWorldRotation();
		const glm::vec2 GetWorldScale();
		
		glm::mat4& GetTransformationMatrix();
	private:
		void UpdateTransformationMatrix();
		void DecomposeMatrix();
		void SetDirty();

		GameObject* m_pGameObject{};

		glm::mat4 m_TransformationMatrix{};

		glm::vec2 m_WorldPosition{};
		float m_WorldRotation{};
		glm::vec2 m_WorldScale{ 1,1 };

		glm::vec2 m_LocalPosition{};
		float m_LocalRotation{};
		glm::vec2 m_LocalScale{ 1,1 };

		bool m_IsDirty = true;
	};
}
