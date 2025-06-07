#ifndef __ZLGCAN_DEMO__H
#define __ZLGCAN_DEMO__H
// header for gui
#define DEFAULT_WINDOW_WIDTH 1920
#define DEFAULT_WINDOW_HEIGHT 1080
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include <stdio.h>          // printf, fprintf
#include <stdlib.h>         // abort
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
// Volk headers
#ifdef IMGUI_IMPL_VULKAN_USE_VOLK
#define VOLK_IMPLEMENTATION
#include <volk.h>
#endif
// std headers
#include <iostream>
#include <vector>
// header for zlgcan
#include "zlgcan.h"
#include "zdbc/zdbc.h"
// header for open directory
#include "nfd.h"
#endif
