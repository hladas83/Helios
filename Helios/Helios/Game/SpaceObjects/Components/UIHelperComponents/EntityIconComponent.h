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

  class EntityIconComponent : public Component
  {
  private:
    Ref<RenderObject> _iconRenderObject;

  public:
    EntityIconComponent(Component *parent, WParamItem &entityConfig);
    ~EntityIconComponent();

    virtual void Draw() override;
  };

 

} //namespace
