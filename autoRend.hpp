#pragma once
#include "EDAF80/CelestialBody.hpp"
#include "core/helpers.hpp"

void autoRend(CelestialBody* planet, int i, std::stack<CelestialBody*>* stack);
