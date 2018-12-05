#include <irrlicht.h>
#include <iostream>

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

enum key_id
{
  STATE_UP = 0,
  STATE_DOWN,
  STATE_ROT_LEFT,
  STATE_ROT_RIGHT,
  STATE_STRAFE_LEFT,
  STATE_STRAFE_RIGHT,
  STATE_MOVING,
  STATE_JUMP,
  STATE_END_ARRAY
};
bool states[STATE_END_ARRAY] = {false}; //up down left right

is::IAnimatedMeshSceneNode *node;
scene::ICameraSceneNode *cam;

using namespace std;
const float g = -0.3;
const int min_y = -10.0;
float vy = 0;
float vy0 = 5;

float speed = 5.0;
float rotation_speed = 5;
float cam_offset = 150.0;

void updateCam()
{
  ic::vector3df cam_position = cam->getPosition();
  ic::vector3df node_position = node->getPosition();
  ic::vector3df node_rotation = node->getRotation();
  // ic::vector3df cam_rotation  = node_rotation;
  float cam_rot_y = 180 - node_rotation.Y;

  ic::vector3df target_position = node_position;
  target_position.Y = 30;

  cam_position = target_position;

  cam_position.Y = node_position.Y + 50;

  cam_position.X += cam_offset * cos(cam_rot_y * M_PI / 180.0);
  cam_position.Z += cam_offset * sin(cam_rot_y * M_PI / 180.0);

  cam->setPosition(cam_position);
  cam->setTarget(target_position);
}

void update()
{
  ic::vector3df position = node->getPosition();
  ic::vector3df rotation = node->getRotation();
  // position.Y += vy;
  // vy += g;
  // // std::cout << vy << std::endl;

  // if(position.Y <= min_y && vy != 0){
  //   vy = 0;
  //   position.Y = min_y;
  //   if(states[STATE_JUMP] == true){
  //     if(states[STATE_MOVING])node->setMD2Animation(is::EMAT_RUN);
  //     else node->setMD2Animation(is::EMAT_STAND);
  //     states[STATE_JUMP] = false;
  //   }
  //   // states[STATE_JUMP] = false;
  // }

  if (states[STATE_UP] || states[STATE_DOWN] || states[STATE_STRAFE_LEFT] || states[STATE_STRAFE_RIGHT])
  {
    if (states[STATE_MOVING] == false)
    {
      states[STATE_MOVING] = true;
      node->setMD2Animation(is::EMAT_RUN);
    }
  }
  else if (states[STATE_MOVING])
  {
    node->setMD2Animation(is::EMAT_STAND);
    states[STATE_MOVING] = false;
  }
  if (states[STATE_UP])
  {
    position.X += speed * cos(rotation.Y * M_PI / 180.0);
    position.Z += -speed * sin(rotation.Y * M_PI / 180.0);
  }
  if (states[STATE_DOWN])
  {
    position.X += -0.5 * speed * cos(rotation.Y * M_PI / 180.0);
    position.Z += 0.5 * speed * sin(rotation.Y * M_PI / 180.0);
  }

  if (states[STATE_STRAFE_LEFT])
  {
    position.X += -speed * cos((90 + rotation.Y) * M_PI / 180.0);
    position.Z += speed * sin((90 + rotation.Y) * M_PI / 180.0);
  }
  if (states[STATE_STRAFE_RIGHT])
  {
    position.X += speed * cos((90 + rotation.Y) * M_PI / 180.0);
    position.Z += -speed * sin((90 + rotation.Y) * M_PI / 180.0);
  }
  if (states[STATE_ROT_RIGHT])
  {
    rotation.Y += rotation_speed;
  }
  if (states[STATE_ROT_LEFT])
  {
    rotation.Y -= rotation_speed;
  }
  node->setPosition(position);
  node->setRotation(rotation);

  updateCam();
}

