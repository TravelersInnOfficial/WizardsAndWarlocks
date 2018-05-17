#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <iostream>

class LevelLoader{

public:
    LevelLoader();
    static bool LoadLevel(std::string jsonPath);

    static std::string m_mapPath;
    static float m_centerX;
    static float m_centerY;
    static float m_sizeMap;
    static float m_zoom;
};

#endif