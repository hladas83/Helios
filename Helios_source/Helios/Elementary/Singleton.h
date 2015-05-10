#pragma once

#ifndef _HELIOS_SINGLETON
#define _HELIOS_SINGLETON

#include "NoCopy.h"

namespace Helios
{
  //===================================================================================================
  /// Basic singleton class

  template<class Type> class Singleton : public NoCopy
  {
  protected:
    static Ref<Type> _instance;    

  public:                    
    //------------------------------------------------------------------------------  
    static Type *GetInstance()                                              
    {                                                                       
      if(!_instance) _instance = new Type();           
      return _instance;                                                      
    };                                                                                  

    //------------------------------------------------------------------------------  
    static void Release()  
    {
      if(_instance) 
        _instance->CleanUp();  
      _instance = nullptr;
    };   
  };

  template<class Type>
  Ref<Type> Singleton<Type>::_instance = nullptr; 

  //===================================================================================================
  /// Singleton class for tasks

  template<class Type>
  class SingletonTask : public Singleton<Type>
  {
  private:

    /// Marks that task is to be terminated
    static bool _terminate;

  public:                                                                                                                                         
    static void Terminate() {_terminate = true;};                           
    static bool IsRunning() {return !_terminate;};       

    //------------------------------------------------------------------------------  
    static void Release()   
    {
      if(_instance) 
        _instance->CleanUp(); 
      _terminate = true; 
      _instance = nullptr;
    };   
  };

  template<class Type>
  bool SingletonTask<Type>::_terminate = false;

}
#endif // _HELIOS_SINGLETON