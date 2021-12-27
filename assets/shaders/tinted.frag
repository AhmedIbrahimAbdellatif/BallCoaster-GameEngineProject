#version 330 core

in Varyings {
    vec4 color;
} fs_in;

out vec4 frag_color;

uniform vec4 tint;

void main(){
    //TODO: Modify the following line to compute the fragment color
    // by multiplying the tint with the vertex color
    frag_color = tint * fs_in.color; // give the original color a tint specified by the uniform variable
}