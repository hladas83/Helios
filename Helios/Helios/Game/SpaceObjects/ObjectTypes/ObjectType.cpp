#include "ObjectType.h"

namespace Helios
{
  //------------------------------------------------------------------------------  

  ObjectType::ObjectType(HString name) : _name(name)
  {
    InitClass();
  };

  //------------------------------------------------------------------------------  

  ObjectType::~ObjectType() 
  {
  };
  
  //------------------------------------------------------------------------------  

  void ObjectType::InitClass()
  {

  }

  //------------------------------------------------------------------------------  

    //! create entity of this type
   Entity *ObjectType::CreateObject(Entity *parent, const HString &name)
   {
     return nullptr;
   }

  //------------------------------------------------------------------------------  

  DEFINE_CASTING(ObjectType);

} // Helios namespace
