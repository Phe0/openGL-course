#include "Triangle.h"

// vertex shader
static const char* vShader = "Shaders/shader.vert";

// fragment shader
static const char* fShader = "Shaders/shader.frag";

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
    this->createShaders();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.05f);

    this->brickTexture = Texture((char*)"Textures/brick.png");
    this->brickTexture.loadTexture();

    this->mainLight = Light(1.0f, 1.0f, 1.0f, 0.2f);

    this->projection = glm::perspective(45.0f, (GLfloat)this->bufferWidth / (GLfloat)this->bufferHeight, 0.1f, 100.0f);
}

void Triangle::render(double time) {

    this->camera.keyControl(this->keys, this->deltaTime);
    this->camera.mouseControl(this->getXChange(), this->getYChange());

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

    this->shaderList[0].useShader();
    this->uniformModel = shaderList[0].getModelLocation();
    this->uniformProjection = shaderList[0].getProjectionLocation();
    this->uniformView = shaderList[0].getViewLocation();
    this->uniformAmbientColor = shaderList[0].getAmbientColorLocation();
    this->uniformAmbientIntensity = shaderList[0].getAmbientIntensityLocation();

    mainLight.useLight(this->uniformAmbientIntensity, this->uniformAmbientColor);

    glm::mat4 model = glm::mat4(1.0f);
    model =
        glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f)) * 
        glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

    glUniformMatrix4fv(this->uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(this->uniformProjection, 1, GL_FALSE, glm::value_ptr(this->projection));
    glUniformMatrix4fv(this->uniformView, 1, GL_FALSE, glm::value_ptr(this->camera.claculateViewMatrix()));
    
    this->brickTexture.useTexture();

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
        -1.0f, -1.0f, 0.0f, 0.0f , 0.0f,
         0.0f, -1.0f, 1.0f, 0.5f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  1.0f, 0.0f, 0.5f, 1.0f
    };

    Mesh* obj1 = new Mesh();
    obj1->createMesh(vertices, indices, 20, 12);
    meshList.push_back(obj1);
}

void Triangle::createShaders() {
    Shader* shader1 = new Shader();
    shader1->createFromFiles(vShader, fShader);
    this->shaderList.push_back(*shader1);
}