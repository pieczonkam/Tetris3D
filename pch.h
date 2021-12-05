// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// Includes necessary for memory leak check
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>  
	#include <crtdbg.h>
#endif

// SFML/OpenGL/ImGui includes
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\OpenGL.hpp>
#include <gl\GLU.h>
#include "imgui.h"
#include "imgui-SFML.h"

// Standard library includes
#include <iostream>
#include <numbers>
#include <cmath>

// User-defined structs includes
#include "Settings.h"
#include "Utils.h"
#include "BlockProperties.h"
#include "Block.h"
#include "Grid.h"
#include "Spherical.h"
#include "Drawing.h"

#endif //PCH_H
