#include "../../Common.h"

#include "Galaxy.h"
#include "StarSystem.h"

#include "../../Engine/Engine.h"
#include "../../Elementary/TaskManager/TaskManager.h"

namespace Helios
{

  //===================================================================================================
  // simulate task 

  //simulate thread parameters
  class GalaxySimulateParams : public ITaskParametrs
  {
    typedef ITaskParametrs base;

  public:
    Ref<StarSystem> _starSystem;
    float _deltaT;

  public:
    GalaxySimulateParams(StarSystem *starSystem, float deltaT);
    GalaxySimulateParams();
    virtual ~GalaxySimulateParams();

    USE_CASTING(base)
  };

  DEFINE_CASTING(GalaxySimulateParams);

  //------------------------------------------------------------------------------

  GalaxySimulateParams::GalaxySimulateParams(StarSystem *starSystem, float deltaT): _starSystem(starSystem), _deltaT(deltaT)
  {
  };

  //------------------------------------------------------------------------------

  GalaxySimulateParams::GalaxySimulateParams(): _starSystem(nullptr), _deltaT(0)
  {
  };

  //------------------------------------------------------------------------------

  GalaxySimulateParams::~GalaxySimulateParams()
  {
  }

  //===================================================================================================
  // Simulation task

    //thread start method
  class GalaxySimulateTask : public Task
  {
    typedef Task base;
  public:
    GalaxySimulateTask(GalaxySimulateParams *param, TaskList *parentList) : base(param, parentList) {};
    void virtual Pefrom() override;

    USE_CASTING(base)
  };

  DEFINE_CASTING(GalaxySimulateTask);

  //------------------------------------------------------------------------------

  void GalaxySimulateTask::Pefrom() 
  {
    GalaxySimulateParams *params = dyn_cast<GalaxySimulateParams>(_parametrs.GetRef());
    if(params)
    {
      if(params->_starSystem) params->_starSystem->Simulate(params->_deltaT);
    }
  };

  //===================================================================================================
  // Space class

  DEFINE_CASTING(Galaxy)

  Galaxy::Galaxy() : Entity()
  {

  };

  Galaxy::Galaxy(Entity *parent, ObjectType *type, const HString &name) : Entity(parent, type, name)
  {
    InitClass();
  }

  //------------------------------------------------------------------------------  

  void Galaxy::InitClass()
  {
    if(_entityConfig.IsClass())
    {
      const WParamItem itemStarsystems = _entityConfig >> "starsystems";
      if(itemStarsystems.IsArray())
      {
        for(int i = 0; i< itemStarsystems.ArraySize(); ++i)
        {
          const WParamItem itemStarsystemsArray = itemStarsystems.ReadArrayValue(i);
          if(!itemStarsystemsArray.IsString()) continue;
          //create new galaxy
          Ref<StarSystem> starSystem(new StarSystem(this, nullptr,itemStarsystemsArray.GetValue(HString(""))));
          //and make it active
          _starSystems.push_front(starSystem);
          if(_starSystems.size() == 1) SetActiveStarSystem(starSystem);
        }
      }
    }
  }

  //------------------------------------------------------------------------------  

  void Galaxy::ReInit()
  {
    base::ReInit();
    InitClass();
  }

  //------------------------------------------------------------------------------

  Galaxy::~Galaxy()
  {
    __asm nop;
  };

  //------------------------------------------------------------------------------

  void Galaxy::SetActiveStarSystem(const Ref<StarSystem> starSystem)
  {
    _activeStarSystem = starSystem;
  }

  //------------------------------------------------------------------------------

  bool Galaxy::IsActiveStarSystem() const
  {
    return _activeStarSystem != nullptr;
  }

  //------------------------------------------------------------------------------

  void Galaxy::Simulate(float deltaT)
  {
    Ref<TaskList> taskList = new TaskList();
    for (std::list<Ref<StarSystem>>::const_iterator iterator = _starSystems.begin(), end = _starSystems.end(); iterator != end; ++iterator) 
    {
      GalaxySimulateTask *simulateTask = new GalaxySimulateTask(new GalaxySimulateParams(*iterator, deltaT), taskList);
      taskList->AddTask(simulateTask);
    }

    GTaskManager->RunTaskListSynchronous(taskList);  
  }

  //------------------------------------------------------------------------------

  void Galaxy::PostSimulate() 
  {
    base::PostSimulate();
    ForEachStarSystem([](Ref<StarSystem> starSystem) {  starSystem->PostSimulate(); });
  }

  //------------------------------------------------------------------------------

  void Galaxy::Draw()
  {
    if(IsActiveStarSystem())
    {
      //draw selected star system
      _activeStarSystem->Draw();
    }
    else
    {
      //draw this star system
    }
  }

  //------------------------------------------------------------------------------

  void Galaxy::PostDraw() 
  {
    base::PostDraw();
    ForEachStarSystem([](Ref<StarSystem> starSystem) {  starSystem->PostDraw(); });
  }

  //------------------------------------------------------------------------------

  //!delete all galaxies 
  void Galaxy::CleanUp()
  {
    ForEachStarSystem([](Ref<StarSystem> starSystem) {  starSystem->CleanUp(); });
    _starSystems.clear();
  }

} // Helios namespace