#include "lab_m1/tema2/tema2.h"
#include "lab_m1/tema1/object2D.h"
#include "lab_m1/tema2/object3D.h"
#include "lab_m1/tema2/path_generator.h"
#include <time.h>
#include "components/text_renderer.h"
#include <vector>
#include <string>
#include <iostream>


/*
        @author Dumitrescu Alexandra 333CA
        @since  Dec 2022
*/

using namespace std;
using namespace m1;

bool mini;
vector<glm::vec3> UpperTrees;
vector<glm::vec3> LowerTrees;
vector<glm::vec3> FarTrees;
vector<int> UpperTreesModels;
vector<int> LowerTreesModels;
vector<int> FarTreesModels;
vector<glm::vec3> UpperBoundPoints;
vector<glm::vec3> UpperPoints2;
vector<glm::vec3> LowerPoints2;

tema2::tema2()
{
}


tema2::~tema2()
{
}


void tema2::Init()
{
    /* Set implemented camera */
    renderCameraTarget = false;
    camera = new implemented::Camera();
  
    /* Set homework's shaders */
    Shader* shader = new Shader("CarRaceShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

    /* Create meshses */
    Mesh* grass = object2D::CreateRectangle("grass", glm::vec3(-11, -5, 0), 20, 20, glm::vec3(1, 0.89, 0.71), 1);
    AddMeshToList(grass);

    Mesh* path = path_generator::CreatePath("path", glm::vec3(1, 1, 0.71));
    AddMeshToList(path);

    Mesh* tree = object3D::CreateParal("tree", glm::vec3(0, 0, 0), 0.1f, 2, 0.1f);
    AddMeshToList(tree);

    Mesh* stick = object3D::CreateParal("stick", glm::vec3(0, 0, 0), 0.08f, 0.6f, 0.08f);
    AddMeshToList(stick);

    Mesh* crown = object3D::CreatePyramid("treeCrown", glm::vec3(0, 0, 0), 1, 1.5f);
    AddMeshToList(crown);

    Mesh* crown2 = object3D::CreateParal("treeCrown2", glm::vec3(0, 0, 0), 0.9f, 0.9f, 0.9f);
    AddMeshToList(crown2);

    Mesh* car2 = object3D::CreateParal("car", glm::vec3(0, 0, 0), 0.4f, 0.4f, 1.5f);
    AddMeshToList(car2);

    Mesh* checkPoint = object3D::CreateParal("point", glm::vec3(0, 0, 0), 0.1f, 0.1f, 0.1f);
    AddMeshToList(checkPoint);

    Mesh* grass2 = object3D::CreateHighPolyRectangle("grass2", glm::vec3(0, 0, 0), 100);
    AddMeshToList(grass2);

    Mesh* car3 = object3D::CreateParal("car2", glm::vec3(0, 0, 0), 0.5f, 0.5f, 1.7f);
    AddMeshToList(car3);

    Mesh* hexa = object3D::CreateHexa("hexa", glm::vec3(0, 0, 0), 1.5f, 1.2f);
    AddMeshToList(hexa);

    Mesh* bordura = object3D::CreateParal("bordura", glm::vec3(0, 0, 0), 0.1, 0.1, 0.1);
    AddMeshToList(bordura);

    /* Create text renderer */
    textRenderer = new gfxc::TextRenderer(window->props.selfDir, window->GetResolution().x, window->GetResolution().y);
    textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Fruit Days.ttf"), 18);

    /* Initialize enemies cars */
    for (int i = 0; i < 5; i++) {
        cars[i].Init();
    }

    /* Initialize player's car */
    playerCar.Init();
    float distance = 5, targetX, targetY, targetZ;

    /* Set camera below player's car*/
    positionCameraX = playerCar.positionX + 5 * cos(playerCar.angle);
    positionCameraZ = playerCar.positionZ + 5 * sin(playerCar.angle);
    positionCameraY = 5;
    targetX = playerCar.positionX;
    targetY = 0;
    targetZ = playerCar.positionZ;
    camera->distanceToTarget = sqrt((positionCameraX - targetX) * (positionCameraX - targetX) + (positionCameraZ - targetZ) * (positionCameraZ - targetZ) + (positionCameraY - targetY) * (positionCameraY - targetY));
    camera->Set(glm::vec3(positionCameraX, positionCameraY, positionCameraZ), glm::vec3(playerCar.positionX, 0, playerCar.positionZ), glm::vec3(0, 1, 0));
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    /* Set minimap resolution */
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);
}



