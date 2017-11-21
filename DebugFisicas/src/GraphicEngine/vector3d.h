#ifndef VECTOR3D_H
#define VECTOR3D_H

template<typename T>
class vector3d{

private:
public:
    // variables of vector
    T X, Y, Z;      
    
    /**
     * Creates and empty 3d vector of all 0;
    */
    vector3d() : X(0), Y(0), Z(0) {}
    
    /**
     * Creates and initializes 3d vector
    */
    vector3d(T nx, T ny, T nz) : X(nx), Y(ny), Z(nz) {}

    /**
     * Copy constructor
    */
    vector3d(const vector3d<T>& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};

//typedef vector3d<float> vector3df;
//typedef vector3d<int> vector3di;

#endif