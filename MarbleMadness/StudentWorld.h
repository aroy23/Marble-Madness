#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <list>

class Actor;
class Player;
class ThiefBot;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    
    // Required Functions
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    void levelFinished(); // Housekeeping method for finishing level
    
    void crystalObtained(); // Decreases crystal count by 1 in the world
    int getCrystals(); // Returns num crystals
    
    bool canPlayerMoveHere(int x, int y); // If player can move to x y
    bool canPushableObjectMoveHere(int x, int y); // If pushable object can move to x y
    bool canEnemyMoveHere(int x, int y); // Determines if enemy can move to x y
    bool pushIfPushableHere(int x, int y, int dir); // Trys to push a pushable objects
    Actor* retrieveObjectSwallower(int x, int y); // Retrieves an object if it can swallow at x y
    
    bool damageActorWithPeaIfHere(int x, int y); // Damage actor with pea if possible
    void firePea(int x, int y, int dir); // Fires a pea

    bool playerHere(int x, int y); // Confirms if player is at x y
    Player* retrievePlayer() { // Retrieve player pointer
        return m_player;
    }
    
    bool canEnemyFire(int x, int y, int dir); // Determines if an enemy is able to shoot a pea
    Actor* canThiefSteal(int x, int y, ThiefBot* p); // Determines if a thief can steal a goodie
    
    int countTheThiefsAroundMe(int factoryX, int factoryY); // Counts the thiefs in a factory's region
    bool isThiefOnMe(int x, int y); // determines if thief is on factory
    void spawnThief(int x, int y, bool meanOrNot); // factory thief spawning
    
    void moveActorToAdjacentOpenSpace(int x, int y, Actor* a); // Moves goodie to adjacent space if one is present and current is occupied
private:
    std::string obtainLevel(); // stringstreams to get the level
    std::string formatDisplayText(int score, int level, int lives, int health, int ammo, int bonus); // formatting level
    void setDisplayText(); // displaying the level
    int countVisibleActorsHereExceptExit(int x, int y); // Counting the visible actors at x y, useful for goodie dropping behavior
    void stealGoodie(int x, int y, Actor* a, ThiefBot* t); // A goodie in the world is stolen
    bool amIInTheRegion(int x, int y, int factoryX, int factoryY); // Determines whether an object is in a factory's region
    Actor* isActorHere(int x, int y); // Gets first actor at x y by traversing forward through list STL
    Actor* isActorHereBackwards(int x, int y); // Gets first actor at x y by traversing backward through list STL
    void startingNewLevel(); // Housekeeping method that gets called in init
    int m_crystals; // crystals in the world
    bool m_levelComplete; // is the level complete or not
    int m_bonus = 1000; // max bonus that can be granted
    std::list<Actor*> m_actors; // STL list of actors
    Player* m_player; // player
};

#endif // STUDENTWORLD_H_
