#include "Square.hpp"

// vertex shader
static const char* vShader = R"""(
    #version 330

    layout(location = 0) in vec3 pos;

    out vec4 vCol;

    uniform mat4 model;

    void main() {
        gl_Position = model * vec4(pos, 1.0);
        vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
    }
)""";

// fragment shader
static const char* fShader = R"""(
    #version 330

    in vec4 vCol;

    out vec4 colour;

    void main() {
        colour = vCol;
    }
)""";

float direction = true;
float offset = 0.0f;
float maxOffset = 0.7f;
float speed = 0.0005f;

float curAngle = 0.0f;

float sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

float const toRadians = 3.14159265f / 180.0f;

Square::Square() {
    width = 800;
    height = 600;
    vao = 0;
    vbo = 0;
}

void Square::startup() {
    this->createSquare();
    this->compileShaders();

    this->uniformModel = glGetUniformLocation(this->program, "model");
}

void Square::render(double time) {

    if (direction) {
        offset += speed;
    }
    else {
        offset -= speed;
    }

    if (abs(offset) >= maxOffset) {
        direction = !direction;
    }

    curAngle += 0.01f;

    if (curAngle >= 360) {
        curAngle -= 360;
    }

    if (sizeDirection) {
        curSize += 0.0001f;
    }
    else {
        curSize -= 0.0001f;
    }

    if (curSize >= maxSize || curSize <= minSize) {
        sizeDirection = !sizeDirection;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(this->program);

    glm::mat4 model = glm::mat4(1.0f);
    model =
        //glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)) *
        //glm::translate(model, glm::vec3(offset, 0.0f, 0.0f)) * 
        glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

    glUniformMatrix4fv(this->uniformModel, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glUseProgram(0);
}

void Square::finish() {

}

void Square::createSquare() {
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

void Square::compileShaders() {
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