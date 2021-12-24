#pragma once

#include <glad/gl.h>

namespace our {

    // This class defined an OpenGL texture which will be used as a GL_TEXTURE_2D
    class Texture2D {
        // The OpenGL object name of this texture 
        GLuint name = 0;
    public:
        // This constructor creates an OpenGL texture and saves its object name in the member variable "name" 
        Texture2D() {
            //TODO: Complete this function
            glGenTextures(1, &name);
        };

        // This deconstructor deletes the underlying OpenGL texture
        ~Texture2D() { 
            //TODO: Complete this function
        }

        // This method binds this texture to GL_TEXTURE_2D
        void bind() const {
            //TODO: Complete this function
        }

        // This static method ensures that no texture is bound to GL_TEXTURE_2D
        static void unbind(){
            //TODO: Complete this function
        }

        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
    };
    
}