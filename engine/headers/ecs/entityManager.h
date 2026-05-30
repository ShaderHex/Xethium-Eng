#pragma once
#include <iostream>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <any>
#include <algorithm>

using Entity = uint32_t;

namespace XENGINE {

class EntityManager {
public:
    Entity CreateEntity() {
        if (!m_freeEntities.empty()) {
            Entity id = m_freeEntities.back();
            m_freeEntities.pop_back();
            m_allEntities.push_back(id);
            return id;
        }
        Entity id = m_nextEntity++;
        m_allEntities.push_back(id);
        return id;
    }

    void DestroyEntity(Entity e) {
        for (auto& [typeIdx, storage] : m_components) {
            auto& map = std::any_cast<std::unordered_map<Entity, std::any>&>(storage);
            map.erase(e);
        }
        m_allEntities.erase(std::remove(m_allEntities.begin(), m_allEntities.end(), e), m_allEntities.end());
        m_freeEntities.push_back(e);
    }

    template <typename T>
    void AddComponent(Entity e, const T& component) {
        auto& storage = getStorage<T>();
        storage[e] = component;
        std::cout << "Added a component to Entity ID: " << e << "\n";
    }

    template <typename T>
    T& GetComponent(Entity e) {
        return getStorage<T>().at(e);
    }

    template <typename T>
    bool HasComponent(Entity e) {
        auto it = m_components.find(typeid(T));
        if (it == m_components.end()) return false;
        auto& storage = std::any_cast<std::unordered_map<Entity, T>&>(it->second);
        return storage.find(e) != storage.end();
    }

    template <typename T>
    void RemoveComponent(Entity e) {
        auto it = m_components.find(typeid(T));
        if (it != m_components.end()) {
            auto& storage = std::any_cast<std::unordered_map<Entity, T>&>(it->second);
            storage.erase(e);
        }
    }

    std::vector<Entity>& GetAllEntity() { return m_allEntities; }
    Entity GetEntityCount() const { return m_nextEntity; }

private:
    template <typename T>
    std::unordered_map<Entity, T>& getStorage() {
        auto it = m_components.find(typeid(T));
        if (it == m_components.end()) {
            m_components[typeid(T)] = std::unordered_map<Entity, T>{};
        }
        return std::any_cast<std::unordered_map<Entity, T>&>(m_components[typeid(T)]);
    }

    std::unordered_map<std::type_index, std::any> m_components;
    std::vector<Entity> m_freeEntities;
    std::vector<Entity> m_allEntities;
    Entity m_nextEntity = 0;
};

}