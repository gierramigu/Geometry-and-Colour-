
#pragma once

// glm
#include <glm/glm.hpp>

// project
#include "opengl.hpp"
#include "triangle.hpp"
#include "objfile.h"
//#include "input/imgui_impl_glfw_gl3.h"


// Main application class
//
class Application {
private:
	// window
	glm::vec2 m_windowsize;
	GLFWwindow *m_window;

	// basic shader
	GLuint m_shader;

	//test triangle model
	//TestTriangle m_model;

	//Colour Input: below is the starting colour 
	glm::vec3 change_color = glm::vec3(0.45f, 0.55f, 0.60f);

	//Light Input: 
	glm::vec3 lightColor = glm::vec3( -1.0f, 1.0f, -1.0f);


	//For Lighting Slider 
	float f = 0.0f;

	//ObjFile model 
	ObjFile obj_model;

public:
	// setup
	Application(GLFWwindow *);

	// disable copy constructors (for safety)
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	// rendering callbacks (every frame)
	void render();
	void renderGUI();

	// input callbacks
	void cursorPosCallback(double xpos, double ypos);
	void mouseButtonCallback(int button, int action, int mods);
	void scrollCallback(double xoffset, double yoffset);
	void keyCallback(int key, int scancode, int action, int mods);
	void charCallback(unsigned int c);
};