#include "Shark.h"

#define NEARBY_DISTANCE		150.0f

Shark::Shark(XMFLOAT3 position, float speed, float scale): DrawableGameObject(position, speed, scale)
{
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	m_pTextureResourceView = nullptr;
	m_pSamplerLinear = nullptr;

	// Initialize the world matrix
	XMStoreFloat4x4(&m_World, XMMatrixIdentity());

	m_nearbyDrawables = nullptr;
	m_position = XMFLOAT3(position.x, position.y, position.z);
	m_oldPosition = XMFLOAT3(position.x, position.y, position.z);
	m_speed = speed;
	m_scale = scale;
	createRandomDirection();
}

Shark::~Shark()
{
	if (m_pVertexBuffer)
		m_pVertexBuffer->Release();

	if (m_pIndexBuffer)
		m_pIndexBuffer->Release();

	if (m_pTextureResourceView)
		m_pTextureResourceView->Release();

	if (m_pSamplerLinear)
		m_pSamplerLinear->Release();

	if (m_nearbyDrawables != nullptr) {
		delete[] m_nearbyDrawables;
	}
}

void Shark::setDirection(XMFLOAT3 direction)
{
	XMVECTOR v = XMLoadFloat3(&direction);
	v = XMVector3Normalize(v);
	XMStoreFloat3(&m_direction, v);
}

HRESULT	Shark::initMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
	SimpleVertex vertices[] =
	{
		{ XMFLOAT3(0.0f, 16.0f, 0.0f), XMFLOAT3(0.0f, 1.5f, -0.5f), XMFLOAT2(0.5f, 0.0f) },
		{ XMFLOAT3(8.0f, 0.0f, 8.0f), XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-8.0f, 0.0f, 8.0f), XMFLOAT3(-4.0f / 3.0f, -2.0f / 3.0f, 2.0f / 3.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(8.0f, 0.0f, 8.0f), XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(8.0f, 0.0f, -8.0f), XMFLOAT3(4.0f / 3.0f, -2.0f / 3.0f, -4.0f / 3.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(-8.0f, 0.0f, 8.0f), XMFLOAT3(-4.0f / 3.0f, -2.0f / 3.0f, 2.0f / 3.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-8.0f, 0.0f, -8.0f), XMFLOAT3(-1.0f, -1.5f, -1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-8.0f, 0.0f, -8.0f), XMFLOAT3(-1.0f, -1.5f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(8.0f, 0.0f, -8.0f), XMFLOAT3(4.0f / 3.0f, -2.0f / 3.0f, -4.0f / 3.0f), XMFLOAT2(1.0f, 1.0f) },




		{ XMFLOAT3(8.0f, 0.0f, 8.0f), XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-8.0f, 0.0f, 8.0f), XMFLOAT3(-4.0f / 3.0f, -2.0f / 3.0f, 2.0f / 3.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-8.0f, 0.0f, -8.0f), XMFLOAT3(-1.0f, -1.5f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(8.0f, 0.0f, -8.0f), XMFLOAT3(4.0f / 3.0f, -2.0f / 3.0f, -4.0f / 3.0f), XMFLOAT2(1.0f, 1.0f) },
	};

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 13;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = vertices;
	HRESULT hr = pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
	if (FAILED(hr))
		return hr;

	// Set vertex buffer
	m_stride = sizeof(SimpleVertex);
	m_offset = 0;
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_stride, &m_offset);

	// Create index buffer
	WORD indices[] =
	{
		2,1,0,
		4,0,3,
		6,5,0,
		8,7,0,
		11,9,10,
		9,11,12,
	};

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 18;        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer);
	if (FAILED(hr))
		return hr;

	// Set index buffer
	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// Set primitive topology
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// load and setup textures
	hr = CreateDDSTextureFromFile(pd3dDevice, L"Resources\\shark.dds", nullptr, &m_pTextureResourceView);
	if (FAILED(hr))
		return hr;

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = pd3dDevice->CreateSamplerState(&sampDesc, &m_pSamplerLinear);

	m_material.Material.Diffuse = XMFLOAT4(0.5f, 1.0f, 1.0f, 1.0f);
	m_material.Material.Specular = XMFLOAT4(1.0f, 0.2f, 0.2f, 1.0f);
	m_material.Material.SpecularPower = 32.0f;
	m_material.Material.UseTexture = true;


	return hr;
}

void Shark::SetVertexIndexBuffer(ID3D11DeviceContext* pContext)
{
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_stride, &m_offset);
	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
}