void tema2::FrameStart()
{
    /* Set clear color cu sky's color */
    glClearColor(0.56, 0.88, 0.94, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void tema2::Update(float deltaTimeSeconds)
{
    /* Render full view */
    mini = false;
    RenderScene(deltaTimeSeconds);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    /* Render view from above using minimap viewport */
    mini = true;
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    RenderSceneMini(deltaTimeSeconds);
}


void tema2::RenderSceneMini(float deltaTimeSeconds)
{
    /* Set camera above the scene */
    glm::vec3 position2 = camera->position;
    glm::vec3 forward2 = camera->forward;
    glm::vec3 right2 = camera->right;
    glm::vec3 up2 = camera->up;

    /* Make camera follow player's car */
    camera->Set(glm::vec3(playerCar.positionX, 20, playerCar.positionZ), glm::vec3(playerCar.positionX, 0, playerCar.positionZ), glm::vec3(0, 0, 1));
    
    /* Render scene */
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.1f, 0));
    RenderSimpleMesh(meshes["path"], shaders["CarRaceShader"], modelMatrix, glm::vec3(1, 0.89, 0.71));

    modelMatrix = glm::mat4(1);
    RenderSimpleMesh(meshes["grass2"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.35, 0.67, 0.38));
    modelMatrix = glm::mat4(1);

    /* Render enemies, player and scene */
    RenderStartSign();
    RenderBoarders();
    RenderTrees();

    for (int i = 0; i < 5; i++) {
        RenderCar(deltaTimeSeconds, i);
    }

    RenderPlayerCar(deltaTimeSeconds);

    /* Reset camera to initial values */
    camera->Set(position2, glm::vec3(playerCar.positionX, 0, playerCar.positionZ), up2);

}

void tema2::RenderScene(float deltaTimeSeconds)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.1f, 0));
    RenderSimpleMesh(meshes["path"], shaders["CarRaceShader"], modelMatrix, glm::vec3(1, 0.89, 0.71));

    modelMatrix = glm::mat4(1);
    RenderSimpleMesh(meshes["grass2"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.35, 0.67, 0.38));
    modelMatrix = glm::mat4(1);

    /* Render information on laps */
    RenderStartSign();
    stringstream stream;
    stream << "Number of laps: " << playerCar.counterRepeats;
    string text = stream.str();
    textRenderer->RenderText(text, 10, 5, 1, glm::vec3(0, 0.61, 0.58));
    stream.str("");
    stream << "Current Lap Time: " << playerCar.time;
    text = stream.str();
    textRenderer->RenderText(text, 10, 25, 1, glm::vec3(0, 0.51, 0.43));
    stream.str("");
    stream << "Fastest Lap: " << playerCar.highScore;
    text = stream.str();
    textRenderer->RenderText(text, 10, 45, 1, glm::vec3(0, 0.41, 0.39));

    RenderBoarders();
    RenderTrees();

    /* Render enemies */
    for (int i = 0; i < 5; i++) {
        RenderCar(deltaTimeSeconds, i);
    }

    /* Render player car */
    RenderPlayerCar(deltaTimeSeconds);
}

