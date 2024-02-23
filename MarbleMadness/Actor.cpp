#include "Actor.h"
#include "StudentWorld.h"

Actor::~Actor()
{}

Actor::Actor(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: GraphObject(imageID, initX, initY, dir)
{
    m_studentWorld = sw;
    m_dead = false;
}

EntityBarrier::EntityBarrier(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: Actor(sw, imageID, initX, initY, dir)
{}

MarbleBarrier::MarbleBarrier(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: Actor(sw, imageID, initX, initY, dir)
{}

Entity::Entity(StudentWorld* sw, int imageID, int initX, int initY, int hp, int dir)
: Actor(sw, imageID, initX, initY, dir)
{
    m_health = hp;
}

Player::Player(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: Entity(sw, imageID, initX, initY, 20, dir)
{
    setVisible(true);
    m_peas = 20;
}

Marble::Marble(StudentWorld* sw, int imageID, int initX, int initY)
: Entity(sw, imageID, initX, initY, 10)
{
    setVisible(true);
}

Wall::Wall(StudentWorld* sw, int imageID, int initX, int initY)
: EntityBarrier(sw, imageID, initX, initY)
{
    setVisible(true);
}

Pea::Pea(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: Actor(sw, imageID, initX, initY, dir)
{
    setVisible(true);
}

Pit::Pit(StudentWorld* sw, int imageID, int initX, int initY)
: EntityBarrier(sw, imageID, initX, initY)
{
    m_marbleOnMe = nullptr;
    setVisible(true);
}

Crystal::Crystal(StudentWorld* sw, int imageID, int initX, int initY)
: MarbleBarrier(sw, imageID, initX, initY)
{
    setVisible(true);
}

Exit::Exit(StudentWorld* sw, int imageID, int initX, int initY)
: MarbleBarrier(sw, imageID, initX, initY)
{
    m_revealed = false;
    setVisible(false);
}

Goodie::Goodie(StudentWorld* sw, int imageID, int initX, int initY)
: MarbleBarrier(sw, imageID, initX, initY)
{
    setVisible(true);
}

ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* sw, int imageID, int initX, int initY)
: Goodie(sw, imageID, initX, initY)
{}

RestoreHealthGoodie::RestoreHealthGoodie(StudentWorld* sw, int imageID, int initX, int initY)
 : Goodie(sw, imageID, initX, initY)
{}

AmmoGoodie::AmmoGoodie(StudentWorld* sw, int imageID, int initX, int initY)
 : Goodie(sw, imageID, initX, initY)
{}

RageBot::RageBot(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: Entity(sw, imageID, initX, initY, 10, dir)
{
    setVisible(true);
    m_ticks = 0;
}

// Entity Functions
void Entity::takeDamage() {
    getHit();
    if(m_health <= 0)
    {
        setDead();
    }
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
            
            if((getWorld()->retrieveKnownPit(getX()+1, getY()) != nullptr && getWorld()->retrieveKnownPit(getX()+1, getY())->getmarbleOnMe() != nullptr) || getWorld()->canNonMarbleEntityMoveHere(getX()+1, getY()))
            {
                moveTo(getX()+1, getY());
            }
            else if(getWorld()->pushIfBarrierMarbleHere(getX()+1, getY(), right))
            {
                moveTo(getX()+1, getY());
            }
        }
        if(key == KEY_PRESS_LEFT)
        {
            setDirection(left);
            if((getWorld()->retrieveKnownPit(getX()-1, getY()) != nullptr && getWorld()->retrieveKnownPit(getX()-1, getY())->getmarbleOnMe() != nullptr) || getWorld()->canNonMarbleEntityMoveHere(getX()-1, getY()))
            {
                moveTo(getX()-1, getY());
            }
            else if(getWorld()->pushIfBarrierMarbleHere(getX()-1, getY(), left))
            {
                moveTo(getX()-1, getY());
            }
        }
        if(key == KEY_PRESS_UP)
        {
            setDirection(up);
            if((getWorld()->retrieveKnownPit(getX(), getY()+1) != nullptr && getWorld()->retrieveKnownPit(getX(), getY()+1)->getmarbleOnMe() != nullptr) || getWorld()->canNonMarbleEntityMoveHere(getX(), getY()+1))
            {
                moveTo(getX(), getY()+1);
            }
            else if(getWorld()->pushIfBarrierMarbleHere(getX(), getY()+1, up))
            {
                moveTo(getX(), getY()+1);
            }
        }
        if(key == KEY_PRESS_DOWN)
        {
            setDirection(down);
            if((getWorld()->retrieveKnownPit(getX(), getY()-1) != nullptr && getWorld()->retrieveKnownPit(getX(), getY()-1)->getmarbleOnMe() != nullptr) || getWorld()->canNonMarbleEntityMoveHere(getX(), getY()-1))
            {
                moveTo(getX(), getY()-1);
            }
            else if(getWorld()->pushIfBarrierMarbleHere(getX(), getY()-1, down))
            {
                moveTo(getX(), getY()-1);
            }
        }
        if(key == KEY_PRESS_SPACE)
        {
            if(m_peas > 0)
            {
                m_peas--;
                fire();
            }
        }
        if(key == KEY_PRESS_ESCAPE)
        {
            setDead();
        }
    }
}

void Player::takeDamage()
{
    getHit();
    if(getHealth() <= 0)
    {
        getWorld()->playSound(SOUND_PLAYER_DIE);
        setDead();
    }
    else
    {
        getWorld()->playSound(SOUND_PLAYER_IMPACT);
    }
}

void Player::fire()
{
    getWorld()->playSound(SOUND_PLAYER_FIRE);
    getWorld()->firePea(getX(), getY(), getDirection());
}

// Wall Functions
void Wall::doSomething()
{
    return;
}

// Marble Functions
void Marble::doSomething()
{
    return;
}

bool Marble::push(int dir)
{
    if(dir == right && getWorld()->canMarbleEntityMoveHere(getX()+1, getY()))
    {
        if(getWorld()->retrieveKnownPit(getX()+1, getY()) != nullptr)
        {
            getWorld()->retrieveKnownPit(getX()+1, getY())->setmarbleOnMe(this);
        }
        moveTo(getX()+1, getY());
        return true;
    }
    if(dir == left && getWorld()->canMarbleEntityMoveHere(getX()-1, getY()))
    {
        if(getWorld()->retrieveKnownPit(getX()-1, getY()) != nullptr)
        {
            getWorld()->retrieveKnownPit(getX()-1, getY())->setmarbleOnMe(this);
        }
        moveTo(getX()-1, getY());
        return true;
    }
    if(dir == up && getWorld()->canMarbleEntityMoveHere(getX(), getY()+1))
    {
        if(getWorld()->retrieveKnownPit(getX(), getY()+1) != nullptr)
        {
            getWorld()->retrieveKnownPit(getX(), getY()+1)->setmarbleOnMe(this);
        }
        moveTo(getX(), getY()+1);
        return true;
    }
    if(dir == down && getWorld()->canMarbleEntityMoveHere(getX(), getY()-1))
    {
        if(getWorld()->retrieveKnownPit(getX(), getY()-1) != nullptr)
        {
            getWorld()->retrieveKnownPit(getX(), getY()-1)->setmarbleOnMe(this);
        }
        moveTo(getX(), getY()-1);
        return true;
    }
    return false;
}

// Pea Functions
void Pea::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    if(getWorld()->damageActorWithPeaIfHere(getX(), getY()))
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
        if(getWorld()->damageActorWithPeaIfHere(getX(), getY()))
        {
            setDead();
        }
    }
    
}

// Pit Functions
void Pit::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    if(getmarbleOnMe() != nullptr)
    {
        getmarbleOnMe()->setDead();
        setDead();
    }
}

