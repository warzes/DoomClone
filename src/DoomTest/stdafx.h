#pragma once

#if defined(_MSC_VER)
#	pragma warning(push, 3)
#	pragma warning(disable : 5039)
#endif

#define GLFW_INCLUDE_NONE

#include <3rdparty/glfw.h>
#include <3rdparty/gl.h>
#include <string>
#include <cstdio>
#include <cstddef>
#include <cstdarg>
#include <cmath>

#include "Engine/EngineApp.h"

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif