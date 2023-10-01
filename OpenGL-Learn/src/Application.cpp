#include<GL/glew.h>
#include <GLFW/glfw3.h>

#include<iostream>
#include<fstream>
#include <string>
#include<sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include"Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

//#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests//TestTexture2D.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const float width = 960.0f;
	const float height = 540.0f;


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwSwapInterval(1);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "ERROR!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	{

		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));


		Renderer renderer;

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui::StyleColorsDark();
		const char* glsl_version = "#version 130";
		ImGui_ImplOpenGL3_Init(glsl_version);

	   test::Test* currentTest = nullptr;
	   test::TestMenu* testMenu = new test::TestMenu(currentTest);
	   currentTest = testMenu;

	   testMenu->RegisterTest<test::TestClearColor>("Clear Color");
	   testMenu->RegisterTest<test::TestTexture2D>("2D Texture");

		while (!glfwWindowShouldClose(window))
		{

			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			renderer.Clear();


			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (currentTest)
			{
				currentTest->OnUpdate(0.0f);
				currentTest->OnRender();
				ImGui::Begin("Test");
				if (currentTest != testMenu && ImGui::Button("Back"))
				{
					delete currentTest;
					currentTest = testMenu;
				}
				currentTest->OnImGuiRender();
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		delete currentTest;
		if (currentTest != testMenu)
			delete testMenu;
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}