// Crystal Functions
void Crystal::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    if(getWorld()->playerHere(getX(), getY()))
    {
        getWorld()->crystalObtained();
        getWorld()->increaseScore(50);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
}

// Exit Functions
void Exit::doSomething()
{
    if(!m_revealed && getWorld()->getCrystals() == 0)
    {
        m_revealed = true;
        setVisible(true);
        getWorld()->playSound(SOUND_REVEAL_EXIT);
    }
    else if(m_revealed && getWorld()->playerHere(getX(), getY()))
    {
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->levelFinished();
    }
}

// Goodie Functions
bool Goodie::gotGoodie(Goodie* g, int x, int y, int score)
{
    if(getWorld()->playerHere(x, y))
    {
        g->getWorld()->increaseScore(score);
        g->setDead();
        g->getWorld()->playSound(SOUND_GOT_GOODIE);
        return true;
    }
    return false;
}

// ExtraLifeGoodie Functions
void ExtraLifeGoodie::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    if(gotGoodie(this, getX(), getY(), 1000))
    {
        getWorld()->incLives();
    }
}

// RestoreHealthGoodie Functions
void RestoreHealthGoodie::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    if(gotGoodie(this, getX(), getY(), 500))
    {
        getWorld()->retrievePlayer()->restoreHealth();
    }
}

// AmmoGoodie Functions
void AmmoGoodie::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    if(gotGoodie(this, getX(), getY(), 100))
    {
        getWorld()->retrievePlayer()->incPeas();
    }
}

// RageBot Functions
void RageBot::doSomething()
{
    int ticks = ((28 - getWorld()->getLevel()) / 4);
    if (ticks < 3)
    ticks = 3;
    m_ticks = ticks-1;
    
    if(!isAlive())
    {
        return;
    }
    if(m_ticks != 0)
    {
        m_ticks--;
        return;
    }
}




