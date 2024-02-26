#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <list>

class Actor;
class Player;
class Pit;
class Goodie;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    void levelFinished();
    
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
    Actor* canBotSteal(int x, int y);
    
    int countTheBotsAroundMe(int factoryX, int factoryY);
    bool isThiefBotOnMe(int x, int y);
    void spawnBot(int x, int y, bool meanOrNot);
    bool canRobotMoveHere(int x, int y);
    
    void moveGoodieToAdjacentOpenSpace(int x, int y, Goodie* g);
private:
    bool m_firstFile;
    std::string obtainLevel();
    std::string formatDisplayText(int score, int level, int lives, int health, int ammo, int bonus);
    void setDisplayText();
    int countVisibleActorsHereExceptExit(int x, int y);
    void stealGoodie(int x, int y, Actor* a);
    bool amIInTheRegion(int x, int y, int factoryX, int factoryY);
    Actor* isActorHere(int x, int y);
    Actor* isActorHereBackwards(int x, int y);
    void startingNewLevel();
    int m_crystals;
    bool m_levelComplete;
    int m_bonus = 1000;
    std::list<Actor*> m_actors;
    Player* m_player;
};

#endif // STUDENTWORLD_H_
