g++ -std=c++11 Camera.cpp Light.cpp Mesh.cpp Shader.cpp main.cpp Renderer.cpp Texture.cpp Triangle.cpp -o main -L/usr/local/lib -lXrandr -lXrender -lXi -lGLEW -lGLU -lm -lGL -lpthread -pthread -lm -ldl -lXdamage -lXfixes -lX11-xcb -lxcb-glx -lxcb-dri2 -lXxf86vm -lXext -ldrm -lglfw3 -lrt -lm -ldl -lX11 -lpthread -lxcb -lXau -lXdmcp -I libs/GLM/include
