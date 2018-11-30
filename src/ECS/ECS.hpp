// What the-
// https://www.youtube.com/watch?v=XsvI8Sng6dk
// Entity Manager, I guess it just makes C++ act more like Unity

#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;

// Keep track of every single component we create
using ComponentID = std::size_t;

inline ComponentID GetComponentTypeID() {
	static ComponentID lastID = 0;
	return lastID++;
}

template <typename T> inline ComponentID GetComponentTypeID() noexcept
{
	static_assert (std::is_base_of<Component, T>::value, "");
	static ComponentID typeID = GetComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32; // Max amount of components an entity is able to hold

using ComponentBitSet = std::bitset<maxComponents>;				// If we need to find out if an entity has a selection of components, we need to be able to compare whether it has it or not.
using ComponentArray = std::array<Component*, maxComponents>;	// Array of the length of the max components

class Component {
private:
public:
	Entity* entity;

	virtual void Create() {};
	virtual void Step() {};
	virtual void Draw() {};

	virtual ~Component() {};
};

class Entity {
private:
	bool active = true; // Remove from the game if this is false
	std::vector<std::unique_ptr<Component>> components; // Array, but with dynamic size

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
public:
	void Step() {
		for(auto& c : components)
			c->Step();
	}
	void Draw() {
		for(auto& c : components)
			c->Draw();
	}
	bool IsActive() const {
		return active;
	}
	void Destroy() {
		active = false;
	}

	template <typename T> bool HasComponent() const {
		return componentBitSet[GetComponentTypeID<T>]; // Check if this object has a particular component attached to it
	}

	template <typename T, typename... TArgs>
	T& AddComponent(TArgs&&... mArgs) {
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[GetComponentTypeID<T>()] = c; // Ensures that a modified (added?) component is put in the same position it originally was in.
		componentBitSet[GetComponentTypeID<T>()] = true;

		c->Create();
		return *c;
	}

	template <typename T> T& GetComponent() const {
		auto ptr(componentArray[GetComponentTypeID<T>()]); // Set pointer to our position in the array
		return *static_cast<T*>(ptr);
	}

	// Because of this template, we can do cool stuff like:
	// gameobject.GetComponent<PositionComponent>().setXpos(25);
};

class Manager {
private:
	std::vector<std::unique_ptr<Entity>> entities; // A vector list of all our entities
public:
	void Step() {
		for (auto& e : entities) e->Step();
	}
	void Draw() {
		for (auto& e : entities) e->Draw();
	}
	
	// Clean up all the inactive entities
	void Refresh() {
		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity> &mEntity) {
				return !mEntity->IsActive();
			}),
		std::end(entities));
	}

	// Add the entity to the world using the manager class
	Entity& AddEntity() {
		Entity* e = new Entity();
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};