#pragma once
#include "DrawableGameObject.h"

class Shark : public DrawableGameObject
{
public:
	   Shark(XMFLOAT3 position, float speed, float scale);
    	~Shark();
	   HRESULT	initMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
     	void	update(float t, const vecDrawables& drawList);
	   XMFLOAT3 calculateSeparationVector(const vecDrawables& drawList);
	   void setDirection(XMFLOAT3 direction);
	   void checkIsOnScreenAndFix(const XMMATRIX&  view, const XMMATRIX&  proj, float height, float width);
	   void	nearbyDrawables(const vecDrawables& drawList);
	   void						draw(ID3D11DeviceContext* pContext);
	   void SetVertexIndexBuffer(ID3D11DeviceContext* pContext);
	   MaterialPropertiesConstantBuffer	getMaterial() { return m_material; }
	   ID3D11ShaderResourceView**			getTextureResourceView() { return &m_pTextureResourceView; }
	   ID3D11SamplerState**				getTextureSamplerState() { return &m_pSamplerLinear; }
	   XMFLOAT4X4*					getTransform() { return &m_World; }
	   XMFLOAT3*					getPosition() { return &m_position; }
	   XMFLOAT3*					getDirection() { return &m_direction; }

private:
	XMFLOAT4X4							m_World;
	ID3D11ShaderResourceView*			m_pTextureResourceView;
	ID3D11SamplerState *				m_pSamplerLinear;
	MaterialPropertiesConstantBuffer	m_material;
	ID3D11Buffer*						m_pVertexBuffer;
	ID3D11Buffer*						m_pIndexBuffer;
	XMFLOAT3							m_position;
	XMFLOAT3							m_oldPosition;
	float                               m_speed, m_scale;

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

