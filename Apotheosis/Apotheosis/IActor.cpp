// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : IActor.cpp
// Description :  IActor 
// Author : Serge Radinovich
// Mail : srad015@hotmail.com


#include "IActor.h"

IActor::IActor()
{
	Transform::rotateAroundRight(&m_transform, -D3DX_PI / 2);
	m_renderTasks.reserve(10);
	m_textureAnimQueues.reserve(10);
}

IActor::~IActor()
{
}


void IActor::initializeRendering(const string& _rkTexture, const string& _rkSuffix, const UINT _kiSpriteCount, const string& _rkFX, const string& _rkTech, float _fDimX, float _fDimY)
{
	m_renderTasks.push_back(RenderTask2D());
	m_textureAnimQueues.push_back(queue<ID3D10ShaderResourceView*>());

	DX10Renderer::loadRenderTask2D(m_renderTasks.back(), _rkTexture, _rkSuffix, _kiSpriteCount, _rkFX, _rkTech, _fDimX, _fDimY);
	m_renderTasks.back().rendering = true;

	//Grab all the shader variables so the render task can access them
	m_renderTasks.back().pFXEyePosWVar = m_renderTasks.back().pFX->GetVariableByName("gEyePosW")->AsVector();

	m_renderTasks.back().pFXWorldVar = m_renderTasks.back().pFX->GetVariableByName("gWorld")->AsMatrix();
	m_renderTasks.back().pFXWVPVar = m_renderTasks.back().pFX->GetVariableByName("gWVP")->AsMatrix();
	m_renderTasks.back().pFXTexMtxVar = m_renderTasks.back().pFX->GetVariableByName("gTexMtx")->AsMatrix();

	//m_renderTasks.back().pFXDiffuseMapVar = m_renderTasks.back().pFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	//m_renderTasks.back().pFXSpecMapVar = m_renderTasks.back().pFX->GetVariableByName("gSpecMap")->AsShaderResource();
	m_renderTasks.back().pFXTexArrayVar = m_renderTasks.back().pFX->GetVariableByName("gTexArray")->AsShaderResource();
	m_renderTasks.back().pFXAnimRateVar = m_renderTasks.back().pFX->GetVariableByName("gAnimRate")->AsScalar();
	//m_renderTasks.back().pFXLastAnimTimeVar = m_renderTasks.back().pFX->GetVariableByName("gLastAnimTime")->AsScalar();
	m_renderTasks.back().pFXSpriteCountVar = m_renderTasks.back().pFX->GetVariableByName("gSpriteCount")->AsScalar();
	
	m_renderTasks.back().pFXGameTimeVar = m_renderTasks.back().pFX->GetVariableByName("gGameTime")->AsScalar();

	m_renderTasks.back().transformMtx = *m_transform.matrix();

	m_renderTasks.back().iSpriteCount = _kiSpriteCount;
	m_renderTasks.back().fAnimRate = m_fAnimRate;


}


void IActor::setCurrState(UINT _iIndex)
{
	assert(_iIndex >= 0 && _iIndex < m_renderTasks.size() && "Invalid index number given for setting IActor state.");

	m_iCurrTaskIndex = _iIndex;
	for (UINT i = 0; i < m_renderTasks.size(); ++i)
	{
		if (i == m_iCurrTaskIndex)
			m_renderTasks[i].rendering = true;
		else
			m_renderTasks[i].rendering = false;
	}
}


void IActor::setActive(bool _bActive)
{
	m_bActive = _bActive;

	for (auto& _rRenderTask : m_renderTasks)
		_rRenderTask.rendering = _bActive;

	for (auto& _rpBody : m_rigidBodies)
		_rpBody->SetActive(_bActive);
}

b2Vec2 IActor::getPosition()
{
	b2Vec2 _return;
	_return.x = m_transform.translation.x;
	_return.y = m_transform.translation.y;
	return _return;
}

void IActor::setPosition(float _fX, float _fY)
{
	m_rigidBodies.back()->SetTransform(b2Vec2(_fX, _fY), m_rigidBodies.back()->GetAngle());
	m_transform.translation.x = _fX;
	m_transform.translation.y = _fY;
	m_renderTasks[m_iCurrTaskIndex].transformMtx = *m_transform.matrix();
}
