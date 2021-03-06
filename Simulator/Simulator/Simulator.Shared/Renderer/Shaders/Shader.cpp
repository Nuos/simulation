#include "pch.h"
#include "Shader.h"
#include <fstream>

using namespace Renderer;

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;

Shader::Shader(const std::shared_ptr<DeviceResources>& devResources) :
deviceResources(devResources),
vsFileName("VertexShader.cso"),
psFileName("PixelShader.cso"),
gsFileName(""),
hsFileName(""),
dsFileName(""),
numDescriptors(0)
{
	SetInputDescriptor(defaultIED, sizeof(defaultIED) / sizeof(D3D11_INPUT_ELEMENT_DESC));
	vertexshader = nullptr;
	pixelshader = nullptr;
	geometryshader = nullptr;
	hullshader = nullptr;
	domainshader = nullptr;
}

Shader::Shader(const std::shared_ptr<DeviceResources>& devResources,
	std::string vsFile, std::string psFile, std::string gsFile, std::string hsFile, std::string dsFile) :
deviceResources(devResources),
vsFileName(vsFile),
psFileName(psFile),
gsFileName(gsFile),
hsFileName(hsFile),
dsFileName(dsFile),
numDescriptors(0)
{
	vertexshader = nullptr;
	pixelshader = nullptr;
	geometryshader = nullptr;
	hullshader = nullptr;
	domainshader = nullptr;
}

void Shader::SetInputDescriptor(std::initializer_list<D3D11_INPUT_ELEMENT_DESC> l)
{
	if (l.size() > 0)
	{
		pIED = new D3D11_INPUT_ELEMENT_DESC[l.size()];
		int n = 0;
		for (const D3D11_INPUT_ELEMENT_DESC* it = begin(l); it != end(l); ++it, ++n)
		{
			pIED[n] = *it;
		}
		numDescriptors = l.size();
	}
}

void Shader::SetInputDescriptor(D3D11_INPUT_ELEMENT_DESC pElemDesc[], int numDesc)
{
	numDescriptors = numDesc;
	pIED = new D3D11_INPUT_ELEMENT_DESC[numDescriptors];
	for (int i = 0; i < numDescriptors; i++)
	{
		pIED[i] = pElemDesc[i];
	}
}

void Shader::Initialize(int cb_bytewidth)
{
	// load the shader files
	Platform::Array<byte>^ VSFile;
	Platform::Array<byte>^ PSFile;
	Platform::Array<byte>^ GSFile;
	Platform::Array<byte>^ DSFile;
	Platform::Array<byte>^ HSFile;

	// create the shader objects
	if (vsFileName.compare(""))
	{
		VSFile = LoadShaderFile(vsFileName);
		deviceResources->GetD3DDevice()->CreateVertexShader(VSFile->Data, VSFile->Length, nullptr, &vertexshader);
	}
	if (gsFileName.compare(""))
	{
		GSFile = LoadShaderFile(gsFileName);
		deviceResources->GetD3DDevice()->CreateGeometryShader(GSFile->Data, GSFile->Length, nullptr, &geometryshader);
	}
	if (hsFileName.compare(""))
	{
		HSFile = LoadShaderFile(hsFileName);
		deviceResources->GetD3DDevice()->CreateHullShader(HSFile->Data, HSFile->Length, nullptr, &hullshader);
	}
	if (dsFileName.compare(""))
	{
		DSFile = LoadShaderFile(dsFileName);
		deviceResources->GetD3DDevice()->CreateDomainShader(DSFile->Data, DSFile->Length, nullptr, &domainshader);
	}
	if (psFileName.compare(""))
	{
		PSFile = LoadShaderFile(psFileName);
		deviceResources->GetD3DDevice()->CreatePixelShader(PSFile->Data, PSFile->Length, nullptr, &pixelshader);
	}
	
	// create the input layout
	deviceResources->GetD3DDevice()->CreateInputLayout(pIED, numDescriptors, VSFile->Data, VSFile->Length, &inputlayout);

	// initialize the constant buffer
	D3D11_BUFFER_DESC bd = { 0 };

	bd.Usage = D3D11_USAGE_DEFAULT;
	// THIS NEEDS TO BE CHANGED PER SHADER, SINCE SHADERS MAY HAVE DIFFERENT CBUFFERS
	bd.ByteWidth = cb_bytewidth;		// must always be a multiple of 16 bytes
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// create the constant buffer
	deviceResources->GetD3DDevice()->CreateBuffer(&bd, nullptr, &constantbuffer);
}

