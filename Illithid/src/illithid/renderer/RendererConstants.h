#pragma once
#include <stdint.h>

namespace itd
{
#ifdef IL_OPENGL_BACKEND

	enum BufferBit : int32_t
	{
		BB_Color = 0x00004000,
		BB_Depth = 0x00000100,
		BB_Stencil = 0x00000400
	};

	enum Capability : int32_t
	{
		Cap_DepthTest = 0x0B71,
		Cap_StencilTest = 0x0B90
	};

	enum PolygonMode : int32_t
	{
		PM_Point = 0x1B00,
		PM_Line = 0x1B01,
		PM_Fill = 0x1B02
	};

	enum PolygonFace : int32_t
	{
		PF_Front = 0x0404,
		PF_Back = 0x0405,
		PF_FrontAndBack = 0x0408
	};

#endif
}