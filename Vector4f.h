#ifndef __Vector4f_h__
#define __Vector4f_h__

class Vector4f{
public:
	Vector4f(){
		x = y = z = w = 0.0;
	}

	Vector4f( float a, float b, float c, float d ){
		x = a; y = b; z = c; w = d;
	}

	void setVector( float a, float b, float c, float d ){
		x = a; y = b; z = c; w = d;
	}

	Vector4f operator * ( Vector4f v ){
		return Vector4f( x + v.x, y + v.y, z + v.z, w + v.w );
	}
public:
	float x, y, z, w;
};

#endif