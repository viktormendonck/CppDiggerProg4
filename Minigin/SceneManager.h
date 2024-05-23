#pragma once
#include <vector>
#include <string>
#include <memory>

#include "GameObject.h"
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update();
		void Init();
		void FixedUpdate();
		void LateUpdate();
		void Render();
		void ImGuiUpdate();
		void HandleDestroy();
		bool SetActiveScene(const std::string& name);
		Scene* GetScene(const std::string& name);
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_Scenes;

		int m_ActiveSceneIndex{};
		
	};
}