void Shark::draw(ID3D11DeviceContext* pContext)
{
	pContext->DrawIndexed(18, 0, 0);
}

void Shark::nearbyDrawables(const vecDrawables& drawList)
{
	if (m_nearbyDrawables == nullptr) {
		if (drawList.size() == 0)
			return;
		m_nearbyDrawables = new unsigned int[drawList.size() + 5];
	}

	unsigned int nearbyIndex = 0;
	for (unsigned int i = 0; i < drawList.size(); i++) {

		// get the distance between the two
		XMFLOAT3 vB = *(drawList[i]->getPosition());
		XMFLOAT3 vDiff = subtractFloat3(m_position, vB);
		float l = magnitudeFloat3(vDiff);
		if (l < NEARBY_DISTANCE) {
			m_nearbyDrawables[nearbyIndex++] = i;
		}
	}

	// set the last unused element of the drawables index to UINT_MAX. Note: because the array is one larger than it needs to be
	// this should always be safe even if *all* are nearby (because self is not included)
	m_nearbyDrawables[nearbyIndex] = UINT_MAX;
}


void Shark::update(float t, const vecDrawables& drawList)
{


	// create a list of nearby boids in the m_nearbyDrawables array
	nearbyDrawables(drawList);

	m_direction = calculateSeparationVector(drawList);


	m_position = multiplyFloat3(m_direction, m_speed);
	m_position = multiplyFloat3(m_position, t);
	m_position = addFloat3(m_oldPosition, m_position);
	m_oldPosition = m_position;
	setPosition(m_position);

	XMMATRIX mSpin = XMMatrixRotationZ(-atan2f(m_direction.x, m_direction.y));

	XMMATRIX mTranslate = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMStoreFloat4x4(&m_World, mSpin * mTranslate);
}

XMFLOAT3 Shark::calculateSeparationVector(const vecDrawables& drawList)
{
	XMFLOAT3 nearby = XMFLOAT3(0, 0, 0);
	XMFLOAT3 sum = XMFLOAT3(0, 0, 0);
	float  tempX = 0.0f, tempY = 0.0f, comp = 800.0f;
	int count;

	if (drawList.size() == 0 || m_nearbyDrawables == nullptr)
		return nearby;


	// loop through all nearby fish
	unsigned int index = 0;

	while (m_nearbyDrawables[index] != UINT_MAX) 
	{
		DrawableGameObject* pFish = (drawList.at(m_nearbyDrawables[index]));
		sum = subtractFloat3(*pFish->getPosition(), *getPosition());
		tempX = sum.x;
		if (tempX < 0.0f)
			tempX = -tempX;
		tempY = sum.y;
		if (tempY < 0.0f)
			tempY = -tempY;
		if (comp > tempX + tempY)
		{
			comp = tempX + tempY;
			count = m_nearbyDrawables[index];
			nearby = normaliseFloat3(sum);
		}
		index++;
	}

	if (comp != 0.0f && comp <= 400.0f)
	{
		if (comp < 4.0f)
		{
			drawList.at(count)->SetToDestroy();	
		}
		return nearby;
	}
	return m_direction;
}

void Shark::checkIsOnScreenAndFix(const XMMATRIX&  view, const XMMATRIX&  proj, float height, float width)
{
	XMFLOAT4 v4;
	v4.x = m_position.x;
	v4.y = m_position.y;
	v4.z = m_position.z;
	v4.w = 1.0f;

	XMVECTOR vScreenSpace = XMLoadFloat4(&v4);
	XMVECTOR vScreenSpace2 = XMVector4Transform(vScreenSpace, view);
	XMVECTOR vScreenSpace3 = XMVector4Transform(vScreenSpace2, proj);

	XMFLOAT4 v;
	XMStoreFloat4(&v, vScreenSpace3);

	v.x /= v.w;
	v.y /= v.w;
	v.z /= v.w;
	v.w /= v.w;

	// this is the position. the fish is visible on screen if the x is -1 to 1 and the same with y
	float fOffset = 10; // a suitable distance to rectify position within clip space
	if (v.x < -1 || v.x > 1 || v.y < -1 || v.y > 1)
	{
		if (v.x < -1) {
			m_position.x = width / 4.0f - 1.0f;
		}
		if (v.y < -1) {
			m_position.y = height / 4.0f - 1.0f;
		}

		if (v.x > 1) {
			m_position.x = -width / 4.0f + 1.0f;
		}
		if (v.y > 1) {
			m_position.y = -height / 4.0f + 1.0f;
		}
		m_oldPosition = m_position;
	}

	return;
}