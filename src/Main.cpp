#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h> 
#include <fstream>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);


static std::string ParseFile(const std::string filePath) {
    std::ifstream file(filePath);
    std::string str;
    std::string content;
    while (std::getline(file, str)) {
        content.append(str + "\n");
    }
    return content;
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (!result) 
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return -1;

    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main()
{
    // ============================= CRIAÇÃO DA JANELA =============================
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Linha comentada para o opengl funcionar sem o VAO por enquanto
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Learning OpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    // ============================= FIM DA CRIAÇÃO DA JANELA =============================

    // ============================= CARREGA O OPENGL COM GLAD =============================

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }  

    // ============================= CARREGA O OPENGL COM GLAD =============================

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  



    //Vertices do triângulo
    float vertices[] = {
        -0.5f, -0.5f, //Bottom Left
         0.5f, -0.5f, //Bottom right
         0.5f,  0.5f, // Top
         -0.5f,  0.5f,
    };


    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };



    //Variavel que vai guardar o id do buffer
    //A Vertex Buffer Object (VBO) is an OpenGL feature 
    // that allows you to store vertex data—such as positions, colors, normals, 
    // and texture coordinates—directly in the GPU's video memory. 
    unsigned int VBO;


    //A Vertex Array Object (VAO) in OpenGL is a container object that stores the state needed for rendering, 
    // specifically describing how vertex data in Vertex Buffer Objects (VBOs) is formatted and accessed
    //unsigned int VAO;
    //glGenVertexArrays(1, &VAO);
    //glBindVertexArray(VAO);


    //Gera o buffer e guarda o id na variavel buffer
    glGenBuffers(1, &VBO);

    //Bind o buffer para tudo que for feito a partir de agora use esse buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //Envia o buffer pra gpu
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Ativa o atributo posição no indice 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);


    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::string vertexShader = ParseFile(std::string(RES_PATH) + "shaders/GL_basic-vert.shader");
    std::string fragmentShader = ParseFile(std::string(RES_PATH) + "shaders/GL_basic-frag.shader");

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    while(!glfwWindowShouldClose(window))
    {
        //Input
        processInput(window);


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
      
        //Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}


