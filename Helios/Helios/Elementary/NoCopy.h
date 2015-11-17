#pragma once

#ifndef _HELIOS_NOCOPY
#define _HELIOS_NOCOPY


namespace Helios
{
  //===================================================================================================
  // Disallows object copy

  class NoCopy
  {
  public:
    NoCopy(){};
    ~NoCopy() {};

    //------------------------------------------------------------------------------  
    NoCopy( const NoCopy& );// = delete;
    NoCopy& operator=( const NoCopy& );// = delete;
  };
}
#endif // _HELIOS_NOCOPY