#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}


// StudentWorld Constructor and Destructor
StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_player = nullptr; // player starts off as a nullptr before it is created in init function
    playerDeleted = false;
}

StudentWorld::~StudentWorld()
{
    cleanUp();  // Destructor and cleanup do the same thing so the destructor just calls cleanup
}


// StudentWorld Functions
int StudentWorld::init()
{
    
    string currLevel = "level0" + to_string(getLevel()) + ".txt"; // loading file based on current level
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
            if(item == Level::marble)
            {
                m_actors.push_back(new Marble(this, IID_MARBLE, x, y));
                cerr << "A Marble is at x = " << x << " and y = " << y << endl;
            }
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    vector<Actor*>::iterator p;
    for(p = m_actors.begin(); p != m_actors.end(); p++)
    {
        (*p)->doSomething(); // Asking Actors to doSomething
        if(!m_player->isAlive())
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
    }
    m_player->doSomething(); // Asking Player to doSomething
    if(!m_player->isAlive())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    vector<Actor*>::iterator q;
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
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator p; // Creating a vector iterator p
    for(p = m_actors.begin(); p != m_actors.end();) // Looping through the vector of actors
    {
        delete (*p); // deleting each actor using the iterator
        p = m_actors.erase(p);
    }
    if(!playerDeleted)
    {
        playerDeleted = true;
        delete m_player; // Deleting the player
    }
}

bool StudentWorld::isBarrierHere(int x, int y)
{
    vector<Actor*>::iterator p;
    for(p = m_actors.begin(); p != m_actors.end(); p++) // Looping through all actors
    {
        if((*p)->isStationary() && (*p)->getX() == x && (*p)->getY() == y) // checking if actors are stationary
        {
            return true; // return true indicating presence of a barrier
        }
    }
    return false;
}

Actor* StudentWorld::actorHere(int x, int y)
{
    vector<Actor*>::iterator p;
    for(p = m_actors.begin(); p != m_actors.end(); p++) // Looping through all actors
    {
        if((*p)->getX() == x && (*p)->getY() == y) // checking if actor is on grid at x y
        {
            return (*p); // returning actor
        }
    }
    return nullptr;
}

bool StudentWorld::pushIfBarrierMarble(int x, int y, int dir)
{
    Marble* p = dynamic_cast<Marble*>(actorHere(x, y));
    if(p != nullptr)
    {
        p->push(dir);
        return true;
    }
    return false;
}

bool StudentWorld::entityHere(int x, int y)
{
    Actor* p = actorHere(x, y);
    if(p != nullptr)
    {
        Entity* q = dynamic_cast<Entity*>(p);
        if(q != nullptr)
        {
            return true;
        }
    }
    return false;
}

void StudentWorld::firePea(int x, int y, int dir)
{
    if(dir == GraphObject::right)
    {
        m_actors.push_back(new Pea(this, IID_PEA, x+1, y, dir));
    }
    else if(dir == GraphObject::left)
    {
        m_actors.push_back(new Pea(this, IID_PEA, x-1, y, dir));
    }
    else if(dir == GraphObject::up)
    {
        m_actors.push_back(new Pea(this, IID_PEA, x, y+1, dir));
    }
    else if(dir == GraphObject::down)
    {
        m_actors.push_back(new Pea(this, IID_PEA, x, y-1, dir));
    }
}
