#version 330 core
out vec4 FragColor;

in vec4 color;

void main(){
    // Based on height
    FragColor = color;
    if (FragColor.a < 0.1)
        discard;
}
