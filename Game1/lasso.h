#ifndef __LASSO_H__
#define __LASSO_H__

#include "MovingObject.h"
#include "coin.h"

#define WINDOW_X 1280
#define WINDOW_Y 960
//#define WINDOW_X 800
//#define WINDOW_Y 600

#define MAX_AXIS 100
#define OMEGA 5
#define STEP_TIME 0.05
#define COIN_OFFSET 250
#define PLAY_X_START 100
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)

#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 1

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG 0
#define MAX_RELEASE_ANGLE_DEG (360-RELEASE_ANGLE_STEP_DEG)
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 32
#define MIN_RELEASE_SPEED 0
#define MAX_RELEASE_SPEED 320
#define INIT_RELEASE_SPEED 160

#define COIN_SPEED 120
#define COIN_ANGLE_DEG 90

#define LASSO_G 30
#define COIN_G 30

#define LASSO_SIZE 10
#define LASSO_RADIUS 50
#define COIN_SIZE 10

class Lasso : public MovingObject {
    double lasso_start_x;
    double lasso_start_y;
    double release_speed;
    double release_angle_deg;
    double lasso_ax;
    double lasso_ay;

    // Moving parts
    Circle lasso_circle;
    Circle lasso_loop;

    // Non-moving parts
    Line lasso_line;
    Line lasso_band;

    // State info
    bool lasso_looped;
    Coin *the_coin;
    int num_coins;

    void initLasso();
 public:
   Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
      release_speed = speed;
      release_angle_deg = angle_deg;
      lasso_ax = argax;
      lasso_ay = argay;
      initLasso();
    }

    void draw_lasso_band();
    int yank();
    void loopit();
    void addAngle(double angle_deg);
    void addSpeed(double speed);
    bool isLooped(){return lasso_looped;}
    void nextStep(double t,bool whatis);
    int check_for_ClosestCoin(vector<Coin*> coinPtrs);
    int getNumCoins() { return num_coins; }
    void incrementCoins(int n){num_coins+=n;}
    void get_the_coin(Coin *coinPtr);
    void resetCoinNum(){num_coins = 0;}
    void resetLoop();
    void HideLassoBand(){lasso_band.hide();}
    void ShowLassoBand(){lasso_band.show();}
}; // End class Lasso

#endif
