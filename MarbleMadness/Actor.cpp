#include "Actor.h"
#include "StudentWorld.h"

Actor::Actor(StudentWorld* sw, int imageID, int initX, int initY)
: GraphObject(imageID, initX, initY), m_studentWorld(sw)
{
    setVisible(true);
}



// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
