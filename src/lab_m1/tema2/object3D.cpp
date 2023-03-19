#include "object3D.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;
/*
        @author Dumitrescu Alexandra 333CA
        @since  Dec 2022
*/

Mesh* object3D::CreateHighPolyRectangle(string name, glm::vec3 center, int length) {
    glm::vec3 startPoint = center + glm::vec3(-length/2 * 1, 0, (length/2) * 1);
    vector<VertexFormat> vertices;
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            vertices.push_back(VertexFormat(glm::vec3(startPoint.x + j * 1, 0, startPoint.z - i * 1)));
        }
    }
    vector<unsigned int> indices;
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length - 1; j++) {
            vector<int> aux
            {
                length * i + j, (length) * (i + 1) + j, length * i + j + 1,
                length* i + j + 1, (length)* (i + 1) + j, (length)* (i + 1) + j + 1
            };
            indices.insert(indices.end(), aux.begin(), aux.end());
        }
    }


    //Mesh* rectangle = new Mesh(name);
    //rectangle->InitFromData(vertices, indices);
    return CreateMesh(name.c_str(), vertices, indices);
}

Mesh* object3D::CreatePyramid(std::string name, glm::vec3 center, float base_length, float height)
{
    vector<VertexFormat> vertices
    {
        VertexFormat(center + glm::vec3(-base_length / 2, 0, -base_length / 2)),
        VertexFormat(center + glm::vec3(-base_length / 2, 0, base_length / 2)),
        VertexFormat(center + glm::vec3(base_length / 2, 0, base_length / 2)),
        VertexFormat(center + glm::vec3(base_length / 2, 0, -base_length / 2)),
        VertexFormat(center + glm::vec3(0, height, 0))
    };

    vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        0, 3, 4
    };



    //Mesh* pyramid = new Mesh(name);
    //pyramid->InitFromData(vertices, indices);
    return CreateMesh(name.c_str(), vertices, indices);
}


Mesh*object3D:: CreateParal(std::string name, glm::vec3 center, float length, float height, float width)
{
    vector<VertexFormat> vertices
    {
        VertexFormat(center + glm::vec3(-width/2, 0, -length/2)),
        VertexFormat(center + glm::vec3(-width/2, 0, length/2)),
        VertexFormat(center + glm::vec3(width / 2, 0, length/2)),
        VertexFormat(center + glm::vec3(width / 2, 0, -length/2)),

        VertexFormat(center + glm::vec3(-width / 2, height, -length / 2)),
        VertexFormat(center + glm::vec3(-width / 2, height, length / 2)),
        VertexFormat(center + glm::vec3(width / 2, height, length / 2)),
        VertexFormat(center + glm::vec3(width / 2, height, -length / 2))
    };


    vector<unsigned int> indices = {
        0, 1, 2,    // indices for first triangle
        0, 2, 3,
        2, 3, 7,
        2, 7, 6,
        2, 7, 3,
        1, 2, 6,
        1, 6, 5,
        0, 1, 5,
        0, 5, 4,
        0, 3, 7,
        0, 7, 4,
        4, 5, 6,
        4, 6, 7
    };


    //Mesh* tree = new Mesh(name);
    //tree->InitFromData(vertices, indices);
    return CreateMesh(name.c_str(), vertices, indices);
}



Mesh* object3D::CreateTree(string name, glm::vec3 position) {
    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(-1, -1,  1) + position, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(0, -1,  1) + position, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(-1, 0,  1) + position, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(0, 0,  1) + position, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(-1, -1,  0) + position, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(0, -1,  0) + position, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(-1, 0,  0) + position, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(0, 0,  0) + position, glm::vec3(0.2, 0.8, 0.6)),
    };

    vector<unsigned int> indices = {
            0, 1, 2,    // indices for first triangle
            1, 3, 2,
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 5, 7,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4
    };

    //Mesh* tree = new Mesh(name);
    //tree->InitFromData(vertices, indices);
    return CreateMesh(name.c_str(), vertices, indices);
}


Mesh* object3D::CreateHexa(std::string name, glm::vec3 position, int height, int length)
{

    vector<VertexFormat> vertices
    {
        VertexFormat(position),
        VertexFormat(position - glm::vec3(length, 0, 0)),
        VertexFormat(position + glm::vec3(length * cos(RADIANS(120)), 0, length * sin(RADIANS(120)))),
        VertexFormat(position + glm::vec3(length * cos(RADIANS(60)), 0, length * sin(RADIANS(60)))),
        VertexFormat(position + glm::vec3(length, 0, 0)),
        VertexFormat(position + glm::vec3(length * cos(RADIANS(-60)), 0, length * sin(RADIANS(-60)))),
        VertexFormat(position + glm::vec3(length * cos(RADIANS(-120)), 0, length * sin(RADIANS(-120)))),

        VertexFormat(position + glm::vec3(0, height, 0)),
        VertexFormat(position - glm::vec3(length, 0, 0) + glm::vec3(0, height, 0)),
        VertexFormat(position + glm::vec3(length * cos(RADIANS(120)), 0, length * sin(RADIANS(120))) + glm::vec3(0, height, 0)),
        VertexFormat(position + glm::vec3(length * cos(RADIANS(60)), 0, length * sin(RADIANS(60))) + glm::vec3(0, height, 0)),
        VertexFormat(position + glm::vec3(length, 0, 0) + glm::vec3(0, height, 0)),
        VertexFormat(position + glm::vec3(length * cos(RADIANS(-60)), 0, length * sin(RADIANS(-60))) + glm::vec3(0, height, 0)),
        VertexFormat(position + glm::vec3(length * cos(RADIANS(-120)), 0, length * sin(RADIANS(-120))) + glm::vec3(0, height, 0))
    };
    vector<unsigned int> indices = {
        1, 0, 2,
        2, 0, 3,
        3, 0, 4,
        4, 0, 5,
        5, 0, 6,
        6, 0, 1,
        8, 7, 9,
        9, 7 , 10,
        10, 7, 11,
        11, 7, 12,
        12, 7, 13,
        13, 7, 8,
        1, 8, 9,
        1, 9, 2,
        2, 9, 10,
        2, 10, 3,
        3, 10, 11,
        3, 11, 4,
        4, 11, 12,
        4, 12, 5,
        5, 12, 13,
        5, 13, 6,
        6, 13, 8,
        6, 8, 1
    };

    return CreateMesh(name.c_str(), vertices, indices);
}


Mesh* object3D::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    Mesh* mesh;
    mesh = new Mesh(name);
    mesh->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    mesh->vertices = vertices;
    mesh->indices = indices;
    return mesh;
}