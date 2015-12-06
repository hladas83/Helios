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

  class EntityOrbitComponent : public ComponentHolder
  {
    typedef ComponentHolder base;

  private:
    Ref<RenderObject> _renderOrbitObject;

  public:
    EntityOrbitComponent();
    ~EntityOrbitComponent();
    virtual void InitClass(ComponentHolder *parent, WParamItem &entityConfig) override;

    virtual void Draw() override;

    USE_CASTING(base);
    DECL_COMPONENTHOLDER_FACTORY_REG(EntityOrbitComponent);
  };

 

} //namespace
