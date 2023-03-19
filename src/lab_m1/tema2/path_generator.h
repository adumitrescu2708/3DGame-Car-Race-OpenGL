#pragma once

#include <vector>
#include "components/simple_scene.h"

/*
		@author Dumitrescu Alexandra 333CA
		@since  Dec 2022
*/

extern std::vector<glm::vec3> StartingPoints;
namespace path_generator
{
	void RenderTreesModels();
	void RenderTreesPosition();
	Mesh* CreatePath(const std::string& name, glm::vec3 color);
}
