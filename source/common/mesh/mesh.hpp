#pragma once

#include <glad/gl.h>
#include "vertex.hpp"
#include <iostream>

namespace our
{

#define ATTRIB_LOC_POSITION 0
#define ATTRIB_LOC_COLOR 1
#define ATTRIB_LOC_TEXCOORD 2
#define ATTRIB_LOC_NORMAL 3
#define NOT_NORMALIZED GL_FALSE
#define NORMALIZED  GL_TRUE
#define NO_OFFSET 0
#define UNBIND 0

    class Mesh
    {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements
        GLsizei elementCount;
        GLsizei vertexCount;

    public:
        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering
        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &elements)
        {
            //TODO: Write this function
            // remember to store the number of elements in "elementCount" since you will need it for drawing
            // For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc
            elementCount = elements.size();
            vertexCount = vertices.size();

            // Generating, binding and loading data for all needed buffers
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);

            // Positions
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, NOT_NORMALIZED, sizeof(Vertex), (void *)NO_OFFSET);
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);
            unsigned long long locationOffset = 3 * sizeof(float);

            // Colors
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, NORMALIZED, sizeof(Vertex), (void *)locationOffset);
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);
            unsigned long long colorOffset = 4 * sizeof(GLubyte);

            // Textures
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, NOT_NORMALIZED, sizeof(Vertex), (void *)(locationOffset + colorOffset));
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
            unsigned long long textureOffset = 2 * sizeof(float);


            // Normals
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, NOT_NORMALIZED, sizeof(Vertex), (void *)(locationOffset + colorOffset + textureOffset));
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);

            // Unbinding all buffers
            glBindVertexArray(UNBIND);
            glBindBuffer(GL_ARRAY_BUFFER, UNBIND);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, UNBIND);
        }

        // this function should render the mesh
        void draw()
        {
            //TODO: Write this function
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (void*) NO_OFFSET);
            glBindVertexArray(0);
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh()
        {
            //TODO: Write this function
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}