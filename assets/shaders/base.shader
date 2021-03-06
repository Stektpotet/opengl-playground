#shader vertex
#version 410

layout(location = 0) in vec4 position;
layout(LOCATION = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 vertex_color_from_program;

noperspective out vec2 texCoord;
smooth out vec4 vertex_color_out;
out vec4 pos;

uniform mat4 projection = mat4(1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1);

uniform float time = 0;

mat4 rotate(float x, float y, float z) {
    return mat4(
        (cos(y + z) + cos(y - z)) / 2,                                                                              (-sin(y + z) + sin(y - z)) / 2,                                                                             -sin(y),                        0,
        (cos(x + y + z) - cos(x - y + z) + cos(x + y - z) - cos(x - y - z) + 2 * sin(x + z) - 2 * sin(x - z)) / 4,  (2 * cos(x + z) + 2 * cos(x - z) - sin(x + y + z) + sin(x - y + z) + sin(x + y - z) - sin(x - y - z)) / 4,  (-sin(x + y) - sin(x - y)) / 2, 0,
        (-2 * cos(x + z) + 2 * cos(x - z) + sin(x + y + z) - sin(x - y + z) + sin(x + y - z) - sin(x - y - z)) / 4, (cos(x + y + z) - cos(x - y + z) - cos(x + y - z) + cos(x - y - z) + 2 * sin(x + z) + 2 * sin(x - z)) / 4,  (cos(x + y) + cos(x - y)) / 2,  0,
        0,                                                                                                          0,                                                                                                          0,                              1
    );
}
/*
}
mat4 modelToWorld(mat3 transform) {
    mat4 m = mat4(1); //ORIGINAL model representation
    mat4 scale = mat4(
        transform[0][0], 0,               0,               0,
        0,               transform[0][1], 0,               0,
        0,               0,               transform[0][2], 0,
        0,               0,               0,               1
    );

    m = scale * m;
    m = rotate(transform[1][0], transform[1][1], transform[1][2]) * m;

    mat4 translate = mat4(  1,               0,               0,               0,
                            0,               1,               0,               0,
                            0,               0,               1,               0,
                            transform[2][0], transform[2][1], transform[2][2], 0);
    m = translate * m;
    return m;
}*/




void main() {

// model to world space transformations = transform
// translation * rotation * scale * vertexPos;

    mat4 translation = mat4(    1,0,0,0,
                                0,1,0,0,
                                0,0,1,0,
                                0,0,-3,0);

    mat4 rot = mat4(0.525322, 0.000000, -0.850904, 0.000000,
                    0.000000, 1.000000, 0.000000, 0.000000,
                    0.850904, 0.000000, 0.525322, 0.000000,
                    0.000000, 0.000000, 0.000000, 1.000000);

    float F = sqrt(position.x*position.x + position.y*position.y + position.z*position.z);
    gl_Position = projection * translation *  rotate(time*0.1*F, time*0.333334*F, time*0.1666666667*F) * position;
    vertex_color_out = vertex_color_from_program;
    texCoord = uv;
    pos = gl_Position;
}

#shader fragment
#version 410
in vec4 gl_FragCoord;
in vec2 gl_PointCoord;
noperspective in vec2 texCoord;
smooth in vec4 vertex_color_out;
in vec4 pos;
out vec4 out_color;

uniform float time = 0;

uniform sampler2D mainTex;

void main() {
    out_color = vec4(texture(mainTex, texCoord).rgb * vertex_color_out.rgb, vertex_color_out.a);
}
//out_color = vec4(texture(mainTex, texCoord).rgb * vertex_color_out.rgb, vertex_color_out.a);
