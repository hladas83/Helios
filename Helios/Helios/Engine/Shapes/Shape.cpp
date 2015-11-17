#include "Shape.h"
#include "../Engine.h"

namespace Helios
{

  //===================================================================================================

  DEFINE_CASTING(Shape)

  //----------------------------------------------------------------------------

  Shape::Shape(HString name): 
    _name(name), 
    _verticesCount(0), 
    _indicesCount(0), 
    _vertices(nullptr), 
    _indices(nullptr)  
  {
    InitClass(); 
  };

  //----------------------------------------------------------------------------

  Shape::~Shape() 
  {
    if(_vertices) delete [] _vertices;
    if(_indices) delete [] _indices;
  };

  //----------------------------------------------------------------------------

  void Shape::InitClass()
  {
    _g_pVertexBuffer = nullptr;
    _g_pIndexBuffer = nullptr;
  }

  //===================================================================================================
  // shape

  bool Shape::InitDX11(ID3D11Device* _g_pd3dDevice)
  {
    return true;
  }

  //------------------------------------------------------------------------------ 

  void Shape::CleanupDX11Device()
  {
    RELEASE_MEM( _g_pVertexBuffer );
    RELEASE_MEM( _g_pIndexBuffer );
  }

  //------------------------------------------------------------------------------ 

  void Shape::Draw(DrawContext &drawContext)
  {

  };

  //===================================================================================================- 

  DEFINE_CASTING(ShapeCircle)

  ShapeCircle::ShapeCircle(HString name): Shape(name)
  {   
    _verticesCount = 2056;

    _vertices = new SimpleVertex[_verticesCount];
    std::memset(_vertices, 0, sizeof(WORD) * _verticesCount);
    float dx = 1.0f / (_verticesCount-1);
    // Create vertex buffer
    for(int i = 0; i < _verticesCount; i++)
    {
      float angle = PI_2 / (_verticesCount-1) * i;
      _vertices[i] = SimpleVertex( 
        XMFLOAT3( cos(angle), 0.0f, sin(angle) ), 
        XMFLOAT3( 0.0f, 1.0f, 0.0f ),
        XMFLOAT2( dx * i, 0 ));
    }
  }

  //------------------------------------------------------------------------------ 

  bool ShapeCircle::InitDX11(ID3D11Device* _g_pd3dDevice)
  {
    HRESULT hr = S_OK;

    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( SimpleVertex ) * _verticesCount;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory( &InitData, sizeof(InitData) );
    InitData.pSysMem = _vertices;
    hr = _g_pd3dDevice->CreateBuffer( &bd, &InitData, &_g_pVertexBuffer );
    if( FAILED( hr ))  return false;

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( WORD ) * 36;        // 36 vertices needed for 12 triangles in a triangle list
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = _indices;
    hr = _g_pd3dDevice->CreateBuffer( &bd, &InitData, &_g_pIndexBuffer );
    if(FAILED( hr )) return false;

    return true;
  }

  //------------------------------------------------------------------------------ 

  void ShapeCircle::Draw(DrawContext &drawContext)
  {

  };

//===================================================================================================- 

  DEFINE_CASTING(ShapeQuad)

  ShapeQuad::ShapeQuad(HString name): Shape(name)
  {   
    _verticesCount = 4;

    _vertices = new SimpleVertex[_verticesCount];
    std::memset(_vertices, 0, sizeof(WORD) * _verticesCount);

    _vertices[0] = SimpleVertex( 
      XMFLOAT3( -1.0f, -1.0f, 0.0f ), 
      XMFLOAT3( 0.0f, 1.0f, 0.0f ),
      XMFLOAT2( 0.0f, 1.0f ));

    _vertices[1] = SimpleVertex( 
      XMFLOAT3( -1.0f , 1.0f, 0.0f ), 
      XMFLOAT3( 0.0f, 1.0f, 0.0f ),
      XMFLOAT2( 0.0f, 0.0f ));

    _vertices[2] = SimpleVertex( 
      XMFLOAT3( 1.0f, -1.0f, 0.0f ), 
      XMFLOAT3( 0.0f, 1.0f, 0.0f ),
      XMFLOAT2( 1.0f, 1.0f ));

    _vertices[3] = SimpleVertex( 
      XMFLOAT3( 1.0f, 1.0f, 0.0f ), 
      XMFLOAT3( 0.0f, 1.0f, 0.0f ),
      XMFLOAT2( 1.0f, 0.0f ));

  }

