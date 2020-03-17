#pragma once

namespace itd
{

#ifdef IL_OPENGL_BACKEND

	enum TextureWrapMode : int32_t
	{
		WM_INVALID = -1,
		WM_REPEAT = 0x2901,
		WM_MIRRORED_REPEAT = 0x8370,
		WM_CLAMP_TO_EDGE = 0x812F,
		WM_CLAMP_TO_BORDER = 0x812D
	};

	enum TextureFilterMode : int32_t
	{
		FM_INVALID = -1,
		FM_NEAREST = 0x2600,
		FM_LINEAR = 0x2601,
		FM_NEAREST_MIPMAP_NEAREST = 0x2700,
		FM_LINEAR_MIPMAP_NEAREST = 0x2701,
		FM_NEAREST_MIPMAP_LINEAR = 0x2702,
		FM_LINEAR_MIPMAP_LINEAR = 0x2703
	};

	enum TextureFormat : int32_t
	{
		TF_INVALID = -1,
		TF_R8 = 0x8229,	//GL_R8
		TF_RG8 = 0x822B,		//GL_RG8
		TF_RGB8 = 0x8051,	//GL_RGB8
		TF_RGBA8 = 0x8058	//GL_RGBA8
	};

	enum PixelFormat : int32_t
	{
		PF_INVALID = -1,
		PF_RED = 0x1903,	//GL_R
		PF_RG = 0x8227,		//GL_RG
		PF_RGB = 0x1907,	//GL_RGB
		PF_RGBA = 0x1908	//GL_RGBA
	};

#else 
#error Illithid currently supports only OpenGL.
#endif
}