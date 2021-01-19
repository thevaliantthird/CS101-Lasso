#include "coin.h"
#include "lasso.h"
#include <random>
#include <cmath>
#define HALF_PIE 1.570796327
void Coin::initCoin() {

  coin_start_x = COIN_OFFSET + (WINDOW_X-COIN_OFFSET)*(((double)rand())/((double)RAND_MAX));
  coin_start_y = PLAY_Y_HEIGHT*((((double)rand())/((double)RAND_MAX)));
  if(coin_start_y<100)coin_start_y+=200;
  if(coin_start_y>PLAY_Y_HEIGHT-100)coin_start_y-=200;
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  coin_circle.setColor(COLOR("gold"));
  coin_circle.setFill(true);
  addPart(&coin_circle);
}
void Coin::initType(int Type){
  type = Type;
  switch(Type){
    case 0:
      d.reset(coin_start_x,coin_start_y,"N");
      break;
    case 1:
      coin_circle.setFill(false);
      d.reset(coin_start_x,coin_start_y,"B");
      break;
    case 2:
      coin_circle.setColor(COLOR("red"));
      d.reset(coin_start_x,coin_start_y,"M");
      break;
    case 3:
      coin_circle.setColor(COLOR("red"));
      d.reset(coin_start_x,coin_start_y,"M");
      break;
    case 4:
      coin_circle.setColor(COLOR("blue"));
      d.reset(coin_start_x,coin_start_y,"S");
      break;
    case 5:
      coin_circle.setColor(COLOR("blue"));
      d.reset(coin_start_x,coin_start_y,"S");
      break;
    case 6:
      coin_circle.setColor(COLOR("green"));
      d.reset(coin_start_x, coin_start_y,"L");
      break;
    case 7:
      coin_circle.setColor(COLOR("orange"));
      d.reset(coin_start_x,coin_start_y,"C");

  }
  addPart(&d);
}

void Coin::initTrajectory(int typ, double creationTime){
    trajectory = typ;
    createTime = creationTime;
    dir_x = -(((double)rand())/((double)RAND_MAX))*(((rand()%2)*2)-1);
    dir_y = sqrt(1-(dir_x*dir_x))*(((rand()%2)*2)-1);
    double angle;
    switch(typ){
      case 0:
        X = 0;
        Y = 0;
        VX = 0;
        VY = COIN_SPEED;
        AX = 0;
        AY = -COIN_G;
        break;
      case 1:
        X = 0;
        Y = 0;
        angle = (((double)rand())/((double)RAND_MAX))*HALF_PIE;
        VX = COIN_SPEED*cos(angle);
        VY = COIN_SPEED*sin(angle);
        AX = 0;
        AY = -COIN_G;
        break;
      case 2:
        E = (((double)rand())/((double)RAND_MAX))+0.1;
        VX = 0;
        VY =COIN_SPEED;
        break;
      case 3:
        VX = COIN_SPEED*sqrt(2);
        VY = COIN_SPEED*sqrt(2);
        break;
    }
    set_vx(VX*dir_x+VY*dir_y);
    set_vy(VX*dir_y-VY*dir_x);
}

void Coin::runTrajectory(double time){
  switch(trajectory){
    case 0:
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
      set_vx(VX*dir_x+VY*dir_y);
      set_vy(VX*dir_y-VY*dir_x);
      break;
    case 1:
      X = X + (VX*STEP_TIME);
      Y = Y + (VY*STEP_TIME);
      VY+=AY*STEP_TIME;
      set_vx(VX*dir_x+VY*dir_y);
      set_vy(VX*dir_y-VY*dir_x);
      break;
    case 2:
      VX = (COIN_SPEED/E)*sin(OMEGA*(time-createTime));
      VY = COIN_SPEED*cos(OMEGA*(time-createTime));
      set_vx(VX*dir_x+VY*dir_y);
      set_vy(VX*dir_y-VY*dir_x);
      break;
    case 3:
      VY = (COIN_SPEED*2/sqrt(2))*cos(OMEGA*(time-createTime));
      set_vx(VX*dir_x+VY*dir_y);
      set_vy(VX*dir_y-VY*dir_x);
      break;
  }
}



void Coin::resetCoin() {
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}
