#include <simplecpp>
#include <string>
#include <limits>
#include "MovingObject.h"
#include "lasso.h"
#include "coin.h"

using namespace simplecpp;

void Lasso::draw_lasso_band() {         //To draw the lasso's handle
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

void Lasso::initLasso() {             //Taking care of the lasso's line(initialization)
  lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
  lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR(255,0,0));
  lasso_circle.setFill(true);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setColor(COLOR(165,42,42));
  lasso_loop.setFill(true);
  addPart(&lasso_circle);
  addPart(&lasso_loop);
  lasso_looped = false;
  the_coin = NULL;
  num_coins = 0;

  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR(165,42,42));

  lasso_band.setColor(COLOR(138,43,226));
  draw_lasso_band();

} // End Lasso::initLasso()

void Lasso::resetLoop(){        //To reset the loop
    lasso_loop.reset(lasso_start_x,lasso_start_y, LASSO_SIZE);
    lasso_loop.setFill(true);
    lasso_looped = false;
    the_coin = NULL;
}

//After we've caught the coin, use it to call back the lasso and return the coin we've collected
int Lasso::yank() {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  if(the_coin != NULL) {
    int t = the_coin->getType();
    //Will deal with coin increment etc. later in the main function itself
    the_coin->hideObject();
    //Two remove parts to remove the Coin's circle and the character at its middle
    //This RemovePart() acts on the last two elements of the MovingObject i.e. lasso
    RemovePart();
    RemovePart();
    delete the_coin;
    the_coin = NULL;
    return t;
  }else{
    return -1;          //When there's no coin
  }

} // End Lasso::yank()

void Lasso::loopit() {
  if(lasso_looped) { return; } //If Already looped, do nothing
  lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
  lasso_loop.setFill(false);
  lasso_looped = true;
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg) {        //Incrementing/Depricating angle of lasso projection
  release_angle_deg += angle_deg;
  //Dealing with cases, when we try to increase angle beyond a point
  if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
  if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed) {          //Incrementing/Depricating the speed of projection
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
  if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

//Running the nextStep!
void Lasso::nextStep(double stepTime,bool whatis) {
  draw_lasso_band();
  MovingObject::nextStep(stepTime,whatis);
  //YANK whenever the lasso crosses the boundary of PlayScreen
  if(getYPos() > PLAY_Y_HEIGHT || getYPos()<0 || getXPos()>WINDOW_X || getXPos()<PLAY_X_START) { yank(); }
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

//Returning the Coin which is closest to the lasso
int Lasso::check_for_ClosestCoin(vector<Coin*> coinPtrs) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double min_dist = numeric_limits<double>::infinity();
  int MinInd = -1;
  for(size_t i = 0;i<coinPtrs.size();i++){
    double coin_x = coinPtrs[i]->getXPos();
    double coin_y = coinPtrs[i]->getYPos();
    double xdiff = (lasso_x - coin_x);
    double ydiff = (lasso_y - coin_y);
    double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
    if(distance<min_dist){        //If the current coin is closer than the previous closest
      min_dist = distance;
      MinInd = i;
    }
  }
  if(min_dist<LASSO_RADIUS){
    return MinInd;
  }else{
    return -1;
  }

} // End Lasso::check_for_coin()

//Attach the Coin to our lasso
void Lasso::get_the_coin(Coin *coinPtr){
  the_coin = coinPtr;
  the_coin->getAttachedTo(this);
}
