#include "Actor.h"
#include "StudentWorld.h"

// Actor Constructor and Destructor
Actor::Actor(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: GraphObject(imageID, initX, initY, dir), m_studentWorld(sw)
{}

Actor::~Actor()
{}

// Wall Constructor
Wall::Wall(StudentWorld* sw, int imageID, int initX, int initY)
: Actor(sw, imageID, initX, initY)
{
    setVisible(true); // Setting Wall to be Visible
}

// Player Constructor
Player::Player(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: Actor(sw, imageID, initX, initY, dir), m_health(20), m_peas(20)
{
    setVisible(true); // Setting Player to be Visible
}

// Actor Functions
StudentWorld* Actor::getWorld() const
{
    return m_studentWorld; // Returning the StudentWorld
}

bool Actor::immovable() const
{
    return true; // Most actors are immovable so return true;
}

// Player Functions
void Player::doSomething()
{
    int key;
    if (getWorld()->getKey(key)) // If a key is pressed get that key
    {
        if (key == KEY_PRESS_RIGHT) // Moving right
        {
            setDirection(right);
            if(!getWorld()->isBarrierHere(getX()+1, getY())) // If no barrier preventing movement, move
            {
                moveTo(getX()+1, getY());
            }
        }
        if(key == KEY_PRESS_LEFT) // Moving left
        {
            setDirection(left);
            if(!getWorld()->isBarrierHere(getX()-1, getY())) // If no barrier preventing movement, move
            {
                moveTo(getX()-1, getY());
            }
        }
        if(key == KEY_PRESS_UP) // Moving up
        {
            setDirection(up);
            if(!getWorld()->isBarrierHere(getX(), getY()+1)) // If no barrier preventing movement, move
            {
                moveTo(getX(), getY()+1);
            }
        }
        if(key == KEY_PRESS_DOWN) // Moving down
        {
            setDirection(down);
            if(!getWorld()->isBarrierHere(getX(), getY()-1)) // If no barrier preventing movement, move
            {
                moveTo(getX(), getY()-1);
            }
        }
    }
}

bool Player::immovable() const
{
    return false; // Setting player to not immovable
}

// Wall Functions
void Wall::doSomething()
{
    return; //Wall should do nothing
}




    

    




