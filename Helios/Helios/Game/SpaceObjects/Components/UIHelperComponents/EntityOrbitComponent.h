#pragma once

//===================================================================================================
// included dependencies

#include "../../../common.h"
#include "../Engine/Components/Component.h"
//===================================================================================================
// forward declared dependencies

namespace Helios
{

  class RenderObject;

  class EntityOrbitComponent : public Component
  {
  private:
    Ref<RenderObject> _renderOrbitObject;

  public:
    EntityOrbitComponent(Component *parent, WParamItem &entityConfig);
    ~EntityOrbitComponent();

    virtual void Draw() override;
  };

 

} //namespace
