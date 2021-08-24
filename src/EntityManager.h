#pragma once
#include <queue>
#include <bitset>
#include <assert.h>
#include "Component.h"



//Max amount of entities allowed in a scene.
const Entity MAX_ENTITIES = 1000;


//Tuple containing all possible components
//Components must be added to this tuple to be usable with entities.
typedef std::tuple<
	Component::Transform[MAX_ENTITIES],
	Component::Material[MAX_ENTITIES],
	Component::Health[MAX_ENTITIES],
	Component::Orbit[MAX_ENTITIES],
	Component::CAnimation[MAX_ENTITIES],
	Component::BoundingBox[MAX_ENTITIES]
> ComponentArrays;



// Entity Manager manages all entity creation and removal.
// Entities consist of components. There is space preallocated for a component for every entity.
// Therefore, each entity has every component. However, components must be enabled 
// for each entity. An entity is a unsigned integer
//
class EntityManager
{
private:
	//Queue of IDs availible for new entities
	std::queue<Entity> _availableIDs;
	std::vector<Entity> _entities;
	std::string _entityTags[MAX_ENTITIES];

	std::vector<Entity> _entitiesToDestroy;
	
	ComponentArrays _components;


	

public:

	//Fills queue with all IDs
	EntityManager() {
		for (Entity e = 0; e < MAX_ENTITIES; e++) 
		{
			_availableIDs.push(e);
		}
	}

	//Marks an entity to be destroyed
	void destroyEntity(const Entity entity) 
	{
		assert(entity < MAX_ENTITIES);
		_entitiesToDestroy.push_back(entity);
	}

	// Creates a new entity from next ID in queue
	// Entity tag is string used to identify entity.
	Entity createEntity(const std::string tag) 
	{
		assert(_availableIDs.size() > 0);

		Entity entity = _availableIDs.front();
		_availableIDs.pop();

		_entityTags[entity] = tag;
		_entities.push_back(entity);
		return entity;
	}

	//Adds a component to an entity.
	//All components are found in Component.h
	template <typename T, typename... TArgs>
	T& addComponent(Entity entity, TArgs&&... args) 
	{
		assert(entity < MAX_ENTITIES);
		auto& component = getComponent<T>(entity);
		component = T(std::forward<TArgs>(args)...);
		component.has = true;
		return component;
	}

	//Remove component from an entity
	template <typename T>
	T& removeComponent(Entity entity) 
	{
		assert(entity < MAX_ENTITIES);
		auto& component = getComponent<T>(entity);
		component.has = false;
		return component;
	}

	//True if entity has a component.
	template <typename T>
	const bool hasComponent(Entity entity) const 
	{
		assert(entity < MAX_ENTITIES);
		return std::get<T[MAX_ENTITIES]>(_components)[entity].has;
	}

	//Returns the instance of the entities component

	template <typename T>
	const T& getComponent(Entity entity) const
	{
		assert(entity < MAX_ENTITIES);
		return std::get<T[MAX_ENTITIES]>(_components)[entity];
	}

	template <typename T>
	T& getComponent(Entity entity) {
		assert(entity < MAX_ENTITIES);
		return std::get<T[MAX_ENTITIES]>(_components)[entity];
	}

	//General tick for manager
	//Removes destroyed entities
	void update() 
	{
		for (Entity e : _entitiesToDestroy) 
		{
			auto iter = std::find(_entities.begin(), _entities.end(), e);
			if (iter != _entities.end()) 
			{
				_entities.erase(iter);
			}
			//Set all components of that ID to not have.
			disableComponents(e, _components);
		}

		_entitiesToDestroy.clear();
	}

	//Helper functions for disabling components of destroyed entity ID.
	template <size_t I = 0, typename... Ts>
	typename std::enable_if<I == sizeof ...(Ts), void>::type
	disableComponents(Entity e, std::tuple<Ts...>& tup)
	{
		return;
	}

	template <size_t I = 0, typename... Ts>
	typename std::enable_if<I < sizeof ...(Ts), void>::type
	disableComponents(Entity e, std::tuple<Ts...>& tup)
	{
		std::get<I>(tup)[e].has = false;
		disableComponents<I + 1>(e, tup);
	}

	//returns amount of active entities
	Entity getEntityCount() 
	{
		return _entities.size();
	}

	//Gets all entities with a certain tag
	std::vector<Entity> getEntities(const std::string& tag) 
	{
		std::vector<Entity> returnEntities;
		for (auto& iter = _entities.begin(); iter != _entities.end(); iter++) 
		{
			if (_entityTags[*iter] == tag) {
				returnEntities.push_back(*iter);
			}
		}
		return returnEntities;
	}

	//Gets all entities
	std::vector<Entity> getEntities() 
	{
		return _entities;
	}

	const bool isAlive(const Entity & entity)
	{
		assert(entity < MAX_ENTITIES);
		return (std::find(_entities.begin(), _entities.end(), entity) != _entities.end());
	}

	const std::string& getTag(const Entity& entity) const 
	{
		assert(entity < MAX_ENTITIES);
		return _entityTags[entity];
	}

};

