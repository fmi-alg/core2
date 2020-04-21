/* Quaternion.h
 *
 *  Description:
 *  This is a Quaternion class for representing 3D 
 *  rotations. It is intended to be used with OpenGL
 *  and GLUT.
 *
 *  		
 *  Author: Surin Ahn (March 2013)
 *  -- supervised by Professor Yap
 *
 ***************************************************/
 
#include <math.h>
// #include <GL/glut.h>

class Quaternion 
{
	float w, x, y, z;
	
	public:
		Quaternion ();
		Quaternion (float,float,float,float);
		void setValues (float,float,float,float);
		float X ();
		float Y ();
		float Z ();
		float W ();
		Quaternion operator* (const Quaternion& other);
		void normalize ();
		float *getMatrix();
};

// Default constructor
Quaternion::Quaternion() 
{
	w = 1.0f;
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	
}

Quaternion::Quaternion (float a, float b, float c, float d) 
{
	w = a;
	x = b;
	y = c;
	z = d;
}

float Quaternion::X ()
{
	return x;
}

float Quaternion::Y ()
{
	return y;
}

float Quaternion::Z ()
{
	return z;
}

float Quaternion::W ()
{
	return w;
}

// Overloaded multiplication operator
Quaternion Quaternion::operator*(const Quaternion& other)
{
	w = (w * other.w) - (x * other.x) - (y * other.y) - (z * other.z);
	x = (w * other.x) + (x * other.w) + (y * other.z) - (z * other.y);
	y = (w * other.y) - (x * other.z) + (y * other.w) + (z * other.x);
	z = (w * other.z) + (x * other.y) - (y * other.x) + (z * other.w);
    return Quaternion(w,x,y,z);
}

// Use when (x^2 + y^2 + z^2 + w^2) is a set tolerance away from 1
// Set tolerance to something small like 0.00001f
void Quaternion::normalize () 
{
	float magnitude = sqrt(x*x + y*y + z*z + w*w);
	x /= magnitude;
	y /= magnitude;
	z /= magnitude;
	w /= magnitude;
}

// Convert Quaternion to Matrix
float *Quaternion::getMatrix()
{
	float x2 = x * x;
	float y2 = y * y;
	float z2 = z * z;
	float xy = x * y;
	float xz = x * z;
	float yz = y * z;
	float wx = w * x;
	float wy = w * y;
	float wz = w * z;
 
	float Matrix[16] = {1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
			2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
			2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
	
	float *m_pointer = Matrix;
	
	return m_pointer;
}
