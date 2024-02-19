#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_player = nullptr;
}

int StudentWorld::init()
{
    string currLevel = "level0" + to_string(getLevel()) + ".txt";
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(currLevel);
    if(result == Level::load_fail_file_not_found || result == Level:: load_fail_bad_format)
    {
        cerr << "Error Loading" << endl;
        return GWSTATUS_CONTINUE_GAME;
    }
    
    for(int x = 0; x < VIEW_WIDTH; x++)
    {
        for(int y = 0; y < VIEW_HEIGHT; y++)
        {
            Level::MazeEntry item = lev.getContentsOf(x, y);
            if(item == Level::player)
            {
                m_player = new Player(this, IID_PLAYER, x, y);
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

/*
 012345678911234
4###############
3#      @      #
2#    b   b    #
1# #         ###
1# #    b    # #
9# #    b    # #
8# #    b    # #
7# #    b    # #
6#a#    b    # #
5###h   b    # #
4#             #
3#           b #
2######   ######
1#      x    o*#
0###############
 
*/

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q

    setGameStatText("Game will end when you type q");
    
	return GWSTATUS_CONTINUE_GAME;
}



void StudentWorld::cleanUp()
{
    // Creating a vector iterator p
    vector<Actor*>::iterator p;
    // Looping through the vector of actors and deleting each using the iterator
    for(p = m_actors.begin(); p != m_actors.end();)
    {
        delete (*p);
        p = m_actors.erase(p);
    }
    // Deleting the player
    delete m_player;
}
