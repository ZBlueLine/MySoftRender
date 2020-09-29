#include "global.h"
#include "Rasterizer.h"
#include "Mesh.h"
#include "Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Rasterizer *r = new Rasterizer(SCR_WIDTH, SCR_HEIGHT);
Camera* cam = new Camera();

int main()
{

    glm::vec3 Up(0, 1, 0);
    glm::vec3 Front(0, 0, 1);
    glm::vec3 UcF = cross(Up, Front);
    glfwInit();
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MySoftRender", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Vertex V1(glm::vec3(-0.5, 0.5, 0), glm::vec4(1, 0, 0, 1));
    Vertex V2(glm::vec3(0.5, 0.1, 0), glm::vec4(0, 1, 0, 1));
    Vertex V3(glm::vec3(0, -0.2, 0), glm::vec4(0, 0, 1, 1));
    Mesh Cube;
    Cube.CreatCube(0, 0.5, 0, 1);
    Triangle MyFirstTriangle(V1, V2, V3);

    r->SetPerspectiveMatrix(GetPerspectiveMatrix(cam->GetFov(), (float)SCR_WIDTH / SCR_HEIGHT, 0.3, 100.f));
    r->SetViewPortMatrix(GetViewPortMatrix(SCR_WIDTH, SCR_HEIGHT));
    float angle = 0; 
    auto  Type = Rasterizer::DrawType::DrawTriangle;
    r->LoadTexture("Texture/mob.jpg");
    std::cout << "是否显示模型线框？Y/N" << std::endl;
    while (1)
    {
        std::string Judge;
        std::cin >> Judge;
        if (Judge[0] == 'Y' || Judge[0] == 'y')
        {
            Type = Rasterizer::DrawType::DrawLine;
            break;
        }
        else if (Judge[0] == 'N'|| Judge[0] == 'n')
            break;
        else 
            std::cout << "输入Y或者N"<<std::endl;
    }
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        r->SetViewMatrix(GetViewMatrix(cam->GetPosition(), cam->GetFront(), cam->GetUp()));
        r->ClearBuffer(glm::vec4(0.1, 0.35, 0.5, 1.0));
        r->DrawMesh(Cube, Type);
        r->Show();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cam->pitch(2.5f);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cam->pitch(-2.5f);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cam->yaw(2.5f);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cam->yaw(-2.5f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam->MoveZ(0.08f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam->MoveZ(-0.08f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam->MoveX(-0.08f);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam->MoveX(0.08f);
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    r->Setsize(width, height);
    r->SetPerspectiveMatrix(GetPerspectiveMatrix(cam->GetFov(), (float)width / height, 0.3, 100.f));
}