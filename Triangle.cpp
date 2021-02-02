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
    width = 1366;
    height = 768;
}

void Triangle::startup() {

    glEnable(GL_DEPTH_TEST);

	this->createTriangle();
    this->createShaders();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.05f);

    this->brickTexture = Texture((char*)"Textures/brick.png");
    this->brickTexture.loadTexture();

    this->shinyMaterial = Material(1.0f, 32);
    this->dullMaterial = Material(0.3f, 4);

    this->mainLight = Light(1.0f, 1.0f, 1.0f, 0.2f, 2.0f, -1.0f, -2.0f, 0.3f);

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

    curAngle += 0.1f;

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
    this->uniformDirection = shaderList[0].getDirectionLocation();
    this->uniformDiffuseIntensity = shaderList[0].getDiffuseIntensityLocation();
    this->uniformEyePosition = shaderList[0].getEyePositionLocation();
    this->uniformSpecularIntensity = shaderList[0].getSpecularIntensityLocation();
    this->uniformShininess = shaderList[0].getShininessLocation();

    mainLight.useLight(this->uniformAmbientIntensity, this->uniformAmbientColor, this->uniformDiffuseIntensity, this->uniformDirection);

    glUniformMatrix4fv(this->uniformProjection, 1, GL_FALSE, glm::value_ptr(this->projection));
    glUniformMatrix4fv(this->uniformView, 1, GL_FALSE, glm::value_ptr(this->camera.claculateViewMatrix()));
    glUniform3f(this->uniformEyePosition, this->camera.getCameraPosition().x, this->camera.getCameraPosition().y, this->camera.getCameraPosition().z);

    glm::mat4 model = glm::mat4(1.0f);
    model =
        glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f)) * 
        glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

    glUniformMatrix4fv(this->uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    
    this->brickTexture.useTexture();
    this->dullMaterial.useMaterial(this->uniformSpecularIntensity, this->uniformShininess);

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
    //   x      y     z     u     v     nx    ny    nz
        -1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.0f, -1.0f,  1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.0f,  1.0f,  0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f,
    };

    calcAverageNormals(indices, 12, vertices, 32, 8, 5);

    Mesh* obj1 = new Mesh();
    obj1->createMesh(vertices, indices, 32, 12);
    meshList.push_back(obj1);
}

void Triangle::createShaders() {
    Shader* shader1 = new Shader();
    shader1->createFromFiles(vShader, fShader);
    this->shaderList.push_back(*shader1);
}

void Triangle::calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticesCount, unsigned int vLength, unsigned int normalOffset) {

    for (size_t i = 0; i < indiceCount; i += 3) {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;

        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);

        normal = glm::normalize(normal);

        in0 += normalOffset;
        in1 += normalOffset;
        in2 += normalOffset;

        vertices[in0] += normal.x;
        vertices[in0 + 1] += normal.y;
        vertices[in0 + 2] += normal.z;

        vertices[in1] += normal.x;
        vertices[in1 + 1] += normal.y;
        vertices[in1 + 2] += normal.z;

        vertices[in2] += normal.x;
        vertices[in2 + 1] += normal.y;
        vertices[in2 + 2] += normal.z;
    }

    for (size_t i = 0; i < verticesCount / vLength; i++) {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x;
        vertices[nOffset + 1] = vec.y;
        vertices[nOffset + 2] = vec.z;
    }
}