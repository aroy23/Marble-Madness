#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* sw, int imageID, int initX, int initY, int dir = none);
    virtual ~Actor();
    virtual void doSomething() = 0; // doSomething function to be called in StudentWorld's move function
    virtual bool immovable() const; // immovable function default set to true
    StudentWorld* getWorld() const; // getWorld function to return each actor's StudentWorld
private:
    StudentWorld* m_studentWorld; // data member so each actor knows its StudentWorld
};

class Wall : public Actor
{
public:
    Wall(StudentWorld* sw, int imageID, int initX, int initY);
    virtual void doSomething();
private:
};

class Player : public Actor 
{
public:
    Player(StudentWorld* sw, int imageID, int initX, int initY, int dir);
    virtual void doSomething();
    virtual bool immovable() const;
private:
    int m_health; // data member for health points
    int m_peas; // data member for pea ammo
};

#endif // ACTOR_H_
