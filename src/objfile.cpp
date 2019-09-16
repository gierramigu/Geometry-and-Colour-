#pragma once
#include "objfile.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <vector>
#include "glm/ext.hpp"

using namespace std;

/* Constructor */
ObjFile::ObjFile()
{
	//ctor

}

/* Destructor */
ObjFile::~ObjFile()
{
}

/* Reading the file and loading the object (obj file) */
void ObjFile::loadOBJ(const char *filename) {

	ifstream in(filename, std::ios::in);
	//file open validation
	if (!in) {
		std::cerr << "Error on reading file! :( " << filename << std::endl;
		exit(1);
	}


	string line;
	while (getline(in, line)) {

		//cout << "First Line: " << line << std::endl;
		//cout << line.substr(0, 1) << endl;

		if (line.substr(0, 2) == "v ") {
			istringstream v(line.substr(2));
			glm::vec3 vertex;
			float x, y, z;
			v >> x; v >> y; v >> z;
			vertex = glm::vec3(x, y, z);
			temp_vertices.push_back(vertex);
			//cout << "Vertices: " << x << " " << y << " " << z <<  endl;
		}
		else if (line.substr(0, 2) == "vt") {
			istringstream v(line.substr(3));
			glm::vec2 uv;
			float U, V;
			v >> U; v >> V;
			uv = glm::vec2(U, V);
			temp_uvs.push_back(uv);
			//cout << "Uv: " << U << " " << V << " " << endl;
		}
		else if (line.substr(0, 2) == "vn") {
			istringstream v(line.substr(2));
			glm::vec3 normal;
			float normalx, normaly, normalz;
			v >> normalx; v >> normaly; v >> normalz;
			normal = glm::vec3(normalx, normaly, normalz);
			temp_normals.push_back(normal);
			//cout << "normals: " << normalx << " " << normaly << " " << normalz << endl;
		}
		else if (line.substr(0, 2) == "f ") {
			

			int v1, v2, v3; //to store pos index
			int vt1, vt2, vt3; //store uv index
			int vn1, vn2, vn3; //to store normal index 

			const char* chh = line.c_str();

			//Reads the starting line on f and stores the corresponding values in the variables
			int number = sscanf_s(chh, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);

			v1--; v2--; v3--;
			vt1--; vt2--; vt3--;
			vn1--; vn2--; vn3--;

			//current: which works
			temp_Indices.push_back(v1);
			temp_Indices.push_back(v2);
			temp_Indices.push_back(v3);

			//obj_normals has to be the same size as the vertices 
			obj_normals.resize(temp_vertices.size());

			//each normal index has to correspond for each vertex position 
			obj_normals.at(v1) = temp_normals[vn1];
			obj_normals.at(v2) = temp_normals[vn2];
			obj_normals.at(v3) = temp_normals[vn3];
		
			/*temp_Indices.push_back(vt1);
			temp_Indices.push_back(vt2);
			temp_Indices.push_back(vt3);
			temp_Indices.push_back(vn1);
			temp_Indices.push_back(vn2);
			temp_Indices.push_back(vn3);*/


			//debugging
			//cout << number << endl;
			//cout << "Vertex Index: " << v1 << " " << v2 << " " << v3 << endl;
			//cout << "UV Index: " << v1 << " " <<  v2 << " "<< v3 << endl;
			//cout << "Normal Index: " << v1 << " " << v2 << " " << v3 << endl;

		}

		obj_positions = temp_vertices;
		//obj_normals = temp_normals;
		obj_uvs = temp_uvs;
		obj_Indices = temp_Indices;

		
		

		//Mesh and other data is calculated through this loop (doesn't work)
		/*for (unsigned int i = 0; i < temp_Indices.size(); i++) {
			/*glm::vec3 meshdata;
			glm::vec2 texturedata;
			glm::vec3 normaldata;
			meshdata = glm::vec3(temp_vertices[vertex_Indices[i]].x, temp_vertices[vertex_Indices[i]].y, temp_vertices[vertex_Indices[i]].z);
			texturedata = glm::vec2(temp_uvs[uv_Indices[i]].x, temp_uvs[uv_Indices[i]].y);
			normaldata = glm::vec3(temp_normals[normal_Indices[i]].x, temp_normals[normal_Indices[i]].y, temp_normals[normal_Indices[i]].z);
			obj_positions.push_back(meshdata);
			obj_uvs.push_back(texturedata);
			obj_normals.push_back(normaldata);
			//cout << "obj_normals vector: " << obj_positions.size() << endl;
		}*/
		//in.close();
	}
}

