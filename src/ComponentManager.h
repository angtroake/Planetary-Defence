#pragma once
class ComponentManager {
private:

public:
	template<typename T>
	void registerComponent() 
	{
		const char typeName = typeid(T).name;

	}
};