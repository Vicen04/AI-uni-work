
#include "Shark.h"

// PLAN **************************
// Use XMVector3s 
// either use simple math library
// or create functions to do the math on xmfloat3
// or (as MSDN says - use xmfloat3s, convert to vector and back again when using maths)


using namespace std;
using namespace DirectX;

#define NUM_VERTICES		36 // do not change!

#define NEARBY_DISTANCE		10.0f

DrawableGameObject::DrawableGameObject(XMFLOAT3 position, float speed, float scale)
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
	m_sharkDistance = float(rand() % 140);
}

void DrawableGameObject::createRandomDirection()
{
	float x = float(rand() % 10000 / 10000.0f);
	float y = float(rand() % 10000 / 10000.0f);
	if (rand() % 2 != 0)
		y = -y;
	if (rand() % 2 != 0)
		x = -x;

	setDirection(XMFLOAT3(x, y, 0));
}

void DrawableGameObject::setDirection(XMFLOAT3 direction)
{
	XMVECTOR v = XMLoadFloat3(&direction);
	v = XMVector3Normalize(v);
	XMStoreFloat3(&m_direction, v);
}

DrawableGameObject::~DrawableGameObject()
{
	if( m_pVertexBuffer ) 
		m_pVertexBuffer->Release();
	
	if( m_pIndexBuffer )
		m_pIndexBuffer->Release();	

	if (m_pTextureResourceView)
		m_pTextureResourceView->Release();

	if (m_pSamplerLinear)
		m_pSamplerLinear->Release();

	if (m_nearbyDrawables != nullptr) {
		delete [] m_nearbyDrawables;
	}
}