void ObjFile::build() {

	/* Build the buffers for GPU */
	if (obj_vao == 0 ) {
		
		//generate the buffers
		glGenVertexArrays(1, &obj_vao);
		glGenBuffers(1, &obj_vbo_pos);
		glGenBuffers(1, &obj_vbo_norm);
		glGenBuffers(1, &obj_vbo_uv);

		//IBO: stores indices which makes up primitives 
		glGenBuffers(1, &obj_ibo);
	

		//VAO
		glBindVertexArray(obj_vao);


		//bind and insert position vertices into this buffer
		glBindBuffer(GL_ARRAY_BUFFER, obj_vbo_pos);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * obj_positions.size(), obj_positions.data(), GL_STATIC_DRAW);
		//this buffer will use location = 0 when we use our VAO
		glEnableVertexAttribArray(0);
		//Is a pointer which iterates through the buffer and allows opengl to know which attribute (normals, pos, uvs) is being used
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		
		//create buffer for normal coordinates 
		glBindBuffer(GL_ARRAY_BUFFER, obj_vbo_norm);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * obj_normals.size(), obj_normals.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr); //the stride is going to the next vertex not the next attrib
	

		//create buffer for uv coordinates 
		glBindBuffer(GL_ARRAY_BUFFER, obj_vbo_uv);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * obj_uvs.size(), obj_uvs.data(), GL_STATIC_DRAW);
		//telling opengl what the layout is of the buffer
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	

		//Index buffer = being able to reuse existing vertices since gpu has limited space in memory 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj_ibo);
		//upload vertices to draw primitives
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * obj_Indices.size(), obj_Indices.data(), GL_STATIC_DRAW);

	
		glBindVertexArray(0);

	}
}

void ObjFile::draw() {

	if (obj_vao == 0) return; 
	// bind our VAO which sets up all our buffers and data for us
	glBindVertexArray(obj_vao);
	// tell opengl to draw our VAO using the draw mode and how many verticies to render v
	glDrawElements(GL_TRIANGLES, obj_Indices.size(), GL_UNSIGNED_INT, 0 );

}

void ObjFile::destroy() {

	// delete the vertex buffers
	glDeleteVertexArrays(1, &obj_vao);
	glDeleteBuffers(1, &obj_vbo_pos);
	glDeleteBuffers(1, &obj_vbo_norm);
	glDeleteBuffers(1, &obj_ibo);
	obj_vao = 0;
}


void ObjFile::printMeshData() {

	 /* For debugging purposes; printing out the raw mesh data */

	//Vertices
	for (int i = 0; i < temp_vertices.size(); i++) {
		std::cout << "Vertex: " << temp_vertices[i].x << " " << temp_vertices[i].y << " " << temp_vertices[i].z << endl ;
	}

	//Uvs
	for (int i = 0; i < temp_uvs.size(); i++) {
		std::cout << "Uvs: " << temp_uvs[i].x << " " << temp_uvs[i].y << endl;
	}

	//Normal
	for (int i = 0; i < temp_normals.size(); i++) {
		std::cout << "Normals: " << temp_normals[i].x << " " << temp_normals[i].y << " " << temp_normals[i].z << endl;
	}


	//Face Values
	cout << "------------------------------------- Faces ---------------------------------------" << endl;
	for (std::vector<unsigned int>::const_iterator i = obj_Indices.begin(); i != obj_Indices.end(); ++i) {
	   cout << " " << *i << "" ;
	}
	 
}

