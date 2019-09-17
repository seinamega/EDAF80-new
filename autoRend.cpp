#include "autoRend.hpp"
#include "core/helpers.hpp"


void autoRend(CelestialBody* planet, int i, std::stack<CelestialBody*>* stack)
{
	std::vector<CelestialBody*> children = &planet.get_children();
	if ((children.size() == 0)&& (i > children.size()))
	{
		stack.push(planet);
		//matrix_stack.push(planet.get_transform());
		return;
	}
	else
	{
		autoRend(children(i), 1,stack);
		autoRend(planet, i + 1,stack);
		return;
	}
}
