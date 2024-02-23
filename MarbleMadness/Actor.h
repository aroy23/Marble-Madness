#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
class Marble;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* sw, int imageID, int initX, int initY, int dir = none);
    virtual ~Actor();
    virtual void doSomething() = 0;
    
    bool isAlive() const {
        return !m_dead;
    }
    void setDead() {
        m_dead = true;
    }
    virtual bool canNonMarbleEntityMoveIn() const {
        return false;
    }
    virtual bool canMarbleMoveIn() const {
        return false;
    }
    virtual bool hasHealth() const {
        return false;
    }
    virtual void takeDamage() {
        return;
    }
    virtual bool canPeaPass() const {
        return false;
    }
    
    StudentWorld* getWorld() const {
        return m_studentWorld;
    }
    
private:
    StudentWorld* m_studentWorld;
    bool m_dead;
};

class Pea : public Actor
{
public:
    Pea(StudentWorld* sw, int imageID, int initX, int initY, int dir);
    virtual void doSomething();
    
    virtual bool canPeaPass() const {
        return true;
    }
    
    virtual bool canNonMarbleEntityMoveIn() const {
        return true;
    }
    
    virtual bool canMarbleMoveIn() const {
        return true;
    }
};

class Entity : public Actor
{
public:
    Entity(StudentWorld* sw, int imageID, int initX, int initY, int hp, int dir = none);
    virtual void doSomething() = 0;
    virtual void takeDamage();
    
    void getHit() {
        m_health -= 2;
    }
    int getHealth() const {
        return m_health;
    }
    void restoreHealth() {
        m_health = 20;
    }
    
    virtual bool hasHealth() const {
        return true;
    }
    
private:
    int m_health;
};

// Entities
class Player : public Entity
{
public:
    Player(StudentWorld* sw, int imageID, int initX, int initY, int dir);
    virtual void doSomething();
    virtual void takeDamage();
    void fire();
    
    void incPeas() {
        m_peas += 20;
    }
    int getPeas() const {
        return m_peas;
    }
private:
    int m_peas;
};

class Marble : public Entity
{
public:
    Marble(StudentWorld* sw, int imageID, int initX, int initY);
    virtual void doSomething();
    bool push(int dir);
};

class RageBot : public Entity
{
public:
    RageBot(StudentWorld* sw, int imageID, int initX, int initY, int dir);
    virtual void doSomething();
    virtual void takeDamage();
private:
    int m_ticks;
    int m_ticksUntilICanMove;
    bool canIMove();
};

class EntityBarrier : public Actor
{
public:
    EntityBarrier(StudentWorld* sw, int imageID, int initX, int initY, int dir = none);
    virtual void doSomething() = 0;
};

// Entity Barriers
class Wall : public EntityBarrier
{
public:
    Wall(StudentWorld* sw, int imageID, int initX, int initY);
    virtual void doSomething();
};

class Pit : public EntityBarrier
{
public:
    Pit(StudentWorld* sw, int imageID, int initX, int initY);
    virtual void doSomething();
    
    virtual bool canPeaPass() const {
        return true;
    }
    virtual bool canMarbleMoveIn() const{
        return true;
    }
    Marble* getmarbleOnMe() const {
        return m_marbleOnMe;
    }
    void setmarbleOnMe(Marble* m) {
        m_marbleOnMe = m;
    }
private:
    Marble* m_marbleOnMe;
};

class MarbleBarrier : public Actor
{
public:
    MarbleBarrier(StudentWorld* sw, int imageID, int initX, int initY, int dir = none);
    virtual void doSomething() = 0;
    
    virtual bool canPeaPass() const {
        return true;
    }
    virtual bool canNonMarbleEntityMoveIn() const {
        return true;
    }
};

class Crystal : public MarbleBarrier
{
public:
    Crystal(StudentWorld* sw, int imageID, int initX, int initY);
    virtual void doSomething();
};

class Exit : public MarbleBarrier
{
public:
    Exit(StudentWorld* sw, int imageID, int initX, int initY);
    virtual void doSomething();
    virtual bool canMarbleMoveIn() const {
        return false;
    }
private:
    bool m_revealed;
};

class Goodie : public MarbleBarrier
{
public:
    Goodie(StudentWorld* sw, int imageID, int initX, int initY);
    virtual void doSomething() = 0;
    bool gotGoodie(Goodie* g, int x, int y, int score);
};

class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(StudentWorld* sw, int imageID, int initX, int initY);
    virtual void doSomething();
};

class RestoreHealthGoodie : public Goodie
{
public:
    RestoreHealthGoodie(StudentWorld* sw, int imageID, int initX, int initY);
    virtual void doSomething();
};

class AmmoGoodie : public Goodie
{
public:
    AmmoGoodie(StudentWorld* sw, int imageID, int initX, int initY);
    virtual void doSomething();
};

#endif // ACTOR_H_