void Shader::UnInitialize()
{

}

void Shader::Apply()
{
	// set the input layout as the active input layout
	deviceResources->GetD3DDeviceContext()->IASetInputLayout(inputlayout.Get());
	// set the shader objects as the active shaders
	deviceResources->GetD3DDeviceContext()->VSSetShader(vertexshader.Get(), nullptr, 0);
	deviceResources->GetD3DDeviceContext()->PSSetShader(pixelshader.Get(), nullptr, 0);
	//deviceResources->GetD3DDeviceContext()->HSSetShader(hullshader.Get(), nullptr, 0);
	//deviceResources->GetD3DDeviceContext()->DSSetShader(domainshader.Get(), nullptr, 0);
	deviceResources->GetD3DDeviceContext()->GSSetShader(geometryshader.Get(), nullptr, 0);
	// set the constant buffer for the active shaders
	if (vertexshader != nullptr)
		deviceResources->GetD3DDeviceContext()->VSSetConstantBuffers(0, 1, constantbuffer.GetAddressOf());
	if (pixelshader != nullptr)
		deviceResources->GetD3DDeviceContext()->PSSetConstantBuffers(0, 1, constantbuffer.GetAddressOf());
	if (geometryshader != nullptr)
		deviceResources->GetD3DDeviceContext()->GSSetConstantBuffers(0, 1, constantbuffer.GetAddressOf());
	if (hullshader != nullptr)
		deviceResources->GetD3DDeviceContext()->HSSetConstantBuffers(0, 1, constantbuffer.GetAddressOf());
	if (domainshader != nullptr)
		deviceResources->GetD3DDeviceContext()->DSSetConstantBuffers(0, 1, constantbuffer.GetAddressOf());
}

void Shader::Disable()
{
	// set the input layout as the active input layout
	deviceResources->GetD3DDeviceContext()->IASetInputLayout(nullptr);
	// set the shader objects as the active shaders
	deviceResources->GetD3DDeviceContext()->VSSetShader(nullptr, nullptr, 0);
	deviceResources->GetD3DDeviceContext()->PSSetShader(nullptr, nullptr, 0);
	deviceResources->GetD3DDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	deviceResources->GetD3DDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	deviceResources->GetD3DDeviceContext()->DSSetShader(nullptr, nullptr, 0);
}

void Shader::Reset()
{
	inputlayout.Reset();

	constantbuffer.Reset();

	vertexshader.Reset();
	pixelshader.Reset();
	geometryshader.Reset();
	hullshader.Reset();
	domainshader.Reset();
}

// this function loads a file into an Array^
Platform::Array<byte>^ Renderer::LoadShaderFile(std::string File)
{
	Platform::Array<byte>^ FileData = nullptr;

	// open the file
	std::ifstream VertexFile(File, std::ios::in | std::ios::binary | std::ios::ate);

	// if open was successful
	if (VertexFile.is_open())
	{
		// find the length of the file
		int Length = (int)VertexFile.tellg();

		// collect the file data
		FileData = ref new Platform::Array<byte>(Length);
		VertexFile.seekg(0, std::ios::beg);
		VertexFile.read(reinterpret_cast<char*>(FileData->Data), Length);
		VertexFile.close();
	}

	return FileData;
}