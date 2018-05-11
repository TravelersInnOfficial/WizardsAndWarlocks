#include "./vector3d.h"

#include <math.h>
#include <Constants.h>

template<typename T>
vector3d<T>::vector3d(){
	X = 0;
	Y = 0;
	Z = 0;
}

template<typename T>
vector3d<T>::vector3d(T n){
	X = n;
	Y = n;
	Z = n;
}

template<typename T>
vector3d<T>::vector3d(T nx, T ny, T nz){
	X = nx;
	Y = ny;
	Z = nz;
}

template<typename T>
vector3d<T>::vector3d(const vector3d<T>& other){
	X = other.X;
	Y = other.Y;
	Z = other.Z;
}

template<typename T>
float vector3d<T>::length(){
	return sqrtf(X*X + Y*Y + Z*Z);
}

template<typename T>
void vector3d<T>::normalize(){
	float l = length();
	if(l == 0){
		X = 0;
		Y = 0;
		Z = 0;
	}else{
		X = X/l;
		Y = Y/l;
		Z = Z/l;
	}
}

template<typename T>
void vector3d<T>::toRadians(){
	float factor = M_PI/180;
	X = X * factor;
	Y = Y * factor;
	Z = Z * factor;
}

template<typename T>
void vector3d<T>::toDegrees(){
    float factor = 180/M_PI;
    X = X * factor;
    Y = Y * factor;
    Z = Z * factor;
}

template<typename T>
void vector3d<T>::setX(T newX){
	X = newX;
}

template<typename T>
void vector3d<T>::setY(T newY){
	Y = newY;
}

template<typename T>
void vector3d<T>::setZ(T newZ){
	Z = newZ;
}

template<typename T>
vector3d<T> vector3d<T>::operator+(const T v) const{
	return vector3d<T>(X + v, Y + v, Z + v);
}

template<typename T>
vector3d<T> vector3d<T>::operator+(vector3d<T> v) const{ 
	return vector3d<T>(X+v.X, Y+v.Y, Z+v.Z); 
}

template<typename T>
vector3d<T> vector3d<T>::operator-(const T v) const{
	return vector3d<T>(X - v, Y - v, Z - v);
}

template<typename T>
vector3d<T> vector3d<T>::operator-(vector3d<T> v) const{ 
	return vector3d<T>(X-v.X, Y-v.Y, Z-v.Z);
}

template<typename T>
vector3d<T> vector3d<T>::operator*(const T v) const{
	return vector3d<T>(X * v, Y * v, Z * v);
}

template<typename T>
vector3d<T> vector3d<T>::operator*(vector3d<T> v) const{ 
	return vector3d<T>(X*v.X, Y*v.Y, Z*v.Z);
}

template<typename T>
vector3d<T> vector3d<T>::operator/(const T v) const{
	return vector3d<T>(X / v, Y / v, Z / v);
}

template<typename T>
vector3d<T> vector3d<T>::operator/(vector3d<T> v) const{ 
	return vector3d<T>(X/v.X, Y/v.Y, Z/v.Z);
}

template<typename T>
void vector3d<T>::operator=(vector3d<T> v){ 
	X = v.X; Y = v.Y; Z = v.Z;
}

template<typename T>
void vector3d<T>::operator*=(const T v){ 
	X = X*v; Y = Y*v; Z = Z*v;
}

template<typename T>
bool vector3d<T>::operator==(const vector3d<T> v){
	if(v.X == X && v.Y == Y && v.Z == Z) return true; 
	return false;
}

template class vector3d<float>;
template class vector3d<int>;