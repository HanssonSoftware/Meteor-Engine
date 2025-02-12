/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#define _CRT_SECURE_NO_WARNINGS
#include "MeshModel.h"
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include <TinyGLTF/tiny_gltf.h>
#include <Log/Exception.h>
#include "D3D11/Direct3DDevice.h"
////#include <DirectXMath.h>
//#include <Application/Application.h>
#include <Application/Application.h>
//#include <Common/MemoryManager.h>

//#pragma comment(lib, "assimp-vc143-mt.lib")

MeshModel::MeshModel()
{
	device = Application::Get()->getWindowManager()->getRenderContext();
}

MeshModel::~MeshModel()
{
	if (data.vertexBuffer)
	{
		data.vertexBuffer->Release();
	}	
	
	if (data.indexBuffer)
	{
		data.indexBuffer->Release();
	}

	vertexes.clear();
}

static tinygltf::TinyGLTF loader;
static tinygltf::Model mdl;
static int sizeA = 0;
void MeshModel::Import(String name)
{
	name = name + ".gltf";

	const size_t& size = wcslen(name.Chr());

	char* buffer = (char*)mrmalloc(size + 1);
	wcstombs(buffer, name.Chr(), size + 1);

	std::string error;
	if (!loader.LoadASCIIFromFile(&mdl, &error,0, buffer))
	{
		mrfree(buffer);
		THROW_EXCEPTION("Unable to load %s", name.Chr());
	}

	mrfree(buffer);

	const size_t& meshCount = mdl.meshes.size();
	for (int i = 0; i < meshCount; i++)
	{
		const tinygltf::Mesh& indexedMesh = mdl.meshes[i];

		for (const tinygltf::Primitive& prim : indexedMesh.primitives)
		{
			size_t vertexesCount = 0;
			const float* vertexValues = nullptr;
			const float* normalValues = nullptr;
			if (prim.attributes.find("POSITION") != prim.attributes.end())
			{
				const int& attributePositionIndex = prim.attributes.find("POSITION")->second;
				const tinygltf::Accessor& indexedAccessor = mdl.accessors[attributePositionIndex];

				const tinygltf::BufferView& indexedBufferView = mdl.bufferViews[indexedAccessor.bufferView];
				const tinygltf::Buffer& indexedBuffer = mdl.buffers[indexedBufferView.buffer];
				vertexValues = reinterpret_cast<const float*>(&indexedBuffer.data[indexedAccessor.byteOffset + indexedBufferView.byteOffset]);
				vertexesCount = indexedAccessor.count;
			}

			if (prim.attributes.find("NORMAL") != prim.attributes.end())
			{
				const int& attributePositionIndex = prim.attributes.find("NORMAL")->second;
				const tinygltf::Accessor& indexedAccessor = mdl.accessors[attributePositionIndex];

				const tinygltf::BufferView& indexedBufferView = mdl.bufferViews[indexedAccessor.bufferView];
				const tinygltf::Buffer& indexedBuffer = mdl.buffers[indexedBufferView.buffer];
				normalValues = reinterpret_cast<const float*>(&indexedBuffer.data[indexedAccessor.byteOffset + indexedBufferView.byteOffset]);
			}


			{
				const tinygltf::Accessor& indexedAccessorA = mdl.accessors[prim.indices];
				const tinygltf::BufferView& indexedBufferViewA = mdl.bufferViews[indexedAccessorA.bufferView];
				const tinygltf::Buffer& indexedBufferA = mdl.buffers[indexedBufferViewA.buffer];

				const uint16_t* valueA = reinterpret_cast<const uint16_t*>(&indexedBufferA.data[indexedAccessorA.byteOffset + indexedBufferViewA.byteOffset]);
				for (size_t h = 0; h < indexedAccessorA.count; h++)
				{
					indicies.push_back(valueA[h]);
				}
			}

			for (size_t j = 0; j < vertexesCount; j++)
			{
				Vertex vtx;
				vtx.Pos = { vertexValues[j * 3], vertexValues[j * 3], vertexValues[j * 3], 1.f };
				vtx.Color = { 1.f };
				vtx.Normal = { normalValues[j * 3], normalValues[j * 3], normalValues[j * 3] };

				vertexes.emplace_back(vtx);
			}
		}
	}

	D3D11_BUFFER_DESC bufferInfo = {};
	bufferInfo.ByteWidth = (UINT)(sizeof(Vertex) * vertexes.size());
	bufferInfo.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA resourceData = {};
	resourceData.pSysMem = vertexes.data();

	IDirect3DDevice* tempDevice = (IDirect3DDevice*)device;
	if (tempDevice->getDevice()->CreateBuffer(&bufferInfo, &resourceData, &data.vertexBuffer) != S_OK)
	{
		THROW_EXCEPTION("Vertex Buffer import error!");
	}

	if (!indicies.empty())
	{
		D3D11_BUFFER_DESC bufferInfoA = {};
		bufferInfoA.ByteWidth = (UINT)(sizeof(uint32_t) * indicies.size());
		bufferInfoA.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA resourceDataA = {};
		resourceDataA.pSysMem = indicies.data();

		if (tempDevice->getDevice()->CreateBuffer(&bufferInfoA, &resourceDataA, &data.indexBuffer) != S_OK)
		{
			THROW_EXCEPTION("Index Buffer import error!");
		}
	}

	bWasSuccessFullyCreated = true;
}

static constexpr int stride = sizeof(Vertex);
static constexpr int offset = 0;
void MeshModel::Render()
{
	if (!bWasSuccessFullyCreated)
		return; 	
	
	if (!getIsVisible())
		return; 

	IDirect3DDevice* castedDevice = (IDirect3DDevice*)device;

	castedDevice->getDeviceContext()->IASetVertexBuffers(0, 1, &data.vertexBuffer, (UINT*)&stride, (UINT*)&offset);
	if (data.indexBuffer) castedDevice->getDeviceContext()->IASetIndexBuffer(data.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	castedDevice->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	castedDevice->getDeviceContext()->VSSetShader(castedDevice->getCompiledVertexShader(), NULL, 0);
	castedDevice->getDeviceContext()->PSSetShader(castedDevice->getCompiledPixelShader(), NULL, 0);

	ID3D11Buffer* b = castedDevice->getWorldBuffer();
	castedDevice->getDeviceContext()->VSSetConstantBuffers(0,1, &b);

	castedDevice->getDeviceContext()->RSSetState(castedDevice->buseWframe ? castedDevice->getRasterizerStateWireFrame() : castedDevice->getRasterizerState());

	if (data.indexBuffer)
		castedDevice->getDeviceContext()->DrawIndexed((UINT)indicies.size(), 0, 0);
	else
		castedDevice->getDeviceContext()->Draw((UINT)vertexes.size(), 0);
}

void MeshModel::bindRender()
{

}

void MeshModel::unbindRender()
{
}




