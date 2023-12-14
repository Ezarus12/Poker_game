#pragma once

#include "Components.h"

class PositionComponent : public Component
{
private:
	int xpos;
	int ypos;

public:

	PositionComponent() {
		xpos = ypos = 0;
	}

	PositionComponent(int x, int y) {
		xpos = x;
		ypos = y;
	}

	void update() override
	{
		//xpos++;
	}

	int x() { return xpos; }
	int y() { return ypos; }

	void x(int x) { xpos = x; }
	void y(int y) { ypos = y; }

	void setPos(int x, int y) {
		xpos = x;
		ypos = y;
	}
};