struct MyEventReceiver : IEventReceiver
{
  bool OnEvent(const SEvent &event)
  {
    // Si l'événement est de type clavier (KEY_INPUT)
    // et du genre pressage de touche
    // et que la touche est escape, on sort du programme
    if (event.EventType == EET_KEY_INPUT_EVENT)
    {

      switch (event.KeyInput.Key)
      {
      case KEY_ESCAPE:
        exit(0);
      case KEY_KEY_Z: // Avance
        states[STATE_UP] = event.KeyInput.PressedDown;
        break;
      case KEY_KEY_S:
        states[STATE_DOWN] = event.KeyInput.PressedDown;
        break;
      case KEY_KEY_A:
        states[STATE_ROT_LEFT] = event.KeyInput.PressedDown;
        break;
      case KEY_KEY_E:
        states[STATE_ROT_RIGHT] = event.KeyInput.PressedDown;
        break;
      case KEY_KEY_Q:
        states[STATE_STRAFE_LEFT] = event.KeyInput.PressedDown;
        break;
      case KEY_KEY_D:
        states[STATE_STRAFE_RIGHT] = event.KeyInput.PressedDown;
        break;
      // case KEY_SPACE:
      // // node->setMD2Animation(is::EMAT_ATTACK);
      // if(event.KeyInput.PressedDown){
      //   if(!states[STATE_JUMP])
      //   {
      //     vy = vy0;
      //     node->setMD2Animation(is::EMAT_JUMP);
      //     states[STATE_JUMP] = true;
      //   }
      // }else{
      //   if(states[STATE_JUMP] && vy == 0)
      //     states[STATE_JUMP] = false;
      // }
      // if(states[STATE_MOVING]){
      //           // node->setMD2Animation(is::EMAT_RUN);

      // }

      // break;
      default:
        break;
      }
    }
    return false;
  }
};
int main()
{
  // Le gestionnaire d'événements
  MyEventReceiver receiver;
  // Création de la fenêtre et du système de rendu.
  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(1366, 768),
                                        16, false, false, false, &receiver);

  iv::IVideoDriver *driver = device->getVideoDriver();
  is::ISceneManager *smgr = device->getSceneManager();

  // Chargement de notre personnage (réutilisé plusieurs fois)
  is::IAnimatedMesh *mesh = smgr->getMesh("data/tris.md2");

  // Attachement de notre personnage dans la scène
  is::IAnimatedMeshSceneNode *perso = smgr->addAnimatedMeshSceneNode(mesh);
  node = perso;
  perso->setMaterialFlag(iv::EMF_LIGHTING, false);
  perso->setMD2Animation(is::EMAT_STAND);
  perso->setMaterialTexture(0, driver->getTexture("data/blue_texture.pcx"));

  // Un deuxième personnage, en rouge et qui cours entre deux points
  is::IAnimatedMeshSceneNode *perso_cours = smgr->addAnimatedMeshSceneNode(mesh);
  perso_cours->setMaterialFlag(iv::EMF_LIGHTING, false);
  perso_cours->setMD2Animation(is::EMAT_RUN);
  perso_cours->setMaterialTexture(0, driver->getTexture("data/red_texture.pcx"));
  is::ISceneNodeAnimator *anim =
      smgr->createFlyStraightAnimator(ic::vector3df(-100, 0, 60),
                                      ic::vector3df(100, 0, 60), 3500, true);
  perso_cours->addAnimator(anim);

  // Un troisième personnage, qui saute en rond
  is::IAnimatedMeshSceneNode *perso_cercle = smgr->addAnimatedMeshSceneNode(mesh);
  perso_cercle->setMaterialFlag(iv::EMF_LIGHTING, false);
  perso_cercle->setMD2Animation(is::EMAT_JUMP);
  perso_cercle->setMaterialTexture(0, driver->getTexture("data/red_texture.pcx"));
  is::ISceneNodeAnimator *anim_cercle =
      smgr->createFlyCircleAnimator(core::vector3df(30, 0, 0), 20.0f);
  perso_cercle->addAnimator(anim_cercle);

  //ajouyt decor
  device->getFileSystem()->addFileArchive("data/map-20kdm2.pk3");
  //bsp
  is::IAnimatedMesh *mesh2 = smgr->getMesh("20kdm2.bsp");
  is::IMeshSceneNode *node2 = smgr->addOctreeSceneNode(mesh2->getMesh(0));
  node2->setPosition(core::vector3df(-1300, -104, -1249));

  //------------COLLISION

  is::ITriangleSelector *selector;
  selector = smgr->createOctreeTriangleSelector(node2->getMesh(), node2);
  node2->setTriangleSelector(selector);

  ic::vector3df ellipseRadius = (perso->getTransformedBoundingBox().MaxEdge - perso->getTransformedBoundingBox().getCenter());

  is::ISceneNodeAnimatorCollisionResponse *collision = smgr->createCollisionResponseAnimator(selector, perso, ellipseRadius);

  selector->drop();

  perso->addAnimator(collision);
  collision->drop();

  smgr->addCameraSceneNode(nullptr, ic::vector3df(0, 30, -70), ic::vector3df(0, 0, 0));
  cam = device->getSceneManager()->getActiveCamera();

  //----------------------

  auto pos = node->getPosition();
  // pos.Y = 20;
  node->setPosition(pos);
  int lastFPS = -1;
  while (device->run())
  {
    driver->beginScene(true, true, iv::SColor(100, 150, 200, 255));

    // Dessin de la scène :
    update();

    smgr->drawAll();
    //
    driver->endScene();

    int fps = driver->getFPS();

    if (lastFPS != fps)
    {
      core::stringw str = L"BARRIERE - BOUCAUD - Irrlicht Engine [";
      str += driver->getName();
      str += "] FPS:";
      str += fps;

      device->setWindowCaption(str.c_str());
      lastFPS = fps;
    }
  }
  device->drop();

  return 0;
}