  //------------------------------------------------------------------------------ 

  bool ShapeQuad::InitDX11(ID3D11Device* _g_pd3dDevice)
  {
    HRESULT hr = S_OK;

    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( SimpleVertex ) * _verticesCount;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory( &InitData, sizeof(InitData) );
    InitData.pSysMem = _vertices;
    hr = _g_pd3dDevice->CreateBuffer( &bd, &InitData, &_g_pVertexBuffer );
    if( FAILED( hr ))  return false;

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( WORD ) * 36;        // 36 vertices needed for 12 triangles in a triangle list
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = _indices;
    hr = _g_pd3dDevice->CreateBuffer( &bd, &InitData, &_g_pIndexBuffer );
    if(FAILED( hr )) return false;

    return true;
  }

  //------------------------------------------------------------------------------ 

  void ShapeQuad::Draw(DrawContext &drawContext)
  {

  };

  //===================================================================================================

  ShapeAssimp::ShapeAssimp(const char* path): Shape(path)
  {
    _vertices = nullptr;
    _indices = nullptr;

    // the global Assimp scene object (mesh loader)
    const struct aiScene* scene;
    scene = aiImportFile(path,aiProcessPreset_TargetRealtime_MaxQuality /*| aiProcess_MakeLeftHanded | aiProcess_FlipUVs */ );

    if(scene && scene->mRootNode)
    {
      //unsigned int i;
      unsigned int n = 0, t;
      struct aiNode* nd = scene->mRootNode;

      if(nd && nd->mNumChildren > 0)  nd = nd->mChildren[0];
      // struct aiMatrix4x4 m = nd->mTransformation;

      if(nd)
      {
        // draw all meshes assigned to this node
        for (; n < nd->mNumMeshes && n <= 1; ++n) 
        {
          const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

          _verticesCount = mesh->mNumVertices;
          _indicesCount = mesh->mNumFaces * 3;

          _vertices = new SimpleVertex[_verticesCount];
          std::memset(_vertices, 0, sizeof(WORD) * _verticesCount);
          _indices = new WORD[_indicesCount];
          std::memset(_indices, 0, sizeof(WORD) * _indicesCount);

          for (t = 0; t < mesh->mNumVertices; ++t) 
          {
            const aiVector3D* vertice = &mesh->mVertices[t];
            const aiVector3D* normal = &mesh->mNormals[t];
            const aiVector3D* uvCoord  = &mesh->mTextureCoords[0][t];

            if(vertice && normal && uvCoord) _vertices[t] =   SimpleVertex( 
              XMFLOAT3( vertice->x, -vertice->z, vertice->y ), 
              XMFLOAT3( normal->x, -normal->z, normal->y ) ,
              XMFLOAT2( uvCoord->x, uvCoord->y ) 
              );
            else _vertices[t] =  SimpleVertex( 
              XMFLOAT3( 0, 0, 0 ), 
              XMFLOAT3( 0, 0, 0),
              XMFLOAT2( 0, 0) );
          }

          for (t = 0; t < mesh->mNumFaces; ++t) 
          {
            const struct aiFace* face = &mesh->mFaces[t];
            if(face->mNumIndices != 3) break; //only triangles supported

            _indices[3*t] =   face->mIndices[0];
            _indices[3*t+1] = face->mIndices[1];
            _indices[3*t+2] = face->mIndices[2];
          }
        }
      }
    }

    aiReleaseImport(scene);
    scene = nullptr;
  }

  //----------------------------------------------------------------------------

  ShapeAssimp::~ShapeAssimp()
  {

  }

  //----------------------------------------------------------------------------

  DEFINE_CASTING(ShapeAssimp)

  bool ShapeAssimp::InitDX11(ID3D11Device* _g_pd3dDevice)
  {
    HRESULT hr = S_OK;

    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( SimpleVertex ) * _verticesCount;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory( &InitData, sizeof(InitData) );
    InitData.pSysMem = _vertices;
    hr = _g_pd3dDevice->CreateBuffer( &bd, &InitData, &_g_pVertexBuffer );
    if( FAILED( hr ))  return false;

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( WORD ) * _indicesCount;      
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = _indices;
    hr = _g_pd3dDevice->CreateBuffer( &bd, &InitData, &_g_pIndexBuffer );
    if(FAILED( hr )) return false;

    return true;
  }

  //------------------------------------------------------------------------------ 

  void ShapeAssimp::Draw(DrawContext &drawContext)
  {

  };

} // Helios namespace