#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
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
    m_levelComplete = false;
    m_player = nullptr;
    m_bonus = 1000;
    m_crystals = 0;
}

StudentWorld::~StudentWorld()
{
    cleanUp();  // Destructor and cleanup do the same thing so the destructor just calls cleanup
}

// StudentWorld Functions
int StudentWorld::init()
{
    m_levelComplete = false;
    string currLevel = "level0" + to_string(getLevel()) + ".txt"; // FIX THIS USING OSS
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(currLevel);
    if(result == Level::load_fail_file_not_found || result == Level:: load_fail_bad_format)
    {
        return -1; // return -1 if level load failed
    }
    for(int x = 0; x < VIEW_WIDTH; x++) // Creating actors based on the level by looping through the file
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
            if(item == Level::crystal)
            {
                m_crystals++;
                m_actors.push_front(new Crystal(this, IID_CRYSTAL, x, y));
                cerr << "A Crystal is at x = " << x << " and y = " << y << endl;
            }
            if(item == Level::extra_life)
            {
                m_actors.push_back(new ExtraLifeGoodie(this, IID_EXTRA_LIFE, x, y));
                cerr << "An Extra Life Goodie is at x = " << x << " and y = " << y << endl;
            }
            if(item == Level::restore_health)
            {
                m_actors.push_back(new RestoreHealthGoodie(this, IID_RESTORE_HEALTH, x, y));
                cerr << "A Restore Health Goodie is at x = " << x << " and y = " << y << endl;
            }
            if(item == Level::ammo)
            {
                m_actors.push_back(new AmmoGoodie(this, IID_AMMO, x, y));
                cerr << "An Ammo Goodie is at x = " << x << " and y = " << y << endl;
            }
            if(item == Level::horiz_ragebot)
            {
                m_actors.push_back(new RageBot(this, IID_RAGEBOT, x, y, GraphObject::right));
                cerr << "An Horizontal RageBot is at x = " << x << " and y = " << y << endl;
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
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
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
    
    setDisplayText();
    
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
    m_bonus = 1000;
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

bool StudentWorld::canNonMarbleEntityMoveHere(int x, int y)
{
    if(isActorHere(x, y) == nullptr || isActorHere(x, y)->canNonMarbleEntityMoveIn())
    {
        return true;
    }
    return false;
}

bool StudentWorld::canMarbleEntityMoveHere(int x, int y)
{
    if(isActorHere(x, y) == nullptr || isActorHere(x, y)->canMarbleMoveIn())
    {
        return true;
    }
    return false;
}

bool StudentWorld::pushIfBarrierMarbleHere(int x, int y, int dir)
{
    Marble* p = dynamic_cast<Marble*>(isActorHere(x, y));
    if(p != nullptr)
    {
        if(p->push(dir))
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
    if(canMarbleEntityMoveHere(x, y) && !canNonMarbleEntityMoveHere(x, y))
    {
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
        if(y == retrievePlayer()->getX() && y < retrievePlayer()->getY())
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
        if(y == retrievePlayer()->getX() && y > retrievePlayer()->getY())
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
