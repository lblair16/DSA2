#include "AppClass.h"

void Application::InitVariables(void)
{
	//init the mesh
	m_pMesh = new MyMesh();
	//m_pMesh->GenerateCube(1.0f, C_WHITE);
	m_pMesh->GenerateCube(.5, C_BLACK);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	static float value = 0.0f;
	matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value, 0.0f, 0.0f));
	value += 0.01f;
	
	//matrix4 m4Model = m4Translate * m4Scale;
	matrix4 m4Model = m4Translate;
	//center block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	matrix4 m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-.5f, 0.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-1.0f, 0.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);
	
	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-1.5f, 0.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);
	
	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-2.0f, 0.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);
	
	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-2.5f, 0.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(.5f, 0.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(1.0f, 0.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(1.5f, 0.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(2.0f, 0.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(2.5f, 0.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(0.0f, -0.5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-0.5f, -0.5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-1.0f, -0.5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-1.5f, -0.5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-2.5f, -0.5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(0.5f, -0.5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(1.0f, -0.5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(1.5f, -0.5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(2.5f, -0.5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-1.5f, -1.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-2.5f, -1.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(1.5f, -1.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(2.5f, -1.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-.5f, -1.5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-1.0f, -1.5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(.5f, -1.5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(1.0f, -1.5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(0.0f, .5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-.5f, .5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-1.5f, .5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-2.0f, .5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(.5f, .5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(1.5f, .5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(2.0f, .5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(0.0f, 1.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-.5f, 1.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-1.0f, 1.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-1.5f, 1.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(.5f, 1.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(1.0f, 1.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(1.5f, 1.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-1.0f, 1.5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(1.0f, 1.5f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(-1.5f, 2.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//matrix to get block to correct x/y coord
	m4TranslateXY = glm::translate(IDENTITY_M4, vector3(1.5f, 2.0f, 0.0f));
	m4Model = m4Translate * m4TranslateXY;

	//render next block
	m_pMesh->Render(m4Projection, m4View, m4Model);



	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//draw gui
	DrawGUI();

	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}