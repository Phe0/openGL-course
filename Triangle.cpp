#include "Triangle.hpp"

// vertex shader
static const char* vShader = R"""(
    #version 330

    layout(location = 0) in vec3 pos;

    uniform float xMove;

    void main() {
        gl_Position = vec4(0.4 * pos.x + xMove, 0.4 * pos.y, pos.z, 1.0);
    }
)""";

// fragment shader
static const char* fShader = R"""(
    #version 330
    out vec4 colour;
    void main() {
        colour = vec4(1.0, 0.0, 0.0, 1.0);
    }
)""";

float direction = true;
float offset = 0.0f;
float maxOffset = 0.7f;
float speed = 0.00005f;

Triangle::Triangle() {
    width = 800;
    height = 600;
    vao = 0;
    vbo = 0;
}

void Triangle::startup() {
	this->createTriangle();
	this->compileShaders();

    this->uniformXMove = glGetUniformLocation(this->program, "xMove");
}

void Triangle::render(double time) {

    if (direction) {
        offset += speed;
    }
    else {
        offset -= speed;
    }

    if (abs(offset) >= maxOffset) {
        direction = !direction;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(this->program);

    glUniform1f(this->uniformXMove, offset);

    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glUseProgram(0);
}

void Triangle::finish() {

}

void Triangle::createTriangle() {
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Triangle::compileShaders() {
    this->program = glCreateProgram();

    if (!this->program) {
        printf("Erro creating shader program!\n");
        return;
    }

    this->addShader(GL_VERTEX_SHADER, vShader);
    this->addShader(GL_FRAGMENT_SHADER, fShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(this->program);
    glGetProgramiv(this->program, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(this->program, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(this->program);
    glGetProgramiv(this->program, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(this->program, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }
}