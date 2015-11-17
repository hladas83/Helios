#pragma once

#ifndef _HELIOS_MATRIX4
#define _HELIOS_MATRIX4


namespace Helios
{


  //===================================================================================================
  //Vector3

  template <typename value_Type> class Vector3T
  {

  private:
    value_Type _vector[3];
    static const int size = 3;

  public:
    Vector3T() {_vector[0] = 0; _vector[1] = 0; _vector[2] = 0;};
    Vector3T(value_Type x, value_Type y, value_Type z) { _vector[0] = x; _vector[1] = y; _vector[2] = z;};

    //------------------------------------------------------------------------------  
    ///translation
    void Set(value_Type x, value_Type y, value_Type z) {_vector[0] = x; _vector[1] = y; _vector[2] = z;};
    __forceinline value_Type X() const {return _vector[0];};
    __forceinline value_Type Y() const {return _vector[1];};
    __forceinline value_Type Z() const {return _vector[2];};

    __forceinline void SetX(value_Type x) {_vector[0] = x;};
    __forceinline void SetY(value_Type y) {_vector[1] = y;};
    __forceinline void SetZ(value_Type z) {_vector[2] = z;};

    //------------------------------------------------------------------------------ 
    // basic operations
    __forceinline value_Type Distance(const Vector3T<value_Type> &vector) const;
    __forceinline value_Type Distance2(const Vector3T<value_Type> &vector) const;
    __forceinline value_Type Length() const;
    __forceinline value_Type Length2() const;
    __forceinline value_Type Dot(const Vector3T<value_Type> &vector) const;
    __forceinline Vector3T<value_Type> Cross(const Vector3T<value_Type> &vector) const;
    __forceinline void Normalize();
    __forceinline Vector3T<value_Type> Normalized() const ;

    //------------------------------------------------------------------------------  
    //basic operators
    Vector3T<value_Type> operator * ( const value_Type &op )            const { return Vector3T<value_Type>(op * X(), op * Y(),op * Z());};
    Vector3T<value_Type> operator + ( const Vector3T<value_Type> &op )  const { return Vector3T<value_Type>(op.X() + X(), op.Y() + Y(), op.Z() + Z());};
    Vector3T<value_Type> operator - ( const Vector3T<value_Type> &op )  const { return Vector3T<value_Type>(op.X() - X(), op.Y() - Y(), op.Z() - Z());};
  };

  //===================================================================================================
  //Matrix3

  template <typename value_Type> class Matrix3T
  {
    typedef Vector3T<value_Type> Vector3Type3;

  private:
    float _matrix[3][3];
    static const int size = 3;

  public:
    Matrix3T() {SetZeroMatrix();};

    //------------------------------------------------------------------------------  
    ///rotation
    void SetRotationX(value_Type angle);
    void SetRotationY(value_Type angle);
    void SetRotationZ(value_Type angle);

    inline value_Type &Set( int i, int j ) {return _matrix[i][j];}

    //------------------------------------------------------------------------------ 
    // dafult matrices
    void SetZeroMatrix() { for(int i= 0; i < 3; i++) for(int j= 0; j < 3; j++)  _matrix[i][j] = 0; };      
    void SetIdentityMatrix() { SetZeroMatrix(); for(int i= 0; i < 3; i++)  _matrix[i][i] = 1; }; 

    //------------------------------------------------------------------------------  
    // orientation
    Vector3Type3 GetAside() const {return Vector3Type3( _matrix[0][0],   _matrix[0][1],   _matrix[0][2]);};
    Vector3Type3 GetUp() const    {return Vector3Type3( _matrix[1][0],   _matrix[1][1],   _matrix[1][2]);};
    Vector3Type3 GetDir() const   {return Vector3Type3( _matrix[2][0],   _matrix[2][1],   _matrix[2][2]);};

    __forceinline void SetAside(const Vector3Type3 &aside)  {_matrix[0][0] = aside.X();  _matrix[0][1] = aside.Y();   _matrix[0][2] = aside.Z();};
    __forceinline void SetUp(const Vector3Type3 &up)        {_matrix[1][0] = up.X();     _matrix[1][1] = up.Y();      _matrix[1][2] = up.Z();};
    __forceinline void SetDir(const Vector3Type3 &dir)      {_matrix[2][0] = dir.X();    _matrix[2][1] = dir.Y();     _matrix[2][2] = dir.Z();};

  };

  //===================================================================================================
  //Vector3

  template <typename value_Type> value_Type Vector3T<value_Type>::Distance(const Vector3T<value_Type> &vector) const
  {
    return sqrt(Distance2(vector));
  };

  //------------------------------------------------------------------------------

  template <typename value_Type> value_Type Vector3T<value_Type>::Distance2(const Vector3T<value_Type> &vector) const
  {
    return (pow(X()-vector.X(),2) + pow(Y()-vector.Y(),2) + pow(Z()-vector.Z(),2)); 
  };

