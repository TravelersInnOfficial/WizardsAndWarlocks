#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <iostream>

class LevelLoader{

public:
    LevelLoader();
    bool LoadLevel(std::string jsonPath);
};

#endif