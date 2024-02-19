#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// Actor Constructor and Destructor
Actor::Actor(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: GraphObject(imageID, initX, initY, dir), m_studentWorld(sw)
{
    setVisible(true);
}

Actor::~Actor()
{
}

// Wall Constructor
Wall::Wall(StudentWorld* sw, int imageID, int initX, int initY)
: Actor(sw, imageID, initX, initY)
{
    
}

// Player Constructor
Player::Player(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: Actor(sw, imageID, initX, initY, dir), m_health(20), m_peas(20)
{
}

// Actor Functions
StudentWorld* Actor::getWorld() const
{
    return m_studentWorld;
}

bool Actor::isImmovable() const
{
    return false;
}

// Player Functions
void Player::doSomething()
{
    int key;
    if (getWorld()->getKey(key))
    {
        if (key == KEY_PRESS_RIGHT)
        {
            setDirection(right);
            if(!getWorld()->isBarrierHere(getX()+1, getY()))
            {
                moveTo(getX()+1, getY());
            }
        }
        if(key == KEY_PRESS_LEFT)
        {
            setDirection(left);
            if(!getWorld()->isBarrierHere(getX()-1, getY()))
            {
                moveTo(getX()-1, getY());
            }
        }
        if(key == KEY_PRESS_UP)
        {
            setDirection(up);
            if(!getWorld()->isBarrierHere(getX(), getY()+1))
            {
                moveTo(getX(), getY()+1);
            }
        }
        if(key == KEY_PRESS_DOWN)
        {
            setDirection(down);
            if(!getWorld()->isBarrierHere(getX(), getY()-1))
            {
                moveTo(getX(), getY()-1);
            }
        }
    }
    /*
        Try to get user input (if any is available)
        If the user pressed the UP key and that square is open then
        Increase my y location by one
        If the user pressed the DOWN key and that square is open then
        Decrease my y location by one
        ...
        If the user pressed the space bar to fire and the player has
        peas, then
        Introduce a new pea object into the game
    */
}

// Wall Functions
void Wall::doSomething()
{
    return;
}

bool Wall::isImmovable() const
{
    return true;
}


    

    




