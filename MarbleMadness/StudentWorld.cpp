#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"
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
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    setGameStatText("Game will end when you type q");
    m_player->doSomething(); // Asking Player to doSomething
    
    vector<Actor*>::iterator p;
    for(p = m_actors.begin(); p != m_actors.end(); p++)
    {
        (*p)->doSomething(); // Asking Actors to doSomething
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
       if((*p)->immovable() && (*p)->getX() == x && (*p)->getY() == y) // checking if actors are immovable
       {
            return true; // return true indicating presence of a barrier
       }
    }
    return false;
}

