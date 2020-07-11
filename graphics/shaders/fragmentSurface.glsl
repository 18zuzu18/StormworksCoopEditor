#version 330 core
out vec4 FragColor;

in vec4 color;

void main()
{
    if (color.a < 0.99f){
        discard;
    }
    FragColor = color;
    // Convert from srgb to linear space
    //    FragColor.r = pow(color.r, 2.2);
    //    FragColor.g = pow(color.g, 2.2);
    //    FragColor.b = pow(color.b, 2.2);
}