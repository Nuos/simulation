#pragma once

#include "pch.h"

using namespace DirectX;

namespace Renderer
{
	// Default Input Element Descriptor 
	static D3D11_INPUT_ELEMENT_DESC defaultIED[] =
	{
		// semantic name, semantic index, format, inputslot, offset, input slot class, instance data step rate
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	static D3D11_INPUT_ELEMENT_DESC gridPointIED[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	static D3D11_SO_DECLARATION_ENTRY gridPointSOIED[] =
	{
		// stream, semantic name, semantic index, start component, component count, output slot
		{ 0, "POSITION", 0, 0, 4, 0 },
		{ 0, "NORMAL", 0, 0, 4, 0 }
	};

	static D3D11_INPUT_ELEMENT_DESC pgmRasterizationIED[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
}