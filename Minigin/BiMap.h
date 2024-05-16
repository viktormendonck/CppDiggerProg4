#pragma once
#include <map>

template <typename KeyType, typename ValueType>
class BiMap {
public:
    BiMap(std::map<KeyType, ValueType> initMap)
    {

    	for (auto& [key, value] : initMap)
		{
            Insert(key, value);
		}
    }
    void Insert(const KeyType& key, const ValueType& value) {
        m_ForwardMap[key] = value;
        m_ReverseMap[value] = key;
    }

    const ValueType GetValue(const KeyType& key, const ValueType defaultValue) const {
        auto it = m_ForwardMap.find(key);
        if (it != m_ForwardMap.end())
			return it->second;
        return defaultValue;
    }

    const KeyType GetValue(const ValueType& value, const KeyType defaultKey) const {
        auto it = m_ReverseMap.find(value);
        if (it != m_ReverseMap.end())
           	return it->second;
        return defaultKey;
    }

private:
    std::map<KeyType, ValueType> m_ForwardMap{};
    std::map<ValueType, KeyType> m_ReverseMap{};
};
