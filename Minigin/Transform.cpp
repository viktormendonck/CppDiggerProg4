#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(disable: 4201)
#include <glm/gtx/quaternion.hpp>



void dae::Transform::Translate(const glm::vec2& delta)
{
	m_Position += delta;
	m_IsDirty = true;
}

void dae::Transform::SetPosition(const glm::vec2& position)
{
	m_Position = position;
	m_IsDirty = true;
}

void dae::Transform::Rotate(const float delta)
{
	m_Rotation += delta;
	m_IsDirty = true;
}

void dae::Transform::SetRotation(const float rotation)
{
	m_Rotation = rotation;
	m_IsDirty = true;
}

void dae::Transform::SetScale(const glm::vec2& scale)
{
	m_Scale = scale;
	m_IsDirty = true;
}

const glm::vec2 dae::Transform::GetPosition()
{
	glm::vec3 position = GetTransformationMatrix()[3];
	return { position.x,position.y};
}

const float dae::Transform::GetRotation()
{
	// Extract the rotation part of the matrix
	return glm::eulerAngles(glm::quat_cast(GetTransformationMatrix())).z;
}

const glm::vec2 dae::Transform::GetScale()
{
	GetTransformationMatrix();
	return { glm::length(m_TransformationMatrix[0]),glm::length(m_TransformationMatrix[1]) };
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

	// Apply transformations
	transformationMatrix = glm::translate(transformationMatrix, {m_Position, 1});

	// Apply rotations around the x, y, and z axes
	transformationMatrix = rotate(transformationMatrix, m_Rotation, glm::vec3(0.f, 0.f, 1.f));

	// Apply scaling
	transformationMatrix = glm::scale(transformationMatrix, { m_Scale, 1});
	m_TransformationMatrix = transformationMatrix;
}
