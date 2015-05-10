#pragma once

#ifndef _HELIOS_COMMON_TYPE
#define _HELIOS_COMMON_TYPE

//------------------------------------------------------------------------------

//release memory macro
#define RELEASE_MEM(XX) if(XX) {XX->Release(); XX = nullptr;} 

//!Basic Singletons
#define GTaskManager TaskManager::GetInstance()
#define GEngine Engine::GetInstance()
#define GGame Game::GetInstance()
#define GCoreConfig *(GEngine->GFileServer())

//------------------------------------------------------------------------------

// Elementary types
#include "Elementary/Types.h"
#include "Elementary/PerfLog.h"
#include "Elementary/LogF.h"
#include "Elementary/assert.h"

// STD thredas
#include <thread>

// External libs.
#include <string>
#include <math.h>

//STL containers
#include <list>
#include <map>

// Elementary draw types
#include "Engine/DX11Draw/DX11DrawTypes.h"

// Elementary config types
#include "Engine/FileServer/ParamItem.h"


#endif // _HELIOS_COMMON_TYPE