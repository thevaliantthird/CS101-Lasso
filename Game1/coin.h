#ifndef __COIN_H__
#define __COIN_H__

#include "MovingObject.h"




class Coin : public MovingObject {
  //Coin Info
  double coin_start_x;
  double coin_start_y;
  double release_speed;
  double release_angle_deg;
  double coin_ax;
  double coin_ay;
  int trajectory;    // 0 for Straightline, 1 for parabolic, 2 for elliptical, 3 for sinusoidal
  //The components of a unit vector of the Line(in case of Straightline), of the axis in case of other conics
  double dir_x,dir_y;
  int type;
  /*0 for normal coin, 1 for Bomb, 2 for Magnet that repels Bombs and attracts coin
  3 for magnets which attracts bomb and repels magnet, but there's no way to know

  4 for Speed_ups, 5 for slowDown coins, 6 for life, 7 for Compulse coin
  */
  /*
    Normal Coin: Golden Color with C written in Middle
    Bomb: Black with B in Middle
    Magnet: Red with M in Middle
    SpeedUps and SLowDows: S IN middle and blue in color
    Life: Green with L in Middle


  */
  float createTime;         //the time when the coin is created
  double VX,VY,AX,AY;     // We will first model everything in 2D with aces along the Co-ordinate axes and then project it to another direction
  double X,Y;       //The Co-ordinates considering motion along axes only

  double E;       //The Ellipse will have a different max velocity at the vertices of the major/minor axes, This gives us the ratio,
  //THe aboce ratio define minor axis/major axis which maybe used to get the eccentricity


  /*
    Further the first level shall introduce us to  Normal coins,
    Every Subsequent levels each have a page which enlists the additional additions
    to them and the total Game Stuffs till now!

    The General FLow of the game is that we will have a number of levels, ANd at each level, some new functionality will be added
    which will bring new challenges:

    1) Level 1 will have random coins with the Spurt to collect 10 coins
    2) Level 2 will have Bombs and Lives Introduced
    3) Level 3 will have Random Coins and Bombs, Magnets apart from the above Stuffs, but two types of magnets, one which attract coins and repel magnets as well as the other way round,
    and we can't distinguish
    4) Level 4 will have Speed-Ups and Speed Downs and WE can't distinguish between them
    5) Level 5 will have lives, and apart from that some special coins which need to be collected, otherwise you lose the game!

    There will also be a vairable of time, which we shall print of every turn somewhere in some corner

    The Score shall be calculated using a formula on the time that has passed for every level,
    We shall maintain two variables of time, One for the particular level and another for overall
    At the end of each level, A Display of Your score and past High Scores for the level
    AND IN THE END, One for overall


  */

  // Moving parts
  Circle coin_circle;
  Text d;

 public:
   //This part won't be needed any longer as now, we won't any longer be having fixed trajectories
   //We'll compensate for it using the initTrajectory and the runTrajectory functions
 Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    initCoin();
  }
  void initType(int Type);        //Initializing the type, i.e. making its shape at the initial position
  void initTrajectory(int typ, double creationTime);  //Initializing the coin's trajectory
  void runTrajectory(double time);    //Running the trajectory, changing velocities after every step
  void initCoin();    //For initializing coin
  void resetCoin();   //For Reseting the Coin
  int getType(){return type;}   //Return the type of a coin
  int getTrajectory(){return trajectory;}     //Return the trajectory of the coin

}; // End class Coin

#endif
