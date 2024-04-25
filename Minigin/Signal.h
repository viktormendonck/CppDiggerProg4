#pragma once
#include <functional>
#include <vector>

namespace dae
{
    template<typename... SignalArgs>
    class Signal final
    {
        typedef std::function<void(SignalArgs...)> FunctionType;
    public:
        Signal() = default;

        ~Signal()
        {
            m_ListenerFunctions.clear();
        }

        Signal(Signal&&) = delete;
        Signal(const Signal&) = delete;
        Signal& operator=(Signal&&) = delete;
        Signal& operator=(const Signal&) = delete;

        size_t AddListener(FunctionType listener)
        {
            m_ListenerFunctions.push_back({ listener, m_NextIdx });
            return m_NextIdx++;
        }
        template<typename ObjectType>
        size_t AddListener(ObjectType* object, void(ObjectType::* memberFunction)(SignalArgs...))
        {
            const auto lambda = [object, memberFunction](SignalArgs... args) { (object->*memberFunction)(args...); };
            m_ListenerFunctions.push_back({ lambda,m_NextIdx });
            return m_NextIdx++;
        }

        void RemoveListener(size_t id)
        {
            std::erase_if(m_ListenerFunctions, [&](auto&& element) {return element.second == id; });
        }

        void Emit(SignalArgs... args)
        {
            for (const auto& listenerFunction : m_ListenerFunctions)
            {
                listenerFunction.first(args...);
            }
        }
    private:
        size_t m_NextIdx{};
    	std::vector<std::pair<FunctionType, size_t>> m_ListenerFunctions{};
    };
}