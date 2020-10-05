#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include "DDSTextureLoader.h"
#include "resource.h"
#include <iostream>
#include "structures.h"

/*
 movement
 a current direction vector
 speed is constant
 for now, new directions are instant (no rotation periods)
*/

using namespace DirectX;
class Shark;


struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 TexCoord;
};

class DrawableGameObject
{
public:
	DrawableGameObject(XMFLOAT3 position, float speed, float scale);
	~DrawableGameObject();

	virtual HRESULT						initMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
	virtual void						update(float t, const vecDrawables& drawList, const unsigned int index, Shark* shark);
	virtual void						draw(ID3D11DeviceContext* pContext);
	void                                SetToDestroy();
	virtual ID3D11Buffer*				getVertexBuffer() { return m_pVertexBuffer; }
	virtual ID3D11Buffer*				getIndexBuffer() { return m_pIndexBuffer; }
	virtual ID3D11ShaderResourceView**			getTextureResourceView() { return &m_pTextureResourceView; 	}
	virtual XMFLOAT4X4*					getTransform() { return &m_World; }
	virtual ID3D11SamplerState**				getTextureSamplerState() { return &m_pSamplerLinear; }
	virtual MaterialPropertiesConstantBuffer	getMaterial() { return m_material;}
	virtual void                        SetVertexIndexBuffer(ID3D11DeviceContext* pContext);

	virtual void						setPosition(XMFLOAT3 position);
	virtual XMFLOAT3*					getPosition() { return &m_position; }
	virtual XMFLOAT3*					getDirection() { return &m_direction; }

	virtual void								checkIsOnScreenAndFix(const XMMATRIX&  view, const XMMATRIX&  proj, float  height, float  width);
	bool                                Destroy() { return m_destroy; };

protected:
	virtual void								setDirection(XMFLOAT3 direction); 

	virtual void								nearbyDrawables(const vecDrawables& drawList, const unsigned int index);
	XMFLOAT3							calculateAlignmentVector(const vecDrawables& drawList);
	XMFLOAT3							calculateCohesionVector(const vecDrawables& drawList);
	virtual XMFLOAT3                            calculateSeparationVector(const vecDrawables& drawList);
	bool                                        CheckForShark(XMFLOAT3 sharkPosition);
	XMFLOAT3								    EvadeShark(XMFLOAT3 sharkPosition);
	virtual void								createRandomDirection();

	//	Helpful math functions
	XMFLOAT3							addFloat3(XMFLOAT3& f1, XMFLOAT3& f2);
	XMFLOAT3							subtractFloat3(XMFLOAT3& f1, XMFLOAT3& f2);
	XMFLOAT3							normaliseFloat3(XMFLOAT3& f1);
	float								magnitudeFloat3(XMFLOAT3& f1);
	XMFLOAT3							multiplyFloat3(XMFLOAT3& f1, const float scalar);
	XMFLOAT3							divideFloat3(XMFLOAT3& f1, const float scalar);


private:
	
	XMFLOAT4X4							m_World;
	ID3D11Buffer*						m_pVertexBuffer;
	ID3D11Buffer*						m_pIndexBuffer;
	ID3D11ShaderResourceView*			m_pTextureResourceView;
	ID3D11SamplerState *				m_pSamplerLinear;
	MaterialPropertiesConstantBuffer	m_material;
	XMFLOAT3							m_position;
	XMFLOAT3							m_oldPosition;
	float                               m_speed, m_scale, m_sharkDistance;
	bool                                m_destroy = false;
	XMFLOAT3							m_direction;
	XMFLOAT3							m_oldDirection;
	XMFLOAT3							m_separation;
	XMFLOAT3							m_cohesion;
	XMFLOAT3							m_aligment;
	XMFLOAT3							m_sum;
	unsigned int*						m_nearbyDrawables;
	UINT                                m_stride;
	UINT                                m_offset;
	
};