HRESULT DrawableGameObject::initMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
	// Create vertex buffer
	SimpleVertex vertices[] =
	{
		{ XMFLOAT3(-2.0f , 2.0f , -2.0f ), XMFLOAT3(0.0f, 2.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(2.0f , 2.0f, -2.0f ), XMFLOAT3(0.0f, 2.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f , 2.0f, 2.0f ), XMFLOAT3(0.0f, 2.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-2.0f , 2.0f, 2.0f ), XMFLOAT3(0.0f, 2.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(-2.0f, -2.0f, -2.0f), XMFLOAT3(0.0f, -2.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f, -2.0f, -2.0f), XMFLOAT3(0.0f, -2.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(2.0f, -2.0f, 2.0f), XMFLOAT3(0.0f, -2.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-2.0f, -2.0f, 2.0f), XMFLOAT3(0.0f, -2.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-2.0f, -2.0f, 2.0f), XMFLOAT3(-2.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-2.0f, -2.0f, -2.0f), XMFLOAT3(-2.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-2.0f, 2.0f, -2.0f), XMFLOAT3(-2.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-2.0f, 2.0f, 2.0f), XMFLOAT3(-2.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(2.0f, -2.0f, 2.0f), XMFLOAT3(2.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f, -2.0f, -2.0f), XMFLOAT3(2.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(2.0f, 2.0f, -2.0f), XMFLOAT3(2.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(2.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },

		{ XMFLOAT3(-2.0f, -2.0f, -2.0f), XMFLOAT3(0.0f, 0.0f, -2.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(2.0f, -2.0f, -2.0f), XMFLOAT3(0.0f, 0.0f, -2.0f) , XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f, 2.0f, -2.0f), XMFLOAT3(0.0f, 0.0f, -2.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-2.0f, 2.0f, -2.0f), XMFLOAT3(0.0f, 0.0f, -2.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(-2.0f, -2.0f, 2.0f), XMFLOAT3(0.0f, 0.0f, 2.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f, -2.0f, 2.0f), XMFLOAT3(0.0f, 0.0f, 2.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 0.0f, 2.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 0.0f, 2.0f), XMFLOAT2(1.0f, 0.0f) },
	};

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 24;
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
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
	};

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * NUM_VERTICES;        // 36 vertices needed for 12 triangles in a triangle list
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
	hr = CreateDDSTextureFromFile(pd3dDevice, L"Resources\\fish.dds", nullptr, &m_pTextureResourceView);
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

	m_material.Material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_material.Material.Specular = XMFLOAT4(1.0f, 0.2f, 0.2f, 1.0f);
	m_material.Material.SpecularPower = 32.0f;
	m_material.Material.UseTexture = true;
	

	return hr;
}

void DrawableGameObject::setPosition(XMFLOAT3 position)
{
	m_position = position;
}

void DrawableGameObject::update(float t, const vecDrawables& drawList, const unsigned int index, Shark* shark)
{
	XMMATRIX mSpin = XMMatrixIdentity();

	// create a list of nearby boids in the m_nearbyDrawables array
	nearbyDrawables(drawList, index);

	// TODO set position here (m_position) calculating separation, cohesion and alignment

	if (!CheckForShark(*shark->getPosition()))
	{
		m_aligment = calculateAlignmentVector(drawList);
		m_cohesion = calculateCohesionVector(drawList);
		m_separation = calculateSeparationVector(drawList);
	}
	else
		EvadeShark(*shark->getPosition());

	

	//m_sum = addFloat3(m_separation, m_cohesion);
	//m_sum = addFloat3(m_sum, m_aligment);
	//m_sum = divideFloat3(m_sum, 3);


	m_position = multiplyFloat3(m_direction, m_speed);
	m_position = multiplyFloat3(m_position, t);
	m_position = addFloat3(m_oldPosition, m_position);
	m_oldPosition = m_position;
	setPosition(m_position);

	XMMATRIX mTranslate = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMStoreFloat4x4(&m_World, mSpin * mTranslate);
}

void DrawableGameObject::nearbyDrawables(const vecDrawables& drawList, const unsigned int index)
{
	if (m_nearbyDrawables == nullptr) {
		if (drawList.size() == 0)
			return;
		m_nearbyDrawables = new unsigned int[drawList.size()+5];
	}

	unsigned int nearbyIndex = 0;
	for (unsigned int i = 0; i < drawList.size(); i++) {
		// ignore self
		if (i == index)
			continue;

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

void DrawableGameObject::SetVertexIndexBuffer(ID3D11DeviceContext* pContext)
{
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_stride, &m_offset);
	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
}

void DrawableGameObject::draw(ID3D11DeviceContext* pContext)
{
	pContext->DrawIndexed(NUM_VERTICES, 0, 0);
}

void DrawableGameObject::checkIsOnScreenAndFix(const XMMATRIX&  view, const XMMATRIX&  proj, float height, float width)
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
		if (v.x < -1 ) {
			m_position.x = width/4.0f - 1.0f;		
		}
		else if (v.x > 1) {
			m_position.x = -width / 4.0f + 1.0f;
		}
		if (v.y < -1 ) {
			m_position.y = height/4.0f - 1.0f;
		}

		
		else if (v.y > 1) {
			m_position.y = -height / 4.0f + 1.0f;
		}
		m_oldPosition = m_position;
	}

	return;
}

XMFLOAT3 DrawableGameObject::calculateSeparationVector(const vecDrawables& drawList)
{
	XMFLOAT3 nearby = XMFLOAT3(0, 0, 0);
	XMFLOAT3 sum = XMFLOAT3(), position = XMFLOAT3();

	if (drawList.size() == 0 || m_nearbyDrawables == nullptr)
		return nearby;


	// loop through all nearby fish
	unsigned int index = 0;

		while (m_nearbyDrawables[index] != UINT_MAX) {
			DrawableGameObject* pFish = (drawList[m_nearbyDrawables[index]]);
			position = *pFish->getPosition();
			XMFLOAT3 vDiff = subtractFloat3(m_position, position);
			float mag = magnitudeFloat3(vDiff);
			sum = multiplyFloat3(vDiff, NEARBY_DISTANCE + 1 - mag);
			index++;
		}

		if (magnitudeFloat3(nearby) != 0)
		nearby = divideFloat3(nearby, index);	

	return nearby; // average direction away from fish to nearby
}

XMFLOAT3 DrawableGameObject::calculateAlignmentVector(const vecDrawables& drawList)
{
	XMFLOAT3 nearby = XMFLOAT3(0, 0, 0);
	XMFLOAT3 sum = XMFLOAT3();
	float speed = 0.0f;

	if (drawList.size() == 0 || m_nearbyDrawables == nullptr)
		return nearby;

	// loop through all nearby fish
	unsigned int index = 0;
	while (m_nearbyDrawables[index] != UINT_MAX) {
		DrawableGameObject* pFish = drawList[m_nearbyDrawables[index]];
		sum = pFish->m_direction;
		speed = speed + pFish->m_speed;
		nearby = addFloat3(nearby, sum);
		index++;
	}

	if (magnitudeFloat3(nearby) != 0)
	{
		nearby = divideFloat3(nearby, index);
		m_direction = nearby;
		m_speed = speed / index;
	}

	return nearby; // return the average direction of nearby drawables
}


XMFLOAT3 DrawableGameObject::calculateCohesionVector(const vecDrawables& drawList)
{
	XMFLOAT3 all = XMFLOAT3(0, 0, 0);
	XMFLOAT3 sum = XMFLOAT3();

	if (drawList.size() == 0)
		return all;

	// loop through all fish
	unsigned int index = 0;
	while (index < drawList.size()) {
		DrawableGameObject* pFish = (drawList[index]);
		sum = *pFish->getPosition();
		all = addFloat3(all, sum);
		index++;
	}

	if (magnitudeFloat3(all) != 0)
	{
		all = divideFloat3(all, drawList.size());
	}
	

	return all; // return avg direction from this fish to all
}

bool DrawableGameObject::CheckForShark(XMFLOAT3 sharkPosition)
{
	XMFLOAT3 vB = sharkPosition;
	XMFLOAT3 vDiff = subtractFloat3(m_position, vB);
	float l = magnitudeFloat3(vDiff);
	if (l < m_sharkDistance) {
		return true;
	}
	else
		return false;
}

XMFLOAT3 DrawableGameObject::EvadeShark(XMFLOAT3 sharkPosition)
{
	m_direction = subtractFloat3(sharkPosition, m_position);
	m_direction = normaliseFloat3(m_direction);
	m_direction = multiplyFloat3(m_direction, -1);
	return m_direction;
}


void DrawableGameObject::SetToDestroy()
{
	m_destroy = true;
}

// --------------------------------------------------------------------------------
// Math functions - do not modify!

XMFLOAT3 DrawableGameObject::addFloat3(XMFLOAT3& f1, XMFLOAT3& f2)
{
	XMFLOAT3 out;
	out.x = f1.x + f2.x;
	out.y = f1.y + f2.y;
	out.z = f1.z + f2.z;

	return out;
}

XMFLOAT3 DrawableGameObject::subtractFloat3(XMFLOAT3& f1, XMFLOAT3& f2)
{
	XMFLOAT3 out;
	out.x = f1.x - f2.x;
	out.y = f1.y - f2.y;
	out.z = f1.z - f2.z;

	return out;
}

XMFLOAT3 DrawableGameObject::multiplyFloat3(XMFLOAT3& f1, const float scalar)
{
	XMFLOAT3 out;
	out.x = f1.x * scalar;
	out.y = f1.y * scalar;
	out.z = f1.z * scalar;

	return out;
}

XMFLOAT3 DrawableGameObject::divideFloat3(XMFLOAT3& f1, const float scalar)
{
	XMFLOAT3 out;
	out.x = f1.x / scalar;
	out.y = f1.y / scalar;
	out.z = f1.z / scalar;

	return out;
}

float DrawableGameObject::magnitudeFloat3(XMFLOAT3& f1)
{
	return sqrt((f1.x * f1.x) + (f1.y * f1.y) + (f1.z * f1.z));
}

XMFLOAT3 DrawableGameObject::normaliseFloat3(XMFLOAT3& f1)
{
	float length = sqrt((f1.x * f1.x) + (f1.y * f1.y) + (f1.z * f1.z));

	f1.x /= length;
	f1.y /= length;
	f1.z /= length;

	return f1;
}