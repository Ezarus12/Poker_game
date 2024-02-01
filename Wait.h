#pragma once
#include <iostream>

inline bool Wait(float deltaTime, float seconds) {
	static float time = 0;
	time += deltaTime;
	if (time >= seconds) {
		std::cout << "Uplynelo " << seconds << " Sekund.\n";
		time = 0;
		return true;
	}
	return false;
}