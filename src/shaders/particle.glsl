#shader vertex
#version 460 core

layout(location = 0) in vec4 position;

void main() {
    gl_Position = position;
    gl_PointSize = 100.0;
}

#shader fragment
#version 460 core

out vec4 fragColor;

void main() {
    vec2 circCoord = 2.0 * gl_PointCoord - 1.0;
    if (dot(circCoord, circCoord) > 1.0) {
        discard;
    }
    fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
