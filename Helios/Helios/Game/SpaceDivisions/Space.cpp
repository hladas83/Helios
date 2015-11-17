#include "../../common.h"

#include "../Game.h"
#include "Space.h"
#include "Galaxy.h"
#include "../../Engine/Engine.h"

namespace Helios
{
  DEFINE_CASTING(SpaceSimulateTaskParams);

  //===================================================================================================
  // thread tasks

  void SpaceSimulateTask::Pefrom() 
  {
    SpaceSimulateTaskParams *params = dyn_cast<SpaceSimulateTaskParams>(_parametrs.GetRef());
    if(params)
    {
      float deltaT = 0.001f;
      if(GGame->GSpace()) GGame->GSpace()->Simulate(deltaT);
    }
  };

  //===================================================================================================
  // Space class

  DEFINE_CASTING(Space);

  Space::Space()
  {
    _previousTime = -1;
    _runSimulation = true;
    SetActiveGalaxy(nullptr);
  };

  //------------------------------------------------------------------------------

  Space::~Space()
  {

  };


  //------------------------------------------------------------------------------

  void Space::LoadGame()
  {
    //create new galaxies
    const WParamItem itemSpace = GCoreConfig >> "space";
    if(itemSpace.IsClass())
    {
      const WParamItem itemgalaxies = itemSpace >> "galaxies";
      if(itemgalaxies.IsArray())
      {
        for(int i = 0; i< itemgalaxies.ArraySize(); ++i)
        {
          const WParamItem itemgalaxiesArray = itemgalaxies.ReadArrayValue(i);
          if(!itemgalaxiesArray.IsString()) continue;
          //create new galaxy
          Ref<Galaxy> galaxy(new Galaxy(nullptr, nullptr,itemgalaxiesArray.GetValue(HString(""))));
          //and make it active
          _galaxies.push_front(galaxy);
          if(_galaxies.size() == 1) SetActiveGalaxy(galaxy);
        }
      }
    }
  };

  //------------------------------------------------------------------------------

  //!Initialize default values
  bool Space::Init()
  {
    return true;
  }

  //------------------------------------------------------------------------------

  int Space::AddGalaxy()
  {
    return 1;
  }

  //------------------------------------------------------------------------------

  void Space::SetActiveGalaxy(Ref<Galaxy> galaxy)
  {
    _activeGalaxy = galaxy;
  }

  //------------------------------------------------------------------------------

  bool Space::IsActiveGalaxy() const 
  {
    return _activeGalaxy != nullptr;
  }

  //------------------------------------------------------------------------------

  void *Space::Simulate(float deltaT)
  {
    // Main loop
    while(_runSimulation && GEngine && GEngine->GDraw() && DX11Draw::IsRunning())
    {
      if(_previousTime < 0) 
      {
        _previousTime = GetTickCount();
        continue;
      }

      long int time = GetTickCount();
      unsigned long deltaT = time - _previousTime;
      _previousTime = time;

      if(_activeGalaxy) 
      {
        _activeGalaxy->Simulate(deltaT * 0.001f);
        //update render visual states, lock render until update is done
        {
          std::unique_lock<std::mutex> lk(_post_simulate_render_lock);
          _activeGalaxy->PostSimulate();
        }
      }
    }

    return nullptr;
  }

  //------------------------------------------------------------------------------

  void Space::FinishSimulation()
  {
    _runSimulation = false;
  }

  //------------------------------------------------------------------------------

  void Space::Draw()
  {
    if(IsActiveGalaxy())
    {
      //draw selected galaxy
      _activeGalaxy->Draw();
    }
    else
    {
      //draw current Space
    }
  }

  //------------------------------------------------------------------------------

  void Space::PostDraw()
  {
    if(IsActiveGalaxy())
    {
      //move to next render visial state, lock simulation until update is done
      {
        std::unique_lock<std::mutex> lk(_post_simulate_render_lock);
        _activeGalaxy->PostDraw();
      }
    }
  }
  //------------------------------------------------------------------------------


  //!delete all galaxies 
  void Space::CleanUp()
  {
    ForEachGalaxy([](Ref<Galaxy> galaxy) {  galaxy->CleanUp();});
    _galaxies.clear();
  }

} // Helios namespace