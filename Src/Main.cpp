#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<imgui/imgui.h>
#include<imgui/imgui_impl_glfw.h>
#include<imgui/imgui_impl_opengl3.h>

#include"Game.h"
#include"Game2D.h"

static double Xpos = 0;
static double Ypos = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    Xpos = xpos;
    Ypos = ypos;
}

int main()
{
    int WIDTH = 1920;
    int HEIGHT = 1080;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //  initialize the GLFW window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Shooter", NULL, NULL);

   

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    /// load the opengl specific feature with glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // call backs
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);
   // glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float lastTime = 0;
    Game2D game(WIDTH, HEIGHT, window);
    //Game game(WIDTH, HEIGHT, window);
    game.Init();


    while (!glfwWindowShouldClose(window))
    {

        /// <summary>
        /// clear color bit
        /// </summary>
        /// <returns></returns>
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float delta = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        game.mouse_callback(Xpos, Ypos);

        // handle inputs
        game.ProcessInput(glfwGetTime(),delta);
        // handle game spatial update

        game.Update(glfwGetTime(),delta);
        
        // handle draw calls
        game.Render(glfwGetTime(), delta);



        // Render dear imgui into screen

        // swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    game.CleanUp();
    glfwTerminate();
    return 0;
}

