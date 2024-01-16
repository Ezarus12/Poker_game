#pragma once

#include "Components.h"

class PositionComponent : public Component
{
private:
	float xpos;
	float ypos;

public:

	PositionComponent() {
		xpos = ypos = 0;
	}

	PositionComponent(int x, int y) {
		xpos = float(x);
		ypos = float(y);
	}

	void update() override
	{
		//xpos++;
	}

	float x() { return xpos; }
	float y() { return ypos; }

	void x(int x) { xpos = x; }
	void y(int y) { ypos = y; }

	void setPos(int x, int y) {
		xpos = x;
		ypos = y;
	}
};
