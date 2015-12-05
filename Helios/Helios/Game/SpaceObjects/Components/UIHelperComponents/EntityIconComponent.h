#pragma once

//===================================================================================================
// included dependencies

#include "../../../common.h"
#include "../Engine/Components/ComponentHolder.h"
//===================================================================================================
// forward declared dependencies

namespace Helios
{

  class RenderObject;

  class EntityIconComponent : public ComponentHolder
  {
  private:
    Ref<RenderObject> _iconRenderObject;

  public:
    EntityIconComponent(ComponentHolder *parent, WParamItem &entityConfig);
    ~EntityIconComponent();

    virtual void Draw() override;
  };

 

} //namespace
