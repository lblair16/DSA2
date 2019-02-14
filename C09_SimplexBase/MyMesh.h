#pragma once
#ifndef __MYMESHCLASS_H_
#define __MYMESHCLASS_H_

#include"Definitions.h"

class MyMesh {
public:
	
	GLuint m_uShaderProgramID = 0; //Shader program
	GLuint m_uVAO = 0; //Vertex Array Object
	GLuint m_uVBO = 0; //Vertex Buffer Object

	std::vector<vector3> m_lVertex;	//list of vertex

	std::vector<vector3> m_lVertexPos;	//list of vertex
	std::vector<vector3> m_lVertexCol;	//list of vertex

	MyMesh();
	void Inti();

	void CompleteMesh();
	void AddPosition(vector3 a_v3Pos);
	void AddColor(vector3 a_v3Pos);
	void AddPosition(vector3 a_v3Pos);
	void CompileOpenGL3x();
};

#endif
