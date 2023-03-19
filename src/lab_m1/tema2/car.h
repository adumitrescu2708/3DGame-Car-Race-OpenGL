#pragma once

#include <vector>
#include <string>
#include "components/simple_scene.h"

/*
		@author Dumitrescu Alexandra 333CA
		@since  Dec 2022
*/

namespace car
{
	class Car {
	public:
		Car();
		~Car();

		void Init();
		void RandomizeCheckPoints();
		void ChooseStartingPoint();
		void Move(float deltaTimeSeconds);
		void Rotate();

		float offset;
		float angle;
		float positionX;
		float positionY;
		std::vector<glm::vec3> checkPoints;
		int currentCheckPoint;
		glm::vec3 position;
		float procent;
	};
}
