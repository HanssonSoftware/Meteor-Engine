/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#define _CRT_SECURE_NO_WARNINGS
#include "MeshModel.h"
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <ThirdParty/TinyGLTF/tiny_gltf.h>
//#include <OpenFBX/ofbx.h>
#include <Graphics/Camera.h>
#include <Types/String.h>
#include <Log/Exception.h>
#include "D3D11/Direct3DDevice.h"
#include <DirectXMath.h>
#include <Application/Application.h>
#include <Window/WindowManager.h>
#include <Common/MemoryManager.h>

static tinygltf::Model mdl;

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
void MeshModel::Import(String name)
{
	std::string er;
	const String gltfed = name + ".gltf";

	const size_t requiredSize = wcstombs(0, gltfed.Chr(), 0);

	char* buf = (char*)mrmalloc((requiredSize + 1) * sizeof(char));
	wcstombs(buf, gltfed.Chr(), requiredSize + 1);

	if (!loader.LoadASCIIFromFile(&mdl, &er, 0, buf))
	{
		mrfree(buf);
		THROW_EXCEPTION(er);
	}

	mrfree(buf);
	const size_t& meshesQuantity = mdl.meshes.size();
	for (int i = 0; i < meshesQuantity; i++)
	{
		const tinygltf::Mesh& indexedMesh = mdl.meshes[i];
		const int& primitivesQuantity = indexedMesh.primitives.size();

		for (int j = 0; j < primitivesQuantity; j++)
		{
			const tinygltf::Primitive& indexedPrimitive2 = indexedMesh.primitives[j];
			if (indexedPrimitive2.attributes.find("POSITION") == indexedPrimitive2.attributes.end())
				continue;

			if (indexedPrimitive2.indices >= 0) {
				const tinygltf::Accessor& indexAccessor = mdl.accessors[indexedPrimitive2.indices];
				const tinygltf::BufferView& indexBufferView = mdl.bufferViews[indexAccessor.bufferView];
				const tinygltf::Buffer& indexBuffer = mdl.buffers[indexBufferView.buffer];

				const unsigned char* indexDataPtr = indexBuffer.data.data() + indexBufferView.byteOffset + indexAccessor.byteOffset;

				int indexElementCount = indexAccessor.count;
				int indexByteStride = indexBufferView.byteStride ? indexBufferView.byteStride : sizeof(uint32_t); // Alapértelmezett 32-bit indexek

				indicies.reserve(indexElementCount);

				for (int k = 0; k < indexElementCount; k++) {

					if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
						const uint16_t* indexValue = reinterpret_cast<const uint16_t*>(indexDataPtr + k * indexByteStride);
						indicies.push_back(static_cast<uint32_t>(*indexValue));
					}
					else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
						const uint32_t* indexValue = reinterpret_cast<const uint32_t*>(indexDataPtr + k * indexByteStride);
						indicies.push_back(*indexValue);
					}
				}
			}

			const int& primitiveAttributeIndex = indexedPrimitive2.attributes.find("POSITION")->second;

			const tinygltf::Accessor& indexedAccessor = mdl.accessors[primitiveAttributeIndex];
			const tinygltf::BufferView& indexedBufferView = mdl.bufferViews[indexedAccessor.bufferView];
			const tinygltf::Buffer& indexedBuffer = mdl.buffers[indexedBufferView.buffer];

			const unsigned char* dataPtr = indexedBuffer.data.data() + indexedBufferView.byteOffset + indexedAccessor.byteOffset;

			const size_t& elementCount = indexedAccessor.count;
			int byteStride = indexedBufferView.byteStride ? indexedBufferView.byteStride : sizeof(float) * 3;

			vertexes.reserve(elementCount);

			for (int k = 0; k < elementCount; k++) {
				const float* position = reinterpret_cast<const float*>(dataPtr + k * byteStride);
				vertexes.push_back({ { position[0], position[1], position[2], 1.f }, { 0, 0, 0, 0 }, {1, 1, 1} });
			}
		}
	}

	D3D11_BUFFER_DESC bufferInfo = {};
	bufferInfo.ByteWidth = (UINT)(sizeof(Vertex) * vertexes.size());
	bufferInfo.Usage = D3D11_USAGE_DEFAULT;
	bufferInfo.CPUAccessFlags = 0;
	bufferInfo.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferInfo.MiscFlags = 0;

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
		bufferInfoA.ByteWidth = (UINT)(sizeof(int) * indicies.size());
		bufferInfoA.Usage = D3D11_USAGE_DEFAULT;
		bufferInfoA.CPUAccessFlags = 0;
		bufferInfoA.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferInfoA.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA resourceDataA = {};
		resourceDataA.pSysMem = indicies.data();

		if (tempDevice->getDevice()->CreateBuffer(&bufferInfoA, &resourceDataA, &data.indexBuffer) != S_OK)
		{
			THROW_EXCEPTION("Index Buffer import error!");
		}
	}

	bWasSuccessFullyCreated = true;
}

void MeshModel::Render()
{
	if (!bWasSuccessFullyCreated)
		return;

	if (getIsVisible() == false)
		return;

	if (!device)
		return;

	IDirect3DDevice* castedDevice = (IDirect3DDevice*)device;

	constexpr UINT stride = sizeof(Vertex);
	constexpr UINT offset = 0;
	castedDevice->getDeviceContext()->IASetVertexBuffers(0, 1, &data.vertexBuffer, &stride, &offset);
	if (data.indexBuffer) castedDevice->getDeviceContext()->IASetIndexBuffer(data.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	castedDevice->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	castedDevice->getDeviceContext()->VSSetShader(castedDevice->getCompiledVertexShader(), NULL, 0);
	castedDevice->getDeviceContext()->PSSetShader(castedDevice->getCompiledPixelShader(), NULL, 0);

	ID3D11Buffer* b = castedDevice->getWorldBuffer();
	castedDevice->getDeviceContext()->VSSetConstantBuffers(0,1, &b);

	castedDevice->getDeviceContext()->RSSetState(castedDevice->buseWframe ? castedDevice->getRasterizerStateWireFrame() : castedDevice->getRasterizerState());

	if (data.indexBuffer)
	{
		castedDevice->getDeviceContext()->DrawIndexed(indicies.size(), 0, 0);
	}
	else
	{
		castedDevice->getDeviceContext()->Draw(vertexes.size(), 0);
	}
}

void MeshModel::bindRender()
{

}

void MeshModel::unbindRender()
{
}




