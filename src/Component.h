#pragma once
#include "base.h"

const int MAX_COMPONENTS = 32;

 
namespace Component {
	class Component{
	public:
		bool has = false;
	};

	class Transform : public Component 
	{
	public:
		Transform() {}
	};
}