  //------------------------------------------------------------------------------

  template <typename value_Type> value_Type Vector3T<value_Type>::Length() const
  {
    return sqrt(Length2());
  };

  //------------------------------------------------------------------------------

  template <typename value_Type> value_Type Vector3T<value_Type>::Length2() const
  {
    return pow(X(),2) + pow(Y(),2) + pow(Z(),2);
  };

  //------------------------------------------------------------------------------

  template <typename value_Type> value_Type Vector3T<value_Type>::Dot(const Vector3T<value_Type> &vector) const
  {
    return (X()*vector.X() + Y()*vector.Y() + Z()*vector.Z());
  };

  //------------------------------------------------------------------------------

  template <typename value_Type> Vector3T<value_Type> Vector3T<value_Type>::Cross(const Vector3T<value_Type> &vector) const
  { 
    return Vector3T<value_Type>(
      Y() * vector.Z() - Z() * vector.Y(),
      Z() * vector.X() - X() * vector.Z(),
      X() * vector.Y() - Y() * vector.X());
  };

  //------------------------------------------------------------------------------

  template <typename value_Type> void Vector3T<value_Type>::Normalize()
  { 
    float length = Length(); 
    _vector[0] /=length; _vector[1] /=length; _vector[2] /=length;
  };

  //------------------------------------------------------------------------------

  template <typename value_Type> Vector3T<value_Type> Vector3T<value_Type>::Normalized() const 
  {
    float length = Length(); 
    return Vector3T<value_Type>(_vector[0]/length, _vector[1]/length, _vector[2]/length);
  };

  //===================================================================================================
  //Matrix3


  template <typename value_Type>   void Matrix3T<value_Type>::SetRotationX(value_Type angle)
  {
    SetIdentityMatrix();
    value_Type s=sin(angle),c=cos(angle);
    Set(1,1)=+c,Set(1,2)=-s;
    Set(2,1)=+s,Set(2,2)=+c;
  }

  //------------------------------------------------------------------------------

  template <typename value_Type>  void Matrix3T<value_Type>::SetRotationY(value_Type angle)
  {
    SetIdentityMatrix();
    value_Type s=sin(angle),c=cos(angle);
    Set(0,0)=+c,Set(0,2)=-s;
    Set(2,0)=+s,Set(2,2)=+c;
  }

  //------------------------------------------------------------------------------

  template <typename value_Type>   void Matrix3T<value_Type>::SetRotationZ(value_Type angle)
  {
    SetIdentityMatrix();
    value_Type s=sin(angle),c=cos(angle);
    Set(0,0)=+c,Set(0,1)=-s;
    Set(1,0)=+s,Set(1,1)=+c;
  }

  //===================================================================================================
  //Matrix4

  template <typename value_Type>  class Matrix4T
  {
    typedef Matrix3T<value_Type> MatrixType3;
    typedef Vector3T<value_Type> Vector3Type3;

  private:
    static const int _rotationSize = 3;
    MatrixType3  _rotation;
    Vector3Type3 _position;

  public:
    Matrix4T();
    Matrix4T(Vector3Type3 aside, Vector3Type3 up, Vector3Type3 dir, Vector3Type3 position);

    //------------------------------------------------------------------------------  
    ///translation
    void SetPosition(value_Type x, value_Type y, value_Type z);
    void SetPosition(const Vector3Type3 &position);
    Vector3Type3 GetPosition() const;

    //------------------------------------------------------------------------------  
    ///rotation
    void SetRotation(MatrixType3 &rotation);
    void SetRotationX(value_Type angle);
    void SetRotationY(value_Type angle);
    void SetRotationZ(value_Type angle);
    MatrixType3 GetRotation() const;
    void GetRotation(MatrixType3 &rotation) const;

    //------------------------------------------------------------------------------  
    // orientation
    __forceinline Vector3Type3 GetAside() const {return _rotation.GetAside();};
    __forceinline Vector3Type3 GetUp() const {return _rotation.GetUp();};
    __forceinline Vector3Type3 GetDir() const {return _rotation.GetDir();};
 
    __forceinline void SetAside(const Vector3Type3 &aside)  {_rotation.SetAside(aside);};
    __forceinline void SetUp(const Vector3Type3 &up)  {_rotation.SetUp(up);};
    __forceinline void SetDir(const Vector3Type3 &dir)  {_rotation.SetDir(dir);};

    //------------------------------------------------------------------------------  
    ///scale
    void SetScale(float scale);
    void SetScale(float scaleX, float scaleY, float scaleZ);

    //------------------------------------------------------------------------------  
    ///reset values
    void SetZeroMatrix();      
    void SetIdentityMatrix();  


    Matrix4T<value_Type> operator * ( const Matrix4T<value_Type> &op ) const;
  };

  //===================================================================================================
  //Matrix4

  template <typename value_Type> Matrix4T<value_Type>::Matrix4T()
  {
    SetIdentityMatrix();
  }

