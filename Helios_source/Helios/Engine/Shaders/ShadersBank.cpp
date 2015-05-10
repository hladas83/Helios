#include "ShadersBank.h"

namespace Helios
{
  //===================================================================================================

  void ShadersBank::Init()
  {
    LoadItem("GameData/FX/Planet.fx");
    LoadItem("GameData/FX/Line.fx");
    LoadItem("GameData/FX/Icon.fx");
    LoadItem("GameData/FX/Star.fx");
    LoadItem("GameData/FX/Atmosphere.fx");
  }
}//namespace