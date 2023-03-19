#include "playerCar.h"
#include "path_generator.h"
#include <vector>
#include <string>
#include <iostream>
#include <math.h>
using namespace std;
using namespace playerCar;


//vector<glm::vec3> StartingPoints;


PlayerCar::PlayerCar()
{
}

PlayerCar::~PlayerCar()
{
}

void PlayerCar::Init()
{
	markedStart = true;
	counterRepeats = 0;
	lastTime = highScore = 0;
	RandomizeInitialPoint();
}

void PlayerCar::checkStart()
{
	time = Engine::GetElapsedTime() - lastTime;
	glm::vec3 CurrentPosition = glm::vec3(positionX, 0, positionZ);
	
	glm::vec3 P1 = StartingPoints[starting];

	glm::vec3 StartLineCenter = glm::vec3(P1.x, 0, P1.z);
	float distance = sqrt((StartLineCenter.x - CurrentPosition.x) * (StartLineCenter.x - CurrentPosition.x) + (StartLineCenter.z - CurrentPosition.z) * (StartLineCenter.z - CurrentPosition.z));
	if (distance < 1.8f && markedStart == false) {
		counterRepeats++;
		markedStart = true;
		lastTime += time;
		if (time < highScore || highScore == 0) {
			highScore = time;
		}
	}
	else {
		if (distance > 2.f) {
			markedStart = false;
		}
	}
}


bool PlayerCar::checkColisions(car::Car* cars)
{
	float radius = 0.6f;
	for (int i = 0; i < 5; i++) {
		glm::vec3 A = cars[i].position;
		glm::vec3 B = glm::vec3(positionX, 0, positionZ);


		float f = sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y) + (A.z - B.z) * (A.z - B.z));
		if (f < 1 * radius) {
			return false;
		}
	}
	return true;
}



bool PlayerCar::checkValidRoad(float distance) {
	bool move = true;

	float half_distance = 2.0f;
	int segments_counter = 0;

	float positionX2 = positionX +  distance * cos(angle);
	float positionZ2 = positionZ +  distance * sin(angle);

	for (int i = 0; i < StartingPoints.size() - 1; i++) {
		float distance_to_segment = 0.0f;
		
		glm::vec3 P1 = StartingPoints[i];
		glm::vec3 P2 = StartingPoints[i + 1];
		glm::vec3 P3 = glm::vec3(positionX2, 0, positionZ2);

		float u = 0.0f;

		glm::vec3 Dif = P1 - P2;
		u = (((P3.x - P1.x) * (P2.x - P1.x) + (P3.z - P1.z) * (P2.z - P1.z))) / (float) abs(Dif.x * Dif.x + Dif.z * Dif.z);
		
		if (u > 1.2f || u < -0.2f) {
			continue;
		}
		float x = P1.x + u * (P2.x - P1.x);
		float z = P1.z + u * (P2.z - P1.z);
		
		glm::vec3 Dist = glm::vec3(x, 0, z);
		glm::vec3 Rez =  Dist - glm::vec3(positionX2, 0, positionZ2);
		distance_to_segment = (float) sqrt(Rez.x * Rez.x + Rez.z * Rez.z);

		if ((distance_to_segment) <= (half_distance)) {
			return true;
		}
	}



	float distance_to_segment = 0.0f;

	glm::vec3 P1 = StartingPoints[StartingPoints.size() - 1];
	glm::vec3 P2 = StartingPoints[0];
	glm::vec3 P3 = glm::vec3(positionX2, 0, positionZ2);

	float u = 0.0f;

	glm::vec3 Dif = P1 - P2;
	u = (((P3.x - P1.x) * (P2.x - P1.x) + (P3.z - P1.z) * (P2.z - P1.z))) / (float) abs(Dif.x * Dif.x + Dif.z * Dif.z);

	if (u >= 1.f || u <= 0.f) {
		return false;
	}
	float x = P1.x + u * (P2.x - P1.x);
	float z = P1.z + u * (P2.z - P1.z);

	glm::vec3 Dist = glm::vec3(x, 0, z);
	glm::vec3 Rez = Dist - glm::vec3(positionX2, 0, positionZ2);
	distance_to_segment = (float) sqrt(Rez.x * Rez.x + Rez.z * Rez.z);

	if ((distance_to_segment) <= (half_distance)) {
		return true;
	}

	return false;
}


void PlayerCar::Rotate(float distance) {
	angle += distance;
}

void PlayerCar::Move(float distance) {
	positionX += distance * cos(angle);
	positionZ += distance * sin(angle);
}

void PlayerCar::RandomizeInitialPoint()
{
	int startIndex = rand() % (StartingPoints.size() - 2);
	starting = startIndex;

	glm::vec3 initialPoint = StartingPoints[startIndex];
	positionX = initialPoint.x;
	positionZ = initialPoint.z;

	startIndex++;
	glm::vec3 nextPoint = StartingPoints[startIndex];
	startIndex--;

	float x1 = initialPoint.x;
	float y1 = initialPoint.z;
	float x2 = nextPoint.x;
	float y2 = nextPoint.z;
	float delta = 0.1f;
	float distance = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

	float m = (float)(y2 - y1) / (x2 - x1);
	angle = atan(m);

	if (!((y2 - delta) < (y1 + sin(angle) * distance) && (y1 + sin(angle) * distance) < (y2 + delta))) {
		angle = angle + RADIANS(180);
	}

	if (!((x2 - delta) < (x1 + cos(angle) * distance) && (x1 + cos(angle) * distance) < (x2 + delta))) {
		angle = RADIANS(180) + angle;
	}

	positionX = initialPoint.x;
	positionZ = initialPoint.z;
}