  //------------------------------------------------------------------------------

  template <typename value_Type>  Matrix4T<value_Type>::Matrix4T(Vector3T<value_Type> aside, Vector3T<value_Type> up, Vector3T<value_Type> dir, Vector3T<value_Type> position)
  {
    SetPosition(position);
    SetAside(aside);
    SetUp(up);
    SetDir(dir);
  }

  //------------------------------------------------------------------------------

  template <typename value_Type> void Matrix4T<value_Type>::SetPosition(value_Type x, value_Type y, value_Type z) 
  {
    _position = Vector3T<value_Type>(x,y,z);
  };

  //------------------------------------------------------------------------------

  template <typename value_Type>  void Matrix4T<value_Type>::SetPosition(const Vector3T<value_Type> &position) 
  {
    _position = position;
  };

  template <typename value_Type>  Vector3T<value_Type> Matrix4T<value_Type>::GetPosition() const
  { 
    return _position;
  };

  //------------------------------------------------------------------------------

  template <typename value_Type>  void Matrix4T<value_Type>::SetRotation(Matrix3T<value_Type> &rotation) 
  {
    SetIdentityMatrix();
    _rotation = rotation;
  };

  //------------------------------------------------------------------------------

  template <typename value_Type>  void Matrix4T<value_Type>::SetRotationX(value_Type angle) 
  {
    SetIdentityMatrix();
    _rotation.SetRotationX(angle);
  };

  //------------------------------------------------------------------------------

  template <typename value_Type>  void Matrix4T<value_Type>::SetRotationY(value_Type angle) 
  {
    SetIdentityMatrix();
    _rotation.SetRotationY(angle);
  };

  //------------------------------------------------------------------------------

  template <typename value_Type>  void Matrix4T<value_Type>::SetRotationZ(value_Type angle) 
  {
    SetIdentityMatrix();
    _rotation.SetRotationZ(angle);
  };

  //------------------------------------------------------------------------------

  template <typename value_Type>  Matrix3T<value_Type> Matrix4T<value_Type>::GetRotation() const
  {
    return _rotation;
  };

  //------------------------------------------------------------------------------

  template <typename value_Type>  void Matrix4T<value_Type>::GetRotation(Matrix3T<value_Type> &rotation) const
  {
    rotation = _rotation;
  };

  //------------------------------------------------------------------------------

  template <typename value_Type>  void Matrix4T<value_Type>::SetScale(float scale)
  {
    SetZeroMatrix();
    _rotation.Set(0,0) = scale;
    _rotation.Set(1,1) = scale;
    _rotation.Set(2,2) = scale;
  }

  //------------------------------------------------------------------------------

  template <typename value_Type>  void Matrix4T<value_Type>::SetScale(float scaleX, float scaleY, float scaleZ)
  {
    SetZeroMatrix();
    _rotation.Set(0,0) = scaleX;
    _rotation.Set(1,1) = scaleY;
    _rotation.Set(2,2) = scaleZ;
  }

  //------------------------------------------------------------------------------

  template <typename value_Type> void Matrix4T<value_Type>::SetZeroMatrix()  
  {
    _rotation.SetZeroMatrix();
    _position = Vector3T<value_Type>(0,0,0);
  }

  //------------------------------------------------------------------------------

  template <typename value_Type> void Matrix4T<value_Type>::SetIdentityMatrix()
  {
    _rotation.SetIdentityMatrix();
    _position = Vector3T<value_Type>(0,0,0);
  }

  //------------------------------------------------------------------------------

  template <typename value_Type> Matrix4T<value_Type> Matrix4T<value_Type>::operator * ( const Matrix4T<value_Type> &op ) const
  {

    // SSE matrix multiplication
    Vector3T<value_Type> aside = 
      op.GetAside() * GetAside().X() +
      op.GetUp()    * GetAside().Y() +
      op.GetDir()   * GetAside().Z();


    Vector3T<value_Type> up = 
      op.GetAside() * GetUp().X() +
      op.GetUp()    * GetUp().Y() +
      op.GetDir()   * GetUp().Z();


    Vector3T<value_Type> dir = 
      op.GetAside() * GetDir().X() +
      op.GetUp()    * GetDir().Y() +
      op.GetDir()   * GetDir().Z();


    Vector3T<value_Type> position = 
      op.GetAside()  * GetPosition().X() + 
      op.GetUp()     * GetPosition().Y() + 
      op.GetDir()    * GetPosition().Z() + 
      op.GetPosition();

    Matrix4T<value_Type> result = Matrix4T<value_Type>(aside, up, dir, position );
    return result;
  }

#define NUMBER_PRECISION    float

typedef Vector3T<NUMBER_PRECISION>  Vector3;
typedef Matrix3T<NUMBER_PRECISION>  Matrix3;
typedef Matrix4T<NUMBER_PRECISION>  Matrix4;


} // Helios namespace

#endif // _HELIOS_MATRIX4