void tema2::RenderStartSign()
{
    /* Check if car finished a lap and update information */
    playerCar.checkStart();

    /* Compute points on upper and down lines of the road */
    int startIndex = playerCar.starting;
    int nextIndex = startIndex + 1;
    glm::vec3 P1 = StartingPoints[startIndex];
    glm::vec3 P2 = StartingPoints[nextIndex];
    glm::vec3 D = StartingPoints[nextIndex] - StartingPoints[startIndex];
    D = glm::normalize(D);
    glm::vec3 P = cross(D, glm::vec3(0, 1, 0));
    glm::vec3 upperPoint = P1 + 2.0f * P;
    glm::vec3 lowerPoint = P1 - 2.0f * P;
    glm::vec3 Rez = upperPoint - lowerPoint;
    Rez = glm::normalize(Rez);

    /* Compute length of the road */
    float len = glm::length(upperPoint - lowerPoint);

    /* Compute rotation angle with OX */
    float angle = acos(dot(Rez, glm::vec3(1, 0, 0)));
    if (Rez.z > 0) {
        angle = -angle;
    }

    /* Render black and white strips for the Finish/Start line */
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, P1 + glm::vec3(0, 0.05, 0));
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(len * 10 + 0.1f, 1.2, 1.5));
    RenderSimpleMesh(meshes["bordura"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.15,0.15,0.152));

    glm::vec3 P3 = P1 + 0.08f * (P2 - P1);
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, P3 + glm::vec3(0, 0.05, 0));
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(len * 10 + 0.1f, 1.2, 1.5));
    RenderSimpleMesh(meshes["bordura"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.95,0.95,0.95));
}


void tema2::RenderBoarders()
{
    glm::mat4 modelMatrix;
    for (int i = 0; i < UpperPoints2.size(); i++) {
        /* For each consecutive sets of points compute distance and rotation angle wth OX axis */
        modelMatrix = glm::mat4(1);
        glm::vec3 P1, P2;
        P1 = UpperPoints2[i];
        if (i == UpperPoints2.size() - 1) {
            P2 = UpperPoints2[0];
        }
        else {
            P2 = UpperPoints2[i + 1];
        }
       
        glm::vec3 Rez = P2 - P1;

        float len = glm::length(Rez);
        Rez = glm::normalize(Rez);
        float angle = acos(dot(Rez, glm::vec3(1, 0, 0)));
        if (Rez.z > 0) {
            angle = -angle;
        }
        /* Render */
        modelMatrix = glm::translate(modelMatrix, P1);
        modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(len * 10 + 0.1f, 1.1, 1.5));
        RenderSimpleMesh(meshes["bordura"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.95, 0.75, 0.59));
    }

    for (int i = 0; i < LowerPoints2.size(); i++) {
        /* For each consecutive sets of points compute distance and rotation angle wth OX axis */
        modelMatrix = glm::mat4(1);
        glm::vec3 P1, P2;
        P1 = LowerPoints2[i];
        if (i == LowerPoints2.size() - 1) {
            P2 = LowerPoints2[0];
        }
        else {
            P2 = LowerPoints2[i + 1];
        }

        glm::vec3 Rez = P2 - P1;
        float len = glm::length(Rez);
        Rez = glm::normalize(Rez);
        float angle = acos(dot(Rez, glm::vec3(1, 0, 0)));
        if (Rez.z > 0) {
            angle = -angle;
        }
        /* Render */
        modelMatrix = glm::translate(modelMatrix, P1);
        modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(len * 10 + 0.1f, 1.1, 1.5));
        RenderSimpleMesh(meshes["bordura"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.95, 0.75, 0.59));
    }
}


void tema2::RenderPlayerCar(float deltaTimeSeconds)
{
    /* Render player car based on its position and rotation angle */
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(playerCar.positionX, 0, playerCar.positionZ));
    modelMatrix = glm::rotate(modelMatrix, -playerCar.angle, glm::vec3(0, 1, 0));
    RenderSimpleMesh(meshes["car2"], shaders["CarRaceShader"], modelMatrix, glm::vec3(1, 0, 0));
}


