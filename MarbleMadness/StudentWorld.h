#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>

class Actor;
class Player;
class Pit;
class Entity;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    void levelFinished();
    std::string formatDisplayText(int score, int level, int lives, int health, int ammo, int bonus);
    void setDisplayText();
    
    void crystalObtained();
    int getCrystals();
    
    bool canNonMarbleEntityMoveHere(int x, int y);
    bool canMarbleEntityMoveHere(int x, int y);
    bool pushIfBarrierMarbleHere(int x, int y, int dir);
    Pit* retrieveKnownPit(int x, int y);
    
    bool damageActorWithPeaIfHere(int x, int y);
    void firePea(int x, int y, int dir);

    bool playerHere(int x, int y);
    Player* retrievePlayer();
    
    bool canBotFire(int x, int y, int dir);
private:
    Actor* isActorHere(int x, int y);
    int m_crystals;
    bool m_levelComplete;
    int m_bonus = 1000;
    std::vector<Actor*> m_actors; // Vector containing all actors but player
    Player* m_player; // Player Actor
};

#endif // STUDENTWORLD_H_




