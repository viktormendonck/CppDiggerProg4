#pragma once
#include "Component.h"
#include <imgui_plot.h>
#include <string>
#include <vector>

namespace dae
{
	class TrashTheCache final : public Component
	{
	public:
		explicit TrashTheCache(GameObject* pParent);
		void ImGuiUpdate() override;
	private:
        const std::vector<float> m_StepSizes{ 1,2,4,8,16,32,64,128,256,512,1024 };
        std::vector<std::vector<float>> m_Times{{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0}};

        std::vector<std::string> m_Labels{"Ints","GameObject3D","GameObject3DAlt","Combined"};
        const std::vector<ImU32> m_Colors{0xFF0000FF,0xFF00FF00,0xFFFF0000};

        std::vector<bool> m_ShowPlot{false,false,false,false};

        std::vector <ImGui::PlotConfig> m_PlotConfigs{{},{},{},{}};
        int m_Samples{10};
        const long long m_ObjectsAmount{ 67'108'864 };
        bool m_UpdateCombined{};


		std::vector<float> GetGOAltTimes();
        std::vector<float> GetGOTimes();
        std::vector<float> GetIntTimes();

        struct Transform
        {
            float matrix[16] = {
                1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                0,0,0,1 };
        };
        struct GameObject3D
        {
            Transform transform;
            int ID{};
        };
        struct GameObject3DAlt
        {
            Transform* transform;
            int ID{};
        };
	};
}

