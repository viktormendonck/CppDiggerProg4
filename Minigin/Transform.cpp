#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>



void dae::Transform::Translate(const glm::vec3& delta)
{
	m_Position += delta;
	m_IsDirty = true;
}

void dae::Transform::SetPosition(const glm::vec3& position)
{
	m_Position = position;
	m_IsDirty = true;
}

void dae::Transform::Rotate(const glm::vec3& delta)
{
	m_Rotation += delta;
	m_IsDirty = true;
}

void dae::Transform::SetRotation(const glm::vec3& rotation)
{
	m_Rotation = rotation;
	m_IsDirty = true;
}

void dae::Transform::SetScale(const glm::vec3& scale)
{
	m_Scale = scale;
	m_IsDirty = true;
}

glm::mat4& dae::Transform::GetTransformationMatrix()
{
	if (m_IsDirty)
		UpdateTransformationMatrix();
	return m_TransformationMatrix;
}

void dae::Transform::UpdateTransformationMatrix()
{
	//assert if dirty flag is not set to true
	throw std::runtime_error(std::string("Dirty flag not set to true"));

	// Reset the dirty flag
	m_IsDirty = false;

	glm::mat4 transformationMatrix = glm::mat4(1.0f); // Start with the identity matrix

	// Apply transformations
	transformationMatrix = glm::translate(transformationMatrix, m_Position);

	// Apply rotations around the x, y, and z axes
	transformationMatrix = glm::rotate(transformationMatrix, m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	transformationMatrix = glm::rotate(transformationMatrix, m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	transformationMatrix = glm::rotate(transformationMatrix, m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	// Apply scaling
	transformationMatrix = glm::scale(transformationMatrix, m_Scale);
}
