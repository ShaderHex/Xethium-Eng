#pragma once
#include <iostream>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <algorithm>

using Entity = uint32_t;
using Component = uint32_t;

namespace XENGINE {

class EntityManager {
    public:
    Entity CreateEntity() {
        if(!m_freeEntities.empty()) {
            Entity freeEntityID = m_freeEntities.back();
            m_allEntities.push_back(freeEntityID);
            m_freeEntities.pop_back();
            std::cout<< "Reusing ID: " << freeEntityID << "\n";
            return freeEntityID;
        }
        Entity newID = m_entity++;
        m_allEntities.push_back(newID);
        std::cout<< "No new ID found! Creating new ID: " << newID << "\n";
        return newID;
    }

    void DestroyEntity(Entity& e) {
        std::cout<< "Destroying ID: " << e << "\n";
        
        for (auto& type : m_entityComponentTracker[e]) {
            m_typeDeleters[type](e);
        }

        m_entityComponentTracker.erase(e);
        m_freeEntities.push_back(e);
    }

    std::vector<Entity> GetAllEntity() {
        return m_allEntities;
    }

    template <typename T>
    void RemoveComponent(Entity& e) {
        if (HasComponent<T>(e)) {
            GetStorage<T>().erase(e);
            m_entityComponentTracker[e].erase(std::remove(m_entityComponentTracker[e].begin(), m_entityComponentTracker[e].end(), typeid(T)) ,m_entityComponentTracker[e].end());
        } else
            std::cout << "Entity: " << e << " does not have the component you just tried to remove\n" << std::endl;
    }

    Entity GetEntityCount() {
        return m_entity;
    }

    template <typename T>
    void AddComponent(Entity e, const T& component) {
        GetStorage<T>()[e] = component;
        m_entityComponentTracker[e].push_back(typeid(T));
        m_typeDeleters[typeid(T)] = [](Entity id) {
            GetStorage<T>().erase(id);
        };
    }

    template <typename T>
    T& GetComponent(Entity e) {
        return GetStorage<T>().at(e);
    }

    template <typename T>
    bool HasComponent(Entity e) {
        return GetStorage<T>().find(e) != GetStorage<T>().end();
    }

    template <typename T>
    static std::unordered_map<Entity, T>& GetStorage() {
        static std::unordered_map<Entity, T> storage;
        return storage;
    }
    // template<typename T1, typename T2> T2 AddComponent(T1, T2)

private:
    std::vector<Entity> m_freeEntities;
    std::unordered_map<Entity, std::vector<std::type_index>> m_entityComponentTracker;
    std::unordered_map<std::type_index, void(*)(Entity)> m_typeDeleters;
    std::vector<Entity> m_allEntities;

    Entity m_entity = 0;
};

}