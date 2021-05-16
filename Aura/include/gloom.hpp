#pragma once
#define STB_IMAGE_IMPLEMENTATION

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "gloomcogs/sys.hpp"
#include "gloomcogs/debug.hpp"
#include "gloomcogs/shader.hpp"
#include "gloomcogs/const.hpp"
#include "gloomcogs/input.hpp"
#include "gloomcogs/camera.hpp"
#include "gloomcogs/model.hpp"
#include "gloomcogs/window.hpp"
#include "gloomcogs/2d.hpp"

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <shellapi.h>
#include <cmath>
#include <thread>