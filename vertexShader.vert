#ifdef GL_ES
precision mediump float;
#endif

attribute highp vec4 vertex;
attribute mediump vec4 texCoord;

uniform mediump mat4 matrix;
uniform mediump vec3 lightPos;

varying mediump vec4 texc;
varying mediump vec3 lPos;

void main(void)
{
    gl_Position = matrix * vertex;
    texc = texCoord;
    lPos = lightPos;
}
