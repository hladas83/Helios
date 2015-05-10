#include "ShapesBank.h"


namespace Helios
{
  //===================================================================================================-

  ShapesBank::ShapesBank() 
  {
  };

  //------------------------------------------------------------------------------

  ShapesBank::~ShapesBank() 
  {
  };

  //------------------------------------------------------------------------------

  void ShapesBank::Init()
  {
    LoadItem("GameData/models/sphere.3DS");
    LoadItem("GameData/models/sphereFliped.3DS");

    //helpers
    _itemsBank["circle"] = new ShapeCircle("circle");
    _itemsBank["quad"] = new ShapeQuad("quad");
  }

  //------------------------------------------------------------------------------

  bool ShapesBank::LoadItem(const char* itemPath)
  {
    //TODO
    HString itemNameLower = HString(itemPath);
    std::transform(itemNameLower.begin(), itemNameLower.end(), itemNameLower.begin(), ::tolower);

    if(!GetItem(itemPath)) _itemsBank[itemNameLower.data()] = new ShapeAssimp(itemNameLower.data());
    return true;
  }

  //------------------------------------------------------------------------------

  void ShapesBank::CleanupDX11Device()
  {
    ForEachItem([](Shape *shape) ->bool {shape->CleanupDX11Device(); return false; });
  }

  //------------------------------------------------------------------------------

} // Helios namespace