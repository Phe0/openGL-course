#include "SpinnyCube.hpp"

const char* vShader =
	"#version 450\n"

	"in vec4 v_pos;"

	"out vec4 vs_color;"

	"uniform mat4 mv_matrix;"
	"uniform mat4 proj_matrix;"

	"void main() {"
	"  gl_Position = proj_matrix * mv_matrix * v_pos;"
	"  vs_color = v_pos * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);"
	"}";

const char* fShader =
	"#version 450\n"

	"out vec4 frag_col;"

	"in vec4 vs_color;"

	"void main() {"
	"  frag_col = vs_color;"
	"}";

SpinnyCube::SpinnyCube() {
    width = 700;
    height = 400;
    mv_location = 0;
    proj_location = 0;
    vao = 0;
    vbo = 0;
}

void SpinnyCube::startup() {

    glEnable(GL_DEPTH_TEST);

	this->program = glCreateProgram();

	this->addShader(GL_VERTEX_SHADER, vShader);
	this->addShader(GL_FRAGMENT_SHADER, fShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

	glLinkProgram(this->program);

    glGetProgramiv(this->program, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(this->program, sizeof(eLog), NULL, eLog);
        printf("Erro linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(this->program);
    glGetProgramiv(this->program, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(this->program, sizeof(eLog), NULL, eLog);
        printf("Erro validating program: '%s'\n", eLog);
        return;
    }

	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

    const GLfloat vertex_positions[] = {
            -0.25f,  0.25f, -0.25f,
            -0.25f, -0.25f, -0.25f,
             0.25f, -0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
             0.25f,  0.25f, -0.25f,
            -0.25f,  0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
             0.25f, -0.25f,  0.25f,
             0.25f,  0.25f, -0.25f,

             0.25f, -0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,
             0.25f,  0.25f, -0.25f,

             0.25f, -0.25f,  0.25f,
            -0.25f, -0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
            -0.25f,  0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
            -0.25f, -0.25f, -0.25f,
            -0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f, -0.25f,
            -0.25f,  0.25f, -0.25f,
            -0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
             0.25f, -0.25f,  0.25f,
             0.25f, -0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
            -0.25f, -0.25f, -0.25f,
            -0.25f, -0.25f,  0.25f,

            -0.25f,  0.25f, -0.25f,
             0.25f,  0.25f, -0.25f,
             0.25f,  0.25f,  0.25f,

             0.25f,  0.25f,  0.25f,
            -0.25f,  0.25f,  0.25f,
            -0.25f,  0.25f, -0.25f
    };

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    this->mv_location = glGetUniformLocation(this->program, "mv_matrix");
    this->proj_location = glGetUniformLocation(this->program, "proj_matrix");
}

void SpinnyCube::render(double time) {

	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(this->program);

    glm::mat4 mv_matrix, proj_matrix;

    proj_matrix = glm::perspective(glm::radians(50.0f),
        (float)this->width / (float)this->height,
        0.1f, 1000.0f);

    glUniformMatrix4fv(this->proj_location, 1, GL_FALSE, glm::value_ptr(proj_matrix));

    int i = 0;

    for (i = 0; i < 24; i++) {
	    float f = (float)i + (float)time * 0.3f;

        mv_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f));

        mv_matrix = glm::rotate(mv_matrix,
            glm::radians((float)time * 45.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
        mv_matrix = glm::rotate(mv_matrix,
            glm::radians((float)time * 21.0f),
            glm::vec3(1.0f, 0.0f, 0.0f));

        mv_matrix = glm::translate(mv_matrix,
            glm::vec3(sinf(2.1f * f) * 2.0f,
                cosf(1.7f * f) * 2.0f,
                sinf(1.3f * f) * cosf(1.5f * f) * 2.0f));

        glUniformMatrix4fv(this->mv_location, 1, GL_FALSE, glm::value_ptr(mv_matrix));


        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void SpinnyCube::finish() {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteProgram(this->program);
    glDeleteBuffers(1, &this->vbo);
}