#include "texture-utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define BASE_IMAGE_LEVEL 0
#define ZERO_BORDER 0
#include <stb/stb_image.h>

#include <iostream>

glm::ivec2 our::texture_utils::loadImage(Texture2D& texture, const char *filename, bool generate_mipmap) {
    glm::ivec2 size;
    int channels;
    //Since OpenGL puts the texture origin at the bottom left while images typically has the origin at the top left,
    //We need to till stb to flip images vertically after loading them
    stbi_set_flip_vertically_on_load(true);
    //Load image data and retrieve width, height and number of channels in the image
    //The last argument is the number of channels we want and it can have the following values:
    //- 0: Keep number of channels the same as in the image file
    //- 1: Grayscale only
    //- 2: Grayscale and Alpha
    //- 3: RGB
    //- 4: RGB and Alpha (RGBA)
    //Note: channels (the 4th argument) always returns the original number of channels in the file
    unsigned char* pixel_data = stbi_load(filename, &size.x, &size.y, &channels, 4);
    if(pixel_data == nullptr){
        std::cerr << "Failed to load image: " << filename << std::endl;
        return {0, 0};
    }
    //Bind the texture such that we upload the image data to its storage
    //TODO: Finish this function
    //HINT: The steps should be as follows: bind the texture, send the pixel data to the GPU, then generate the mipmap (if requested).
    texture.bind();
    glTexImage2D(GL_TEXTURE_2D, BASE_IMAGE_LEVEL, GL_RGBA, size.x, size.y, ZERO_BORDER, GL_RGBA, GL_UNSIGNED_BYTE, pixel_data);
    if (generate_mipmap == true)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(pixel_data); //Free image data after uploading to GPU
    texture.unbind();
    return size;
}