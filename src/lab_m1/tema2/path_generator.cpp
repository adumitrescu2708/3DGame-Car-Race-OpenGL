#include "path_generator.h"
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace path_generator;

/*
		@author Dumitrescu Alexandra 333CA
		@since  Dec 2022
*/

/* make relevant points visible and accessible from all project */
static float upperBound = 2.f;
static float lowerBound = 2.f;
extern vector<glm::vec3> UpperBoundPoints;
static vector<glm::vec3> LowerBoundPoints;
extern vector<glm::vec3> UpperTrees;
extern vector<glm::vec3> LowerTrees;
extern vector<glm::vec3> FarTrees;
extern vector<int> UpperTreesModels;
extern vector<int> LowerTreesModels;
extern vector<int> FarTreesModels;
extern vector<glm::vec3> UpperPoints2;
extern vector<glm::vec3> LowerPoints2;


void path_generator::RenderTreesModels() {
	/* Project has 4 different types of trees, generate a
	corresponding random one for each one */
	srand((unsigned)time(NULL));
	int upperTreeBound, lowerTreeBound;
	for (int i = 0; i < LowerTrees.size(); i++) {
		int lowerTreeModel = rand() % 4;
		LowerTreesModels.push_back(lowerTreeModel);
	}
	for (int i = 0; i < UpperTrees.size(); i++) {
		int lowerTreeModel = rand() % 4;
		UpperTreesModels.push_back(lowerTreeModel);
	}

	for (int i = 0; i < FarTrees.size(); i++) {
		int farTreeModel = rand() % 4;
		FarTreesModels.push_back(farTreeModel);
	}
}

void path_generator::RenderTreesPosition() 
{
	vector<glm::vec3> UpperTreesModelsRandom;
	vector<glm::vec3> LowerTreesModelsRandom;
	vector<bool> UpperTreesModelsBool;
	vector<bool> LowerTreesModelsBool;

	/* geneate tree position starting from starting points of the road */
	for (int i = 1; i < StartingPoints.size(); i+=2) {
		srand((unsigned)time(NULL));
		float upperTreeBound, lowerTreeBound;
		lowerTreeBound = 0.8;
		upperTreeBound = 0.8;
		i--;
		glm::vec3 P1 = StartingPoints[i];
		i++;
		glm::vec3 P2 = StartingPoints[i];
		glm::vec3 D = P2 - P1;
		D = glm::normalize(D);
		glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));
		glm::vec3 upperPoint = P1 + (upperBound + upperTreeBound) * P;
		glm::vec3 lowerPoint = P1 - (lowerBound + lowerTreeBound) * P;
		if (upperBound + upperTreeBound > upperBound) {
			UpperTreesModelsRandom.push_back(upperPoint);
		}
		if (lowerBound + lowerTreeBound > lowerBound) {
			LowerTreesModelsRandom.push_back(lowerPoint);
		}
	}

	/* eliminate trees with colisions */
	for (int i = 0; i < UpperTreesModelsRandom.size(); i++) {
		bool toRender = true;
		for (int j = i + 1; j < UpperTreesModelsRandom.size(); j++) {
			if (i != j) {
				glm::vec3 P1 = UpperTreesModelsRandom[i];
				glm::vec3 P2 = UpperTreesModelsRandom[j];
				glm::vec3 R = P2 - P1;
				if ((float) sqrt(R.x * R.x + R.y * R.y + R.z * R.z) < 1.5f) {
					toRender = false;
					UpperTreesModelsBool.push_back(0);
					break;
				}
			}
		}
		if (toRender == true) {
			UpperTreesModelsBool.push_back(1);
		}
	}

	for (int i = 0; i < UpperTreesModelsRandom.size(); i++) {
		if (UpperTreesModelsBool[i] == true) {
			UpperTrees.push_back(UpperTreesModelsRandom[i]);
		}
	}

	/* eliminate trees wih colisions */
	for (int i = 0; i < LowerTreesModelsRandom.size(); i++) {
		bool toRender = true;
		for (int j = i + 1; j < LowerTreesModelsRandom.size(); j++) {
			if (i != j) {
				glm::vec3 P1 = LowerTreesModelsRandom[i];
				glm::vec3 P2 = LowerTreesModelsRandom[j];
				glm::vec3 R = P2 - P1;
				if ((float)sqrt(R.x * R.x + R.y * R.y + R.z * R.z) < 1.5f) {
					toRender = false;
					LowerTreesModelsBool.push_back(0);
					break;
				}
			}
		}
		if (toRender == true) {
			LowerTreesModelsBool.push_back(1);
		}
	}

	for (int i = 0; i < LowerTreesModelsRandom.size(); i++) {
		if (LowerTreesModelsBool[i] == true) {
			LowerTrees.push_back(LowerTreesModelsRandom[i]);
		}
	}
	
	RenderTreesModels();
}


