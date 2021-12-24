#pragma once

#include "texture2d.hpp"

#include <glad/gl.h>
#include <glm/vec2.hpp>

namespace our::texture_utils {
    // This function loads an image and sends its data to the given Texture2D 
    glm::ivec2 loadImage(Texture2D& texture, const char* filename, bool generate_mipmap = true);
}