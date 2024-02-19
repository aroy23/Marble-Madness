#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>

class Actor;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool isBarrierHere(int x, int y); // Function to return if a barrier is at argument coords
private:
    std::vector<Actor*> m_actors; // Vector containing all actors but player
    Actor* m_player; // Player Actor
    bool playerDeleted; // bool marker to ensure player is only deleted once
};

#endif // STUDENTWORLD_H_
