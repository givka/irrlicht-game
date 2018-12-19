#include <fstream>
#include <sstream>
#include <iostream>
#include "level.hpp"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "../utils.hpp"

Level::Level()
{
}

bool Level::loadFromJSON(std::string path_to_json, irr::IrrlichtDevice *device, iv::IVideoDriver *driver, is::ISceneManager *smgr)
{
    //Careful, no type checks
    //Careful, assumes checkpoints are sorted by id

    std::ifstream json_file_stream(path_to_json);
    std::stringstream buffer;
    buffer << json_file_stream.rdbuf();

    rapidjson::Document d;
    d.Parse(buffer.str().c_str());
    std::string archive_path = d["map"]["archive_path"].GetString();
    std::string mesh_path = d["map"]["mesh_path"].GetString();
    auto map_pos_array = d["map"]["position"].GetArray();

    ic::vector3df map_position;
    map_position.X = map_pos_array[0].GetFloat();
    map_position.Y = map_pos_array[1].GetFloat();
    map_position.Z = map_pos_array[2].GetFloat();

    device->getFileSystem()->addFileArchive(archive_path.c_str());
    is::IQ3LevelMesh *const mesh = (is::IQ3LevelMesh *)smgr->getMesh(mesh_path.c_str());
    m_map_node = smgr->addOctreeSceneNode(mesh->getMesh(0));
    m_map_node->setPosition(map_position);

    const is::IMesh *const additional_mesh = mesh->getMesh(is::quake3::E_Q3_MESH_ITEMS);
    for (irr::u32 i = 0; i != additional_mesh->getMeshBufferCount(); ++i)
    {
        const is::IMeshBuffer *meshBuffer = additional_mesh->getMeshBuffer(i);
        const iv::SMaterial &material = meshBuffer->getMaterial();
        const irr::s32 shaderIndex = (irr::s32)material.MaterialTypeParam2;
        const is::quake3::IShader *shader = mesh->getShader(shaderIndex);
        if (0 == shader)
            continue;
        smgr->addQuake3SceneNode(meshBuffer, shader);
    }

    //loading spawnpoints
    auto spawn_points = d["spawn_points"].GetArray();
    ic::vector3df position;
    ic::vector3df orientation;
    for (auto it = spawn_points.Begin(); it != spawn_points.End(); it++)
    {
        int id = (*it)["id"].GetInt();
        auto pos_coords = (*it)["position"].GetArray();
        position.X = pos_coords[0].GetFloat();
        position.Y = pos_coords[1].GetFloat();
        position.Z = pos_coords[2].GetFloat();

        auto orient_coords = (*it)["orientation"].GetArray();
        orientation.X = orient_coords[0].GetFloat();
        orientation.Y = orient_coords[1].GetFloat();
        orientation.Z = orient_coords[2].GetFloat();

        //TODO: spheres are for debugging
        m_spawnPoints.push_back(SpawnPoint(id, position, orientation, true, smgr->addSphereSceneNode(5.0f, 16, 0, -1, position)));
    }
    //loading lights: TODO
}
SpawnPoint Level::getSpawnPoint(int id)
{
    return m_spawnPoints[id]; //TODO: add range check
}

is::IMeshSceneNode *Level::getMapNode()
{
    return m_map_node;
}
