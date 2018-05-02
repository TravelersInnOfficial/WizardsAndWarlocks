#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

class ResourceManager{

public:
    ResourceManager();
    static void LoadResources();
    static void ReadTextures();
    static void ReadMeshes();
};

#endif