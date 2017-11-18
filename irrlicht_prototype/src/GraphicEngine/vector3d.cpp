#include "vector3d.h"

vector3df::vector3df(){

}

vector3df::vector3df(float x, float y, float z){
    privateVector3df = irr::core::vector3df(x, y, z);
}

