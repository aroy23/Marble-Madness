#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
class Marble;
class Goodie;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* sw, int imageID, int initX, int initY, int dir = none);
    virtual ~Actor();
    virtual void doSomething() = 0;
    void setCanBeStolenStatus(bool state);
    
    bool isAlive() const {
        return !m_dead;
    }
    void setDead() {
        m_dead = true;
    }
    virtual bool canPlayerMoveIn() const {
        return false;
    }
    virtual bool canPushableObjectMoveIn() const {
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
    virtual bool canSteal() const {
        return false;
    }
    bool canBeStolen() const {
        return m_canBeStolen;
    }
    virtual bool canBePushed() const {
        return false;
    }
    virtual bool stealable() const {
        return false;
    }
    StudentWorld* getWorld() const {
        return m_studentWorld;
    }
    virtual bool push(int dir) {
        return false;
    }
    virtual bool allowsMarble() const {
        return false;
    }
    
private:
    StudentWorld* m_studentWorld;
    bool m_dead;
    bool m_canBeStolen;
};

class Pea : public Actor
{
public:
    Pea(StudentWorld* sw, int imageID, int initX, int initY, int dir);
    virtual void doSomething();
    
    virtual bool canPeaPass() const {
        return true;
    }
    virtual bool canPlayerMoveIn() const {
        return true;
    }
};

class Entity : public Actor
{
public:
    Entity(StudentWorld* sw, int imageID, int initX, int initY, int hp, int dir = none);
    virtual void doSomething() = 0;
    virtual void takeDamage();
    void setHealth(int amt);
    void getHit();
    
    int getHealth() const {
        return m_health;
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
    void incPeas();
    
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
    virtual bool push(int dir);
    
    virtual bool canBePushed() const {
        return true;
    }
};

class Robot : public Entity
{
public:
    Robot(StudentWorld* sw, int imageID, int initX, int initY, int hp, int dir);
    virtual void doSomething() = 0;
    virtual void takeDamage() {
        return;
    }
    void decreaseTicks();
    void equalizeTicks();
    
    int getTicks() const {
        return m_ticks;
    }
private:
    int m_ticks;
    int m_ticksUntilICanMove;
};

class RageBot : public Robot
{
public:
    RageBot(StudentWorld* sw, int imageID, int initX, int initY, int dir);
    virtual void doSomething();
    virtual void takeDamage();
};

class ThiefBot : public Robot
{
public:
    ThiefBot(StudentWorld* sw, int imageID, int initX, int initY, int dir, int hp = 5);
    virtual void doSomething();
    virtual void takeDamage();
    void setMyGoodie(Goodie* g);
    Goodie* returnMyGoodie() const;
    void resetSquaresMoved();
    void increaseSquaresMoved();
    void setDistanceBeforeTurning(int d);
    void shootStealMoveOrTurn(bool mean);
    void thiefBotAttacked();
    
    virtual bool canSteal() const {
        return true;
    }
    int getSquaresMoved() const {
        return m_squaresMoved;
    }
    int getDistanceBeforeTurn() const {
        return m_distanceBeforeTurning;
    }
    void gotGoodie() {
        m_haveGoodie = true;
    }
    bool returnGoodieStatus() const {
        return m_haveGoodie;
    }
    
private:
    bool m_haveGoodie;
    int m_squaresMoved;
    int m_distanceBeforeTurning;
    Goodie* m_myGoodie;
};

class MeanThiefBot : public ThiefBot
{
public:
    MeanThiefBot(StudentWorld* sw, int imageID, int initX, int initY, int dir);
    virtual void doSomething();
    virtual void takeDamage();
private:
    
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
    virtual bool canPushableObjectMoveIn() const {
        return true;
    }
    
    virtual bool allowsMarble() const {
        return true;
    }
    Marble* getmarbleOnMe() const;
    void setmarbleOnMe(Marble* m);
private:
    Marble* m_marbleOnMe;
};

class ThiefBotFactory : public EntityBarrier
{
public:
    ThiefBotFactory(StudentWorld* sw, int imageID, int initX, int initY, bool meanOrNot);
    virtual void doSomething();
private:
    bool m_mean;
};

class MarbleBarrier : public Actor
{
public:
    MarbleBarrier(StudentWorld* sw, int imageID, int initX, int initY, int dir = none);
    virtual void doSomething() = 0;
    
    virtual bool canPeaPass() const {
        return true;
    }
    virtual bool canPlayerMoveIn() const {
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
    virtual bool canPushableObjectMoveIn() const {
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
    
    virtual bool stealable() const {
        return true;
    }
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

