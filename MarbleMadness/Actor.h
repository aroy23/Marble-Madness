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
    virtual bool isStationary() const; // immovable function default set to true
    bool isAlive() const; // Checks if actor is alive
    void setDead(); // Sets actor state to dead
    StudentWorld* getWorld() const; // getWorld function to return each actor's StudentWorld
private:
    StudentWorld* m_studentWorld; // data member so each actor knows its StudentWorld
    bool m_dead; // data member for each actor to check if alive or dead
};

class Wall : public Actor
{
public:
    Wall(StudentWorld* sw, int imageID, int initX, int initY);
    virtual void doSomething();
private:
};

class Entity : public Actor
{
public:
    Entity(StudentWorld* sw, int imageID, int initX, int initY, int hp, int dir = none);
    virtual void doSomething() = 0;
    void takeDamage(); // reduces health
private:
    int m_health; // data member for health points
};

class Player : public Entity
{
public:
    Player(StudentWorld* sw, int imageID, int initX, int initY, int dir);
    virtual void doSomething();
    virtual bool isStationary() const;
    void fire();
private:
    int m_peas; // data member for number of peas
};

class Marble : public Entity
{
public:
    Marble(StudentWorld* sw, int imageID, int initX, int initY);
    virtual void doSomething();
    void push(int dir);
};

class Pea : public Actor
{
public:
    Pea(StudentWorld* sw, int imageID, int initX, int initY, int dir);
    virtual bool isStationary() const;
    virtual void doSomething();
};
#endif // ACTOR_H_
