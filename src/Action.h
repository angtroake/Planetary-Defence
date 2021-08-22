#pragma once

enum ActionType {
	KEY_PRESS,
	KEY_RELEASE
};

struct Action {
	Action() {}
	Action(ActionType type) : type(type) {}

	ActionType type;

	bool operator <(Action a) const{
		return(type < a.type);
	}
};

struct KeyAction : Action{
	KeyAction() {}
	KeyAction(int keyCode, ActionType type) : keyCode(keyCode) {
		this->type = type;
	}
	
	int keyCode;
};