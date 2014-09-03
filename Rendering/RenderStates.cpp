//***************************************************************************************
// RenderStates.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "RenderStates.h"

ID3D11RasterizerState* RenderStates::WireframeRS = 0;
ID3D11RasterizerState* RenderStates::NoCullRS    = 0;
ID3D11RasterizerState* RenderStates::CullFrontRS = 0;
ID3D11RasterizerState* RenderStates::ReverseWindingRS = 0;
	 
ID3D11BlendState*      RenderStates::AlphaToCoverageBS = 0;
ID3D11BlendState*      RenderStates::TransparentBS     = 0;

ID3D11DepthStencilState* RenderStates::DisableDepthDSS = 0;
ID3D11DepthStencilState* RenderStates::DontWriteDepthDSS = 0;

void RenderStates::InitAll(ID3D11Device* device)
{
	//
	// WireframeRS
	//
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&wireframeDesc, &WireframeRS));

	//
	// NoCullRS
	//
	D3D11_RASTERIZER_DESC noCullDesc;
	ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.CullMode = D3D11_CULL_NONE;
	noCullDesc.FrontCounterClockwise = false;
	noCullDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&noCullDesc, &NoCullRS));

	//
	// CullFront
	//
	D3D11_RASTERIZER_DESC cullFrontDesc;
	ZeroMemory(&cullFrontDesc, sizeof(D3D11_RASTERIZER_DESC));
	cullFrontDesc.FillMode = D3D11_FILL_SOLID;
	cullFrontDesc.CullMode = D3D11_CULL_FRONT;
	cullFrontDesc.FrontCounterClockwise = false;
	cullFrontDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&cullFrontDesc, &CullFrontRS));

	//
	// ReverseWinding
	//
	D3D11_RASTERIZER_DESC reverseWindingDesc;
	ZeroMemory(&reverseWindingDesc, sizeof(D3D11_RASTERIZER_DESC));
	reverseWindingDesc.FillMode = D3D11_FILL_SOLID;
	reverseWindingDesc.CullMode = D3D11_CULL_BACK;
	reverseWindingDesc.FrontCounterClockwise = true;
	reverseWindingDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&reverseWindingDesc, &ReverseWindingRS));

	//
	// AlphaToCoverageBS
	//

	D3D11_BLEND_DESC alphaToCoverageDesc = {0};
	alphaToCoverageDesc.AlphaToCoverageEnable = true;
	alphaToCoverageDesc.IndependentBlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(device->CreateBlendState(&alphaToCoverageDesc, &AlphaToCoverageBS));

	//
	// TransparentBS
	//

	D3D11_BLEND_DESC transparentDesc = {0};
	transparentDesc.AlphaToCoverageEnable = false;
	transparentDesc.IndependentBlendEnable = false;

	transparentDesc.RenderTarget[0].BlendEnable = true;
	transparentDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
	transparentDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA;
	transparentDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
	transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	transparentDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(device->CreateBlendState(&transparentDesc, &TransparentBS));

	//
	// DisableDepthDSS
	//

	D3D11_DEPTH_STENCIL_DESC disableDepthDesc = {0};
	disableDepthDesc.DepthEnable = true;
	disableDepthDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	HR(device->CreateDepthStencilState(&disableDepthDesc, &DisableDepthDSS));

	//
	// DontWriteDepthDSS
	//

	D3D11_DEPTH_STENCIL_DESC dontWriteDepthDesc = {0};
	dontWriteDepthDesc.DepthEnable = false;
	dontWriteDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	HR(device->CreateDepthStencilState(&dontWriteDepthDesc, &DontWriteDepthDSS));
}

void RenderStates::DestroyAll()
{
	ReleaseCOM(WireframeRS);
	ReleaseCOM(NoCullRS);
	ReleaseCOM(CullFrontRS);
	ReleaseCOM(AlphaToCoverageBS);
	ReleaseCOM(TransparentBS);
}