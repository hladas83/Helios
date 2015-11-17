#pragma once

#ifndef _HELIOS_COLOR_H
#define _HELIOS_COLOR_H

#include <stdio.h>


namespace Helios
{
  //===================================================================================================
  // Basic color representation

  class HColor
  {
  private:
    float _data[4];

  public:
    //------------------------------------------------------------------------------  
    HColor()
    { 
      memset(_data, 0, 4); 
    };

    //------------------------------------------------------------------------------  
    HColor(float r, float g, float b,float a)
    {
      SetColor(r, g, b, a);
    };

    //------------------------------------------------------------------------------  
    __inline float GetR() const {return _data[0];};
    __inline float GetG() const {return _data[1];};
    __inline float GetB() const {return _data[2];};
    __inline float GetA() const {return _data[3];};

    //------------------------------------------------------------------------------  
    __inline void SetR(float value) {_data[0] = value;};
    __inline void SetG(float value) {_data[1] = value;};
    __inline void SetB(float value) {_data[2] = value;};
    __inline void SetA(float value) {_data[3] = value;};

    //------------------------------------------------------------------------------  
    void SetColor(float r, float g, float b,float a) 
    {
      _data[0] = r;
      _data[1] = g;
      _data[2] = b;
      _data[3] = a;
    };
  };

}
#endif // _HELIOS_COLOR_H