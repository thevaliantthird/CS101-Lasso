#include "coin.h"
#include "lasso.h"
#include <random>
#include <cmath>
#define HALF_PIE 1.570796327

//Initializing the coin's initial position, and stuff, making them all golden for starters
void Coin::initCoin() {

  coin_start_x = COIN_OFFSET + (WINDOW_X-COIN_OFFSET)*(((double)rand())/((double)RAND_MAX));
  coin_start_y = PLAY_Y_HEIGHT*((((double)rand())/((double)RAND_MAX)));
  if(coin_start_y<100)coin_start_y+=200;
  if(coin_start_y>PLAY_Y_HEIGHT-100)coin_start_y-=200;
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  coin_circle.setColor(COLOR(255,215,0));
  coin_circle.setFill(true);
  addPart(&coin_circle);
}

//Initializing the type of the coin, hence changing its looks and colors and adding a character in the middle
void Coin::initType(int Type){
  type = Type;
  switch(Type){
    case 0:     //Normal Coin
      d.reset(coin_start_x,coin_start_y,"N");
      break;
    case 1:     //Bomb
      coin_circle.setFill(false);
      d.reset(coin_start_x,coin_start_y,"B");
      break;
    case 2:     //Coin-attracting and bomb-repelling magnet
      coin_circle.setColor(COLOR(255,0,0));
      d.reset(coin_start_x,coin_start_y,"M");
      break;    //Bomb-attracting and coin-repelling magnet
    case 3:
      coin_circle.setColor(COLOR(255,0,0));
      d.reset(coin_start_x,coin_start_y,"M");
      break;
    case 4:     //SpeedUp
      coin_circle.setColor(COLOR(0,0,255));
      d.reset(coin_start_x,coin_start_y,"S");
      break;
    case 5:     //SlowDown
      coin_circle.setColor(COLOR(0,0,255));
      d.reset(coin_start_x,coin_start_y,"S");
      break;
    case 6:      //Life
      coin_circle.setColor(COLOR(0,255,0));
      d.reset(coin_start_x, coin_start_y,"L");
      break;
    case 7:       //Compulsory Coin
      coin_circle.setColor(COLOR(255,140,0));
      d.reset(coin_start_x,coin_start_y,"C");

  }
  addPart(&d);      //Adding the text in middle
}

void Coin::initTrajectory(int typ, double creationTime){
    trajectory = typ;
    createTime = creationTime;
    dir_x = -(((double)rand())/((double)RAND_MAX))*(((rand()%2)*2)-1);
    dir_y = sqrt(1-(dir_x*dir_x))*(((rand()%2)*2)-1);
    double angle;
    switch(typ){
      case 0:   //Straight Line
        X = 0;
        Y = 0;
        VX = 0;
        VY = COIN_SPEED;
        AX = 0;
        AY = -COIN_G;
        break;
      case 1:   //Parabola
        X = 0;
        Y = 0;
        angle = (((double)rand())/((double)RAND_MAX))*HALF_PIE;
        VX = COIN_SPEED*cos(angle);
        VY = COIN_SPEED*sin(angle);
        AX = 0;
        AY = -COIN_G;
        break;
      case 2:     //Ellipse
        E = (((double)rand())/((double)RAND_MAX))+0.1;
        VX = 0;
        VY =COIN_SPEED;
        break;
      case 3:     //Sinusoidal
        VX = COIN_SPEED*sqrt(2);
        VY = COIN_SPEED*sqrt(2);
        break;
    }
    //Changing the orientation of movement, to the random direction chosen during trajectory initialization
    set_vx(VX*dir_x+VY*dir_y);
    set_vy(VX*dir_y-VY*dir_x);
}

void Coin::runTrajectory(double time){
  switch(trajectory){
    case 0:       //Simple Straight Line
      if(Y<-1e-5){
        X = 0;
        Y = 0;
        VX = 0;
        VY = COIN_SPEED;
        AX = 0;
        AY = -COIN_G;
      }else{
        Y = Y + (VY*STEP_TIME);
        VY = VY + (AY*STEP_TIME);
        VX = VX + (AX*STEP_TIME);
      }
      //Changing the orientation of movement, to the random direction chosen during trajectory initialization
      set_vx(VX*dir_x+VY*dir_y);
      set_vy(VX*dir_y-VY*dir_x);
      break;
    case 1:     //Parabolic
      X = X + (VX*STEP_TIME);
      Y = Y + (VY*STEP_TIME);
      VY+=AY*STEP_TIME;
      //Changing the orientation of movement, to the random direction chosen during trajectory initialization
      set_vx(VX*dir_x+VY*dir_y);
      set_vy(VX*dir_y-VY*dir_x);
      break;
    case 2:       //Elliptical
      VX = (COIN_SPEED/E)*sin(OMEGA*(time-createTime));
      VY = COIN_SPEED*cos(OMEGA*(time-createTime));
      //Changing the orientation of movement, to the random direction chosen during trajectory initialization
      set_vx(VX*dir_x+VY*dir_y);
      set_vy(VX*dir_y-VY*dir_x);
      break;
    case 3:       //Sinusoidal
      VY = (COIN_SPEED*2/sqrt(2))*cos(OMEGA*(time-createTime));
      //Changing the orientation of movement, to the random direction chosen during trajectory initialization
      set_vx(VX*dir_x+VY*dir_y);
      set_vy(VX*dir_y-VY*dir_x);
      break;
  }
}

//For resetting all of the coins opertations
void Coin::resetCoin() {
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}