Mesh* path_generator::CreatePath(const std::string& name, glm::vec3 color)
{
	/* geogebra generated */
	StartingPoints = {
		glm::vec3(-13.62, 0, 6.27),
		glm::vec3(-12.81, 0, 8.23),
		glm::vec3(-11.35, 0, 9.72),
		glm::vec3(-8.87, 0, 10.1),
		glm::vec3(-6.65, 0, 9.98),
		glm::vec3(-4.28, 0, 10.02),
		glm::vec3(-2, 0, 10),
		glm::vec3(0, 0, 10),
		glm::vec3(2, 0, 10),
		glm::vec3(4, 0, 10),
		glm::vec3(5.58, 0, 10),
		glm::vec3(8, 0, 10),
		glm::vec3(10.46, 0, 9.83),
		glm::vec3(12.91, 0, 9.96),
		glm::vec3(14.77, 0, 8.84),
		glm::vec3(15.7, 0, 7.45),
		glm::vec3(15.7, 0, 6.04),
		glm::vec3(15.02, 0, 4.72),
		glm::vec3(12.87, 0, 3.89),
		glm::vec3(11.04, 0, 3.86),
		glm::vec3(8, 0, 4),
		glm::vec3(5.45, 0, 3.89),
		glm::vec3(3.46, 0, 3.89),
		glm::vec3(1.21, 0, 3.86),
		glm::vec3(-0.62, 0, 3.53),
		glm::vec3(-1.55, 0, 2.41),
		glm::vec3(-1.77, 0, 0.87),
		glm::vec3(-1.77, 0, 0.32),
		glm::vec3(-1.42, 0, -0.59),
		glm::vec3(-0.62, 0, -1.51),
		glm::vec3(1.09, 0, -2.47),
		glm::vec3(2.56, 0, -2.89),
		glm::vec3(4.46, 0, -3.11),
		glm::vec3(6.77, 0, -3.02),
		glm::vec3(8.86, 0, -2.92),
		glm::vec3(11.17, 0, -2.98),
		glm::vec3(13.42, 0, -2.79),
		glm::vec3(15.15, 0, -3.3),
		glm::vec3(15.86, 0, -5.01),
		glm::vec3(15.86, 0, -6.32),
		glm::vec3(15.38, 0, -8.64),
		glm::vec3(14.51, 0, -9.63),
		glm::vec3(13.26, 0, -9.99),
		glm::vec3(10.98, 0, -10.02),
		glm::vec3(8.41, 0, -9.99),
		glm::vec3(6, 0, -10),
		glm::vec3(4, 0, -10),
		glm::vec3(1.66, 0, -9.86),
		glm::vec3(0, 0, -10),
		glm::vec3(-2.48, 0,-9.92),
		glm::vec3(-5.34, 0, -9.92),
		glm::vec3(-8.32, 0, -9.82),
		glm::vec3(-11.05, 0, -9.82),
		glm::vec3(-12.92, 0, -9.02),
		glm::vec3(-13.43, 0, -7.38),
		glm::vec3(-13.98, 0,-5.1),
		glm::vec3(-14.27, 0, -2.47),
		glm::vec3(-14.14, 0, -0.38),
		glm::vec3(-14, 0, 2),
		glm::vec3(-13.95,0, 4.26)
	};

	/* compute with the given formula point from upper and down sides of the road */
	for (int i = 1; i < StartingPoints.size(); i++) {
		i--;
		glm::vec3 P1 = StartingPoints[i];
		i++;
		glm::vec3 P2 = StartingPoints[i];
		glm::vec3 D = P2 - P1;
		D = glm::normalize(D);
		glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));
		glm::vec3 upperPoint = P1 + upperBound * P;
		glm::vec3 lowerPoint = P1 - lowerBound * P;
		UpperBoundPoints.push_back(upperPoint);
		LowerBoundPoints.push_back(lowerPoint);
	}
	
	
	glm::vec3 P1 = StartingPoints[StartingPoints.size() - 1];
	glm::vec3 P2 = StartingPoints[0];
	glm::vec3 D = P2 - P1;
	D = glm::normalize(D);
	glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));
	glm::vec3 upperPoint = P1 + upperBound * P;
	glm::vec3 lowerPoint = P1 - lowerBound * P;
	UpperBoundPoints.push_back(upperPoint);
	LowerBoundPoints.push_back(lowerPoint);

	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;

	/* compute a high poly road*/
	for (int i = 0; i < StartingPoints.size() - 1; i++) {
		glm::vec3 upperP1 = UpperBoundPoints[i];
		glm::vec3 upperP2 = UpperBoundPoints[i + 1];
		glm::vec3 lowerP1 = LowerBoundPoints[i];
		glm::vec3 lowerP2 = LowerBoundPoints[i + 1];

		glm::vec3 UpperRez = upperP2 - upperP1;
		glm::vec3 LowerRez = lowerP2 - lowerP1;

		UpperPoints2.push_back(upperP1);
		LowerPoints2.push_back(lowerP1);
		for (int j = 1; j < 10; j++) {
			UpperPoints2.push_back(upperP1 + j * 0.1f * UpperRez);
			LowerPoints2.push_back(lowerP1 + j * 0.1f * LowerRez);
		}
	}

	glm::vec3 upperP1 = UpperBoundPoints[StartingPoints.size() - 1];
	glm::vec3 upperP2 = UpperBoundPoints[0];
	glm::vec3 lowerP1 = LowerBoundPoints[StartingPoints.size() - 1];
	glm::vec3 lowerP2 = LowerBoundPoints[0];

	glm::vec3 UpperRez = upperP2 - upperP1;
	glm::vec3 LowerRez = lowerP2 - lowerP1;

	UpperPoints2.push_back(upperP1);
	LowerPoints2.push_back(lowerP1);
	for (int j = 1; j < 10; j++) {
		UpperPoints2.push_back(upperP1 + j * 0.1f * UpperRez);
		LowerPoints2.push_back(lowerP1 + j * 0.1f * LowerRez);
	}

	int counter = StartingPoints.size() + 9 * StartingPoints.size();

	for (int i = 0; i < counter; i++) {
		vertices.push_back(VertexFormat(UpperPoints2[i], color)); // nr pare
		vertices.push_back(VertexFormat(LowerPoints2[i], color)); // nr impare
	}

	for (int i = 0; i < counter - 1; i++) {
		unsigned int upperIdx1 = 2 * i, upperIdx2 = 2 * i + 2;
		unsigned int lowerIdx1 = 2 * i + 1, lowerIdx2 = 2 * i + 3;
		indices.insert(indices.end(), { upperIdx1, lowerIdx1, lowerIdx2 });
		indices.insert(indices.end(), { upperIdx2, lowerIdx2, upperIdx1 });
	}

	unsigned int upperIdx1 = 2 * ( counter - 1), upperIdx2 = 0;
	unsigned int lowerIdx1 = 2 * (counter - 1) + 1, lowerIdx2 = 1;
	indices.insert(indices.end(), { upperIdx1, lowerIdx1, lowerIdx2 });
	indices.insert(indices.end(), { upperIdx2, lowerIdx2, upperIdx1 });

	Mesh* path = new Mesh(name);
	path->SetDrawMode(GL_TRIANGLE_STRIP);
	path->InitFromData(vertices, indices);
	RenderTreesPosition();
	return path;
}

