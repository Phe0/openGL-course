#include "Triangle.hpp"

// vertex shader
static const char* vShader = R"""(
    #version 330

    layout(location = 0) in vec3 pos;

    out vec4 vCol;

    uniform mat4 model;
    uniform mat4 projection;

    void main() {
        gl_Position = projection * model * vec4(pos, 1.0);
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

Triangle::Triangle() {
    width = 800;
    height = 600;
}

void Triangle::startup() {

    glEnable(GL_DEPTH_TEST);

	this->createTriangle();
	this->compileShaders();

    this->uniformModel = glGetUniformLocation(this->program, "model");
    this->uniformProjection = glGetUniformLocation(this->program, "projection");

    this->projection = glm::perspective(45.0f, (GLfloat)this->bufferWidth / (GLfloat)this->bufferHeight, 0.1f, 100.0f);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(this->program);

    glm::mat4 model = glm::mat4(1.0f);
    model =
        glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f)) * 
        glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

    glUniformMatrix4fv(this->uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(this->uniformProjection, 1, GL_FALSE, glm::value_ptr(this->projection));

    meshList[0]->renderMesh();

    glUseProgram(0);
}

void Triangle::finish() {

}

void Triangle::createTriangle() {

    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    Mesh* obj1 = new Mesh();
    obj1->createMesh(vertices, indices, 12, 12);
    meshList.push_back(obj1);
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