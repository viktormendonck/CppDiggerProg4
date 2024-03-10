#include "TrashTheCache.h"
#include <chrono>
#include <algorithm>
#include <iostream>
#include <vector>


dae::TrashTheCache::TrashTheCache(GameObject* pParent)
	: Component(pParent)
{
	for (size_t i{};i<m_PlotConfigs.size();i++)
	{
		m_PlotConfigs[i].scale.min = 0;
		m_PlotConfigs[i].tooltip.show = true;
		m_PlotConfigs[i].tooltip.format = "Step: %.0f, Time: %.0f ms";
		m_PlotConfigs[i].grid_x.show = true;
		m_PlotConfigs[i].grid_y.show = true;
		m_PlotConfigs[i].frame_size = ImVec2(150, 100);
		m_PlotConfigs[i].line_thickness = 2.f;
	}
}

void dae::TrashTheCache::ImGuiUpdate()
{

	ImGui::Begin("TrashTheCache");
	ImGui::InputInt("Samples", &m_Samples, 1);
	

	if (ImGui::SmallButton("Trash the cache with Ints"))
	{
		m_Times[0].clear();
		m_Times[0] = GetIntTimes();
		m_ShowPlot[0] = true;
		m_PlotConfigs[0].scale.max = *std::ranges::max_element(m_Times[0]);
		m_PlotConfigs[0].values.xs = m_StepSizes.data();
		m_PlotConfigs[0].values.ys = m_Times[0].data();
		m_PlotConfigs[0].values.count = static_cast<int>(m_Times[0].size());
		m_PlotConfigs[0].values.color = m_Colors[0];
		m_UpdateCombined = true;
	}
	if (ImGui::SmallButton("Trash the cache with GameObject3D"))
	{
		m_Times[1].clear();
		m_Times[1] = GetGOTimes();
		m_ShowPlot[1] = true;
		m_PlotConfigs[1].scale.max = *std::ranges::max_element(m_Times[1]);
		m_PlotConfigs[1].values.xs = m_StepSizes.data();
		m_PlotConfigs[1].values.ys = m_Times[1].data();
		m_PlotConfigs[1].values.count = static_cast<int>(m_Times[1].size());
		m_PlotConfigs[1].values.color = m_Colors[1];
		m_UpdateCombined = true;
	}
	if (ImGui::SmallButton("Trash the cache with GameObject3DAlt"))
	{
		m_Times[2].clear();
		m_Times[2] = GetGOAltTimes();
		m_ShowPlot[2] = true;
		m_PlotConfigs[2].scale.max = *std::ranges::max_element(m_Times[2]);
		m_PlotConfigs[2].values.xs = m_StepSizes.data();
		m_PlotConfigs[2].values.ys = m_Times[2].data();
		m_PlotConfigs[2].values.count = static_cast<int>(m_Times[2].size());
		m_PlotConfigs[2].values.color = m_Colors[2];
		m_UpdateCombined = true;
	}

	int amountOfGraphs{};
	for (size_t i{};i<m_ShowPlot.size();i++)  
	{
		if (m_ShowPlot[i])
		{
			ImGui::Text(m_Labels[i].c_str());
			++amountOfGraphs;
			ImGui::Plot("Plot", m_PlotConfigs[i]);
		}
		if (amountOfGraphs >1 && (!m_ShowPlot[3]|| m_UpdateCombined))
		{
			m_ShowPlot[3] = true;
			m_UpdateCombined = false;

			std::vector<float> tempMax{ m_PlotConfigs[0].scale.max,m_PlotConfigs[1].scale.max,m_PlotConfigs[2].scale.max };
			m_PlotConfigs[3].scale.max = *std::ranges::max_element(tempMax);
			m_PlotConfigs[3].values.xs = m_StepSizes.data();
			//convert the data into a const float* array so the ImGui_plot code can use it
			static const float* yData[]{ m_Times[0].data(), m_Times[1].data(), m_Times[2].data()};
			//update the data 
			yData[0] = m_Times[0].data();
			yData[1] = m_Times[1].data();
			yData[2] = m_Times[2].data();
			m_PlotConfigs[3].values.ys_list = yData;
			m_PlotConfigs[3].values.ys_count = 3;
			std::vector<int> tempCount{ static_cast<int>(m_Times[0].size()),static_cast<int>(m_Times[0].size()),static_cast<int>(m_Times[0].size()) };
			m_PlotConfigs[3].values.count = *std::ranges::max_element(tempCount);
			m_PlotConfigs[3].values.colors = m_Colors.data();
			ImGui::Text(m_Labels[3].c_str());
			ImGui::Plot("Plot", m_PlotConfigs[3]);
		}
	}
	ImGui::End();
}


std::vector<float> dae::TrashTheCache::GetGOAltTimes()
{
	std::vector<GameObject3DAlt> values(m_ObjectsAmount);
	std::vector<float> times;

	for (int stepSize = 1; stepSize <= 1024; stepSize *= 2)
	{
		std::vector<float> temp; //store in temp to remove outliers
		for (int i = 0; i < m_Samples; ++i)
		{
			auto start = std::chrono::high_resolution_clock::now();
			for (size_t j = 0; j < m_ObjectsAmount; j += stepSize)
			{
				values[j].ID += 1;
			}
			auto end = std::chrono::high_resolution_clock::now();
			float timeInMs = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
			temp.push_back(timeInMs);
		}
		std::ranges::sort(temp);

		float sum{};
		for (size_t i{ 1 }; i < temp.size() - 1; ++i)
		{
			sum += temp[i];
		}
		times.emplace_back(sum / (temp.size()-2));
	}

	return times;
}

std::vector<float> dae::TrashTheCache::GetGOTimes()
{
	std::vector<GameObject3D> values(m_ObjectsAmount);
	std::vector<float> times;

	for (int stepSize = 1; stepSize <= 1024; stepSize *= 2)
	{
		std::vector<float> temp; //store in temp to remove outliers
		for (int i = 0; i < m_Samples; ++i)
		{
			auto start = std::chrono::high_resolution_clock::now();
			for (size_t j = 0; j < m_ObjectsAmount; j += stepSize)
			{
				values[j].ID += 1;
			}
			auto end = std::chrono::high_resolution_clock::now();
			float timeInMs = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
			temp.push_back(timeInMs);
		}
		std::ranges::sort(temp);

		float sum{};
		for (size_t i{ 1 }; i < temp.size() - 1; ++i)
		{
			sum += temp[i];
		}
		times.emplace_back(sum / (temp.size() - 2));
	}

	return times;
}

std::vector<float> dae::TrashTheCache::GetIntTimes()
{
	std::vector<int> values(m_ObjectsAmount);
	std::vector<float> times;

	for (int stepSize = 1; stepSize <= 1024; stepSize *= 2)
	{
		std::vector<float> temp; //store in temp to remove outliers
		for (int i = 0; i < m_Samples; ++i)
		{
			auto start = std::chrono::high_resolution_clock::now();
			for (size_t j = 0; j < m_ObjectsAmount; j += stepSize)
			{
				values[j] += 1;
			}
			auto end = std::chrono::high_resolution_clock::now();
			float timeInMs = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
			temp.push_back(timeInMs);
		}
		std::ranges::sort(temp);

		float sum{};
		for (size_t i{ 1 }; i < temp.size() - 1; ++i)
		{
			sum += temp[i];
		}
		times.emplace_back(sum / (temp.size() - 2));
	}

	return times;
}
