#pragma once

//std 
#include <vector>

// glm
#include <glm/glm.hpp>

// project
#include "opengl.hpp"




using namespace std;


class ObjFile
{
private: 

	// CPU-side data
	// vectors (STL) to store the data
	std::vector<glm::vec3> obj_positions;
	std::vector<glm::vec2> obj_uvs;
	std::vector<glm::vec3> obj_normals;
	std::vector<unsigned int> vertex_Indices, uv_Indices, normal_Indices; //test
	std::vector<unsigned int> obj_Indices; //currently working 

	

	//temporary vectors 
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	std::vector<unsigned int> temp_Indices;

	// GPU-side data
	//VAO & VBO
	GLuint obj_vao = 0;
	GLuint obj_vbo_pos = 0;
	GLuint obj_vbo_norm = 0;
	GLuint obj_vbo_uv = 0;

	//IBO
	GLuint obj_ibo = 0;
	


public:

	ObjFile();
	~ObjFile();

	void loadOBJ(const char *filename);


	void build();
	void draw();
	void destroy(); 

	void printMeshData();
	
};

