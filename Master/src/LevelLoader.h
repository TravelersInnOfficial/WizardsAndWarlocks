#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <iostream>

class LevelLoader{

public:
    LevelLoader();

    bool loadLobby();
    bool readJson(std::string jsonPath);
};

#endif