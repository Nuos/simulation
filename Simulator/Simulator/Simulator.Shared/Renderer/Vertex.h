#pragma once

#include "Base\Math\Vector.h"

namespace Renderer
{
	template<typename T>
	struct BaseVertex
	{
		T x, y, z;			// position
		T nx, nt, nz;		// normal direction
	};
}