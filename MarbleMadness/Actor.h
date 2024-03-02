#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject // Base Class for Every Actor, Actor
{
public:
    Actor(StudentWorld* sw, int imageID, int initX, int initY, int dir = none);
    virtual ~Actor();
    virtual void doSomething() = 0;
    void setCanBeStolenStatus(bool state);
    Actor* getItemOnMe() const;
    void setItemOnMe(Actor* m);
    
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
    Actor* m_itemOnMe;
    bool m_dead;
    bool m_canBeStolen;
};

// The comments describe the polymorphic structure below

class Pea : public Actor // A pea is an Actor
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

class Entity : public Actor // An Entity is a type of actor which has health
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
class Player : public Entity // A player is a type of entity
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

class Marble : public Entity // A marble is a type of entity
{
public:
    Marble(StudentWorld* sw, int imageID, int initX, int initY);
    virtual void doSomething();
    virtual bool push(int dir);
    
    virtual bool canBePushed() const {
        return true;
    }
};

class Robot : public Entity // A robot is a type of entity
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

class RageBot : public Robot // A ragebot is a type of entity, but it is also a type of robot, thus it derives entity and robot
{
public:
    RageBot(StudentWorld* sw, int imageID, int initX, int initY, int dir);
    virtual void doSomething();
    virtual void takeDamage();
};

class ThiefBot : public Robot // A ThiefBot is a type of entity, but it is also a type of robot, thus it derives entity and robot
{
public:
    ThiefBot(StudentWorld* sw, int imageID, int initX, int initY, int dir, int hp = 5);
    virtual void doSomething();
    virtual void takeDamage();
    void setMyGoodie(Actor* a);
    Actor* returnMyGoodie() const;
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
    void directionDeciding(int dir, int x, int y);
    void directionTuning(int dir, int x, int y, bool vert);
    bool moveIfAvailableRoom(int dir, int x, int y);
    Actor* m_myGoodie;
};

class MeanThiefBot : public ThiefBot // A  Mean ThiefBot is a type of entity, but it is also a type of robot, but it is also a type of ThiefBot thus it derives entity, robot, and ThiefBot
{
public:
    MeanThiefBot(StudentWorld* sw, int imageID, int initX, int initY, int dir);
    virtual void doSomething();
    virtual void takeDamage();
private:
    
};

class EntityBarrier : public Actor // An EntityBarrier is a type of actor which stops those of class type entity from moving, with one exception (marbles)
{
public:
    EntityBarrier(StudentWorld* sw, int imageID, int initX, int initY, int dir = none);
    virtual void doSomething() = 0;
};

// Entity Barriers
class Wall : public EntityBarrier // A wall is a type of entity barrier
{
public:
    Wall(StudentWorld* sw, int imageID, int initX, int initY);
    virtual void doSomething();
};

class Pit : public EntityBarrier // A pit is a type of entity barrier
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
};

class ThiefBotFactory : public EntityBarrier // A ThiefBot Factory is a type of Entity Barrier
{
public:
    ThiefBotFactory(StudentWorld* sw, int imageID, int initX, int initY, bool meanOrNot);
    virtual void doSomething();
private:
    bool m_mean;
};

class PushableBarrier : public Actor // A PushableBarrier is a type of actor which stops pushable objects from moving
{
public:
    PushableBarrier(StudentWorld* sw, int imageID, int initX, int initY, int dir = none);
    virtual void doSomething() = 0;
    
    virtual bool canPeaPass() const {
        return true;
    }
    virtual bool canPlayerMoveIn() const {
        return true;
    }
};

class Crystal : public PushableBarrier // A crystal is a type of pushable barrier
{
public:
    Crystal(StudentWorld* sw, int imageID, int initX, int initY);
    virtual void doSomething();
};

class Exit : public PushableBarrier // an exit is a type of pushable barrier
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

class Goodie : public PushableBarrier // A Goodie is a type of pushable barrier
{
public:
    Goodie(StudentWorld* sw, int imageID, int initX, int initY);
    bool gotThisGoodie(Goodie* g, int x, int y, int score);
    
    virtual void doSomething() = 0;
    
    virtual bool stealable() const {
        return true;
    }
private:
};

class ExtraLifeGoodie : public Goodie // An ExtraLifeGoodie is a type of PushableBarrier, but it is also a type of Goodie, thus it derives PushableBarrier and Goodie
{
public:
    ExtraLifeGoodie(StudentWorld* sw, int imageID, int initX, int initY);
    virtual void doSomething();
};

class RestoreHealthGoodie : public Goodie // A RestorHealthGoodie is a type of PushableBarrier, but it is also a type of Goodie, thus it derives PushableBarrier and Goodie
{
public:
    RestoreHealthGoodie(StudentWorld* sw, int imageID, int initX, int initY);
    virtual void doSomething();
};

class AmmoGoodie : public Goodie // An AmmoGoodie is a type of PushableBarrier, but it is also a type of Goodie, thus it derives PushableBarrier and Goodie
{
public:
    AmmoGoodie(StudentWorld* sw, int imageID, int initX, int initY);
    virtual void doSomething();
};

#endif // ACTOR_H_
