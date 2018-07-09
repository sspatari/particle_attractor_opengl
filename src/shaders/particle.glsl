#shader vertex
#version 460 core

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec4 in_color;

out vec4 vColor;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    gl_Position = u_projection * u_view * u_model * in_position;
    vColor = in_color;
    gl_PointSize = 2.0;
}

#shader fragment
#version 460 core

in vec4 vColor;
out vec4 fragColor;

void main() {
    vec2 circCoord = 2.0 * gl_PointCoord - 1.0;
    if (dot(circCoord, circCoord) > 1.0) {
        discard;
    }
    fragColor = vColor;
}
