#pragma once

#ifndef _HELIOS_FILESERVER
#define _HELIOS_FILESERVER

//===================================================================================================
// included dependencies

#include "../common.h"
#include "ParamItem.h"

namespace Helios
{

  //===================================================================================================
  // FileServer class

  class FileServer : public Singleton<FileServer>, public RefCount, public OLinkBase<FileServer>
  {

  private:
    ParamClass _coreClass;
    HString _position;

  public:
    FileServer(): _position("GameData\\configs\\"), _coreClass("core") {};
    virtual ~FileServer() {};

    /// core config
    bool LoadParams();
    /// load given config
    bool LoadParams(const char *path);

    ///delete all types
    void CleanUp();

  private:

    //------------------------------------------------------------------------------
    ///read params from included file
    bool CheckInclude(HString &param);
    ///read item and it's parmetrs, currentClass is owner of parametr
    bool CheckClass(HString &param, std::ifstream *file, ParamClass *currentClass);
    bool CheckArray(HString &param, std::ifstream *file, ParamClass *currentClass);
    bool CheckScalarOrString(HString &param, ParamClass *currentClass);

    //------------------------------------------------------------------------------
    ///remove // comments from line
    void RemoveComments(HString &line);

    ///get parametr from line
    bool ParseLine(std::string &rawLine, std::ifstream *file, ParamClass *currentClass);
    bool CheckFirstChar(HString &line, char c);

  public:
    virtual WParamItem operator >> (const HString &name) { return WParamItem(_coreClass >> name);};

    USE_CASTING_ROOT;
  };

} // Helios namespace

#endif //_HELIOS_FILESERVER