
// std
#include <iostream>
#include <string>
#include <chrono>

// glm
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "application.hpp"
#include "cgra/cgra_gui.hpp"
#include "cgra/cgra_shader.hpp"
#include "imgui.h"




using namespace std;
using namespace cgra;
using namespace glm;


Application::Application(GLFWwindow *window) : m_window(window) {
	
	// build the shader
	shader_builder color_sb;
	color_sb.set_shader(GL_VERTEX_SHADER, CGRA_SRCDIR + std::string("//res//shaders//default_vert.glsl"));
	color_sb.set_shader(GL_FRAGMENT_SHADER, CGRA_SRCDIR + std::string("//res//shaders//default_frag.glsl"));
	m_shader = color_sb.build();

	//build the mesh for the triangle
    //m_model.setup();
}


void Application::render() {
	
	// retrieve the window hieght
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height); 

	m_windowsize = vec2(width, height); // update window size
	glViewport(0, 0, width, height); // set the viewport to draw to the entire window

	// clear the back-buffer
	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	// enable flags for normal/forward rendering
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LESS);


	// calculate the projection and view matrix 
	mat4 proj = perspective(1.f, float(width) / height, 0.1f, 1000.f);
	mat4 view = translate(mat4(1), vec3(0, -5, -20));


	// set shader and upload variables
	glUseProgram(m_shader);
	glUniformMatrix4fv(glGetUniformLocation(m_shader, "uProjectionMatrix"), 1, false, value_ptr(proj)); //this is where the value of the imgui comes from
	glUniformMatrix4fv(glGetUniformLocation(m_shader, "uModelViewMatrix"), 1, false, value_ptr(view));
	//test - Color Input - uniform location 
    glUniform3f(glGetUniformLocation(m_shader, "u_Color"), change_color.x, change_color.y, change_color.z ); //the shader program gets the values from this uniform location and passes it to the variable as 
																										     //"uniform vec4 u_Color;" in the frag shader program 

	//Light Input
	glUniform3f(glGetUniformLocation(m_shader, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
	//Lighting 
	//glUniform3f(glGetUniformLocation(m_shader, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
	//glUniform3f(glGetUniformLocation(m_shader, "objectColor"), objectColor.x, objectColor.y, objectColor.z);
	//glUniform3f(glGetUniformLocation(m_shader, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	//shader_builder m_shader.setVec3("lightPos", lightPos);

	
	//draw the model
	//m_model.draw();
	obj_model.draw();

}


void Application::renderGUI() {

	ImGui::CreateContext();


	// setup window
	ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(500, 190), ImGuiSetCond_Once);
	ImGui::Begin("Mesh loader", 0);
	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui::Text("Light Direction: ");
	ImGui::SliderFloat(" = x ", &lightColor.x, 0.0f, 1.0f);
	ImGui::SliderFloat(" = y ", &lightColor.y, 0.0f, 1.0f); 
	ImGui::SliderFloat(" = z ", &lightColor.z, 0.0f, 1.0f);


	//ImGui::ColorEdit3("Lighting", (float*)&lightColor);
	ImGui::ColorEdit3("Color", (float*)&change_color);



	// Loading buttons
	static char filename[512] = "";

	ImGui::InputText("", filename, 512); //ImGui input controls
	ImGui::SameLine();

	if (ImGui::Button("Load")) {
		// TODO load mesh from 'filename'
		obj_model.loadOBJ(filename);
		obj_model.build();	
	
	}

	ImGui::SameLine();
	if (ImGui::Button("Print")) {
		// TODO print mesh data
		obj_model.printMeshData();
	}

	ImGui::SameLine();
	if (ImGui::Button("Unload")) {
		// TODO unload mesh
		obj_model.destroy();
	}

	// finish creating window
	ImGui::End();
}


void Application::cursorPosCallback(double xpos, double ypos) {
	(void)xpos, ypos; // currently un-used


}


void Application::mouseButtonCallback(int button, int action, int mods) {
	(void)button, action, mods; // currently un-used
	std::cout << button << action << mods << endl;
}


void Application::scrollCallback(double xoffset, double yoffset) {
	(void)xoffset, yoffset; // currently un-used
}


void Application::keyCallback(int key, int scancode, int action, int mods) {
	(void)key, (void)scancode, (void)action, (void)mods; // currently un-used
}


void Application::charCallback(unsigned int c) {
	(void)c; // currently un-used
}