#pragma once

#include <vector>
#include <string>
#include "components/simple_scene.h"

/*
		@author Dumitrescu Alexandra 333CA
		@since  Dec 2022
*/
namespace object3D
{
	Mesh* CreateHighPolyRectangle(std::string name, glm::vec3 center, int length);
	Mesh* CreatePyramid(std::string name, glm::vec3 center, float base_length, float height);
	Mesh* CreateTree(std::string name, glm::vec3 position);
	Mesh* CreateParal(std::string name, glm::vec3 center, float length, float height, float width);
	Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
	Mesh* CreateHexa(std::string name, glm::vec3 position, int height, int length);
}
