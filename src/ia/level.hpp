#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include "irrlicht.h"
#include "spawnPoint.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class Level
{
  public:
    Level();

    //sets up:
    // model, pk3, adds to scene node
    // collisions,
    // navmesh ?
    //spawnpoints
    bool loadFromJSON(std::string path_to_json, irr::IrrlichtDevice *device, iv::IVideoDriver *driver, is::ISceneManager *smgr);

    SpawnPoint getSpawnPoint(int id);
    is::IMeshSceneNode *getMapNode();

  private:
    std::vector<SpawnPoint> m_spawnPoints;
    is::IMeshSceneNode *m_map_node;
};

#endif