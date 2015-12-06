#include "UIBasicInfo.h"

namespace Helios
{
  DEFINE_CASTING(UIBasicInfo);
  DEF_COMPONENTHOLDER_FACTORY_REG(UIBasicInfo, HString("uibasicinfo"));

  void UIBasicInfo::InitClass(ComponentHolder *parent, WParamItem &entityConfig)
  {

    //load UI info
    const WParamItem itemColor = entityConfig >> "color";
    if (itemColor.IsArray() && itemColor.ArraySize() == 4)
    {
      SetInfoColor(HColor(
        itemColor.ReadArrayValue(0).GetValue<float>(),
        itemColor.ReadArrayValue(1).GetValue<float>(),
        itemColor.ReadArrayValue(2).GetValue<float>(),
        itemColor.ReadArrayValue(3).GetValue<float>()));
    }

  };

}