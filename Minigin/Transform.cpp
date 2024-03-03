#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(disable: 4201)
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include "GameObject.h"



dae::Transform::Transform(GameObject* GameObject)
	:m_pGameObject(GameObject)
{
}

void dae::Transform::Translate(const glm::vec2& delta)
{
	m_LocalPosition += delta;
	SetDirty();
}
void dae::Transform::Rotate(const float delta)
{
	m_LocalRotation += delta;
	SetDirty();
}

void dae::Transform::SetLocalPosition(const glm::vec2& position)
{
	m_LocalPosition = position;
	SetDirty();
}
void dae::Transform::SetLocalRotation(const float rotation)
{
	m_LocalRotation = rotation;
	SetDirty();
}
void dae::Transform::SetLocalScale(const glm::vec2& scale)
{
	m_LocalScale = scale;
	SetDirty();
}


const glm::vec2 dae::Transform::GetWorldPosition()
{
	if (m_IsDirty)
		UpdateTransformationMatrix();
	return m_WorldPosition;
}
const float dae::Transform::GetWorldRotation()
{
	if (m_IsDirty)
		UpdateTransformationMatrix();
	return m_WorldRotation;
}
const glm::vec2 dae::Transform::GetWorldScale()
{
	if (m_IsDirty)
		UpdateTransformationMatrix();
	return m_WorldScale;
}

glm::mat4& dae::Transform::GetTransformationMatrix()
{
	if (m_IsDirty)
		UpdateTransformationMatrix();
	return m_TransformationMatrix;
}

void dae::Transform::UpdateTransformationMatrix()
{
	// Reset the dirty flag
	m_IsDirty = false;

	glm::mat4 transformationMatrix = glm::mat4(1.0f); // Start with the identity matrix
	transformationMatrix = glm::translate(transformationMatrix, {m_LocalPosition, 1});
	transformationMatrix = rotate(transformationMatrix, m_LocalRotation, glm::vec3(0.f, 0.f, 1.f));
	transformationMatrix = glm::scale(transformationMatrix, { m_LocalScale, 1});
	if (m_pGameObject->GetParent() == nullptr)
	{
		m_TransformationMatrix = transformationMatrix;
	}
	else
	{
		Transform& parentTransform{ m_pGameObject->GetParent()->GetTransform() };
		m_TransformationMatrix = parentTransform.GetTransformationMatrix() * transformationMatrix;

	}
	DecomposeMatrix();
}

void dae::Transform::DecomposeMatrix()
{
	// Extract position, rotation, and scale from the transformation matrix
	m_WorldPosition = m_TransformationMatrix[3];
	
	m_WorldRotation = glm::eulerAngles(glm::quat_cast(m_TransformationMatrix)).z;
	m_WorldScale = { glm::length(m_TransformationMatrix[0]),glm::length(m_TransformationMatrix[1]) };
}

void dae::Transform::SetDirty()
{
	m_IsDirty = true;

	for (auto& child : m_pGameObject->GetChildren())
	{
		child->GetTransform().SetDirty();
	}
}