#pragma once

#ifndef _SPACE_TYPE
#define _SPACE_TYPE

//===================================================================================================
// included dependencies

#include "../../common.h"
#include "../../Elementary/TaskManager/TaskManager.h"

namespace Helios
{

  //===================================================================================================
  // forward declared dependencies

  class Galaxy;
  class Space;

  //===================================================================================================
  //!Simulate thread parameters
  
  class SpaceSimulateTaskParams : public ITaskParametrs
  {
    typedef ITaskParametrs base;
  
  private:

  public:
    SpaceSimulateTaskParams()
    {

    };

    virtual ~SpaceSimulateTaskParams(){};

    USE_CASTING(base)
  };

  //===================================================================================================
  //!Simulate task 
  class SpaceSimulateTask : public Task
  {
    typedef Task base;

  public:
    SpaceSimulateTask(SpaceSimulateTaskParams *param) : base(param)  {};
    void virtual Pefrom() override;
  };

  //===================================================================================================
  //!Space class
  class Space : public SingletonTask<Space>, public RefCount, public OLinkBase<Space>
  {

  private:
    //!list of all galaxies in space
    std::list<Ref<Galaxy>> _galaxies;
    //!selected galaxy
    Ref<Galaxy> _activeGalaxy;

    //!synchronizatio after render / simulation lock
    std::mutex _post_simulate_render_lock;  

    long int _previousTime;

  public:

  private:
    bool _runSimulation;

  public:
    Space();
    virtual ~Space();

    //!Initialize default values
    bool Init();

    //!Load data based on config
    void LoadGame();

    //!Galaxies handling
    int AddGalaxy();
    //!Set selected galaxy
    void SetActiveGalaxy(const Ref<Galaxy> galaxy);
    //!True if we are in some galaxy
    __forceinline bool IsActiveGalaxy() const;

    //!Simulate loop
    void *Simulate(float deltaT);
    void FinishSimulation();

    //! Draw thread method
    void Draw();
    void PostDraw();

    //!delete all galaxies 
    void CleanUp();

    //! perform function for each type
    template<class Functor> void ForEachGalaxy(Functor &f)
    {
      std::for_each(_galaxies.begin(), _galaxies.end(), f);
    };

    USE_CASTING_ROOT;
  };

} // Helios namespace

#endif