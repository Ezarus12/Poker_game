#pragma once

//Entity Component System

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;

using ComponentID = std::size_t;

inline ComponentID getComponentID() {
	static ComponentID lastID = 0;
	return lastID++;
}

template <typename T> inline ComponentID GetComponentTypeID() noexcept
{
	static ComponentID typeID = getComponentID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>; 

class Component {
public:
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}
};

class Entity {
private:
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;

public:
	void update() {
		for (auto& c : components) c->update();
	}
	void draw() {
		for (auto& c : components) c->draw();
	}
	bool isActive() { return active; }
	void destroy() { active = false; }

	template <typename T> bool hasComponent() const
	{
		return componentBitSet[getComponentID];  //change
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs) 
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[GetComponentTypeID<T>()] = c;
		componentBitSet[GetComponentTypeID<T>()] = true;

		c->init();
		return *c;

	}

	template<typename T> T& getComponent() const {
		auto ptr(componentArray[GetComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

};

//Manager class keeping the list of all of the entities

class Manager {
private:
	std::vector<std::unique_ptr<Entity>> entities;

public:
	void update()
	{
		for (auto& e : entities) {
			e->update();
		}
	}
	void draw()
	{
		for (auto& e : entities) {
			e->draw();
		}
	}
	void refresh()
	{
		entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<Entity>& mEntity)
			{
				return !mEntity->isActive();
			}),
			std::end(entities));
	}

	Entity& addEntity() {
		Entity* e = new Entity();
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));

		return *e;
	}

};