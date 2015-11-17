#pragma once

#ifndef  _HELIOS_SHAPE_TYPE
#define  _HELIOS_SHAPE_TYPE

//===================================================================================================
// included dependencies

#include "../../common.h"

#include <windows.h>
#include <d3dcompiler.h>
//#include "../DX11Draw/DX11Draw.h"

// assimp include files. These three are usually needed.
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Helios
{

  class DrawContext;

  //===================================================================================================
  // Shape class

  class Shape : public RefCount, public OLinkBase<Shape>
  {
  protected:
    //! unieque shape name
    HString _name;
    int _verticesCount, _indicesCount;

    SimpleVertex *_vertices;
    WORD *_indices;

  public:
    ID3D11Buffer*           _g_pVertexBuffer;
    ID3D11Buffer*           _g_pIndexBuffer;

  private:
    // initialize this class (not virtual, so it can be called from constructor)
    void InitClass();

  public:
    Shape(HString name);
    virtual ~Shape();

    //----------------------------------------------------------------------------
    __forceinline int GetVerticesCount() const {return _verticesCount;}; 
    __forceinline int GetIindicesCount() const {return _indicesCount;}; 

    //----------------------------------------------------------------------------
    // draw thread methods

    virtual bool InitDX11(ID3D11Device* _g_pd3dDevice);
    virtual void Draw(DrawContext &drawContext);
    virtual void CleanupDX11Device();

    USE_CASTING_ROOT;
  };


  //===================================================================================================
  // ShapeCircle class

  class ShapeCircle : public Shape
  {
    typedef Shape base;

  public:
    ShapeCircle(HString name);
    virtual ~ShapeCircle(){};

    //----------------------------------------------------------------------------
    // draw thread methods

    virtual bool InitDX11(ID3D11Device* _g_pd3dDevice);
    virtual void Draw(DrawContext &drawContext);


    USE_CASTING(base)
  };

  //===================================================================================================
  // ShapeCircle class

  class ShapeQuad : public Shape
  {
    typedef Shape base;

  public:
    ShapeQuad(HString name);
    virtual ~ShapeQuad(){};

    //----------------------------------------------------------------------------
    // draw thread methods

    virtual bool InitDX11(ID3D11Device* _g_pd3dDevice);
    virtual void Draw(DrawContext &drawContext);


    USE_CASTING(base)
  };

  //===================================================================================================
  // ShapeAssimp class

  class ShapeAssimp : public Shape
  {
    typedef Shape base;

  public:
    ShapeAssimp(const char* path);
    virtual ~ShapeAssimp();

    //--------------------------------------------------------------------------
    // draw thread methods

    virtual bool InitDX11(ID3D11Device* _g_pd3dDevice);
    virtual void Draw(DrawContext &drawContext);


    USE_CASTING(base)
  };

} // Helios namespace 

#endif //  _HELIOS_SHAPE_TYPE