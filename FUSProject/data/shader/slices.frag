// for raycasting
#version 400

in vec3 Color;
in vec2 TexCoord;
layout (location = 0) out vec4 FragColor;

uniform sampler2DArray TexArray;
uniform float CurrSample;
uniform vec2 ScreenSize;
uniform float Opacity;

void main()
{
    //vec3 coordPoint = vec3(gl_FragCoord.st / ScreenSize, CurrSample);
    vec3 coordPoint = vec3(TexCoord.y, TexCoord.x, CurrSample);
    float texturePoint = texture(TexArray, coordPoint).r;
    
    //FragColor = vec4(texturePoint, 0.0, 0.0, 1.0);
    float alpha = 1.0;
    if (texturePoint < Opacity)
      alpha = 0.0f;
    FragColor = vec4(texturePoint, texturePoint, texturePoint, alpha);
    
    
    //FragColor = vec4(Color, 1.0);
}
