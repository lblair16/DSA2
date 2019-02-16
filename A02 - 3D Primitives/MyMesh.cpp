#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}

std::vector<vector3> MyMesh::GenerateCircle(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color, float a_fZOffset)
{

	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	/*
	Calculate a_nSubdivisions number of points around a center point in a radial manner
	then call the AddTri function to generate a_nSubdivision number of faces
	*/
	//loop to calculate points and generate tris
	std::vector<vector3> points;
	for (int i = 0; i < a_nSubdivisions; i++) {

		//current angle
		float theta = 2.0f * 3.1415926f * (float)i / (float)a_nSubdivisions;

		//x component
		float x = a_fRadius * cosf(theta);
		//y component
		float y = a_fRadius * sinf(theta);

		points.push_back(vector3(x, y, a_fZOffset));

	}

	if (a_fZOffset == 0) {
		for (int i = 0; i < points.size(); i++) {
			if (i == points.size() - 1) {
				AddTri(vector3(0, 0, 0), points[i], points[0]);
			}
			else {
				AddTri(vector3(0, 0, 0), points[i], points[i + 1]);
			}

		}
	}
	else {
		for (int i = points.size()-1; i >= 0; i--) {
			if (i == points.size() - 1) {
				AddTri(vector3(0, 0, a_fZOffset), points[0], points[i]);
			}
			else {
				AddTri(vector3(0, 0, a_fZOffset), points[i+1], points[i]);
			}

		}
	}

	return points;
}



