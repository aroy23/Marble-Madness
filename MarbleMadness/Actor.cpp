#include "Actor.h"
#include "StudentWorld.h"

// Actor Constructor and Destructor
Actor::Actor(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: GraphObject(imageID, initX, initY, dir), m_studentWorld(sw), m_dead(false)
{}

Actor::~Actor()
{}

// Entity Constructor
Entity::Entity(StudentWorld* sw, int imageID, int initX, int initY, int hp, int dir)
: Actor(sw, imageID, initX, initY, dir), m_health(hp)
{}

// Wall Constructor
Wall::Wall(StudentWorld* sw, int imageID, int initX, int initY)
: Actor(sw, imageID, initX, initY)
{
    setVisible(true); // Setting Wall to be Visible
}

// Player Constructor
Player::Player(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: Entity(sw, imageID, initX, initY, 20, dir), m_peas(20)
{
    setVisible(true); // Setting Player to be Visible
}

// Marble Constructor
Marble::Marble(StudentWorld* sw, int imageID, int initX, int initY)
: Entity(sw, imageID, initX, initY, 10)
{
    setVisible(true); // Seting Marble to be Visible
}

// Pea Constructor
Pea::Pea(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: Actor(sw, imageID, initX, initY, dir)
{
    setVisible(true); // Setting Pea to be Visible
}

// Actor Functions
StudentWorld* Actor::getWorld() const
{
    return m_studentWorld; // Returning the StudentWorld
}

bool Actor::isStationary() const
{
    return true; // Most actors are immovable so return true;
}

bool Actor::isAlive() const
{
    return !m_dead; // Returning if actor is alive or not
}

void Actor::setDead()
{
    m_dead = true;
}

// Entity Functions
void Entity::takeDamage()
{
    m_health -= 2;
    if(m_health <= 0)
    {
        setDead();
    }
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
            else if(getWorld()->pushIfBarrierMarble(getX()+1, getY(), right)) // if the barrier is a marble move if possible
            {
                if(!getWorld()->isBarrierHere(getX()+1, getY()))
                {
                    moveTo(getX()+1, getY());
                }
            }
        }
        if(key == KEY_PRESS_LEFT) // Moving left
        {
            setDirection(left);
            if(!getWorld()->isBarrierHere(getX()-1, getY())) // If no barrier preventing movement, move
            {
                moveTo(getX()-1, getY());
            }
            else if(getWorld()->pushIfBarrierMarble(getX()-1, getY(), left)) // if the barrier is a marble move if possible
            {
                if(!getWorld()->isBarrierHere(getX()-1, getY()))
                {
                    moveTo(getX()-1, getY());
                }
            }
        }
        if(key == KEY_PRESS_UP) // Moving up
        {
            setDirection(up);
            if(!getWorld()->isBarrierHere(getX(), getY()+1)) // If no barrier preventing movement, move
            {
                moveTo(getX(), getY()+1);
            }
            else if(getWorld()->pushIfBarrierMarble(getX(), getY()+1, up)) // if the barrier is a marble move if possible
            {
                if(!getWorld()->isBarrierHere(getX(), getY()+1))
                {
                    moveTo(getX(), getY()+1);
                }
            }
        }
        if(key == KEY_PRESS_DOWN) // Moving down
        {
            setDirection(down);
            if(!getWorld()->isBarrierHere(getX(), getY()-1)) // If no barrier preventing movement, move
            {
                moveTo(getX(), getY()-1);
            }
            else if(getWorld()->pushIfBarrierMarble(getX(), getY()-1, down)) // if the barrier is a marble move if possible
            {
                if(!getWorld()->isBarrierHere(getX(), getY()-1))
                {
                    moveTo(getX(), getY()-1);
                }
            }
        }
        if(key == KEY_PRESS_SPACE) // Attempting to fire
        {
            if(m_peas > 0)
            {
                m_peas--;
                fire();
            }
        }
    }
}

bool Player::isStationary() const
{
    return false; // Setting player to not stationary
}

void Player::fire()
{
    getWorld()->playSound(SOUND_PLAYER_FIRE);
    getWorld()->firePea(getX(), getY(), getDirection());
}

// Wall Functions
void Wall::doSomething()
{
    return; // Wall should do nothing
}

// Marble Functions
void Marble::doSomething()
{
    return; // Marble should do nothing
}

void Marble::push(int dir)
{
    if(dir == right && !getWorld()->isBarrierHere(getX()+1, getY())) // If direction is right and no barrier, push
    {
        moveTo(getX()+1, getY());
    }
    if(dir == left && !getWorld()->isBarrierHere(getX()-1, getY())) // If direction is left and no barrier, push
    {
        moveTo(getX()-1, getY());
    }
    if(dir == up && !getWorld()->isBarrierHere(getX(), getY()+1)) // If direction is up and no barrier, push
    {
        moveTo(getX(), getY()+1);
    }
    if(dir == down && !getWorld()->isBarrierHere(getX(), getY()-1)) // If direction is down and no barrier, push
    {
        moveTo(getX(), getY()-1);
    }
}

// Pea Functions
void Pea::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    if(getWorld()->entityHere(getX(), getY()))
    {
        Entity* p = dynamic_cast<Entity*>(getWorld()->actorHere(getX(), getY()));
        p->takeDamage();
        setDead();
    }
    else if(getWorld()->isBarrierHere(getX(), getY()))
    {
        setDead();
    }
    else
    {
        int dir = getDirection();
        if(dir == right)
        {
            moveTo(getX()+1, getY());
        }
        else if(dir == left)
        {
            moveTo(getX()-1, getY());
        }
        else if(dir == up)
        {
            moveTo(getX(), getY()+1);
        }
        else if(dir == down)
        {
            moveTo(getX(), getY()-1);
        }
        if(getWorld()->entityHere(getX(), getY()))
        {
            Entity* p = dynamic_cast<Entity*>(getWorld()->actorHere(getX(), getY()));
            p->takeDamage();
            setDead();
        }
        else if(getWorld()->isBarrierHere(getX(), getY()))
        {
            setDead();
        }
    }
}

bool Pea::isStationary() const
{
    return false; // Setting pea to not stationary
}











