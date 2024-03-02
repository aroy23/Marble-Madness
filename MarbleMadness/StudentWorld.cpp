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
    if(getLevel() == 100) // If level 100, player wins
    {
        return GWSTATUS_PLAYER_WON;
    }
    startingNewLevel(); // general function to initialize bonus, crystals, etc.
    string currLevel = obtainLevel(); // using stringstreams to obtain level
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(currLevel);
    if(result == Level:: load_fail_file_not_found || result == Level:: load_fail_bad_format)
    {
        return GWSTATUS_LEVEL_ERROR;
    }
    for(int x = 0; x < VIEW_WIDTH; x++) // For each square in the level push back the relevant actor
    {
        for(int y = 0; y < VIEW_HEIGHT; y++)
        {
            Level::MazeEntry item = lev.getContentsOf(x, y);
            if(item == Level::player)
            {
                m_player = new Player(this, IID_PLAYER, x, y, GraphObject::right);
            }
            if(item == Level::wall)
            {
                m_actors.push_back(new Wall(this, IID_WALL, x, y));
            }
            if(item == Level::pit)
            {
                m_actors.push_front(new Pit(this, IID_PIT, x, y));
            }
            if(item == Level::extra_life)
            {
                m_actors.push_front(new ExtraLifeGoodie(this, IID_EXTRA_LIFE, x, y));
            }
            if(item == Level::restore_health)
            {
                m_actors.push_front(new RestoreHealthGoodie(this, IID_RESTORE_HEALTH, x, y));
            }
            if(item == Level::ammo)
            {
                m_actors.push_front(new AmmoGoodie(this, IID_AMMO, x, y));
            }
            if(item == Level::crystal)
            {
                m_crystals++;
                m_actors.push_front(new Crystal(this, IID_CRYSTAL, x, y));
            }
            if(item == Level::horiz_ragebot)
            {
                m_actors.push_back(new RageBot(this, IID_RAGEBOT, x, y, GraphObject::right));
            }
            if(item == Level::vert_ragebot)
            {
                m_actors.push_back(new RageBot(this, IID_RAGEBOT, x, y, GraphObject::down));
            }
            if(item == Level::marble)
            {
                m_actors.push_back(new Marble(this, IID_MARBLE, x, y));
            }
            if(item == Level::exit)
            {
                m_actors.push_back(new Exit(this, IID_EXIT, x, y));
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
    setDisplayText(); // Setting top level display text
    
    list<Actor*>::iterator p;
    for(p = m_actors.begin(); p != m_actors.end(); p++)
    {
        (*p)->doSomething(); // All actors do something, if this kills player, player dies, if level is complete, finish level
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
    m_player->doSomething(); // All player does something, if this kills player, player dies, if level is complete, finish level
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
            delete (*q); // Delete all dead actors
            q = m_actors.erase(q);
        }
        else
        {
            q++;
        }
    }
    
    if(m_bonus > 0) // Decreasing bonus by one each tick
    {
        m_bonus--;
    }
    // After all is done, check once again to see if player dead or level complete
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

void StudentWorld::cleanUp() // Deleting all actors and player
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

void StudentWorld::setDisplayText() // Sets top text
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

string StudentWorld::formatDisplayText(int score, int level, int lives, int health, int ammo, int bonus) // Formats top text
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

string StudentWorld::obtainLevel() // Obtains current level using stringstreams
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

Actor* StudentWorld::isActorHere(int x, int y) // find an actor at x, y, private to StudentWorld
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

Actor* StudentWorld::isActorHereBackwards(int x, int y) // find an actor backwards in the list STL due to the varying ways actors are added to the STL (pushback vs pushfront)
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

bool StudentWorld::canPlayerMoveHere(int x, int y) // Determines if player can move to x, y
{
    if(isActorHere(x, y) == nullptr || (isActorHere(x, y)->canPlayerMoveIn() && !isActorHereBackwards(x, y)->hasHealth() && !isActorHereBackwards(x, y)->allowsMarble())) // If no actor in space or actor allows player movement and no enemy and no pit
    {
        return true;
    }
    return false;
}

bool StudentWorld::canEnemyMoveHere(int x, int y) // Determines if an enemy (robot) can move to x, y
{
    if((isActorHere(x, y) == nullptr && !playerHere(x, y)) || (!playerHere(x, y) && isActorHere(x, y)->canPlayerMoveIn() && !isActorHereBackwards(x, y)->hasHealth() && !isActorHereBackwards(x, y)->allowsMarble())) // If no actor and no player or no player and actor allows player movement and no enemy and no pit
    {
        return true;
    }
    return false;
}

bool StudentWorld::canPushableObjectMoveHere(int x, int y) // Determines if a pushable object, marble can move
{
    if(isActorHere(x, y) == nullptr || isActorHere(x, y)->canPushableObjectMoveIn()) // If no actor or pit present, return true
    {
        return true;
    }
    return false;
}

bool StudentWorld::pushIfPushableHere(int x, int y, int dir) // Pushing if a pushable object if present
{
    if(isActorHere(x, y)->canBePushed()) // If pushable object such as marble present
    {
        if(isActorHere(x, y)->push(dir)) // push if possible
        {
            return true;
        }
    }
    return false;
}

bool StudentWorld::damageActorWithPeaIfHere(int x, int y) // Damage enemy (robot) or player with a be if hit
{
    if((isActorHereBackwards(x, y) != nullptr && !isActorHereBackwards(x, y)->canPeaPass()) || playerHere(x, y)) // If actor present and the pea can't pass
    {
        if(isActorHereBackwards(x, y)->hasHealth()) // Enemy present
        {
            isActorHereBackwards(x, y)->takeDamage();
        }
        else if(playerHere(x, y)) // Player present
        {
            retrievePlayer()->takeDamage();
        }
        return true;
    }
    return false;
}

void StudentWorld::firePea(int x, int y, int dir) // Firing pea
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

Actor* StudentWorld::retrieveObjectSwallower(int x, int y) // Retrieving objects that can swallow (pit)
{
    if(canPushableObjectMoveHere(x, y) && !canPlayerMoveHere(x, y)) // If pushable object can move and player can't move, must be a pit
    {
        return isActorHere(x, y);
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

bool StudentWorld::canEnemyFire(int x, int y, int dir) // Determines if enemy can fire
{
    if(dir == GraphObject::right) // In right direction
    {
        if(y == retrievePlayer()->getY() && x < retrievePlayer()->getX())
        {
            int distanceBetween = retrievePlayer()->getX()-x-1; // Finds distance between player and enemy
            if(distanceBetween == 0) // If player in front of enemy return true
            {
                return true;
            }
            for(int i = 1; i < distanceBetween+1; i++) // If there is an obstruction between player and enemy return false
            {
                if(isActorHere(x+i, y) != nullptr && !isActorHere(x+i, y)->canPeaPass())
                {
                    return false;
                }
            }
            return true;
        }
    }
    else if(dir == GraphObject::left) // In left direction
    {
        if(y == retrievePlayer()->getY() && x > retrievePlayer()->getX())
        {
            int distanceBetween = abs(retrievePlayer()->getX()-x)-1; // Finds distance between player and enemy
            if(distanceBetween == 0) // If player in front of enemy return true
            {
                return true;
            }
            for(int i = 1; i < distanceBetween+1; i++) // If there is an obstruction between player and enemy return false
            {
                if(isActorHere(x-i, y) != nullptr && !isActorHere(x-i, y)->canPeaPass())
                {
                    return false;
                }
            }
            return true;
        }
    }
    else if(dir == GraphObject::up) // In up direction
    {
        if(x == retrievePlayer()->getX() && y < retrievePlayer()->getY())
        {
            int distanceBetween = retrievePlayer()->getY()-y-1; // Finds distance between player and enemy
            if(distanceBetween == 0) // If player in front of enemy return true
            {
                return true;
            }
            for(int i = 1; i < distanceBetween+1; i++) // If there is an obstruction between player and enemy return false
            {
                if(isActorHere(x, y+i) != nullptr && !isActorHere(x, y+i)->canPeaPass())
                {
                    return false;
                }
            }
            return true;
        }
    }
    else if(dir == GraphObject::down) // In down direction
    {
        if(x == retrievePlayer()->getX() && y > retrievePlayer()->getY())
        {
            int distanceBetween = abs(retrievePlayer()->getY()-y)-1; // Finds distance between player and enemy
            if(distanceBetween == 0) // If player in front of enemy return true
            {
                return true;
            }
            for(int i = 1; i < distanceBetween+1; i++) // If there is an obstruction between player and enemy return false
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

Actor* StudentWorld::canThiefSteal(int x, int y, ThiefBot* t)
{
    list<Actor*>::iterator p;
    for(p = m_actors.begin(); p != m_actors.end(); p++)
    {
        if((*p)->canBeStolen() && (*p)->getX() == x && (*p)->getY() == y) // If unstolen goodie has same x and y coords
        {
            if(randInt(1, 10) == 1) // 1 in 10 chance it can be stolen
            {
                stealGoodie((*p)->getX(), (*p)->getY(), (*p), t);
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

void StudentWorld::stealGoodie(int x, int y, Actor* a, ThiefBot* t) // steals goodie
{
    a->setVisible(false);
    a->setCanBeStolenStatus(false); // goodie cant be stolen anymore
    t->setMyGoodie(a); // thiefbot now "has" the goodie
}

void StudentWorld::startingNewLevel()
{
    m_crystals = 0;
    m_bonus = 1000;
    m_levelComplete = false;
}

int StudentWorld::countTheThiefsAroundMe(int factoryX, int factoryY) // Counting thiefbots around factory
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

bool StudentWorld::amIInTheRegion(int myX, int myY, int factoryX, int factoryY) // Checks if coords is within a factory region
{
    if(myX <= factoryX+3 && myX >= factoryX-3 && myY<= factoryY+3 && myY >= factoryY-3) // Bounding box for factory region
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
    if(meanOrNot) // Determines wheter a mean ThiefBot or Regular ThiefBot should be spawned
    {
        m_actors.push_back(new MeanThiefBot(this, IID_MEAN_THIEFBOT, x, y, GraphObject::right));
    }
    else
    {
        m_actors.push_back(new ThiefBot(this, IID_MEAN_THIEFBOT, x, y, GraphObject::right));
    }
}

void StudentWorld::moveActorToAdjacentOpenSpace(int x, int y, Actor* a) // Used mainly for moving goodie to open open space if the current one is occupied by another goodie
{
    int count = countVisibleActorsHereExceptExit(x, y);
    if(count > 2) // Count is greater than two to account for the pea when ThiefBot Dies
    {
        // Moves to right direction if possible, if not then left, up, down in that order
        if(countVisibleActorsHereExceptExit(x+1, y) == 0 || (countVisibleActorsHereExceptExit(x+1, y) == 1 && !playerHere(x+1, y) && isActorHereBackwards(x+1, y)->hasHealth() && !isActorHereBackwards(x+1, y)->canBePushed())) // If right space is open or no exit, no player, no marble, and yes enemy, move to that space
        {
            a->moveTo(x+1, y);
        }
        else if(countVisibleActorsHereExceptExit(x-1, y) == 0 || (countVisibleActorsHereExceptExit(x-1, y) == 1 && !playerHere(x-1, y) && isActorHereBackwards(x-1, y)->hasHealth() && !isActorHereBackwards(x-1, y)->canBePushed())) // If left space is open or no exit, no player, no marble, and yes enemy, move to that space
        {
            a->moveTo(x-1, y);
        }
        else if(countVisibleActorsHereExceptExit(x, y+1) == 0 || (countVisibleActorsHereExceptExit(x, y+1) == 1 && !playerHere(x, y+1) && isActorHereBackwards(x, y+1)->hasHealth() && !isActorHereBackwards(x, y+1)->canBePushed())) // If up space is open or no exit, no player, no marble, and yes enemy, move to that space
        {
            a->moveTo(x, y+1);
        }
        else if(countVisibleActorsHereExceptExit(x, y-1) == 0 || (countVisibleActorsHereExceptExit(x, y-1) == 1 && !playerHere(x, y-1) && isActorHereBackwards(x, y-1)->hasHealth() && !isActorHereBackwards(x, y-1)->canBePushed())) // If down space is open or no exit, no player, no marble, and yes enemy, move to that space
        {
            a->moveTo(x, y-1);
        }
        else
        {
            a->moveTo(x, y);
        }
    }
    else
    {
        a->moveTo(x, y);
    }
}

int StudentWorld::countVisibleActorsHereExceptExit(int x, int y) // Counting the visible actors present at x, y
{
    int count = 0;
    list<Actor*>::iterator p;
    for(p = m_actors.begin(); p != m_actors.end(); p++)
    {
        if((*p)->getX() == x && (*p)->getY() == y)
        {
            if((*p)->stealable() && (*p)->canBeStolen()) // If goodie is still stealable, its visible so count it
            {
                count++;
                continue;
            }
            else if((*p)->stealable() && !(*p)->canBeStolen()) // If its stolen, its invisible so move on to next actor
            {
                continue;
            }
            else
            {
                count++;
            }
        }
    }
    if(retrievePlayer()->getX() == x && retrievePlayer()->getY() == y) { // count the player too if its at x, y
        count++;
    }
    return count;
}
