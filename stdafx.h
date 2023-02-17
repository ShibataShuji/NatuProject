#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <d3dx9.h>
#include <D3D11.h>
#include <D3DX11tex.h>
#include <io.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <shlwapi.h>
#include <random>
#include <string>
#include <math.h>
#include <corecrt_math_defines.h>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <array>
#include <vector>
#include <list>
#include <typeinfo>
#include <xaudio2.h>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"




// main.h
#include "dinput.h"
#include "mmsystem.h"
#include "input.h"


#include "primitive.h"
#include "renderer_Support.h"
#include "Burush_Support.h"

#include "GameEngineData.h"

#include "main.h"
#include "scene.h"
#include "manager.h"
#include "gui.h"
#include "renderer.h"
#include "model.h"
#include "audio.h"
#include "Savedata.h"

#include "testPolygon.h"

#include "SimpleBoundingBox2D.h"
#include "SimpleBoundingBox3D.h"


// Scene
#include "Game.h"
#include "CreateNewObjectScene.h"
#include "CreateNewStageScene.h"
#include "ImageDisplayFull.h"
#include "Title.h"
#include "Result.h"
#include "StageSelectScene.h"


#include "gameObject.h"
#include "MeshFieldObject.h"

// 0
#include "FPSCamera.h"
#include "camera.h"
// 1
#include "field.h"
#include "Sky.h"
#include "OriginalBlock.h"
#include "LoadedObject.h"
#include "Shadow.h"

#include "brush.h"

// 2
#include "polygon2D.h"


#include "CComponent.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "ModelComponent.h"