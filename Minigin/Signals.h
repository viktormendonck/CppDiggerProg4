//credits to Leander Flossie, Patrick Nellessen, Viktor Mendonck and Leen Ritserveldt
#pragma once
#include <functional>
#include <set>
#include <vector>
#include <algorithm>


template<typename... Args>
class Signal
{
    typedef std::function<void(Args...)> functionType;
    //typedef std::function<void(Args...)> functionType;
public:
    Signal() = default;
    ~Signal();

    void AddListener(functionType listener) { m_ListenerFunctions.push_back(listener); }
    void RemoveListener(functionType listener)
    {
        std::erase_if(m_ListenerFunctions, [&](const functionType& listenerFunction) {
            return GetAddress(listenerFunction) == GetAddress(listener);
        });
    }
    void Emit(Args... args);
private:
    std::vector<functionType> m_ListenerFunctions{};

    template<typename T, typename... U>
    size_t GetAddress(std::function<T(U...)> func) { //code is evil, but it works
    	typedef T(fnType)(U...);
        fnType** fnPointer = func.template target<fnType*>();
        return reinterpret_cast<size_t>(*fnPointer);
    }
};

template<typename ...Args>
inline Signal<Args...>::~Signal()
{
    m_ListenerFunctions.clear();
}


template<typename ...Args>
inline void Signal<Args...>::Emit(Args ...args)
{
    for (auto& listenerFunction : m_ListenerFunctions)
    {
        listenerFunction(args...);
    }
}