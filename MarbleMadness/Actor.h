#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* sw, int imageID, int initX, int initY, int dir = none);
    virtual ~Actor();
    virtual void doSomething() = 0;
    virtual bool isImmovable() const;
    StudentWorld* getWorld() const;
private:
    StudentWorld* m_studentWorld;
};

class Wall : public Actor
{
public:
    Wall(StudentWorld* sw, int imageID, int initX, int initY);
    virtual void doSomething();
    virtual bool isImmovable() const;
private:
    
};

class Player : public Actor 
{
public:
    Player(StudentWorld* sw, int imageID, int initX, int initY, int dir);
    virtual void doSomething();
private:
    int m_health;
    int m_peas;
};

#endif // ACTOR_H_
