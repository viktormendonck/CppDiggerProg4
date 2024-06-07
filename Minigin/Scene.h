#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(const std::shared_ptr<GameObject>& object);
		void RemoveAll();
		void HandleDestruction();

		void FixedUpdate();
		void Update();
		void Init();
		void LateUpdate();
		void Render() const;
		void ImGuiUpdate();

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;
		void Destroy() { m_Destroyed = true; }
		bool IsDestroyed() const { return m_Destroyed; }

		const std::string& GetName() const { return m_Name; }
	private: 
		explicit Scene(const std::string& name);
		void HandleObjectDestruction(const ::std::shared_ptr<GameObject>& object);

		std::string m_Name;
		std::vector < std::shared_ptr<GameObject>> m_Objects{};

		static unsigned int m_IdCounter;
		bool m_Destroyed{};
	};

}
