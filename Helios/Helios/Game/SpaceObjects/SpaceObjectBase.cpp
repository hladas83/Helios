#include "SpaceObjectBase.h"
#include "../SpaceObjects/ObjectTypes/OrbiterType.h"
#include "../SpaceObjects/ObjectTypes/TypesBank.h"
#include "SpaceObjects/Components/MovementComponents/OrbiterSimulation.h"
#include "../../Engine/Engine.h"
#include "../Game.h"
#include "SpaceObjects/Components/UIHelperComponents/UIBasicInfo.h"
#include "SpaceObjects/Components/UIHelperComponents/EntityIconComponent.h"

namespace Helios
{

  //===================================================================================================
  // simulate thread methods

  DEFINE_CASTING(SpaceObjectBase)

  SpaceObjectBase::SpaceObjectBase(Entity *parent, ObjectType *type, const HString &name): base(parent, type, name) 
  {
    InitClass();
  };

  //------------------------------------------------------------------------------  

  void SpaceObjectBase::InitClass()
  {
    HString infoType = _entityConfig.ReadValue(HString("infoType") ,HString("planetInfoType"));
    WParamItem infoTypeCfg = GCoreConfig >> infoType;

    std::vector<HString> textures;
    textures.push_back(infoTypeCfg.ReadValue(HString("icon") ,HString("GameData/Textures/icons/planet.jpg")));

    
    EntityIconComponent *entityIconComponent = new EntityIconComponent(this, _entityConfig);
    _componentList.push_back(entityIconComponent);

    _UIBasicInfo = new UIBasicInfo();

    //load orbiters
    const WParamItem itemColor = infoTypeCfg >> "color";
    if(itemColor.IsArray() && itemColor.ArraySize() == 4)
    {
      _UIBasicInfo->SetInfoColor(HColor(
        itemColor.ReadArrayValue(0).GetValue<float>(),
        itemColor.ReadArrayValue(1).GetValue<float>(),
        itemColor.ReadArrayValue(2).GetValue<float>(),
        itemColor.ReadArrayValue(3).GetValue<float>()));
    }
  }

  //------------------------------------------------------------------------------  

  void SpaceObjectBase::ReInit()
  {
    base::ReInit();
    InitClass();
  }

  //------------------------------------------------------------------------------

  SpaceObjectBase::~SpaceObjectBase() 
  {
  };
  
  //------------------------------------------------------------------------------

  void SpaceObjectBase::Simulate(float deltaT)
  {
    CheckNull(GetMovementSimulation());
    GetMovementSimulation()->Simulate(this, deltaT);
    base::Simulate(deltaT);
  }

  //------------------------------------------------------------------------------

  void SpaceObjectBase::PostSimulate()
  {
    base::PostSimulate();
  }

  //------------------------------------------------------------------------------

  void SpaceObjectBase::Draw()
  {
    ////render info type icon
    //DrawContext context = DrawContext(_iconRenderObject, GetRenderVisualState()->_frame);
    //context.SetScale(0.02f / GEngine->GDraw()->GetAspectRatio(), 0.02f, 1.0f);
    //context.SetColor(_iconRenderObject->GetInfoColor());

    //GEngine->GDraw()->RenderObject(context);
    base::Draw();
  }

  //------------------------------------------------------------------------------

  void SpaceObjectBase::PostDraw()
  {
    base::PostDraw();
  } 
} //namespace Helios