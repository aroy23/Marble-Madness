#include "Actor.h"
#include "StudentWorld.h"

Actor::~Actor()
{}

Actor::Actor(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: GraphObject(imageID, initX, initY, dir) {
    m_itemWithMe = nullptr; // Actors don't start with an item on them
    m_canBeStolen = false;
    setVisible(true);
    m_studentWorld = sw; // Setting StudentWorld appropriately
    m_dead = false;
}

EntityBarrier::EntityBarrier(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: Actor(sw, imageID, initX, initY, dir)
{}

PushableBarrier::PushableBarrier(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: Actor(sw, imageID, initX, initY, dir)
{}

Entity::Entity(StudentWorld* sw, int imageID, int initX, int initY, int hp, int dir)
: Actor(sw, imageID, initX, initY, dir) {
    m_health = hp;
}

Player::Player(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: Entity(sw, imageID, initX, initY, 20, dir) {
    m_peas = 20; // Player must start with 20 peas
}

Marble::Marble(StudentWorld* sw, int imageID, int initX, int initY)
: Entity(sw, imageID, initX, initY, 10) // Marble has 10 health
{}

Wall::Wall(StudentWorld* sw, int imageID, int initX, int initY)
: EntityBarrier(sw, imageID, initX, initY)
{}

Pea::Pea(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: Actor(sw, imageID, initX, initY, dir)
{}

Pit::Pit(StudentWorld* sw, int imageID, int initX, int initY)
: EntityBarrier(sw, imageID, initX, initY)
{}

Crystal::Crystal(StudentWorld* sw, int imageID, int initX, int initY)
: PushableBarrier(sw, imageID, initX, initY)
{}

Exit::Exit(StudentWorld* sw, int imageID, int initX, int initY)
: PushableBarrier(sw, imageID, initX, initY) {
    m_revealed = false;
    setVisible(false); // Exit should not be visible
}

Goodie::Goodie(StudentWorld* sw, int imageID, int initX, int initY)
: PushableBarrier(sw, imageID, initX, initY)
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
: Entity(sw, imageID, initX, initY, hp, dir) {
    int ticksUntilMove = (28 - getWorld()->getLevel()) / 4;
    if (ticksUntilMove < 3)
    {
        ticksUntilMove = 3;
    }
    m_ticksUntilICanMove = ticksUntilMove-1; // Assigning appropriate ticks per level
    m_ticks = ticksUntilMove-1; // Assigning appropriate ticks per level
}

RageBot::RageBot(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: Robot(sw, imageID, initX, initY, 10, dir) // RageBot has 10 health
{}

ThiefBot::ThiefBot(StudentWorld* sw, int imageID, int initX, int initY, int dir, int hp)
: Robot(sw, imageID, initX, initY, hp, dir) { // ThiefBot has 5 health (default parameter in header file)
    m_haveGoodie = false; // ThiefBots don't start with goodie
    m_squaresMoved = 0; // Keeps track of squares moved
    m_distanceBeforeTurning = randInt(1, 6); // Randomly selecting distance before turning
    m_myGoodie = nullptr; // No goodie to start
}

MeanThiefBot::MeanThiefBot(StudentWorld* sw, int imageID, int initX, int initY, int dir)
: ThiefBot(sw, imageID, initX, initY, dir, 8) // Mean ThiefBot has 8 health
{}

ThiefBotFactory::ThiefBotFactory(StudentWorld* sw, int imageID, int initX, int initY, bool meanOrNot)
: EntityBarrier(sw, imageID, initX, initY) {
    m_mean = meanOrNot; // Variable so thiefbot factory can spawn correct enemy
}

// Actor Functions
void Actor::setCanBeStolenStatus(bool state) {
    m_canBeStolen = state; // States an actor to a stolen state
}

// Entity Functions
void Entity::takeDamage() {
    getHit(); // General takeDamage method should result in entity losing two health
    if(m_health <= 0) {
        setDead();
    }
}

void Entity::setHealth(int amt) {
    m_health = amt; // Used to restore health
}

void Entity::getHit() {
    m_health -= 2;
}

Actor* Actor::getItemWithMe() const {
    return m_itemWithMe; // Used to get the item on the actor (e.g. marble)
}

void Actor::setItemWithMe(Actor* m) {
    m_itemWithMe = m; // Used to set the item on the actor (e.g. marble)
}

// Player Functions
void Player::doSomething() {
    int key;
    if (getWorld()->getKey(key)) {
        if (key == KEY_PRESS_RIGHT) { // If dir is right and the player can move there, move, else if marble there preventing movement, push if possible and move
            setDirection(right);
            
            if((getWorld()->retrieveObjectSwallower(getX()+1, getY()) != nullptr && getWorld()->retrieveObjectSwallower(getX()+1, getY())->getItemWithMe() != nullptr) || getWorld()->canPlayerMoveHere(getX()+1, getY())) {
                moveTo(getX()+1, getY());
            }
            else if(getWorld()->pushIfPushableHere(getX()+1, getY(), right)) {
                moveTo(getX()+1, getY());
            }
        }
        if(key == KEY_PRESS_LEFT) { // If dir is left and the player can move there, move, else if marble there preventing movement, push if possible and move
            setDirection(left);
            if((getWorld()->retrieveObjectSwallower(getX()-1, getY()) != nullptr && getWorld()->retrieveObjectSwallower(getX()-1, getY())->getItemWithMe() != nullptr) || getWorld()->canPlayerMoveHere(getX()-1, getY())) {
                moveTo(getX()-1, getY());
            }
            else if(getWorld()->pushIfPushableHere(getX()-1, getY(), left)) {
                moveTo(getX()-1, getY());
            }
        }
        if(key == KEY_PRESS_UP) { // If dir is up and the player can move there, move, else if marble there preventing movement, push if possible and move
            setDirection(up);
            if((getWorld()->retrieveObjectSwallower(getX(), getY()+1) != nullptr && getWorld()->retrieveObjectSwallower(getX(), getY()+1)->getItemWithMe() != nullptr) || getWorld()->canPlayerMoveHere(getX(), getY()+1)) {
                moveTo(getX(), getY()+1);
            }
            else if(getWorld()->pushIfPushableHere(getX(), getY()+1, up)) {
                moveTo(getX(), getY()+1);
            }
        }
        if(key == KEY_PRESS_DOWN) { // If dir is down and the player can move there, move, else if marble there preventing movement, push if possible and move
            setDirection(down);
            if((getWorld()->retrieveObjectSwallower(getX(), getY()-1) != nullptr && getWorld()->retrieveObjectSwallower(getX(), getY()-1)->getItemWithMe() != nullptr) || getWorld()->canPlayerMoveHere(getX(), getY()-1)) {
                moveTo(getX(), getY()-1);
            }
            else if(getWorld()->pushIfPushableHere(getX(), getY()-1, down)) {
                moveTo(getX(), getY()-1);
            }
        }
        if(key == KEY_PRESS_SPACE) { // If space is pressed, fire if ammo present
            if(m_peas > 0) {
                m_peas--;
                fire();
            }
        }
        if(key == KEY_PRESS_ESCAPE) { // Die if esc pressed
            setDead();
            getWorld()->playSound(SOUND_PLAYER_DIE);
        }
    }
}

void Player::takeDamage() {
    getHit();
    if(getHealth() <= 0) {
        getWorld()->playSound(SOUND_PLAYER_DIE);
        setDead();
    }
    else {
        getWorld()->playSound(SOUND_PLAYER_IMPACT);
    }
}

void Player::fire() {
    getWorld()->playSound(SOUND_PLAYER_FIRE);
    getWorld()->firePea(getX(), getY(), getDirection());
}

void Player::incPeas() {
    m_peas += 20;
}

// Wall Functions
void Wall::doSomething() {
    return;
}

// Marble Functions
void Marble::doSomething() {
    return;
}

bool Marble::push(int dir) { // Determines if a marble can be pushed in the current dir
    if(dir == right && getWorld()->canPushableObjectMoveHere(getX()+1, getY())) {
        if(getWorld()->retrieveObjectSwallower(getX()+1, getY()) != nullptr) {
            getWorld()->retrieveObjectSwallower(getX()+1, getY())->setItemWithMe(this);
        } // If pushable object can move in current dir, move. It pit is here then swallow marble
        moveTo(getX()+1, getY());
        return true;
    }
    if(dir == left && getWorld()->canPushableObjectMoveHere(getX()-1, getY())) {
        if(getWorld()->retrieveObjectSwallower(getX()-1, getY()) != nullptr) {
            getWorld()->retrieveObjectSwallower(getX()-1, getY())->setItemWithMe(this);
        } // If pushable object can move in current dir, move. It pit is here then swallow marble
        moveTo(getX()-1, getY());
        return true;
    }
    if(dir == up && getWorld()->canPushableObjectMoveHere(getX(), getY()+1)) {
        if(getWorld()->retrieveObjectSwallower(getX(), getY()+1) != nullptr) {
            getWorld()->retrieveObjectSwallower(getX(), getY()+1)->setItemWithMe(this);
        } // If pushable object can move in current dir, move. It pit is here then swallow marble
        moveTo(getX(), getY()+1);
        return true;
    }
    if(dir == down && getWorld()->canPushableObjectMoveHere(getX(), getY()-1)) {
        if(getWorld()->retrieveObjectSwallower(getX(), getY()-1) != nullptr) {
            getWorld()->retrieveObjectSwallower(getX(), getY()-1)->setItemWithMe(this);
        } // If pushable object can move in current dir, move. It pit is here then swallow marble
        moveTo(getX(), getY()-1);
        return true;
    }
    return false; // If marble can't be pushed return false
}

// Pea Functions
void Pea::doSomething() {
    if(!isAlive()) {
        return;
    }
    if(getWorld()->damageActorWithPeaIfHere(getX(), getY())) {
        setDead(); // if actor takes damage from pea, pea dies
    }
    else { // Moving one step forward in direction fired
        int dir = getDirection();
        if(dir == right) {
            moveTo(getX()+1, getY());
        }
        else if(dir == left) {
            moveTo(getX()-1, getY());
        }
        else if(dir == up) {
            moveTo(getX(), getY()+1);
        }
        else if(dir == down) {
            moveTo(getX(), getY()-1);
        }
        if(getWorld()->damageActorWithPeaIfHere(getX(), getY())) {
            setDead(); // if actor takes damage from pea, pea dies
        }
    }
    
}

// Pit Functions
void Pit::doSomething() {
    if(!isAlive()) {
        return;
    }
    if(getItemWithMe() != nullptr) { // If item is on the pit (marble) pit dies and so does item on it
        getItemWithMe()->setDead();
        setDead();
    }
}

// Crystal Functions
void Crystal::doSomething() {
    if(!isAlive()) {
        return;
    }
    if(getWorld()->playerHere(getX(), getY())) {
        getWorld()->crystalObtained(); // decreases crystal count in the world
        getWorld()->increaseScore(50);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
}

// Exit Functions
void Exit::doSomething() {
    if(!m_revealed && getWorld()->getCrystals() == 0) { // If not revealed yet and no crystals left
        m_revealed = true; // makes sure reveal sound only played once
        setVisible(true);
        getWorld()->playSound(SOUND_REVEAL_EXIT);
    }
    else if(m_revealed && getWorld()->playerHere(getX(), getY())) { // If player on the exit finish level
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->levelFinished();
    }
}

// Goodie Functions
bool Goodie::gotThisGoodie(Goodie* g, int goodieX, int goodieY, int score) { // General function for if goodie stolen
    if(getWorld()->playerHere(goodieX, goodieY) && canBeStolen()) {
        g->getWorld()->increaseScore(score);
        g->setDead();
        g->getWorld()->playSound(SOUND_GOT_GOODIE);
        return true;
    }
    return false;
}

// ExtraLifeGoodie Functions
void ExtraLifeGoodie::doSomething() {
    if(!isAlive()) {
        return;
    }
    if(gotThisGoodie(this, getX(), getY(), 1000)) {
        getWorld()->incLives(); // Gives one more life to player
    }
}

// RestoreHealthGoodie Functions
void RestoreHealthGoodie::doSomething() {
    if(!isAlive()) {
        return;
    }
    if(gotThisGoodie(this, getX(), getY(), 500)) {
        getWorld()->retrievePlayer()->setHealth(20); // Sets the player's health back to 20
    }
}

// AmmoGoodie Functions
void AmmoGoodie::doSomething() {
    if(!isAlive()) {
        return;
    }
    if(gotThisGoodie(this, getX(), getY(), 100)) {
        getWorld()->retrievePlayer()->incPeas(); // gives 20 more peas to player
    }
}

// Robot Functions
void Robot::decreaseTicks() {
    m_ticks--; // decreasing tick by one each time
}
void Robot::equalizeTicks() {
    m_ticks = m_ticksUntilICanMove; // equalizing ticks so next action will be taken at correct time
}

// RageBot Functions
void RageBot::doSomething() {
    if(!isAlive()) {
        return;
    }
    if(getTicks() != 0) { // Robot can only act when ticks equal 0
        decreaseTicks();
        return;
    }
    else if(getWorld()->canEnemyFire(getX(), getY(), getDirection())) { // reset ticks, fire if possible
        equalizeTicks();
        getWorld()->playSound(SOUND_ENEMY_FIRE);
        getWorld()->firePea(getX(), getY(), getDirection());
        return;
    }
    else { // if firing not possible move if possible
        equalizeTicks();
        if(getDirection() == right) { // Moving right
            if(getWorld()->canEnemyMoveHere(getX()+1, getY())) {
                moveTo(getX()+1, getY());
            }
            else { // If obstruction in the way, set direction to opposite way (left)
                setDirection(left);
            }
        }
        else if(getDirection() == left) { // Moving left
            if(getWorld()->canEnemyMoveHere(getX()-1, getY())) {
                moveTo(getX()-1, getY());
            }
            else {
                setDirection(right); // If obstruction in the way, set direction to opposite way (right)
            }
        }
        else if(getDirection() == up) { // Moving up
            if(getWorld()->canEnemyMoveHere(getX(), getY()+1)) {
                moveTo(getX(), getY()+1);
            }
            else {
                setDirection(down); // If obstruction in the way, set direction to opposite way (down)
            }
        }
        else if(getDirection() == down) { // Moving down
            if(getWorld()->canEnemyMoveHere(getX(), getY()-1)) {
                moveTo(getX(), getY()-1);
            }
            else {
                setDirection(up); // If obstruction in the way, set direction to opposite way (up)
            }
        }
    }
}

void RageBot::takeDamage() {
    getHit();
    if(getHealth() <= 0) {
        getWorld()->playSound(SOUND_ROBOT_DIE);
        setDead();
        getWorld()->increaseScore(100);
    }
    else {
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    }
}

// ThiefBot Functions
void ThiefBot::doSomething() {
    if(!isAlive()) {
        return;
    }
    if(getTicks() != 0) { // Robot can only act when ticks equal 0
        decreaseTicks();
        return;
    }
    shootStealMoveOrTurn(false); // General function responsible for all thiefbot actions, used to prevent repetitive code between regular ThiefBots and Mean ThiefBots
}

void ThiefBot::thiefBotAttacked() { // General function for when ThiefBot's of all variety get attacked used to preven repetitive code
    getHit();
    if(getHealth() <= 0) {
        if(returnGoodieStatus()) { // If ThiefBot has a good
            getWorld()->moveActorToAdjacentOpenSpace(getX(), getY(), returnMyGoodie()); // Move the goodie to an open adjacent space if it exists if current space is occupied
            returnMyGoodie()->setVisible(true); // Make goodie visible
            returnMyGoodie()->setCanBeStolenStatus(true); // Make goodie be able to be stolen again
        }
        getWorld()->playSound(SOUND_ROBOT_DIE);
        setDead();
    }
    else {
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    }
}

void ThiefBot::takeDamage() {
    thiefBotAttacked(); // calling general thiefbot attacked functions
    if(!isAlive()) {
        getWorld()->increaseScore(10);
    }
}

void ThiefBot::setMyGoodie(Actor* a) { // Setting a ThiefBot's goodie
    m_myGoodie = a;
}

Actor* ThiefBot::returnMyGoodie() const { // Returning a ThiefBot's goodie
    return m_myGoodie;
}

void ThiefBot::resetSquaresMoved() { // method to reset a ThiefBot's squares moved if it turns
    m_squaresMoved = 0;
}

void ThiefBot::increaseSquaresMoved() {
    m_squaresMoved++;
}

void ThiefBot::setDistanceBeforeTurning(int d) { // Distance a ThiefBot can move before turning
    m_distanceBeforeTurning = d;
}

void ThiefBot::directionTuning(int dir, int x, int y, bool vert) // General function used to prevent repetitive code for ThiefBot turning
{
    if(vert) { // current direction is Horizontal consider the vertical directions for turning if possible, move
        if(getWorld()->canEnemyMoveHere(x, y+1)) {
            setDirection(up);
            moveTo(x, y+1);
            increaseSquaresMoved();
        }
        else if(getWorld()->canEnemyMoveHere(x, y-1)) {
            setDirection(down);
            moveTo(x, y-1);
            increaseSquaresMoved();
        }
        else {
            setDirection(dir);
        }
    }
    else { // current direction is Vertical, consider the Horizontal directions for turning if possible, move
        if(getWorld()->canEnemyMoveHere(x+1, y)) {
            setDirection(right);
            moveTo(x+1, y);
            increaseSquaresMoved();
        }
        else if(getWorld()->canEnemyMoveHere(x-1, y)) {
            setDirection(left);
            moveTo(x-1, y);
            increaseSquaresMoved();
        }
        else {
            setDirection(dir);
        }
    }
}

bool ThiefBot::moveIfAvailableRoom(int dir, int x, int y) // General function used to prevent repetitive code for ThiefBot movement
{
    int newX = 0, newY = 0;
    if(dir == right) {
        newX = x+1; newY = y;
    }
    else if(dir == left) {
        newX = x-1; newY = y;
    }
    else if(dir == up) {
        newX = x; newY = y+1;
    }
    else if(dir == down) {
        newX = x; newY = y-1;
    }
    if(getWorld()->canEnemyMoveHere(newX, newY)) { // If ThiefBot can move in current dir, return true and move
        moveTo(newX, newY);
        increaseSquaresMoved();
        return true;
    }
    return false; // else return false
}

void ThiefBot::shootStealMoveOrTurn(bool mean)
{
    if(mean && getWorld()->canEnemyFire(getX(), getY(), getDirection())) // If the thiefbot is mean, fire if possible
    {
        equalizeTicks();
        getWorld()->playSound(SOUND_ENEMY_FIRE);
        getWorld()->firePea(getX(), getY(), getDirection());
        return;
    }
    else if(!returnGoodieStatus() && (getWorld()->canThiefSteal(getX(), getY(), this) != nullptr)) // Else if ThiefBot can steal, steal
    {
        equalizeTicks();
        gotGoodie(); // Setting ThiefBot's status to having the goodie
        getWorld()->playSound(SOUND_ROBOT_MUNCH);
        return;
    }
    else if(getSquaresMoved() < getDistanceBeforeTurn()) // If ThiefBot still has squares left to move, move if no obstruction
    {
        equalizeTicks();
        if(moveIfAvailableRoom(getDirection(), getX(), getY())) {
            return;
        }
    }
    // If none of the above apply, choose a new direction and move if possible
    equalizeTicks();
    resetSquaresMoved();
    setDistanceBeforeTurning(randInt(1, 6));
    int directionPicker= randInt(1, 4);
    int direction = -1;
    if(directionPicker == 1) {
        direction = right;
    }
    else if(directionPicker == 2) {
        direction = left;
    }
    else if(directionPicker == 3) {
        direction = up;
    }
    else if(directionPicker == 4) {
        direction = down;
    }
    if(direction == right) // If direction picked is right
    {
        if(getWorld()->canEnemyMoveHere(getX()+1, getY())) // Consider moving right
        {
            setDirection(right);
            moveTo(getX()+1, getY());
            increaseSquaresMoved();
            return;
        }
        else if(getWorld()->canEnemyMoveHere(getX()-1, getY())) // Consider moving left
        {
            setDirection(left);
            moveTo(getX()-1, getY());
            increaseSquaresMoved();
            return;
        }
        directionTuning(direction, getX(), getY(), true); // Consider vertical directions
        return;
    }
    else if(direction == left) // If direction picked is left
    {
        if(getWorld()->canEnemyMoveHere(getX()-1, getY())) // Consider moving left
        {
            setDirection(left);
            moveTo(getX()-1, getY());
            increaseSquaresMoved();
            return;
        }
        else if(getWorld()->canEnemyMoveHere(getX()+1, getY())) // Consider moving right
        {
            setDirection(right);
            moveTo(getX()+1, getY());
            increaseSquaresMoved();
            return;
        }
        directionTuning(direction, getX(), getY(), true); // Consider vertical directions
        return;
    }
    else if(direction == up) // If direction picked is up
    {
        if(getWorld()->canEnemyMoveHere(getX(), getY()+1)) // Consider moving up
        {
            setDirection(up);
            moveTo(getX(), getY()+1);
            increaseSquaresMoved();
            return;
        }
        if(getWorld()->canEnemyMoveHere(getX(), getY()-1)) // Consider moving down
        {
            setDirection(down);
            moveTo(getX(), getY()-1);
            increaseSquaresMoved();
            return;
        }
        directionTuning(direction, getX(), getY(), false); // Consider horizontal directions
        return;
    }
    else if(direction == down) // If direction picked is down
    {
        if(getWorld()->canEnemyMoveHere(getX(), getY()-1)) // Consider moving down
        {
            setDirection(down);
            moveTo(getX(), getY()-1);
            increaseSquaresMoved();
            return;
        }
        if(getWorld()->canEnemyMoveHere(getX(), getY()+1)) // Consider moving up
        {
            setDirection(up);
            moveTo(getX(), getY()+1);
            increaseSquaresMoved();
            return;
        }
        directionTuning(direction, getX(), getY(), false); // Consider horizontal directions
        return;
    }
}

// Mean ThiefBot Functions
void MeanThiefBot::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    if(getTicks() != 0) // Robot can only act when ticks equal 0
    {
        decreaseTicks();
        return;
    }
    shootStealMoveOrTurn(true); // General function responsible for all thiefbot actions, used to prevent repetitive code between regular ThiefBots and Mean ThiefBots
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
    if(getWorld()->countTheThiefsAroundMe(getX(), getY()) >= 3 || getWorld()->isThiefOnMe(getX(), getY())) // Do nothing greater 2 thiefbots present in region or thief is on factory
    {
        return;
    }
    else
    {
        int chance = randInt(1, 50); // 1 and 50 chance of spawning per tick
        if(chance == 1)
        {
            if(m_mean == true) // if mean thiefbot factory, spawn mean thiefbot
            {
                getWorld()->spawnThief(getX(), getY(), true);
            }
            else // if regular thiefbot factory, spawn regular thiefbot
            {
                getWorld()->spawnThief(getX(), getY(), false);
            }
            getWorld()->playSound(SOUND_ROBOT_BORN);
        }
    }
}
