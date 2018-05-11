#include "./vector4d.h"

#include <math.h>

template<typename T>
vector4d<T>::vector4d(){
	X = 0;
	Y = 0;
	X2 = 0;
	Y2 = 0;
}

template<typename T>
vector4d<T>::vector4d(T nx, T ny, T nx2, T ny2){
	X = nx;
	Y = ny;
	X2 = nx2;
	Y2 = ny2;
}

template<typename T>
vector4d<T>::vector4d(const vector4d<T>& other){
	X = other.X;
	Y = other.Y;
	X2 = other.X2;
	Y2 = other.Y2;
}

template<typename T>
float vector4d<T>::length(){
	return sqrtf(X*X + Y*Y + X2*X2);
}

template<typename T>
void vector4d<T>::setX(T newX){
	X = newX;
}

template<typename T>
void vector4d<T>::setY(T newY){
	Y = newY;
}

template<typename T>
void vector4d<T>::setX2(T newX2){
	X2 = newX2;
}

template<typename T>
void vector4d<T>::setY2(T newY2){
	Y2 = newY2;
}

template<typename T>
vector4d<T> vector4d<T>::operator+(vector4d<T> v) const{ 
	return vector4d<T>(X+v.X, Y+v.Y, X2+v.X2, Y2+v.Y2);
}

template<typename T>
vector4d<T> vector4d<T>::operator-(vector4d<T> v) const{ 
	return vector4d<T>(X-v.X, Y-v.Y, X2-v.X2, Y2-v.Y2);
}

template<typename T>
vector4d<T> vector4d<T>::operator*(const T v) const{ 
	return vector4d<T>(X * v, Y * v, X2 * v, Y2 * v);
}

template<typename T>
vector4d<T> vector4d<T>::operator/(const T v) const{ 
	return vector4d<T>(X / v, Y / v, X2 / v, Y2 / v);
}

template class vector4d<float>;
template class vector4d<int>;