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

    const auto& GetValue(const KeyType& key) const {
        return m_ForwardMap.find(key);
    }

    const auto& GetValue(const ValueType& value) const {
        return m_ReverseMap.find(value);
    }

    typename std::map<KeyType, ValueType>::iterator ForwardEnd() const {
		return  m_ForwardMap.end();
	}
    
    typename std::map<ValueType, KeyType>::iterator ReverseEnd() const {
        return m_ReverseMap.end();
    }
private:
    std::map<KeyType, ValueType> m_ForwardMap{};
    std::map<ValueType, KeyType> m_ReverseMap{};
};