void tema2::RenderCar(float deltaTimeSeconds, int index) {
    glm::mat4 modelMatrix = glm::mat4(1);
    cars[index].Move(deltaTimeSeconds);
    modelMatrix = glm::translate(modelMatrix, cars[index].position);
    
    float aux = cars[index].angle;
    modelMatrix = glm::rotate(modelMatrix, -aux, glm::vec3(0, 1, 0));
    RenderSimpleMesh(meshes["car"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.47, 0.53, 0.60));

    for (int i = 0; i < StartingPoints.size(); i+=2) {
          modelMatrix = glm::mat4(1);
          glm::vec3 P1 = StartingPoints[i], P2;
          if (i == StartingPoints.size() - 1) {
              P2 = StartingPoints[0];
          }
          else {
              P2 = StartingPoints[i + 1];
          }
          glm::vec3 Rez = P2 - P1;
          Rez = glm::normalize(Rez);
          float angle = acos(dot(Rez, glm::vec3(1, 0, 0)));
          if (Rez.z > 0) {
              angle = -angle;
          }

          modelMatrix = glm::translate(modelMatrix, glm::vec3(StartingPoints[i].x, 0, StartingPoints[i].z));
          modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
          modelMatrix = glm::scale(modelMatrix, glm::vec3(4.f, 1.1f, 1.4f));
          RenderSimpleMesh(meshes["point"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.95, 0.75, 0.59));      
    }
}

void tema2::RenderTrees() {
    srand((unsigned)time(NULL));

    glm::mat4 modelMatrix = glm::mat4(1);
    for (int i = 0; i < LowerTrees.size(); i++) {
        modelMatrix = glm::mat4(1);
        int treeModel = LowerTreesModels[i];
        modelMatrix = glm::translate(modelMatrix, LowerTrees[i]);
        if (treeModel == 0) {
            RenderTreeModel1(modelMatrix);
        }
        else if (treeModel == 1) {
            RenderTreeModel2(modelMatrix);
        }
        else {
            RenderTreeModel3(modelMatrix);
        }
    }

    for (int i = 0; i < UpperTrees.size(); i++) {
        modelMatrix = glm::mat4(1);
        int treeModel = UpperTreesModels[i];
        modelMatrix = glm::translate(modelMatrix, UpperTrees[i]);
        if (treeModel == 0) {
            RenderTreeModel1(modelMatrix);
        }
        else if (treeModel == 1) {
            RenderTreeModel2(modelMatrix);
        }
        else if(treeModel == 2){
            RenderTreeModel3(modelMatrix);
        }
        else {
            RenderTreeModel4(modelMatrix);
        }
    }
}

/* 4 methods to render 4 different types of trees */
void tema2::RenderTreeModel4(glm::mat4 modelMatrix)
{
    RenderSimpleMesh(meshes["tree"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.43, 0.30, 0.25));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.2f, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6f, 1, 0.6f));
    RenderSimpleMesh(meshes["hexa"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.27, 0.43, 0.29));
    glm::mat4 cp = modelMatrix;
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.02f, -0.6f, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(-45.0f), glm::vec3(1, 0, 0));
    RenderSimpleMesh(meshes["stick"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.43, 0.30, 0.25));

    modelMatrix = glm::translate(cp, glm::vec3(0.02f, -0.4f, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(49.0f), glm::vec3(1, 0, 0));
    RenderSimpleMesh(meshes["stick"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.43, 0.30, 0.25));
}

void tema2::RenderTreeModel3(glm::mat4 modelMatrix)
{
    RenderSimpleMesh(meshes["tree"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.43, 0.30, 0.25));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f, 1.2f, 0.8f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.3f, 0));
    RenderSimpleMesh(meshes["treeCrown"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.27, 0.43, 0.29));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5f, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, 0.8f));
    RenderSimpleMesh(meshes["treeCrown"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.24, 0.47, 0.29));
}


