#include "MyMesh.h"

void MyMesh::Inti()
{
	m_uShaderProgramID = 0; //Shader program
	m_uVAO = 0; //Vertex Array Object
	m_uVBO = 0; //Vertex Buffer Object
	m_uShaderProgramID = LoadShaders("Shaders//Basic.vs", "Shaders//BasicColor.fs");
}

void MyMesh::CompleteMesh()
{
	for (uint i = 0; i < m_lVertexPos.size(); i++) {
		m_lVertexCol.push_back(C_PURPLE);
	}
	for (uint i = 0; i < m_lVertexPos.size(); i++) {
		m_lVertex.push_back(C_PURPLE);
	}
}

void MyMesh::AddPosition(vector3 a_v3Pos)
{
	m_lVertexPos.push_back(a_v3Pos);
}

void MyMesh::AddColor(vector3 a_v3Pos)
{
	m_lVertexCol.push_back(a_v3Pos);
}

void MyMesh::CompileOpenGL3x()
{
	glGenVertexArrays(1, &m_uVAO);//Generate vertex array object
	glGenBuffers(1, &m_uVBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_uVAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_uVBO);//Bind the VBO

										  //Generate space for the VBO (vertex count times size of vec3)
	glBufferData(GL_ARRAY_BUFFER, m_lVertex.size() * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);

	//count the attributes
	int attributeCount = 2;

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attributeCount * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, attributeCount * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));
}
