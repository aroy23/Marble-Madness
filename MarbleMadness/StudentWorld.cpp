#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    string currLevel = "level00.txt";
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
                cerr << "The Player is at x = " << x << " and y = " << y << endl;
            }
            if(item == Level::wall)
            {
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
}
