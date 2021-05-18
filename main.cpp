
#include "Rasterizer.h"
#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"
#include "SkyBoxShader.h"
#include "Material.h"
#include "Model.h"
#include "global.h"
#include "vertexAnimationShader.h"
#include "TstShader.h"
#include <thread>
#include <Windows.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Rasterizer *r = new Rasterizer(SCR_WIDTH, SCR_HEIGHT);
Camera* cam = new Camera();

int fps = 0;

void ShowFps(GLFWwindow* window)
{
    while (true)
    {
        Sleep(1000);
        std::string FPS = "Fps " + std::to_string(fps);
        glfwSetWindowTitle(window, FPS.c_str());
        fps = 0;
    }
}

int main()
{
    //Material bodyMat;
    //Texture bodyTexture;
    //bodyTexture.LoadTexture("Models/neptune/Texf_body02.jpg");
    //bodyMat.SetTexture(bodyTexture);

    //Material faceMat;
    //Texture faceTexture;
    //faceTexture.LoadTexture("Models/neptune/Tex002f_body01.jpg");
    //faceMat.SetTexture(faceTexture);

    //Material mouseMat;
    //Texture mouseTexture;
    //mouseTexture.LoadTexture("Models/neptune/Texf_mouse.jpg");
    //mouseMat.SetTexture(mouseTexture);

    //Material eyeMat;
    //Texture eyeTexture;
    //eyeTexture.LoadTexture("Models/neptune/Tex001f_eye.jpg");
    //eyeMat.SetTexture(eyeTexture);

    //Model model("Models/neptune/neptune.obj");
    //model.SetMaterial(0, mouseMat);
    //model.SetMaterial(1, faceMat);
    //model.SetMaterial(2, bodyMat);
    //model.SetMaterial(3, eyeMat);

    //-------------Ke Qing--------------------------
    Shader *KeqingShader = new TsTShader;

    Material KeHairMat;
    KeHairMat.SetShader(KeqingShader);
    Texture *KeHairTexture = new Texture;
    KeHairTexture->LoadTexture("Models/Keqing/tex/Hair.png");
    KeHairMat.SetTexture(KeHairTexture);

    Material KeClothMat;
    KeClothMat.SetShader(KeqingShader);
    Texture *KeClothTexture = new Texture;
    KeClothTexture->LoadTexture("Models/Keqing/tex/Cloth.png");
    KeClothMat.SetTexture(KeClothTexture);

    Material KeSkinMat;
    KeSkinMat.SetShader(KeqingShader);
    Texture *KeSkinTexture = new Texture;
    KeSkinTexture->LoadTexture("Models/Keqing/tex/Skin.png");
    KeSkinMat.SetTexture(KeSkinTexture);

    Material KeExpressionMat;
    KeExpressionMat.SetShader(KeqingShader);
    Texture *KeExpressionTexture = new Texture;
    KeExpressionTexture->LoadTexture("Models/Keqing/tex/Expression.png");
    KeExpressionMat.SetTexture(KeExpressionTexture);

    Material KeFaceMat;
    KeFaceMat.SetShader(KeqingShader);
    Texture *KeFaceTexture = new Texture;
    KeFaceTexture->LoadTexture("Models/Keqing/tex/Face.png");
    KeFaceMat.SetTexture(KeFaceTexture);

    Model Keqing("Models/Keqing/Keqing.obj");
    Keqing.SetMaterial(0, KeFaceMat);
    Keqing.SetMaterial(1, KeFaceMat);
    Keqing.SetMaterial(2, KeHairMat);
    Keqing.SetMaterial(3, KeFaceMat);
    Keqing.SetMaterial(4, KeFaceMat);
    Keqing.SetMaterial(5, KeFaceMat);
    Keqing.SetMaterial(6, KeFaceMat);
    Keqing.SetMaterial(7, KeFaceMat);
    Keqing.SetMaterial(8, KeHairMat);
    Keqing.SetMaterial(9, KeHairMat);
    Keqing.SetMaterial(10, KeClothMat);
    Keqing.SetMaterial(11, KeClothMat);
    Keqing.SetMaterial(12, KeClothMat);
    Keqing.SetMaterial(13, KeClothMat);
    Keqing.SetMaterial(14, KeClothMat);
    Keqing.SetMaterial(15, KeClothMat);
    Keqing.SetMaterial(16, KeSkinMat);
    Keqing.SetMaterial(17, KeClothMat);
    Keqing.SetMaterial(18, KeClothMat);

    //Material spotMat;
    //spotMat.SetShader(new vertexAnimationShader);
    //Texture spotTexture;
    //spotTexture.LoadTexture("Models/neptune/spot/spot_texture.png");
    //spotMat.SetTexture(spotTexture);
    //vertexAnimationShader* pureColorShader = new vertexAnimationShader();
    //spotMat.SetShader(pureColorShader);

    //Model spot("Models/neptune/spot/spot_triangulated_good.obj");
    //spot.SetMaterial(0, spotMat);

    Mesh box;
    box.CreatCube(0.0, 0.0, 0.0, 0.5);
    Material mat;
    Texture *boxt = new Texture;
    boxt->LoadTexture("Texture/mob.jpg");
    mat.SetTexture(boxt);
    Object Cube(box, mat);


    Mesh skyBox;
    skyBox.CreatCube(0.0, 0.0, 0.0, 0.5);

    Material skyBoxmat;
    TextureCUBE *skyBoxTex = new TextureCUBE;

    std::vector<std::string> TexPaths;
    TexPaths.push_back("Texture/skybox/front.jpg");
    TexPaths.push_back("Texture/skybox/back.jpg");

    TexPaths.push_back("Texture/skybox/top.jpg");
    TexPaths.push_back("Texture/skybox/bottom.jpg");

    TexPaths.push_back("Texture/skybox/left.jpg");
    TexPaths.push_back("Texture/skybox/right.jpg");

    skyBoxTex->LoadTexture(TexPaths);
    skyBoxmat.SetShader(new SkyBoxShader);
    skyBoxmat.SetTexture(skyBoxTex);
    Object skyBoxModel(skyBox, skyBoxmat);
    //加载模型

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
    //Mesh Cube;
    //Cube.CreatCube(0, 0.5, 0, 1);
    //Triangle MyFirstTriangle(V1, V2, V3);

    r->SetPerspectiveMatrix(GetPerspectiveMatrix(cam->GetFov(), (float)SCR_WIDTH / SCR_HEIGHT, 0.01, 100.f));
    r->SetViewPortMatrix(GetViewPortMatrix(SCR_WIDTH, SCR_HEIGHT));
    float angle = 0; 
    auto  Type = Rasterizer::DrawType::DrawTriangle;

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
    std::thread CntFps(ShowFps, window);
    CntFps.detach();
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        r->SeteyePoint(cam->GetPosition());
        glm::mat4 viewMat = GetViewMatrix(cam->GetPosition(), cam->GetFront(), cam->GetUp());
        r->SetViewMatrix(viewMat);
        r->ClearBuffer(glm::vec4(0.1, 0.35, 0.5, 1.0));

        r->SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0, -2, 0)));
        r->DrawObject(Cube, Type);

        glm::mat4 ModelMat = glm::translate(glm::mat4(1.0f), glm::vec3(2, -2, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.01, 0.01, 0.01));
        ModelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, -1, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0.1));
        r->SetModelMatrix(ModelMat);
        r->DrawModel(Keqing, Type);

        r->SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(3, 3, 3)));
        r->DrawObject(skyBoxModel, Type);

        /*ModelMat = RodriguesRotationFormula(glm::vec3(2, 0, 0), glm::vec3(0, 1, 0), 180) * glm::translate(glm::mat4(1.0f), glm::vec3(2, 0, 0));
        r->SetModelMatrix(ModelMat);
        r->DrawModel(spot, Type);*/

        r->Show();
        glfwSwapBuffers(window);
        ++fps;
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow* window)
{

#ifdef _DEBUG
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cam->pitch(5.5f);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cam->pitch(-5.5f);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cam->yaw(5.5f);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cam->yaw(-5.5f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam->MoveZ(1.1f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam->MoveZ(-1.1f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam->MoveX(-1.1f);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam->MoveX(1.1f);
#else
    float MoveSpeed = 0.04f;
    float RotationgSpeed = 1;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        MoveSpeed *= 2;
        RotationgSpeed = 4;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cam->pitch(RotationgSpeed);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cam->pitch(-RotationgSpeed);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cam->yaw(RotationgSpeed);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cam->yaw(-RotationgSpeed);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam->MoveZ(MoveSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam->MoveZ(-MoveSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam->MoveX(-MoveSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam->MoveX(MoveSpeed);
#endif
    
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    r->Setsize(width, height);
    r->SetPerspectiveMatrix(GetPerspectiveMatrix(cam->GetFov(), (float)width / height, 0.3, 100.f));
}