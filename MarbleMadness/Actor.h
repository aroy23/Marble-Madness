#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* sw, int imageID, int initX, int initY);
    virtual ~Actor();
private:
    StudentWorld* m_studentWorld;
};

class Wall : public Actor
{
public:
    Wall(StudentWorld* sw, int imageID, int initX, int initY);
private:
    
};

class Player : public Actor 
{
public:
    Player(StudentWorld* sw, int imageID, int initX, int initY);
private:
};

#endif // ACTOR_H_