void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	//base
	std::vector<vector3> base = GenerateCircle(a_fRadius, a_nSubdivisions, a_v3Color, 0.0f);

	//connect base to tip
	for (int i = 0; i < base.size(); i++) {
		if (i == base.size() - 1) {
			AddTri(base[0], base[i], vector3(0, 0, -a_fHeight));
		}
		else {
			AddTri(base[i+1], base[i], vector3(0, 0, -a_fHeight));
		}
	}
	
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code

	//top
	std::vector<vector3> top = GenerateCircle(a_fRadius, a_nSubdivisions, a_v3Color, -a_fHeight);
	//bottom
	std::vector<vector3> bottom = GenerateCircle(a_fRadius, a_nSubdivisions, a_v3Color, 0.0f);
	

	//connect top to bottom
	for (int i = 0; i < bottom.size(); i++) {
		if (i == bottom.size() - 1) {
			AddQuad(top[i], top[0], bottom[i], bottom[0]);
		}
		else {
			AddQuad(top[i], top[i+1], bottom[i], bottom[i+1]);
		}
	}
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	//get the "circles" for all 4 rings
	std::vector<vector3> innerBottom;
	for (int i = 0; i < a_nSubdivisions; i++) {

		//current angle
		float theta = 2.0f * 3.1415926f * (float)i / (float)a_nSubdivisions;

		//x component
		float x = a_fInnerRadius * cosf(theta);
		//y component
		float y = a_fInnerRadius * sinf(theta);

		innerBottom.push_back(vector3(x, y, 0));

	}

	std::vector<vector3> outterBottom;
	for (int i = 0; i < a_nSubdivisions; i++) {

		//current angle
		float theta = 2.0f * 3.1415926f * (float)i / (float)a_nSubdivisions;

		//x component
		float x = a_fOuterRadius * cosf(theta);
		//y component
		float y = a_fOuterRadius * sinf(theta);

		outterBottom.push_back(vector3(x, y, 0));

	}

	std::vector<vector3> innerTop;
	for (int i = 0; i < a_nSubdivisions; i++) {

		//current angle
		float theta = 2.0f * 3.1415926f * (float)i / (float)a_nSubdivisions;

		//x component
		float x = a_fInnerRadius * cosf(theta);
		//y component
		float y = a_fInnerRadius * sinf(theta);

		innerTop.push_back(vector3(x, y, -a_fHeight));

	}

	std::vector<vector3> outterTop;
	for (int i = 0; i < a_nSubdivisions; i++) {

		//current angle
		float theta = 2.0f * 3.1415926f * (float)i / (float)a_nSubdivisions;

		//x component
		float x = a_fOuterRadius * cosf(theta);
		//y component
		float y = a_fOuterRadius * sinf(theta);

		outterTop.push_back(vector3(x, y, -a_fHeight));

	}

	//connect inner and outter bottom
	for (int i = 0; i < innerBottom.size(); i++) {
		if (i == innerBottom.size() - 1) {
			AddQuad(outterBottom[i], outterBottom[0], innerBottom[i], innerBottom[0]);
		}
		else {
			AddQuad(outterBottom[i], outterBottom[i+1], innerBottom[i], innerBottom[i+1]);
		}
	}

	//connect inner and outter top
	for (int i = 0; i < innerTop.size(); i++) {
		if (i == innerTop.size() - 1) {
			AddQuad(innerTop[i], innerTop[0], outterTop[i], outterTop[0]);
		}
		else {
			AddQuad(innerTop[i], innerTop[i + 1], outterTop[i], outterTop[i + 1]);
		}
	}

	//connect outer bottom to outer top
	for (int i = 0; i < outterBottom.size(); i++) {
		if (i == outterBottom.size() - 1) {
			AddQuad(outterTop[i], outterTop[0], outterBottom[i], outterBottom[0]);
		}
		else {
			AddQuad(outterTop[i], outterTop[i + 1], outterBottom[i], outterBottom[i + 1]);
		}
	}

	//connect inner bottom to inner top
	for (int i = 0; i < innerBottom.size(); i++) {
		if (i == innerBottom.size() - 1) {
			AddQuad(innerBottom[i], innerBottom[0], innerTop[i], innerTop[0]);
		}
		else {
			AddQuad(innerBottom[i], innerBottom[i + 1], innerTop[i], innerTop[i + 1]);
		}
	}
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code
	GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	// Replace this with your code
	//generate faces to subdivide
	//top
	std::vector<vector3> topFront;
	topFront.push_back(vector3(0, a_fRadius, 0));
	topFront.push_back(vector3(-a_fRadius,0 , a_fRadius));
	topFront.push_back(vector3(a_fRadius, 0, a_fRadius));
	
	Subdivide(topFront[0].x, topFront[0].y, topFront[0].z, topFront[1].x, topFront[1].y, topFront[1].z, topFront[2].x, topFront[2].y, topFront[2].z, a_nSubdivisions, a_fRadius);

	std::vector<vector3> topLeft;
	topLeft.push_back(vector3(0, a_fRadius, 0));
	topLeft.push_back(vector3(-a_fRadius, 0, -a_fRadius));
	topLeft.push_back(vector3(-a_fRadius, 0, a_fRadius));
	
	Subdivide(topLeft[0].x, topLeft[0].y, topLeft[0].z, topLeft[1].x, topLeft[1].y, topLeft[1].z, topLeft[2].x, topLeft[2].y, topLeft[2].z, a_nSubdivisions, a_fRadius);

	std::vector<vector3> topRight;
	topRight.push_back(vector3(0, a_fRadius, 0));
	topRight.push_back(vector3(a_fRadius, 0, a_fRadius));
	topRight.push_back(vector3(a_fRadius, 0, -a_fRadius));
	
	Subdivide(topRight[0].x, topRight[0].y, topRight[0].z, topRight[1].x, topRight[1].y, topRight[1].z, topRight[2].x, topRight[2].y, topRight[2].z, a_nSubdivisions, a_fRadius);

	std::vector<vector3> topBack;
	topBack.push_back(vector3(0, a_fRadius, 0));
	topBack.push_back(vector3(a_fRadius, 0, -a_fRadius));
	topBack.push_back(vector3(-a_fRadius, 0, -a_fRadius));
	
	Subdivide(topBack[0].x, topBack[0].y, topBack[0].z, topBack[1].x, topBack[1].y, topBack[1].z, topBack[2].x, topBack[2].y, topBack[2].z, a_nSubdivisions, a_fRadius);

	//bottom
	std::vector<vector3> bottomFront;
	bottomFront.push_back(vector3(0, -a_fRadius, 0));
	bottomFront.push_back(vector3(a_fRadius, 0, a_fRadius));
	bottomFront.push_back(vector3(-a_fRadius, 0, a_fRadius));
	Subdivide(bottomFront[0].x, bottomFront[0].y, bottomFront[0].z, bottomFront[1].x, bottomFront[1].y, bottomFront[1].z, bottomFront[2].x, bottomFront[2].y, bottomFront[2].z, a_nSubdivisions, a_fRadius);

	std::vector<vector3> bottomLeft;
	bottomLeft.push_back(vector3(0, -a_fRadius, 0));
	bottomLeft.push_back(vector3(-a_fRadius, 0, a_fRadius));
	bottomLeft.push_back(vector3(-a_fRadius, 0, -a_fRadius));
	Subdivide(bottomLeft[0].x, bottomLeft[0].y, bottomLeft[0].z, bottomLeft[1].x, bottomLeft[1].y, bottomLeft[1].z, bottomLeft[2].x, bottomLeft[2].y, bottomLeft[2].z, a_nSubdivisions, a_fRadius);


	std::vector<vector3> bottomRight;
	bottomRight.push_back(vector3(0, -a_fRadius, 0));
	bottomRight.push_back(vector3(a_fRadius, 0, -a_fRadius));
	bottomRight.push_back(vector3(a_fRadius, 0, a_fRadius));
	Subdivide(bottomLeft[0].x, bottomRight[0].y, bottomRight[0].z, bottomRight[1].x, bottomRight[1].y, bottomRight[1].z, bottomRight[2].x, bottomRight[2].y, bottomRight[2].z, a_nSubdivisions, a_fRadius);


	std::vector<vector3> bottomBack;
	bottomBack.push_back(vector3(0, -a_fRadius, 0));
	bottomBack.push_back(vector3(-a_fRadius, 0, -a_fRadius));
	bottomBack.push_back(vector3(a_fRadius, 0, -a_fRadius));
	Subdivide(bottomBack[0].x, bottomBack[0].y, bottomBack[0].z, bottomBack[1].x, bottomBack[1].y, bottomBack[1].z, bottomBack[2].x, bottomBack[2].y, bottomBack[2].z, a_nSubdivisions, a_fRadius);


	
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::Subdivide(float v1x, float v1y, float v1z,
	float v2x, float v2y, float v2z,
	float v3x, float v3y, float v3z,
	int level, float a_fRadius)
{
	if (level == 0) {
		//reached end of tesselation
		// ... and renormalize it to get a point on the sphere.
		float s = a_fRadius / sqrt(v1x * v1x + v1y * v1y + v1z * v1z);
		v1x *= s;
		v1y *= s;
		v1z *= s;

		s = a_fRadius / sqrt(v2x * v2x + v2y * v2y + v2z * v2z);
		v2x *= s;
		v2y *= s;
		v2z *= s;

		s = a_fRadius / sqrt(v3x * v3x + v3y * v3y + v3z * v3z);
		v3x *= s;
		v3y *= s;
		v3z *= s;
		AddTri(vector3(v1x, v1y, v1z), vector3(v2x, v2y, v2z), vector3(v3x, v3y, v3z));
	}
	else {
		//calculate the middle of each edge
		float v12x = 0.5f * (v1x + v2x);
		float v12y = 0.5f * (v1y + v2y);
		float v12z = 0.5f * (v1z + v2z);
		// ... and renormalize it to get a point on the sphere.
		float s = a_fRadius / sqrt(v12x * v12x + v12y * v12y + v12z * v12z);
		v12x *= s;
		v12y *= s;
		v12z *= s;

		// Same thing for the middle of the other two edges.
		float v13x = 0.5f * (v1x + v3x);
		float v13y = 0.5f * (v1y + v3y);
		float v13z = 0.5f * (v1z + v3z);
		s = a_fRadius / sqrt(v13x * v13x + v13y * v13y + v13z * v13z);
		v13x *= s;
		v13y *= s;
		v13z *= s;

		float v23x = 0.5f * (v2x + v3x);
		float v23y = 0.5f * (v2y + v3y);
		float v23z = 0.5f * (v2z + v3z);
		s = a_fRadius / sqrt(v23x * v23x + v23y * v23y + v23z * v23z);
		v23x *= s;
		v23y *= s;
		v23z *= s;

		// Make the recursive calls.
		Subdivide(v1x, v1y, v1z,
			v12x, v12y, v12z,
			v13x, v13y, v13z,
			level - 1, a_fRadius);
		Subdivide(v12x, v12y, v12z,
			v2x, v2y, v2z,
			v23x, v23y, v23z,
			level - 1, a_fRadius);
		Subdivide(v13x, v13y, v13z,
			v23x, v23y, v23z,
			v3x, v3y, v3z,
			level - 1, a_fRadius);
		Subdivide(v12x, v12y, v12z,
			v23x, v23y, v23z,
			v13x, v13y, v13z,
			level - 1, a_fRadius);

		
	}
}