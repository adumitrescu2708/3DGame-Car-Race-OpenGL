#include "car.h"
#include <vector>
#include <string>
#include <iostream>


/*
		@author Dumitrescu Alexandra 333CA
		@since  Dec 2022
*/


using namespace std;
using namespace car;

float upperBound = 1.3f;
float lowerBound = 1.3f;
vector<glm::vec3> StartingPoints;
static vector<bool> busyWaiting(150);
static int counter = 0;

Car::Car()
{
}

Car::~Car()
{
}

/*
	Randomize check points and choose a random starting point.
	The percentage of walked road is set to 0
*/
void Car::Init()
{
	RandomizeCheckPoints();
	ChooseStartingPoint();
	procent = 0;
}

void Car::Move(float deltaTimeSeconds)
{
	/* Increment current check point index and check circularity for obtaining 
		next point from starting points */
	currentCheckPoint++;
	if (currentCheckPoint >= checkPoints.size()) {
		currentCheckPoint = 0;
	}
	glm::vec3 nextCheckPoint = checkPoints[currentCheckPoint];
	currentCheckPoint--;
	if (currentCheckPoint < 0) {
		currentCheckPoint = checkPoints.size() - 1;
	}

	/* Compute vector from current point to next point */
	glm::vec3 Res = nextCheckPoint - checkPoints[currentCheckPoint];

	/* Move with a given distance and increment percentage */
	position += deltaTimeSeconds * 0.5f * Res;
	procent += deltaTimeSeconds * 0.5f;

	/* If pthe percentage is almost one, move to the next check point and rotate car corresponding */
	if (procent > 0.95f) {
		currentCheckPoint++;
		/* For circularity */
		if (currentCheckPoint == checkPoints.size()) {
			currentCheckPoint = 0;
		}
		Rotate();
		/* Reinitialize percentage */
		procent = 0;
	}

}

void Car::Rotate()
{
	/* Obtaing current point */
	glm::vec3 P1 = checkPoints[currentCheckPoint];

	currentCheckPoint--;
	if (currentCheckPoint < 0) {
		currentCheckPoint = checkPoints.size() - 1;
	}
	/* Obtain before point */
	glm::vec3 P3 = checkPoints[currentCheckPoint];
	if (currentCheckPoint == checkPoints.size() - 1) {
		currentCheckPoint = -1;
	}
	currentCheckPoint++;

	currentCheckPoint++;
	if (currentCheckPoint >= checkPoints.size()) {
		currentCheckPoint = 0;
	}
	/* Obtain next point */
	glm::vec3 P2 = checkPoints[currentCheckPoint];
	currentCheckPoint--;
	if (currentCheckPoint < 0) {
		currentCheckPoint = checkPoints.size() - 1;
	}

	/* Compute vector from current point to next one */
	glm::vec3 Res = P2 - P1;
	glm::vec3 Prev = glm::vec3(1, 0, 0);
	Res = glm::normalize(Res);
	Prev = glm::normalize(Prev);

	/* Compute angle with OX */
	float angle2 = (acos(dot(Res, Prev)));
	float x1 = P1.x, y1 = P1.z;
	float x2 = P2.x, y2 = P2.z;

	/* Check trigonometry function acos */
	if (Res.z <= 0) {
		angle = -angle2;
	}
	else {
		angle = angle2;
	}

	/* Translate to next point */
	position[0] = checkPoints[currentCheckPoint].x;
	position[2] = checkPoints[currentCheckPoint].z;
}


void Car::ChooseStartingPoint()
{
	srand((unsigned)time(0));
	/* Randomize start point */
	currentCheckPoint = -1;
	while (currentCheckPoint == -1 || busyWaiting[currentCheckPoint] == true ) {
		currentCheckPoint = rand() % checkPoints.size();
	}
	busyWaiting[currentCheckPoint] = true;
	if (currentCheckPoint < 0) {
		currentCheckPoint = -currentCheckPoint;
	}
	glm::vec3 P1 = checkPoints[currentCheckPoint];

	currentCheckPoint++;
	if (currentCheckPoint >= checkPoints.size()) {
		currentCheckPoint = 0;
	}
	glm::vec3 P2 = checkPoints[currentCheckPoint];
	currentCheckPoint--;
	if (currentCheckPoint < 0) {
		currentCheckPoint = checkPoints.size() - 1;
	}

	/* Compute initial rotation angle */
	float x1 = P1.x, y1 = P1.z;
	float x2 = P2.x, y2 = P2.z;
	float distance = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	float delta = 0.1f;
	float m = (float) (y2 - y1) / (x2 - x1);
	angle = atan(m);


	if (!((y2 - delta) < (y1 + sin(angle) * distance) && (y1 + sin(angle) * distance) < (y2 + delta))) {
		angle = angle + RADIANS(180);
	}

	if (!((x2 - delta) < (x1 + cos(angle) * distance) && (x1 + cos(angle) * distance) < (x2 + delta))) {
		angle = RADIANS(180) + angle;
	}

	positionX = P1.x;
	positionY = P1.z;
	position = glm::vec3(P1.x, 0, P1.z);
}

void Car::RandomizeCheckPoints() 
{
	/* obtain a proper offset for check points */
	while (offset > 1.8 || offset == 0 || offset < -1.8) {		
		offset = rand() % 500;
		int par = rand();
		/* negative or positive */
		if (par % 2 == 0) {
			offset = -offset;
		}
		offset /= 100;
	}

	/* similar to the path generator */
	for (int i = 1; i < StartingPoints.size(); i++) {
		i--;
		glm::vec3 P1 = StartingPoints[i];
		i++;
		glm::vec3 P2 = StartingPoints[i];
		glm::vec3 D = P2 - P1;
		D = glm::normalize(D);
		glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));
		glm::vec3 checkPoint = P1 + offset * P;
		checkPoints.push_back(checkPoint);
	}

	glm::vec3 P1 = StartingPoints[StartingPoints.size() - 1];
	glm::vec3 P2 = StartingPoints[0];
	glm::vec3 D = P2 - P1;
	D = glm::normalize(D);
	glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));
	glm::vec3 upperPoint = P1 + upperBound * P;
	glm::vec3 lowerPoint = P1 - lowerBound * P;
	glm::vec3 checkPoint = P1 + offset * P;
	checkPoints.push_back(checkPoint);

}