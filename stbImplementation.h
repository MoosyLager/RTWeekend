#ifndef STB_IMPLEMENTATION_H
#define STB_IMPLEMENTATION_H

//! Disable strict warnings for this header from MSVC
#ifdef _MSC_VER
#pragma warning(push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb/stb_image.h"
#define STBI_MSC_SECURE_CRT

//! Restore MSVC compiler warnings
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif