#include "./vector2d.h"

#include <math.h>
#include <Constants.h>
#include <vector3d.h>

template<typename T>
vector2d<T>::vector2d(){
	X = 0;
	Y = 0;
}

template<typename T>
vector2d<T>::vector2d(T nx, T ny){
	X = nx;
	Y = ny;
}

template<typename T>
vector2d<T>::vector2d(const vector2d<T>& other){
	X = other.X;
	Y = other.Y;
}

template<typename T>
vector2d<T>::vector2d(const vector3d<T>& other){
	X = other.X;
	Y = other.Y;
}

template<typename T>
float vector2d<T>::length(){
	return sqrtf(X*X + Y*Y);
}

template<typename T>
vector2d<T> vector2d<T>::absolute(){
	return vector2d(std::abs(X), std::abs(Y));
}

template<typename T>
void vector2d<T>::normalize(){
	float l = length();
        if(l==0){
            X = 0;
            Y = 0;
        }
        else{
            X = X/l;
            Y = Y/l;
        }
}

template<typename T>
void vector2d<T>::mapToRangePI(){
	if(X>M_PI){
        X = X - 2*M_PI;
    }else if(X<-M_PI){
        X = X + 2*M_PI;
    }
    if(Y>M_PI){
        Y = Y - 2*M_PI;
    }else if(Y<-M_PI){
        Y = Y + 2*M_PI;
    }
}

template<typename T>
void vector2d<T>::setX(T newX){
	X = newX;
}

template<typename T>
void vector2d<T>::setY(T newY){
	Y = newY;
}

template<typename T>
vector2d<T> vector2d<T>::operator+(vector2d<T> v) const{
	return vector2d<T>(X+v.X, Y+v.Y);
}

template<typename T>
vector2d<T> vector2d<T>::operator-(vector2d<T> v) const{
	return vector2d<T>(X-v.X, Y-v.Y);
}

template<typename T>
vector2d<T> vector2d<T>::operator*(const T v) const{
	return vector2d<T>(X * v, Y * v);
}

template<typename T>
vector2d<T> vector2d<T>::operator/(const T v) const{
	return vector2d<T>(X / v, Y / v);
}

template<typename T>
void vector2d<T>::operator=(const vector2d<T> v){
	X = v.X; Y = v.Y;
}

template<typename T>
void vector2d<T>::operator*=(const T v){
	X = X*v; Y = Y*v;
}

template<typename T>
bool vector2d<T>::operator==(const vector2d<T> v){
	if(v.X == X && v.Y == Y) return true; 
	return false;
}

template class vector2d<float>;
template class vector2d<int>;
