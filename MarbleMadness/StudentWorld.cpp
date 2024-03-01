#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// StudentWorld Constructor and Destructor
StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_firstFile = true;
    m_levelComplete = false;
    m_player = nullptr;
    m_bonus = 1000;
    m_crystals = 0;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

// StudentWorld Functions
int StudentWorld::init()
{
    if(getLevel() == 100)
    {
        return GWSTATUS_PLAYER_WON;
    }
    startingNewLevel();
    string currLevel = obtainLevel();
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(currLevel);
    if(result == Level:: load_fail_file_not_found && !m_firstFile)
    {
        return GWSTATUS_PLAYER_WON;
    }
    else if(result == Level:: load_fail_file_not_found && m_firstFile)
    {
        return GWSTATUS_LEVEL_ERROR;
    }
    else if(result == Level:: load_fail_bad_format)
    {
        return GWSTATUS_LEVEL_ERROR;
    }
    m_firstFile = false;
    for(int x = 0; x < VIEW_WIDTH; x++)
    {
        for(int y = 0; y < VIEW_HEIGHT; y++)
        {
            Level::MazeEntry item = lev.getContentsOf(x, y);
            if(item == Level::player)
            {
                m_player = new Player(this, IID_PLAYER, x, y, GraphObject::right);
                cerr << "The Player is at x = " << x << " and y = " << y << endl;
            }
            if(item == Level::wall)
            {
                m_actors.push_back(new Wall(this, IID_WALL, x, y));
                cerr << "A Wall is at x = " << x << " and y = " << y << endl;
            }
            if(item == Level::pit)
            {
                m_actors.push_front(new Pit(this, IID_PIT, x, y));
                cerr << "A Pit is at x = " << x << " and y = " << y << endl;
            }
            if(item == Level::extra_life)
            {
                m_actors.push_front(new ExtraLifeGoodie(this, IID_EXTRA_LIFE, x, y));
                cerr << "An Extra Life Goodie is at x = " << x << " and y = " << y << endl;
            }
            if(item == Level::restore_health)
            {
                m_actors.push_front(new RestoreHealthGoodie(this, IID_RESTORE_HEALTH, x, y));
                cerr << "A Restore Health Goodie is at x = " << x << " and y = " << y << endl;
            }
            if(item == Level::ammo)
            {
                m_actors.push_front(new AmmoGoodie(this, IID_AMMO, x, y));
                cerr << "An Ammo Goodie is at x = " << x << " and y = " << y << endl;
            }
            if(item == Level::crystal)
            {
                m_crystals++;
                m_actors.push_front(new Crystal(this, IID_CRYSTAL, x, y));
                cerr << "A Crystal is at x = " << x << " and y = " << y << endl;
            }
            if(item == Level::horiz_ragebot)
            {
                m_actors.push_back(new RageBot(this, IID_RAGEBOT, x, y, GraphObject::right));
                cerr << "An Horizontal RageBot is at x = " << x << " and y = " << y << endl;
            }
            if(item == Level::vert_ragebot)
            {
                m_actors.push_back(new RageBot(this, IID_RAGEBOT, x, y, GraphObject::down));
                cerr << "A Vertical RageBot is at x = " << x << " and y = " << y << endl;
            }
            if(item == Level::marble)
            {
                m_actors.push_back(new Marble(this, IID_MARBLE, x, y));
                cerr << "A Marble is at x = " << x << " and y = " << y << endl;
            }
            if(item == Level::exit)
            {
                m_actors.push_back(new Exit(this, IID_EXIT, x, y));
                cerr << "An Exit is at x = " << x << " and y = " << y << endl;
            }
            if(item == Level::mean_thiefbot_factory)
            {
                m_actors.push_back(new ThiefBotFactory(this, IID_ROBOT_FACTORY, x, y, true));
            }
            if(item == Level::thiefbot_factory)
            {
                m_actors.push_back(new ThiefBotFactory(this, IID_ROBOT_FACTORY, x, y, false));
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    setDisplayText();
    
    list<Actor*>::iterator p;
    for(p = m_actors.begin(); p != m_actors.end(); p++)
    {
        (*p)->doSomething();
        if(!m_player->isAlive())
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        if(m_levelComplete)
        {
            increaseScore(m_bonus);
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    m_player->doSomething();
    if(!m_player->isAlive())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    if(m_levelComplete)
    {
        increaseScore(m_bonus);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    list<Actor*>::iterator q;
    for(q = m_actors.begin(); q != m_actors.end();)
    {
        if(!(*q)->isAlive())
        {
            delete (*q);
            q = m_actors.erase(q);
        }
        else
        {
            q++;
        }
    }
    
    if(m_bonus > 0)
    {
        m_bonus--;
    }
    
    if(!m_player->isAlive())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    if(m_levelComplete)
    {
        increaseScore(m_bonus);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    list<Actor*>::iterator p;
    for(p = m_actors.begin(); p != m_actors.end();)
    {
        delete (*p);
        p = m_actors.erase(p);
    }
    delete m_player;
    m_player = nullptr;
}

void StudentWorld::setDisplayText()
{
    int score = getScore();
    int level = getLevel();
    unsigned int bonus = m_bonus;
    int livesLeft = getLives();
    int health = m_player->getHealth();
    int ammo = m_player->getPeas();
    
    string s = formatDisplayText(score, level, livesLeft, health, ammo, bonus);
    setGameStatText(s);
}

string StudentWorld::formatDisplayText(int score, int level, int lives, int health, int ammo, int bonus)
{
    ostringstream oss;
    oss << "Score: ";
    oss.fill('0');
    oss << setw(7) << score << "  Level: ";
    oss << setw(2) << level << "  Lives: ";
    oss.fill(' ');
    oss << setw(2) << lives << "  Health: ";
    oss << setw(3) << health*5 << "%  Ammo: ";
    oss << setw(3) << ammo << "  Bonus: ";
    oss << setw(4) << bonus;
    
    string s = oss.str();
    return s;
}

string StudentWorld::obtainLevel()
{
    ostringstream oss;
    oss << "level";
    oss.fill('0');
    oss << setw(2) << getLevel() << ".txt";
    
    string s = oss.str();
    return s;
}

void StudentWorld::crystalObtained()
{
    m_crystals--;
}

int StudentWorld::getCrystals()
{
    return m_crystals;
}

void StudentWorld::levelFinished()
{
    m_levelComplete = true;
    increaseScore(2000);
    m_crystals = 0;
}

Actor* StudentWorld::isActorHere(int x, int y)
{
    list<Actor*>::iterator p;
    for(p = m_actors.begin(); p != m_actors.end(); p++)
    {
        if((*p)->getX() == x && (*p)->getY() == y)
        {
            return (*p);
        }
    }
    return nullptr;
}

Actor* StudentWorld::isActorHereBackwards(int x, int y)
{
    list<Actor*>::reverse_iterator p;
    for(p = m_actors.rbegin(); p != m_actors.rend(); p++)
    {
        if((*p)->getX() == x && (*p)->getY() == y)
        {
            return (*p);
        }
    }
    return nullptr;
}

bool StudentWorld::canPlayerMoveHere(int x, int y)
{
    if(isActorHere(x, y) == nullptr || (isActorHere(x, y)->canPlayerMoveIn() && !isActorHereBackwards(x, y)->hasHealth() && !isActorHereBackwards(x, y)->allowsMarble()))
    {
        return true;
    }
    return false;
}

bool StudentWorld::canEnemyMoveHere(int x, int y)
{
    if((isActorHere(x, y) == nullptr && !playerHere(x, y)) || (!playerHere(x, y) && isActorHere(x, y)->canPlayerMoveIn() && !isActorHereBackwards(x, y)->hasHealth() && !isActorHereBackwards(x, y)->allowsMarble()))
    {
        return true;
    }
    return false;
}

bool StudentWorld::canPushableObjectMoveHere(int x, int y)
{
    if(isActorHere(x, y) == nullptr || isActorHere(x, y)->canPushableObjectMoveIn())
    {
        return true;
    }
    return false;
}

bool StudentWorld::pushIfBarrierMarbleHere(int x, int y, int dir)
{
    if(isActorHere(x, y)->canBePushed())
    {
        if(isActorHere(x, y)->push(dir))
        {
            return true;
        }
    }
    return false;
}

bool StudentWorld::damageActorWithPeaIfHere(int x, int y)
{
    if((isActorHereBackwards(x, y) != nullptr && !isActorHereBackwards(x, y)->canPeaPass()) || playerHere(x, y))
    {
        if(isActorHereBackwards(x, y)->hasHealth())
        {
            isActorHereBackwards(x, y)->takeDamage();
        }
        else if(playerHere(x, y))
        {
            retrievePlayer()->takeDamage();
        }
        return true;
    }
    return false;
}

void StudentWorld::firePea(int x, int y, int dir)
{
    if(dir == GraphObject::right)
    {
        m_actors.push_front(new Pea(this, IID_PEA, x+1, y, dir));
    }
    else if(dir == GraphObject::left)
    {
        m_actors.push_front(new Pea(this, IID_PEA, x-1, y, dir));
    }
    else if(dir == GraphObject::up)
    {
        m_actors.push_front(new Pea(this, IID_PEA, x, y+1, dir));
    }
    else if(dir == GraphObject::down)
    {
        m_actors.push_front(new Pea(this, IID_PEA, x, y-1, dir));
    }
}

Pit* StudentWorld::retrieveKnownPit(int x, int y)
{
    if(canPushableObjectMoveHere(x, y) && !canPlayerMoveHere(x, y))
    {
        // When this function is called, the caller knows for sure that the actor on this square is a pit
        // This is due to the if statement which specifies that a marble can move in but nothing else can
        // DOES NOT violate the spec because dynamic cast is being used after the objects have already been identified
        // e.g. NOT using dynamic cast to check for common types of objects which the advises against
        Pit* p = dynamic_cast<Pit*>(isActorHere(x, y));
        return p;
    }
    return nullptr;
}

bool StudentWorld::playerHere(int x, int y)
{
    if(m_player->getX() == x && m_player->getY() == y)
    {
        return true;
    }
    return false;
}

Player* StudentWorld::retrievePlayer()
{
    return m_player;
}

bool StudentWorld::canBotFire(int x, int y, int dir)
{
    if(dir == GraphObject::right)
    {
        if(y == retrievePlayer()->getY() && x < retrievePlayer()->getX())
        {
            int distanceBetween = retrievePlayer()->getX()-x-1;
            if(distanceBetween == 0)
            {
                return true;
            }
            for(int i = 1; i < distanceBetween+1; i++)
            {
                if(isActorHere(x+i, y) != nullptr && !isActorHere(x+i, y)->canPeaPass())
                {
                    return false;
                }
            }
            return true;
        }
    }
    else if(dir == GraphObject::left)
    {
        if(y == retrievePlayer()->getY() && x > retrievePlayer()->getX())
        {
            int distanceBetween = abs(retrievePlayer()->getX()-x)-1;
            if(distanceBetween == 0)
            {
                return true;
            }
            for(int i = 1; i < distanceBetween+1; i++)
            {
                if(isActorHere(x-i, y) != nullptr && !isActorHere(x-i, y)->canPeaPass())
                {
                    return false;
                }
            }
            return true;
        }
    }
    else if(dir == GraphObject::up)
    {
        if(x == retrievePlayer()->getX() && y < retrievePlayer()->getY())
        {
            int distanceBetween = retrievePlayer()->getY()-y-1;
            if(distanceBetween == 0)
            {
                return true;
            }
            for(int i = 1; i < distanceBetween+1; i++)
            {
                if(isActorHere(x, y+i) != nullptr && !isActorHere(x, y+i)->canPeaPass())
                {
                    return false;
                }
            }
            return true;
        }
    }
    else if(dir == GraphObject::down)
    {
        if(x == retrievePlayer()->getX() && y > retrievePlayer()->getY())
        {
            int distanceBetween = abs(retrievePlayer()->getY()-y)-1;
            if(distanceBetween == 0)
            {
                return true;
            }
            for(int i = 1; i < distanceBetween+1; i++)
            {
                if(isActorHere(x, y-i) != nullptr && !isActorHere(x, y-i)->canPeaPass())
                {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

Actor* StudentWorld::canBotSteal(int x, int y)
{
    list<Actor*>::iterator p;
    for(p = m_actors.begin(); p != m_actors.end(); p++)
    {
        if((*p)->canBeStolen() && (*p)->getX() == x && (*p)->getY() == y)
        {
            if(randInt(1, 10) == 1)
            {
                stealGoodie((*p)->getX(), (*p)->getY(), (*p));
                return (*p);
            }
            else
            {
                return nullptr;
            }
        }
    }
    return nullptr;
}

void StudentWorld::stealGoodie(int x, int y, Actor* a)
{
    // When this function is called, the caller knows for sure that the two actors on this square
    // are a ThiefBot and a Goodie because this function is only called within another function in StudentWorld
    // so using dynamic cast DOES NOT violate the spec because dynamic cast is being used
    // after the objects have already been identified
    // e.g. NOT using dynamic cast to check for common types of objects which the spec advises against
    a->setVisible(false);
    a->setCanBeStolenStatus(false);
    ThiefBot* p = dynamic_cast<ThiefBot*>(isActorHereBackwards(x, y));
    p->setMyGoodie(g);
}

void StudentWorld::startingNewLevel()
{
    m_crystals = 0;
    m_bonus = 1000;
    m_levelComplete = false;
}

int StudentWorld::countTheThiefsAroundMe(int factoryX, int factoryY)
{
    int count = 0;
    list<Actor*>::reverse_iterator p;
    for(p = m_actors.rbegin(); p != m_actors.rend(); p++)
    {
        if((*p)->canSteal() && amIInTheRegion((*p)->getX(), (*p)->getY(), factoryX, factoryY))
        {
            count++;
        }
    }
    return count;
}

bool StudentWorld::amIInTheRegion(int myX, int myY, int factoryX, int factoryY)
{
    if(myX <= factoryX+3 && myX >= factoryX-3 && myY<= factoryY+3 && myY >= factoryY-3)
    {
        return true;
    }
    return false;
}

bool StudentWorld::isThiefOnMe(int x, int y)
{
    if(isActorHereBackwards(x, y)->canSteal())
    {
        return true;
    }
    return false;
}

void StudentWorld::spawnThief(int x, int y, bool meanOrNot)
{
    if(meanOrNot)
    {
        m_actors.push_back(new MeanThiefBot(this, IID_MEAN_THIEFBOT, x, y, GraphObject::right));
    }
    else
    {
        m_actors.push_back(new ThiefBot(this, IID_MEAN_THIEFBOT, x, y, GraphObject::right));
    }
}

void StudentWorld::moveGoodieToAdjacentOpenSpace(int x, int y, Goodie* g)
{
    int count = countVisibleActorsHereExceptExit(x, y);
    if(count > 2)
    {
        if(countVisibleActorsHereExceptExit(x+1, y) == 0 || (countVisibleActorsHereExceptExit(x+1, y) == 1 && isActorHereBackwards(x+1, y)->hasHealth() && !isActorHereBackwards(x+1, y)->canBePushed()))
        {
            g->moveTo(x+1, y);
        }
        else if(countVisibleActorsHereExceptExit(x-1, y) == 0 || (countVisibleActorsHereExceptExit(x-1, y) == 1 && isActorHereBackwards(x-1, y)->hasHealth() && !isActorHereBackwards(x-1, y)->canBePushed()))
        {
            g->moveTo(x-1, y);
        }
        else if(countVisibleActorsHereExceptExit(x, y+1) == 0 || (countVisibleActorsHereExceptExit(x, y+1) == 1 && isActorHereBackwards(x, y+1)->hasHealth() && !isActorHereBackwards(x, y+1)->canBePushed()))
        {
            g->moveTo(x, y+1);
        }
        else if(countVisibleActorsHereExceptExit(x, y-1) == 0 || (countVisibleActorsHereExceptExit(x, y-1) == 1 && isActorHereBackwards(x, y-1)->hasHealth() && !isActorHereBackwards(x, y-1)->canBePushed()))
        {
            g->moveTo(x, y-1);
        }
        else
        {
            g->moveTo(x, y);
        }
    }
    else
    {
        g->moveTo(x, y);
    }
}

int StudentWorld::countVisibleActorsHereExceptExit(int x, int y)
{
    int count = 0;
    list<Actor*>::iterator p;
    for(p = m_actors.begin(); p != m_actors.end(); p++)
    {
        if((*p)->getX() == x && (*p)->getY() == y)
        {
            if((*p)->stealable() && (*p)->canBeStolen())
            {
                count++;
                continue;
            }
            else if((*p)->stealable() && !(*p)->canBeStolen())
            {
                continue;
            }
            else
            {
                count++;
            }
        }
    }
    return count;
}
