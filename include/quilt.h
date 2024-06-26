#pragma once

/* C/C++ standard library */
#include <cctype>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
/* Quilt Headers */
#include "quilt/quilt_camera.h"
#include "quilt/quilt_constant.h"
#include "quilt/quilt_debug.h"
#include "quilt/quilt_settings.h"
#include "quilt/quilt_util.h"
/* Other */
// (imgui was already #included in quilt_util)

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <nfd.h>
#include <rapidxml.hpp>

namespace fs = std::filesystem;