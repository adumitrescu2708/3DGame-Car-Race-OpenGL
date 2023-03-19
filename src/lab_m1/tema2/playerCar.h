#pragma once

#include <vector>
#include <string>
#include "components/simple_scene.h"
#include "lab_m1/tema2/car.h"

namespace playerCar
{
	class PlayerCar {
	public:
		PlayerCar();
		~PlayerCar();

		void Init();
		void RandomizeInitialPoint();
		void Rotate(float distance);
		void Move(float distance);
		bool checkValidRoad(float distance);
		bool checkColisions(car::Car *cars);
		void checkStart();

		float positionX;
		float positionZ;
		float angle;
		int starting;
		bool markedStart;
		int counterRepeats;
		float highScore;
		float time;
		float lastTime;
	};
}