void tema2::RenderTreeModel2(glm::mat4 modelMatrix)
{
    RenderSimpleMesh(meshes["tree"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.43, 0.30, 0.25));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f, 0.8f, 1.f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.1f, 0));
    RenderSimpleMesh(meshes["treeCrown"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.36, 0.37, 0.26)); //0.53, 0.66, 0.42
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5f, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f, 0.8f, 0.8f));
    RenderSimpleMesh(meshes["treeCrown"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.38, 0.39, 0.22));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5f, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f, 0.8f, 0.8f));
    RenderSimpleMesh(meshes["treeCrown"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.40, 0.38, 0.21));
}


void tema2::RenderTreeModel1(glm::mat4 modelMatrix)
{
    RenderSimpleMesh(meshes["tree"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.43, 0.30, 0.25));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.8, 0));
    RenderSimpleMesh(meshes["treeCrown"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.29, 0.38, 0.26));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.7, 0));
    RenderSimpleMesh(meshes["treeCrown"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.32, 0.42, 0.25));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.7, 0));
    RenderSimpleMesh(meshes["treeCrown"], shaders["CarRaceShader"], modelMatrix, glm::vec3(0.2963, 0.43, 0.275));
}


void tema2::FrameEnd()
{
}



void tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, glm::vec3 color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    int modelLocation = glGetUniformLocation(shader->GetProgramID(), "Model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int viewLocation = glGetUniformLocation(shader->GetProgramID(), "View");
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    int projectionLocation = glGetUniformLocation(shader->GetProgramID(), "Projection");
    if (mini == true) {
        glm::mat4 projectionMatrix2 = glm::ortho(-15.f, 10.f, -15.f, 10.f, 0.05f, 500.0f);
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    }
    else {
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    }


    glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));
    carPosition = glm::vec3(playerCar.positionX, 0.0f, playerCar.positionZ);
    glUniform3fv(glGetUniformLocation(shader->program, "car_position"), 1, glm::value_ptr(carPosition));

    if (mini == true) {
        test = glm::vec3(1, 1, 1);
        glUniform3fv(glGetUniformLocation(shader->program, "minimap"), 1, glm::value_ptr(test));
    }
    else {
        test = glm::vec3(0, 0, 0);
        glUniform3fv(glGetUniformLocation(shader->program, "minimap"), 1, glm::value_ptr(test));
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void tema2::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 2.0f;
    float sensivityOY = 0.001f;

    if (window->KeyHold(GLFW_KEY_D)) {
        playerCar.Rotate(cameraSpeed * deltaTime);
        camera->RotateThirdPerson_OY(-cameraSpeed * deltaTime);

    }

    if (window->KeyHold(GLFW_KEY_A)) {
        playerCar.Rotate(-cameraSpeed * deltaTime);
        camera->RotateThirdPerson_OY(cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_W)) {
        if (playerCar.checkValidRoad(-2 * cameraSpeed * deltaTime) == true && playerCar.checkColisions(cars) == true) {
            playerCar.Move(-cameraSpeed * deltaTime * 2);
            positionCameraX = playerCar.positionX + 5 * cos(playerCar.angle);
            positionCameraZ = playerCar.positionZ + 5 * sin(playerCar.angle);
            camera->Set(glm::vec3(positionCameraX, positionCameraY, positionCameraZ), glm::vec3(playerCar.positionX, 0, playerCar.positionZ), glm::vec3(0, 1, 0));
        }
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        if (playerCar.checkValidRoad(2 * cameraSpeed * deltaTime) == true && playerCar.checkColisions(cars) == true) {
            playerCar.Move(2 * cameraSpeed * deltaTime);
            positionCameraX = playerCar.positionX + 5 * cos(playerCar.angle);
            positionCameraZ = playerCar.positionZ + 5 * sin(playerCar.angle);
            camera->Set(glm::vec3(positionCameraX, positionCameraY, positionCameraZ), glm::vec3(playerCar.positionX, 0, playerCar.positionZ), glm::vec3(0, 1, 0));
        }
    }

}


void tema2::OnKeyPress(int key, int mods)
{
}


void tema2::OnKeyRelease(int key, int mods)
{
}


void tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}


void tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}


void tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void tema2::OnWindowResize(int width, int height)
{
}
