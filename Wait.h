#pragma once
#include <iostream>

inline bool Wait(float deltaTime, float seconds) {
	static float time = 0;
	time += deltaTime;
	if (time >= seconds) {
		time = 0;
		return true;
	}
	return false;
}