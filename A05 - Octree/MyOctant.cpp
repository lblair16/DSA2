#include "MyOctant.h"
using namespace Simplex;
uint MyOctant::m_uOctantCount = 0;
uint MyOctant::m_uMaxLevel = 3;
uint MyOctant::m_uIdealEntityCount = 5;
uint MyOctant::GetOctantCount(void) { return m_uOctantCount; }
void MyOctant::Init(void) {
	m_uChildren = 0;

	// init vars
	m_fSize = 0.0f;
	m_uID = m_uOctantCount;
	m_uLevel = 0;

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	// get instances of mesh manager and entity manager
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	// set nodes to null pointers
	m_pRoot = nullptr;
	m_pParent = nullptr;
	for (uint n = 0; n < 8; n++) {
		m_pChild[n] = nullptr;
	}
}
void MyOctant::Swap(MyOctant& other) {
	std::swap(m_uChildren, other.m_uChildren);

	// swap vars between octants
	std::swap(m_fSize, other.m_fSize);
	std::swap(m_uID, other.m_uID);
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_lChild, other.m_lChild);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);

	// set the mesh manager and entity manager
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	// swap nodes
	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_pParent, other.m_pParent);
	for (uint i = 0; i < 8; i++) {
		std::swap(m_pChild[i], other.m_pChild[i]);
	}
}
MyOctant * MyOctant::GetParent(void) { return m_pParent; }
void MyOctant::Release(void) {
	//kill branches and reset vars
	if (m_uLevel == 0) {
		KillBranches();
	}
	m_uChildren = 0;
	m_fSize = 0.0f;
	m_EntityList.clear();
	m_lChild.clear();
}
// The big 3
MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount) {
	// init vars
	Init();
	m_uOctantCount = 0;
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
	m_uID = m_uOctantCount;
	m_pRoot = this;
	m_lChild.clear();
	std::vector<vector3> pointList;
	uint Entities = m_pEntityMngr->GetEntityCount();

	// add global min and max of entities to list
	for (uint i = 0; i < Entities; i++) {
		MyEntity* entity = m_pEntityMngr->GetEntity(i);
		MyRigidBody* rigidBody = entity->GetRigidBody();
		pointList.push_back(rigidBody->GetMinGlobal());
		pointList.push_back(rigidBody->GetMaxGlobal());
	}

	// create a new rigidbody
	MyRigidBody* rigidBody = new MyRigidBody(pointList);
	vector3 halfWidth = rigidBody->GetHalfWidth();
	float fMax = halfWidth.x;

	for (int i = 1; i < 3; i++) {
		if (fMax < halfWidth[i])
			fMax = halfWidth[i];
	}

	// set the center of the rigidbody and delete it
	vector3 v3Center = rigidBody->GetCenterLocal();
	pointList.clear();
	SafeDelete(rigidBody);

	// set the member variables
	m_fSize = fMax * 2.0f;
	m_v3Center = v3Center;
	m_v3Min = m_v3Center - (vector3(fMax));
	m_v3Max = m_v3Center + (vector3(fMax));

	//increase octant count and create tree
	m_uOctantCount++;
	ConstructTree(m_uMaxLevel);
}
MyOctant::MyOctant(vector3 a_v3Center, float a_fSize) {
	// initialize vars
	Init();
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	// set member variables
	m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);
	m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);

	// increase octant count
	m_uOctantCount++;
}
MyOctant::MyOctant(MyOctant const& other) {
	// set vars to the other octant's values
	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;

	m_fSize = other.m_fSize;
	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;

	m_uChildren = other.m_uChildren;
	m_pParent = other.m_pParent;
	m_pRoot, other.m_pRoot;
	m_lChild, other.m_lChild;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	for (uint i = 0; i < 8; i++) {
		m_pChild[i] = other.m_pChild[i];
	}
}
MyOctant& MyOctant::operator=(MyOctant const& other) {
	//don't perform if the octant is the same
	if (this != &other) {
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}
MyOctant::~MyOctant() { Release(); };
//Accessors
float MyOctant::GetSize(void) { return m_fSize; }
vector3 MyOctant::GetCenterGlobal(void) { return m_v3Center; }
vector3 MyOctant::GetMinGlobal(void) { return m_v3Min; }
vector3 MyOctant::GetMaxGlobal(void) { return m_v3Max; }
void MyOctant::Display(uint a_nIndex, vector3 a_v3Color) {
	// display correct octant based on ID
	if (m_uID == a_nIndex) {
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
			glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
		return;
	}

	//continue to search for correct octant to display
	for (uint i = 0; i < m_uChildren; i++) {
		m_pChild[i]->Display(a_nIndex);
	}
}
void MyOctant::Display(vector3 a_v3Color) {

	for (uint i = 0; i < m_uChildren; i++) {
		m_pChild[i]->Display(a_v3Color);
	}
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
}
void MyOctant::Subdivide(void) {
	// if max level return
	if (m_uLevel >= m_uMaxLevel)
		return;

	//dont subdivide octants that have been subdivided already
	if (m_uChildren != 0)
		return;

	// create childern octants
	m_uChildren = 8;

	float sizeH = m_fSize / 4.0f;
	float sizeF = sizeH * 2.0f;
	vector3 center;
	// bottom left back octant
	center = m_v3Center;
	center.x -= sizeH;
	center.y -= sizeH;
	center.z -= sizeH;
	m_pChild[0] = new MyOctant(center, sizeF);

	// bottom right back octant
	center.x += sizeF;
	m_pChild[1] = new MyOctant(center, sizeF);

	// bottom right front octant
	center.z += sizeF;
	m_pChild[2] = new MyOctant(center, sizeF);

	// bottom left front octant
	center.x -= sizeF;
	m_pChild[3] = new MyOctant(center, sizeF);

	// top left front octant
	center.y += sizeF;
	m_pChild[4] = new MyOctant(center, sizeF);

	// top left back octant
	center.z -= sizeF;
	m_pChild[5] = new MyOctant(center, sizeF);

	// top right back octant
	center.x += sizeF;
	m_pChild[6] = new MyOctant(center, sizeF);

	// top right front octant
	center.z += sizeF;
	m_pChild[7] = new MyOctant(center, sizeF);

	// set root, parent, and level
	// subdivide further if needed
	for (uint i = 0; i < 8; i++) {
		m_pChild[i]->m_pRoot = m_pRoot;
		m_pChild[i]->m_pParent = this;
		m_pChild[i]->m_uLevel = m_uLevel + 1;
		if (m_pChild[i]->ContainsMoreThan(m_uIdealEntityCount)) {
			m_pChild[i]->Subdivide();
		}
	}
}
MyOctant * MyOctant::GetChild(uint a_nChild) {
	if (a_nChild > 7) return nullptr;
	return m_pChild[a_nChild];
}
bool MyOctant::IsColliding(uint a_uRBIndex) {
	uint objects = m_pEntityMngr->GetEntityCount();
	// return false for invalid index
	if (a_uRBIndex >= objects)
		return false;

	// get global max and min of entity
	MyEntity* entity = m_pEntityMngr->GetEntity(a_uRBIndex);
	MyRigidBody* rigidBody = entity->GetRigidBody();
	vector3 min = rigidBody->GetMinGlobal();
	vector3 max = rigidBody->GetMaxGlobal();

	// check x/y/z values for a valid collision
	if (m_v3Max.x < min.x)
		return false;
	if (m_v3Min.x > max.x)
		return false;

	if (m_v3Max.y < min.y)
		return false;
	if (m_v3Min.y > max.y)
		return false;

	if (m_v3Max.z < min.z)
		return false;
	if (m_v3Min.z > max.z)
		return false;

	// if you're here then there is a collision
	return true;
}
bool MyOctant::IsLeaf(void) { return m_uChildren == 0; }
bool MyOctant::ContainsMoreThan(uint a_nEntities) {
	uint count = 0;
	uint objects = m_pEntityMngr->GetEntityCount();
	// checks if there are too many objects in one octant
	for (uint n = 0; n < objects; n++) {
		if (IsColliding(n))
			count++;
		if (count > a_nEntities)
			return true;
	}
	return false;
}
void MyOctant::KillBranches(void) {
	// recursively kill branches
	for (uint i = 0; i < m_uChildren; i++) {
		m_pChild[i]->KillBranches();
		delete m_pChild[i];
		m_pChild[i] = nullptr;
	}
	m_uChildren = 0;
}
void MyOctant::DisplayLeafs(vector3 a_v3Color) {
	uint leaves = m_lChild.size();
	//display all leaves
	for (uint i = 0; i < leaves; i++) {
		m_lChild[i]->DisplayLeafs(a_v3Color);
	}
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
}
void MyOctant::ClearEntityList(void) {
	for (uint i = 0; i < m_uChildren; i++) {
		m_pChild[i]->ClearEntityList();
	}
	m_EntityList.clear();
}
void MyOctant::ConstructTree(uint a_nMaxLevel) {
	//only do this from the root
	if (m_uLevel != 0)
		return;

	// set octant count and max level count
	m_uMaxLevel = a_nMaxLevel;
	m_uOctantCount = 1;

	// clear entity list
	m_EntityList.clear();

	// clear tree
	KillBranches();
	m_lChild.clear();

	// subdivide if too many entities
	if (ContainsMoreThan(m_uIdealEntityCount)) {
		Subdivide();
	}

	// aassign the id of which octant the entity is in
	AssignIDtoEntity();

	// construct the list
	ConstructList();
}
void MyOctant::AssignIDtoEntity(void) {
	// make sure to perform this on all leaves
	for (uint i = 0; i < m_uChildren; i++) {
		m_pChild[i]->AssignIDtoEntity();
	}
	// found a leaf
	if (m_uChildren == 0) {
		uint objects = m_pEntityMngr->GetEntityCount();
		for (uint i = 0; i < objects; i++) {
			if (IsColliding(i)) {
				m_EntityList.push_back(i);
				m_pEntityMngr->AddDimension(i, m_uID);
			}
		}
	}
}
void MyOctant::ConstructList(void) {
	// recursively construct list for each child of octree
	for (uint i = 0; i < m_uChildren; i++) {
		m_pChild[i]->ConstructList();
	}
	if (m_EntityList.size() > 0) {
		m_pRoot->m_lChild.push_back(this);
	}
}
