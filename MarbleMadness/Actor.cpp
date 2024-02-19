#include "Actor.h"
#include "StudentWorld.h"

Actor::Actor(StudentWorld* sw, int imageID, int initX, int initY)
: GraphObject(imageID, initX, initY), m_studentWorld(sw)
{
    setVisible(true);
}

Actor::~Actor()
{
}

Wall::Wall(StudentWorld* sw, int imageID, int initX, int initY)
: Actor(sw, imageID, initX, initY)
{
}

Player::Player(StudentWorld* sw, int imageID, int initX, int initY)
 : Actor(sw, imageID, initX, initY)
{
}


// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
