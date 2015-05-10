//===================================================================================================
// included dependencies
#include "FileServer.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <cctype>
#include <clocale>
#include <algorithm>

namespace Helios
{
  //===================================================================================================
  //FileServer
  DEFINE_CASTING(FileServer);

  //------------------------------------------------------------------------------
  // trim from start
  static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
  };

  //------------------------------------------------------------------------------
  // trim from end
  static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
  };

  //------------------------------------------------------------------------------
  // trim from both ends
  static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
  };


  //------------------------------------------------------------------------------
  // slip by char 
  static std::vector<std::string> &Split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
      elems.push_back(item);
    }
    return elems;
  };

  //------------------------------------------------------------------------------

  static std::vector<std::string> Split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    Split(s, delim, elems);
    return elems;
  };

  //------------------------------------------------------------------------------
  // remove "" from string
  static inline std::string ParseString(std::string &s) {
    HString trimmed = trim(s);
    std::vector<std::string> valuesSplited = Split(trimmed, '"');
    
    if(valuesSplited.size() > 0) return valuesSplited[1];
    else return "";
  };

  //------------------------------------------------------------------------------

  // convert to float
  static inline double ParseScalar(std::string &s) {
    return atof(s.data());
  };


  //------------------------------------------------------------------------------

  //delete all types
  void FileServer::CleanUp()
  {   

  };

  //------------------------------------------------------------------------------

  bool FileServer::LoadParams()
  {
    return LoadParams("core.hcf");
  };

  //------------------------------------------------------------------------------

  void FileServer::RemoveComments(HString &line)
  {
    const char* comPos = strstr(line.data(), "//");
    if(comPos != nullptr) line = line.substr(0, comPos - line.data());
  }

  //------------------------------------------------------------------------------

  bool FileServer::ParseLine(std::string &rawLine, std::ifstream *file, ParamClass *currentClass)
  {
    std::transform(rawLine.begin(), rawLine.end(), rawLine.begin(), ::tolower);
    std::string lowTrimmedString(trim(rawLine));
    RemoveComments(lowTrimmedString);

    if (CheckInclude(lowTrimmedString)) return true;
    if (CheckClass(lowTrimmedString, file, currentClass)) return true;
    if (CheckArray(lowTrimmedString, file, currentClass)) return true;
    if (CheckScalarOrString(lowTrimmedString, currentClass)) return true;

    return false;
  }

  //------------------------------------------------------------------------------

  bool FileServer::LoadParams(const char *path)
  {
    HString fullPath = _position + path;

    std::ifstream file(fullPath);
    std::string rawLine;
    while(std::getline(file, rawLine)) 
    {
      ParseLine(rawLine, &file, &_coreClass);
    }

    return true;
  }

  //------------------------------------------------------------------------------

  bool FileServer::CheckInclude(HString &param)
  {
    const char* includePos = strstr(param.data(), "include");
    if(includePos != nullptr)
    {
      HString path = ParseString(HString(includePos + strlen("include")));
      LoadParams(path.data());
    }
    return false;
  }

  //------------------------------------------------------------------------------

  bool FileServer::CheckClass(HString &param, std::ifstream *file, ParamClass *currentClass)
  {
    if(!file) return false;
    std::string rawLine;

    const char* classPos = strstr(param.data(), "class");
    if(classPos != nullptr)
    {
      HString className = "", parentName = "";

      const char* start = classPos + strlen("class");
      const char* parentStartstd = std::strchr(start, ':');

      if(parentStartstd != nullptr)
      {
        className = HString(start).substr(0, parentStartstd - start);
        className = trim(className);

        parentStartstd = parentStartstd + 1;
        parentName = trim(HString(parentStartstd));
      }
      else
      {
        className = trim(HString(start));
      }

      ParamClass *newClass = currentClass->AddClass(className, parentName);
      if(!newClass) return false;

      int scopeStart = 0, scopeEnd = 0;
      while(std::getline(*file, rawLine)) 
      {
        if(CheckFirstChar(rawLine,'{')) scopeStart++;
        //get parametr
        ParseLine(rawLine, file, newClass);
        //check class scope end
        if(CheckFirstChar(rawLine,'}')) scopeEnd++;
        //if scopeStart == scopeEnd, all scopes are closed. There must be at least one scope
        if(scopeStart == scopeEnd && scopeStart > 0) break;
      }

      return true;
    }

    return false;
  }

  //------------------------------------------------------------------------------

  bool FileServer::CheckArray(HString &param, std::ifstream *file, ParamClass *currentClass)
  {
    if(!file) return false;
    std::string rawLine;

    const char* arrayPos = strstr(param.data(), "[]");
    if(arrayPos != nullptr)
    {
      HString arrayName = param.substr(0, arrayPos - param.data());
      arrayName = trim(arrayName);

      HString arrayValues = "";

      const char* start = std::strchr(param.data(), '{');
      const char* end = std::strchr(param.data(), '}');
      const char* mistake = std::strchr(param.data(), ';');
      
      ParamArray *paramArray = currentClass->AddArray(arrayName);
      while(true)
      {
        //todo
        if(!start) 
          start = param.data() - 1;
        if(!end) 
          end = param.data() + param.size() + 1;

        HString values = param.substr(start - param.data() + 1, end - start - 1);
        std::vector<std::string> valuesSplited = Split(values, ',');
        for(unsigned int i = 0; i < valuesSplited.size(); i++)
        {
          if(std::strchr(valuesSplited[i].data(), '"'))
          {
            //String
            paramArray->AddItem(new ParamString("", ParseString(valuesSplited[i])));
          }
          else if(std::strstr(valuesSplited[i].data(), "true"))
          {
            //bool - true
            currentClass->AddBool(trim(valuesSplited[0]), true);
          }
          else if(std::strstr(valuesSplited[i].data(), "false"))
          {
            //bool - false
            currentClass->AddBool(trim(valuesSplited[0]), false);
          }
          else 
          {
            //scalar
            paramArray->AddItem(new ParamScalar("", (float)ParseScalar(valuesSplited[i])));
          }
        }

        if(std::strchr(param.data(), '}')) break;
        if(mistake) break;

        //read next line of parametrs
        std::getline(*file, rawLine);

        //todo
        std::transform(rawLine.begin(), rawLine.end(), rawLine.begin(), ::tolower);
        std::string lowTrimmedString(trim(rawLine));
        RemoveComments(lowTrimmedString);

        param = lowTrimmedString;

        start = std::strchr(param.data(), '{');
        end = std::strchr(param.data(), '}');
        mistake = std::strchr(param.data(), ';');
      } 


      return true;
    }

    return false;
  }

  //------------------------------------------------------------------------------

  bool FileServer::CheckScalarOrString(HString &param, ParamClass *currentClass)
  {
    std::vector<std::string> valuesSplited = Split(param, '=');
    if(valuesSplited.size() > 1)
    {
      
      if(std::strchr(valuesSplited[1].data(), '"'))
      {
        //String
        currentClass->AddString(trim(valuesSplited[0]), ParseString(valuesSplited[1]));
      }
      else if(std::strstr(valuesSplited[1].data(), "true"))
      {
        //bool - true
        currentClass->AddBool(trim(valuesSplited[0]), true);
      }
      else if(std::strstr(valuesSplited[1].data(), "false"))
      {
        //bool - false
        currentClass->AddBool(trim(valuesSplited[0]), false);
      }
      else
      {
        //scalar
        currentClass->AddScalar(trim(valuesSplited[0]), (float)ParseScalar(valuesSplited[1]));
      }
      return true;
    }
    return false;
  }

  //------------------------------------------------------------------------------

  bool FileServer::CheckFirstChar(HString &line, char c)
  {
    std::string trimmedString(trim(line));
    if(strlen(trimmedString.data())>=1 && trimmedString[0] == c) return true;
    return false;
  }

} // Helios namespace