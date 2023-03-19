#pragma once

#include <vector>
#include "lab_m1/tema2/car.h"
#include "lab_m1/tema2/playerCar.h"
#include "components/simple_scene.h"
#include "lab_m1/lab5/lab_camera.h"
#include "components/text_renderer.h"


/*
        @author Dumitrescu Alexandra 333CA
        @since  Dec 2022
*/


namespace m1
{
    class tema2 : public gfxc::SimpleScene
    {

        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };



     public:
        tema2();
        ~tema2();

        void Init() override;

    protected:
        implemented::Camera* camera;
        implemented::Camera* cameraMiniMap;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        float positionCameraX;
        float positionCameraY;
        float positionCameraZ;


     private:
        void RenderCar(float deltaTimeSeconds, int index);
        void RenderPlayerCar(float deltaTimeSeconds);
        void RenderTrees();
        void RenderTreeModel1(glm::mat4 modelMatrix);
        void RenderTreeModel2(glm::mat4 modelMatrix);
        void RenderTreeModel3(glm::mat4 modelMatrix);
        void RenderTreeModel4(glm::mat4 modelMatrix);
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void RenderScene(float deltaTimeSeconds);
        void RenderSceneMini(float deltaTimeSeconds);
        void RenderBoarders();
        void RenderStartSign();

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, glm::vec3 color);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;


        glm::vec3 carPosition;
        playerCar::PlayerCar playerCar;
        car::Car *cars = new car::Car[6];

        glm::vec3 test;

        ViewportArea miniViewportArea;
        gfxc::TextRenderer *textRenderer;
    };
}   // namespace m1
