#include "Actor.h"
#include "StudentWorld.h"

Actor::~Actor()
{}

Actor::Actor(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: GraphObject(imageID, initX, initY, dir)
{
    m_canBeStolen = false;
    setVisible(true);
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
    m_peas = 20;
}

Marble::Marble(StudentWorld* sw, int imageID, int initX, int initY)
: Entity(sw, imageID, initX, initY, 10)
{}

Wall::Wall(StudentWorld* sw, int imageID, int initX, int initY)
: EntityBarrier(sw, imageID, initX, initY)
{
    setVisible(true);
}

Pea::Pea(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: Actor(sw, imageID, initX, initY, dir)
{}

Pit::Pit(StudentWorld* sw, int imageID, int initX, int initY)
: EntityBarrier(sw, imageID, initX, initY)
{
    m_marbleOnMe = nullptr;
}

Crystal::Crystal(StudentWorld* sw, int imageID, int initX, int initY)
: MarbleBarrier(sw, imageID, initX, initY)
{}

Exit::Exit(StudentWorld* sw, int imageID, int initX, int initY)
: MarbleBarrier(sw, imageID, initX, initY)
{
    m_revealed = false;
    setVisible(false);
}

Goodie::Goodie(StudentWorld* sw, int imageID, int initX, int initY)
: MarbleBarrier(sw, imageID, initX, initY)
{
    setCanBeStolenStatus(true);
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

Robot::Robot(StudentWorld* sw, int imageID, int initX, int initY, int hp, int dir)
: Entity(sw, imageID, initX, initY, hp, dir)
{
    int ticksUntilMove = (28 - getWorld()->getLevel()) / 4;
    if (ticksUntilMove < 3)
    {
        ticksUntilMove = 3;
    }
    m_ticksUntilICanMove = ticksUntilMove-1;
    m_ticks = ticksUntilMove-1;
    setVisible(true);
    m_ticks = ticksUntilMove-1;
}

RageBot::RageBot(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: Robot(sw, imageID, initX, initY, 10, dir)
{}

ThiefBot::ThiefBot(StudentWorld* sw, int imageID, int initX, int initY, int dir, int hp)
: Robot(sw, imageID, initX, initY, hp, dir)
{
    m_haveGoodie = false;
    m_squaresMoved = 0;
    m_distanceBeforeTurning = randInt(1, 6);
    m_myGoodie = nullptr;
}

MeanThiefBot::MeanThiefBot(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: ThiefBot(sw, imageID, initX, initY, dir, 8)
{}

ThiefBotFactory::ThiefBotFactory(StudentWorld* sw, int imageID, int initX, int initY, bool meanOrNot)
: EntityBarrier(sw, imageID, initX, initY)
{
    m_mean = meanOrNot;
}

// Actor Functions
void Actor::setCanBeStolenStatus(bool state)
{
    m_canBeStolen = state;
}

// Entity Functions
void Entity::takeDamage() {
    getHit();
    if(m_health <= 0)
    {
        setDead();
    }
}

void Entity::setHealth(int amt)
{
    m_health = amt;
}

void Entity::getHit()
{
    m_health -= 2;
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

void Player::incPeas()
{
    m_peas += 20;
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

Marble* Pit::getmarbleOnMe() const 
{
    return m_marbleOnMe;
}

void Pit::setmarbleOnMe(Marble* m)
{
    m_marbleOnMe = m;
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
    if(getWorld()->playerHere(x, y) && canBeStolen())
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
        getWorld()->retrievePlayer()->setHealth(20);
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

// Robot Functions
void Robot::decreaseTicks() {
    m_ticks--;
}
void Robot::equalizeTicks() {
    m_ticks = m_ticksUntilICanMove-1;
}

// RageBot Functions
void RageBot::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    if(getTicks() != 0)
    {
        decreaseTicks();
        return;
    }
    else if(getWorld()->canBotFire(getX(), getY(), getDirection()))
    {
        equalizeTicks();
        getWorld()->firePea(getX(), getY(), getDirection());
        return;
    }
    else
    {
        equalizeTicks();
        if(getDirection() == right)
        {
            if(getWorld()->canRobotMoveHere(getX()+1, getY()))
            {
                moveTo(getX()+1, getY());
            }
            else
            {
                setDirection(left);
            }
        }
        else if(getDirection() == left)
        {
            if(getWorld()->canRobotMoveHere(getX()-1, getY()))
            {
                moveTo(getX()-1, getY());
            }
            else
            {
                setDirection(right);
            }
        }
        else if(getDirection() == up)
        {
            if(getWorld()->canRobotMoveHere(getX(), getY()+1))
            {
                moveTo(getX(), getY()+1);
            }
            else
            {
                setDirection(down);
            }
        }
        else if(getDirection() == down)
        {
            if(getWorld()->canRobotMoveHere(getX(), getY()-1))
            {
                moveTo(getX(), getY()-1);
            }
            else
            {
                setDirection(up);
            }
        }
    }
}

void RageBot::takeDamage()
{
    getHit();
    if(getHealth() <= 0)
    {
        getWorld()->playSound(SOUND_ROBOT_DIE);
        setDead();
        getWorld()->increaseScore(100);
    }
    else
    {
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    }
}

// ThiefBot Functions
void ThiefBot::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    if(getTicks() != 0)
    {
        decreaseTicks();
        return;
    }
    shootStealMoveOrTurn(false);
}

void ThiefBot::thiefBotAttacked()
{
    getHit();
    if(getHealth() <= 0)
    {
        if(returnGoodieStatus())
        {
            getWorld()->moveGoodieToAdjacentOpenSpace(getX(), getY(), returnMyGoodie());
            returnMyGoodie()->setVisible(true);
            returnMyGoodie()->setCanBeStolenStatus(true);
        }
        getWorld()->playSound(SOUND_ROBOT_DIE);
        setDead();
    }
    else
    {
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    }
}

void ThiefBot::takeDamage()
{
    thiefBotAttacked();
    if(!isAlive())
    {
        getWorld()->increaseScore(10);
    }
}

void ThiefBot::setMyGoodie(Goodie* g)
{
    m_myGoodie = g;
}

Goodie* ThiefBot::returnMyGoodie() const
{
    return m_myGoodie;
}

void ThiefBot::resetSquaresMoved()
{
    m_squaresMoved = 0;
}

void ThiefBot::increaseSquaresMoved()
{
    m_squaresMoved++;
}

void ThiefBot::setDistanceBeforeTurning(int d)
{
    m_distanceBeforeTurning = d;
}

void ThiefBot::shootStealMoveOrTurn(bool mean)
{
    if(mean && getWorld()->canBotFire(getX(), getY(), getDirection()))
    {
        equalizeTicks();
        getWorld()->firePea(getX(), getY(), getDirection());
        return;
    }
    else if(!returnGoodieStatus() && (getWorld()->canBotSteal(getX(), getY()) != nullptr))
    {
        equalizeTicks();
        gotGoodie();
        getWorld()->playSound(SOUND_ROBOT_MUNCH);
        return;
    }
    else if(getSquaresMoved() < getDistanceBeforeTurn())
    {
        equalizeTicks();
        if(getDirection() == right)
        {
            if(getWorld()->canRobotMoveHere(getX()+1, getY()))
            {
                moveTo(getX()+1, getY());
                increaseSquaresMoved();
                return;
            }
        }
        else if(getDirection() == left)
        {
            if(getWorld()->canRobotMoveHere(getX()-1, getY()))
            {
                moveTo(getX()-1, getY());
                increaseSquaresMoved();
                return;
            }
        }
        else if(getDirection() == up)
        {
            if(getWorld()->canRobotMoveHere(getX(), getY()+1))
            {
                moveTo(getX(), getY()+1);
                increaseSquaresMoved();
                return;
            }
        }
        else if(getDirection() == down)
        {
            if(getWorld()->canRobotMoveHere(getX(), getY()-1))
            {
                moveTo(getX(), getY()-1);
                increaseSquaresMoved();
                return;
            }
        }
    }
    equalizeTicks();
    resetSquaresMoved();
    setDistanceBeforeTurning(randInt(1, 6));
    int directionPicker= randInt(1, 4);
    int direction = -1;
    if(directionPicker == 1)
    {
        direction = right;
    }
    else if(directionPicker == 2)
    {
        direction = left;
    }
    else if(directionPicker == 3)
    {
        direction = up;
    }
    else if(directionPicker == 4)
    {
        direction = down;
    }
    if(direction == right)
    {
        if(getWorld()->canRobotMoveHere(getX()+1, getY()))
        {
            setDirection(right);
            moveTo(getX()+1, getY());
            increaseSquaresMoved();
            return;
        }
        else if(getWorld()->canRobotMoveHere(getX()-1, getY()))
        {
            setDirection(left);
            moveTo(getX()-1, getY());
            increaseSquaresMoved();
            return;
        }
        else if(getWorld()->canRobotMoveHere(getX(), getY()+1))
        {
            setDirection(up);
            moveTo(getX(), getY()+1);
            increaseSquaresMoved();
            return;
        }
        else if(getWorld()->canRobotMoveHere(getX(), getY()-1))
        {
            setDirection(down);
            moveTo(getX(), getY()-1);
            increaseSquaresMoved();
            return;
        }
        else
        {
            setDirection(right);
            return;
        }
    }
    else if(direction == left)
    {
        if(getWorld()->canRobotMoveHere(getX()-1, getY()))
        {
            setDirection(left);
            moveTo(getX()-1, getY());
            increaseSquaresMoved();
            return;
        }
        else if(getWorld()->canRobotMoveHere(getX()+1, getY()))
        {
            setDirection(right);
            moveTo(getX()+1, getY());
            increaseSquaresMoved();
            return;
        }
        else if(getWorld()->canRobotMoveHere(getX(), getY()+1))
        {
            setDirection(up);
            moveTo(getX(), getY()+1);
            increaseSquaresMoved();
            return;
        }
        else if(getWorld()->canRobotMoveHere(getX(), getY()-1))
        {
            setDirection(down);
            moveTo(getX(), getY()-1);
            increaseSquaresMoved();
            return;
        }
        else
        {
            setDirection(left);
            return;
        }
    }
    else if(direction == up)
    {
        if(getWorld()->canRobotMoveHere(getX(), getY()+1))
        {
            setDirection(up);
            moveTo(getX(), getY()+1);
            increaseSquaresMoved();
            return;
        }
        if(getWorld()->canRobotMoveHere(getX()-1, getY()))
        {
            setDirection(left);
            moveTo(getX()-1, getY());
            increaseSquaresMoved();
            return;
        }
        else if(getWorld()->canRobotMoveHere(getX()+1, getY()))
        {
            setDirection(right);
            moveTo(getX()+1, getY());
            increaseSquaresMoved();
            return;
        }
        else if(getWorld()->canRobotMoveHere(getX(), getY()-1))
        {
            setDirection(down);
            moveTo(getX(), getY()-1);
            increaseSquaresMoved();
            return;
        }
        else
        {
            setDirection(up);
            return;
        }
    }
    else if(direction == down)
    {
        if(getWorld()->canRobotMoveHere(getX(), getY()-1))
        {
            setDirection(down);
            moveTo(getX(), getY()-1);
            increaseSquaresMoved();
            return;
        }
        if(getWorld()->canRobotMoveHere(getX()-1, getY()))
        {
            setDirection(left);
            moveTo(getX()-1, getY());
            increaseSquaresMoved();
            return;
        }
        else if(getWorld()->canRobotMoveHere(getX()+1, getY()))
        {
            setDirection(left);
            moveTo(getX()+1, getY());
            increaseSquaresMoved();
            return;
        }
        else if(getWorld()->canRobotMoveHere(getX(), getY()+1))
        {
            setDirection(up);
            moveTo(getX(), getY()+1);
            increaseSquaresMoved();
            return;
        }
        else
        {
            setDirection(down);
            return;
        }
    }
}

// Mean ThiefBot Functions
void MeanThiefBot::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    if(getTicks() != 0)
    {
        decreaseTicks();
        return;
    }
    shootStealMoveOrTurn(true);
}

void MeanThiefBot::takeDamage()
{
    thiefBotAttacked();
    if(!isAlive())
    {
        getWorld()->increaseScore(20);
    }
}

// ThiefBot Factory Functions
void ThiefBotFactory::doSomething()
{
    if(getWorld()->countTheBotsAroundMe(getX(), getY()) >= 3 || getWorld()->isThiefBotOnMe(getX(), getY()))
    {
        return;
    }
    else
    {
        int chance = randInt(1, 50);
        if(chance == 1)
        {
            if(m_mean == true)
            {
                getWorld()->spawnBot(getX(), getY(), true);
                getWorld()->playSound(SOUND_ROBOT_BORN);
            }
            else
            {
                getWorld()->spawnBot(getX(), getY(), false);
                getWorld()->playSound(SOUND_ROBOT_BORN);
            }
        }
    }
}

