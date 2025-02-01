#version 120
uniform sampler2D image;
uniform float blur_radius;
uniform float dark_factor;
uniform vec2 resolution;

float normpdf(in float x, in float sigma)
{
    return 0.39894*exp(-0.5*x*x/(sigma*sigma))/sigma;
}


void main()
{
    vec2 coord = gl_TexCoord[0].xy;

    vec2 offx = vec2(blur_radius, 0.0);
    vec2 offy = vec2(0.0, blur_radius);

    vec4 pixel = texture2D(image, coord)  * 4.0 +
    texture2D(image, coord - offx)        * 2.0 +
    texture2D(image, coord + offx)        * 2.0 +
    texture2D(image, coord - offy)        * 2.0 +
    texture2D(image, coord + offy)        * 2.0 +
    texture2D(image, coord - offx - offy) * 1.0 +
    texture2D(image, coord - offx + offy) * 1.0 +
    texture2D(image, coord + offx - offy) * 1.0 +
    texture2D(image, coord + offx + offy) * 1.0;

    vec4 color = gl_Color * (pixel / 16.0);


    vec2 uv = gl_FragCoord.xy / resolution;
    vec2 center = vec2(0.5, 0.5);
    float dist = length(uv - center);
    float darkness = (1.0 - smoothstep(0.0, 1.0, dist) * dark_factor);
    
    darkness = darkness == 0 ? 0 : pow(2.0, 10.0 * darkness - 10.0);

    gl_FragColor = vec4(color.rgb * darkness, color.a);
}