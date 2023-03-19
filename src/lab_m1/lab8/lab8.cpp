#include "lab_m1/lab8/lab8.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab8::Lab8()
{
}


Lab8::~Lab8()
{
}


void Lab8::Init()
{
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab8", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab8", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Light & material properties
    {
        lightPosition = glm::vec3(0, 1, 1);
        lightDirection = glm::vec3(0, -1, 0);

        lightPosition2 = glm::vec3(1, 1, 0);
        lightDirection2 = glm::vec3(0, -1, 0);

    }
}


void Lab8::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab8::Update(float deltaTimeSeconds)
{
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
        // TODO(student): Add or change the object colors
        RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0, 1));

    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
        // TODO(student): Add or change the object colors
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0.25f, 0.75f));

    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.25, 0.75, 0.75));
    }

    // Render ground
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.01f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
        // TODO(student): Add or change the object colors
        RenderSimpleMesh(meshes["plane"], shaders["LabShader"], modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));

    }

    // Render the point light in the scene
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, lightPosition);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, lightPosition2);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix);
    }
}


void Lab8::FrameEnd()
{
    DrawCoordinateSystem();
}


void Lab8::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color)
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab8::OnInputUpdate(float deltaTime, int mods)
{

}


void Lab8::OnKeyPress(int key, int mods)
{
}


void Lab8::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab8::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab8::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab8::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab8::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab8::OnWindowResize(int width, int height)
{
}
