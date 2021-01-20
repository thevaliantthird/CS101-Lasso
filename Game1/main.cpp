#include <simplecpp>
#include <string>
#include <random>
#include <cmath>
#include "MovingObject.h"
#include "lasso.h"
#include "coin.h"



#define XSTART LASSO_X_OFFSET + 300        //Co-ordinates for Displaying Lives
#define YFIXED WINDOW_Y-50               //Co-ordinates for Displaying lives

using namespace simplecpp;


int CumulativeScore;
vector<int> Scores;
bool endgame;               //To decide when to end the game
string Player;              //Store the Player Name
int CoinRequirement;        //Number of Coins required to cross a particular level
int CoinCount[8];

int nextS(Lasso &lasso, char c, vector<Coin*> &coins){
  int z = -1;     //-1 if, there's been nothing special like catching a special coin

  switch(c) {
  case 't':
    lasso.unpause();
    break;
  case 'y':
    z = lasso.yank();
    break;
  case 'l':
    if(lasso.getParts().size()<3 && !lasso.isLooped()){
      int index = lasso.check_for_ClosestCoin(coins);
      if(index==-1){}else{
        lasso.get_the_coin(coins[index]);
        CoinCount[coins[index]->getType()]--;
        coins.erase(coins.begin()+index);
      }
    }
    if(!lasso.isLooped())lasso.loopit();

    //wait(STEP_TIME*5);
    break;
  case '[':
    if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
    break;
  case ']':
    if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
    break;
  case '-':
    if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
    break;
  case '=':
    if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
    break;
  case 'q':
    exit(0);
  default:
    break;
  }
  return z;
}


void LevelUp(int n, XEvent &E){
  switch(n){
    case 2:
      {Text T(WINDOW_X/2+100,WINDOW_Y/2-300,"LEVEL UP! Welcome to Level 2!");
      T.setScale(10);
      Text T1(WINDOW_X/2+100, WINDOW_Y/2,"Here's what this Level brings new:");
      T1.setScale(8);
      Text T4(WINDOW_X/2+100,WINDOW_Y/2 + 30,"Now, On we introduce the concept of Lives!");
      Text T6(WINDOW_X/2+100,WINDOW_Y/2 + 60,"Look at the Lives bar in the bottom to keep track of your life!");
      Circle c(WINDOW_X/2-70,WINDOW_Y/2 +90,10);
      Text T3(WINDOW_X/2-70,WINDOW_Y/2 +90,"B");
      Text T2(WINDOW_X/2+100, WINDOW_Y/2+90,"Bomb: Catch this by mistake and you'll lose a life");

      Text T7(WINDOW_X/2+100, WINDOW_Y/2+120,"Press any key to continue!");
      T2.setScale(5);
      T3.setScale(5);
      T4.setScale(5);
      T6.setScale(5);
      T7.setScale(5);
      while(true){
        if(checkEvent(E))break;
      }
      while(true){
        if(checkEvent(E))break;
      }
      T.hide();
      T1.hide();
      T2.hide();
      T3.hide();
      T4.hide();
      T6.hide();
      T7.hide();
      c.hide();
      CoinRequirement = 20;
      break;}
    case 3:
      {Text t0(WINDOW_X/2+100,WINDOW_Y/2-300,"LEVEL UP! Welcome to Level 3!");
      t0.setScale(3);
      Text t01(WINDOW_X/2+100,WINDOW_Y/2-280,"Here's what this Level brings new:");
      t01.setScale(2);
      Text t04(WINDOW_X/2+100,WINDOW_Y/2-260,"Now, We introduce Magnets:");
      Text t06(WINDOW_X/2+100,WINDOW_Y/2-230,"Look for a Timer at Bottom which keeps track of Magnet's Time Left");
      Circle c1(WINDOW_X/2-70,WINDOW_Y/2-200,10);
      c1.setFill(true);
      c1.setColor(COLOR("red"));
      Text t03(WINDOW_X/2-70,WINDOW_Y/2-200,"M");
      Text t02(WINDOW_X/2+100,WINDOW_Y/2-170,"Magnet: Upon collection it'll be useable for a 10-second period!");
      Text t05(WINDOW_X/2+100,WINDOW_Y/2-140,"There are two types of Magnets: Ones which attract coins & repel Bombs and the other way round, But you can't distinguish between them!");

      Text t07(WINDOW_X/2+100,WINDOW_Y/2-110,"Press any key to continue!");
      while(true){
        if(checkEvent(E))break;
      }
      while(true){
        if(checkEvent(E))break;
      }
      t0.hide();
      t01.hide();
      t02.hide();
      t03.hide();
      t04.hide();
      t05.hide();
      t06.hide();
      c1.hide();
      CoinRequirement = 25;
      break;}
    case 4:
      {Text t(WINDOW_X/2+100,WINDOW_Y/2-300,"LEVEL UP! Welcome to Level 4!");
      t.setScale(3);
      Text t1(WINDOW_X/2+100,WINDOW_Y/2-280,"What's new in this level:");
      t1.setScale(2);
      Text t4(WINDOW_X/2+100,WINDOW_Y/2-260,"Now, We introduce Speed Tokens!:");
      Text t6(WINDOW_X/2+100,WINDOW_Y/2-230,"Look out for a Timer at Bottom which keeps track of Speed Token's Time Left");
      Circle c2(WINDOW_X/2-70,WINDOW_Y/2-200,10);
      c2.setFill(true);
      c2.setColor(COLOR("blue"));
      Text t3(WINDOW_X/2-70,WINDOW_Y/2-200,"S");
      Text t2(WINDOW_X/2+100,WINDOW_Y/2-170,"Speed Token: Upon collection it'll be useable for a 10-second period!");
      Text t5(WINDOW_X/2+100,WINDOW_Y/2-140,"There are two types of Tokens: Ones which speed up and others which speed down!");

      Text t7(WINDOW_X/2+100,WINDOW_Y/2-110,"Press any key to continue!");
      while(true){
        if(checkEvent(E))break;
      }
      while(true){
        if(checkEvent(E))break;
      }
      t.hide();
      t1.hide();
      t2.hide();
      t3.hide();
      t4.hide();
      t5.hide();
      t6.hide();
      t7.hide();
      c2.hide();
      CoinRequirement = 30;
      break;}
    case 5:
    {Text T0(WINDOW_X/2+100,WINDOW_Y/2-300,"LEVEL UP! Welcome to Level 5!");
    T0.setScale(3);
    Text T01(WINDOW_X/2+100,WINDOW_Y/2-280,"What's new in this level:");
    T01.setScale(2);
    Text T04(WINDOW_X/2+100,WINDOW_Y/2-260,"Now, We introduce Lives and Compulse Tokens!:");
    Text T06(WINDOW_X/2+100,WINDOW_Y/2-240,"Lives: Collect them to increase your lives");
    Circle C3(WINDOW_X/2-70,WINDOW_Y/2-240,10);
    Text T08(WINDOW_X/2-70,WINDOW_Y/2-240,"L");
    C3.setFill(true);
    C3.setColor(COLOR("green"));
    Circle C2(WINDOW_X/2-70,WINDOW_Y/2-210,10);
    C2.setFill(true);
    C2.setColor(COLOR("orange"));
    Text T03(WINDOW_X/2-70,WINDOW_Y/2-210,"C");
    Text T02(WINDOW_X/2+100,WINDOW_Y/2-210,"Compulse: If you don't catch these you lose a life!");
    Text T05(WINDOW_X/2+100,WINDOW_Y/2-180,"Also, If you watch a compulse coin, you get two points/Equivalent Coin Number.");

    Text T07(WINDOW_X/2+100,WINDOW_Y/2-150,"Press any key to continue!");
    while(true){
      if(checkEvent(E))break;
    }
    while(true){
      if(checkEvent(E))break;
    }
    T0.hide();
    T01.hide();
    T02.hide();
    T03.hide();
    T04.hide();
    T05.hide();
    T06.hide();
    T07.hide();
    T08.hide();
    C3.hide();
    C2.hide();
    CoinRequirement = 30;
      break;}
    case 6:
      {Text t11(WINDOW_X/2+100,WINDOW_Y/2-300,"Congratulations!"+Player+", You've Completed the Game!");
      t11.setScale(4);
      Text t12(WINDOW_X/2+100,WINDOW_Y/2-200,"You're probably bored of hearing this by now, But Press a key to End!");
      while(true){
        if(checkEvent(E))break;
      }
      t11.hide();
      t12.hide();
      endgame = true;
      break;}
  }
}

bool Feasible(int type){
  switch(type){
    case 0:
      return true;
      break;
    case 1:
      return CoinCount[1]<2;
      break;
    case 2:
      return CoinCount[2] + CoinCount[3]<1;
      break;
    case 3:
      return CoinCount[3]+CoinCount[2]<1;
      break;
    case 4:
      return CoinCount[4]+CoinCount[5]<1;
      break;
    case 5:
      return CoinCount[4]+CoinCount[5]<1;
      break;
    case 6:
      return CoinCount[6]<1;
      break;
    case 7:
      return CoinCount[7]<2;
      break;
    default:
      return true;
      break;
  }
}


void GameOver(Lasso &L, vector<Coin*> &coins, XEvent &E){
  L.hideObject();
  for(size_t i = 0;i<coins.size();i++){
    coins[i]->hideObject();
  }
  Text t(WINDOW_X/2+100,WINDOW_Y/2-300,"Game Over!");
  t.setScale(4);
  Text t1(WINDOW_X/2+100,WINDOW_Y/2-250,"Better Luck next time," + Player+"!");
  t1.setScale(3);
  getClick();
  Text t3(WINDOW_X/2+100,WINDOW_Y/2-200,"Press any key to continue!");
  while(true){
    if(checkEvent(E))break;
  }
  while(true){
    if(checkEvent(E))break;
  }
  endgame = true;
}

void StartGame(XEvent &E){
  Text t(WINDOW_X/2+100,WINDOW_Y/2-300,"Welcome to Lasso!");
  t.setScale(20);
  Text t1(WINDOW_X/2+100, WINDOW_Y/2-270,"Instructions:");
  t1.setScale(16);
  Text t3(WINDOW_X/2+100, WINDOW_Y/2-240,"The Game is simple. You have to throw the Lasso and collect coins.");
  Text t4(WINDOW_X/2+100, WINDOW_Y/2-210, "You can adjust the angle of Your Lasso throw by using '[' and ']' keys, and then throw by pressing 'T'!");
  Text t5(WINDOW_X/2+100,WINDOW_Y/2-180,"You may also adjust your Lasso's speed by using the '=/+' and '-' keys at the end of the Function-keys strip.");
  Text t6(WINDOW_X/2+100,WINDOW_Y/2-150,"You need to press 'L' as soon as the Lasso gets near the Coin, It'll loop and the nearest coin will be collected!");
  Text t7(WINDOW_X/2+100, WINDOW_Y/2-120,"After the coin is collected in the loop, Press 'Y' to yank the loop and add the coin to your collection!");
  Text t8(WINDOW_X/2+100, WINDOW_Y/2-90,"Every Subsequent Level shall introduce more subtleties, Also don't keep pushing a key for long as that crashes the game!");
  Text t9(WINDOW_X/2+100, WINDOW_Y/2-60, "Press any where on the screen to continue!");
  //t2.setScale(5);
  t3.setScale(10);
  t4.setScale(10);
  t5.setScale(10);
  t6.setScale(10);
  t7.setScale(10);
  t8.setScale(10);
  t9.setScale(10);
  while(true){
    if(checkEvent(E))break;
  }
  t.hide();
  t1.hide();
  t6.hide();
  t3.hide();
  t4.hide();
  t5.hide();
  t7.hide();
  t8.hide();
  t9.hide();
}


main_program {

  cout<<"Welcome to the Game of Lasso!"<<endl;
  cout<<"Please Enter Your Name:"<<endl;
  cin>>Player;
  Scores.clear();
  CumulativeScore = 0;
  CoinRequirement = 15;
  initCanvas("Lasso", WINDOW_X, WINDOW_Y);
  int stepCount = 0;
  double stepTime = STEP_TIME;
  double runTime = -1; // sec; -ve means infinite
  double currTime = 0;
  int level = 1;      // The Level count for the game
  double levelTime = 0;    //This contains time elapsed in any particular level
  size_t lives = 5;
  vector<Circle> Lives;             //Maintaining different Circles for Lives
  vector<Coin*> coinRadar;      //To keep track of the on-screen coins
  int i = -1;        //Buffer variables to be used later for different purposes
  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  endgame = false;
  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  b1.setColor(COLOR("blue"));
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  b2.setColor(COLOR("blue"));
  bool attract = false;             //Deciding if a coin-attracting magnet is active
  bool repel = false;               //Deciding if a coin-repelling magnet is active
  bool speedup = false;         //Deciding if we have to speed up
  bool slowdown = false;        // //Deciding if we have to slow down
  int TimeUpForMagnet;              //To decide at what time, we'll end magnet
  int TimeUpForSpeed;               //To Decide at what time, we'll end Speed-Ups and Downs
  bool LivesChanged = false;        //To make sure that we aren't redundant in Drawing the Number of Lives
  queue<int> ToReleaseCoins;        //Maintaining a queue of coins which are to be released later
  int CoinTP = 20;                      //The time period at interval of which coin is released
  int BombTP = 20;                     //The time period at interval of which Bomb is released
  int MagnetTP = 25;             //The time period at interval of which AttractMagnet is released
  int SpeedTP = 30;                //The time period at interval of which LassoSpeedDown is released
  int CompulsionTP = 25;                //The time period at interval of which a Compulsion coin will be released
  int LiveTP = 30;                     //The time period at interval of which Lives are released
  //StartGame();
  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
  char coinScoreStr[256];
  sprintf(coinScoreStr, "%d", lasso.getNumCoins());
  Text coinScore(PLAY_X_START+100, PLAY_Y_HEIGHT+50, coinScoreStr);
  Text coinLabel(PLAY_X_START+50, PLAY_Y_HEIGHT+50, "Coins: ");
  paused = true; rtheta = true;
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  double coin_ax = 0;
  double coin_ay = COIN_G;
  //Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
  Text MagnetTime(WINDOW_X-200,PLAY_Y_HEIGHT+20,"ATTRACT/REPEL:");
  Text SpeedTime(WINDOW_X-200,PLAY_Y_HEIGHT+50,"SPEED:");
  Text LivesLabel(XSTART+100, YFIXED,"Lives:");
  LivesLabel.hide();
  MagnetTime.hide();
  SpeedTime.hide();
  XEvent e1;
  StartGame(e1);

  for(int u = 0;u<8;u++){CoinCount[u]=0;}
/*
  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end = 0;
*/
  // When t is pressed, throw lasso
  // If lasso within range, make coin stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // When q is pressed, quit
  ToReleaseCoins.push(0);
  ToReleaseCoins.push(0);

  for(;;) {
    if((runTime > 0) && (currTime > runTime)) { break; }

    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {
      char c = charFromEvent(e);
      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);
      i = nextS(lasso,c,coinRadar);
    }
    switch(i){
      case 0:
          lasso.incrementCoins(1);
          i = -1;
          break;
      case 1:
          lives--;
          LivesChanged = true;
          i = -1;
          break;
      case 2:
          attract = true;
          MagnetTime.show();
          if(repel){
            TimeUpForMagnet = 10.0-TimeUpForMagnet+(2*levelTime);
            repel = false;
          }else TimeUpForMagnet = levelTime+10.0;
          i = -1;
          break;
      case 3:
          repel = true;
          MagnetTime.show();
          if(attract){
              TimeUpForMagnet = 10.0-TimeUpForMagnet+(2*levelTime);
              attract = false;
            }else TimeUpForMagnet = levelTime+10.0;
            i = -1;
            break;
      case 4:
          speedup = true;
          SpeedTime.show();
          if(slowdown){
            TimeUpForSpeed = 10.0-TimeUpForSpeed+(2*levelTime);
            slowdown = false;
          }else TimeUpForSpeed = levelTime + 10.0;
          i = -1;
          break;
      case 5:
        slowdown = true;
        SpeedTime.show();
        if(speedup){
          TimeUpForSpeed = 10.0-TimeUpForSpeed+(2*levelTime);
          speedup = false;
        }else TimeUpForSpeed = levelTime + 10.0;
        i = -1;
        break;
      case 6:
        lives++;
        LivesChanged = true;
        i = -1;
        break;
      case 7:
        lasso.incrementCoins(2);               //2 points for the Compulsory Coin
        i = -1;
        break;
      default:
      break;

    }

    if(stepCount%CoinTP==0){
      ToReleaseCoins.push(0);
    }
    if(stepCount%BombTP==0 && level >=2){
      ToReleaseCoins.push(1);
    }
    if(stepCount%MagnetTP==0 && level >= 3){
      ToReleaseCoins.push((rand()%2)+2);
    }
    if(stepCount%SpeedTP==0 && level >= 4){
      ToReleaseCoins.push((rand()%2)+4);
    }
    if(stepCount%LiveTP==0 && level==5 ){
      ToReleaseCoins.push(6);
    }
    if(stepCount%CompulsionTP==0 && level==5){
      ToReleaseCoins.push(7);
    }

    if(ToReleaseCoins.size()>0 && coinRadar.size()<7){
      int y = ToReleaseCoins.front();

      ToReleaseCoins.pop();
      if(Feasible(y)){
        CoinCount[y]++;
        Coin *newCoin = new Coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, false, rtheta);
        newCoin->initType(y);
        if(y==0){
          newCoin->initTrajectory(rand()%4,levelTime);
        }else if(y==1){
          newCoin->initTrajectory((rand()%2)*2,levelTime);
        }else if(y==7){
          int l = rand()%3;
          if(l==2)l = 3;
          newCoin->initTrajectory(l,levelTime);
        }else{
          newCoin->initTrajectory((rand()%2)*2+1,levelTime);
        }

        coinRadar.push_back(newCoin);
      }
    }
    if(speedup){
      lasso.set_vx(lasso.get_vx()*2);
      lasso.set_vy(lasso.get_vy()*2);
    }
    if(slowdown){
      lasso.set_vx(lasso.get_vx()/2);
      lasso.set_vy(lasso.get_vy()/2);
    }
    lasso.nextStep(stepTime,true);
    if(speedup){
      lasso.set_vx(lasso.get_vx()/2);
      lasso.set_vy(lasso.get_vy()/2);
    }
    if(slowdown){
      lasso.set_vx(lasso.get_vx()*2);
      lasso.set_vy(lasso.get_vy()*2);
    }
    if(attract && (levelTime-TimeUpForMagnet)>1e-5){
      attract = false;
    }
    if(repel && (levelTime-TimeUpForMagnet)>1e-5){
      repel = false;
    }
    if(speedup && (levelTime-TimeUpForSpeed)>1e-5){
      speedup = false;
    }
    if(slowdown && (levelTime-TimeUpForSpeed)>1e-5){
      slowdown = false;
    }
    for(size_t o = 0;o<coinRadar.size();o++){
      double X = lasso.getXPos();
      double Y = lasso.getYPos();
      coinRadar[o]->nextStep(stepTime,false);
      coinRadar[o]->runTrajectory(levelTime);
      if(attract || repel){
        double V_X, V_Y;
        V_X = X - coinRadar[o]->getXPos();
        V_Y = Y - coinRadar[o]->getYPos();
        double mod = sqrt(V_X*V_X + V_Y*V_Y);
        V_X/=mod;
        V_Y/=mod;
        V_X*=COIN_SPEED*2;
        V_Y*=COIN_SPEED*2;
        if((coinRadar[o]->getType()==1 && attract) || (coinRadar[o]->getType()!=1 && repel) ){
          coinRadar[o]->set_vx(coinRadar[o]->get_vx() - V_X);
          coinRadar[o]->set_vy(coinRadar[o]->get_vy() - V_Y);
        }else{
          coinRadar[o]->set_vx(coinRadar[o]->get_vx() + V_X);
          coinRadar[o]->set_vy(coinRadar[o]->get_vy() + V_Y);
        }
      }
    }
    if((speedup || slowdown) && levelTime==TimeUpForSpeed){
      if(speedup){
        speedup = false;
      }else{
        slowdown = false;
      }
    }

    for(size_t h = 0;h<coinRadar.size();h++){
      double XI = coinRadar[h]->getXPos();
      double YI = coinRadar[h]->getYPos();
      if(XI<PLAY_X_START || XI>WINDOW_X || YI>PLAY_Y_HEIGHT || YI<0){
        coinRadar[h]->hideObject();
        if(coinRadar[h]->getType()==7){
          lives--;
          LivesChanged = true;
        }
        CoinCount[coinRadar[h]->getType()]--;
        delete coinRadar[h];
        coinRadar.erase(coinRadar.begin()+h);
      }
    }
    /*if(coin.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	       coin.unpause();
      }
    }

    if(coin.getYPos() > PLAY_Y_HEIGHT) {
      coin.resetCoin();
      last_coin_jump_end = currTime;
    } */
    if(level>=2 && LivesChanged){
      if(lives>0){
        while(lives>Lives.size()){
          Circle c;
          Lives.push_back(c);
          Lives[Lives.size()-1].reset(XSTART+120+((Lives.size()-1)*25),YFIXED,10);
          Lives[Lives.size()-1].setColor(COLOR("green"));
          Lives[Lives.size()-1].setFill(true);
        }
        while(lives<Lives.size()){
          Lives[Lives.size()-1].hide();
          Lives.pop_back();
        }
        LivesChanged = false;
      }else{
        XEvent f;
        GameOver(lasso, coinRadar,f);
        endgame = true;
      }
    }
    if(repel){
      MagnetTime.setMessage("REPEL FOR: "+ to_string(TimeUpForMagnet-levelTime)+" s");
    }
    if(attract){
      MagnetTime.setMessage("ATTRACT FOR: "+ to_string(TimeUpForMagnet-levelTime)+" s");
    }
    if(!repel && !attract)MagnetTime.hide();
    if(speedup){
      SpeedTime.setMessage("SPED UP FOR:"+to_string(TimeUpForSpeed-levelTime)+ " s");
    }
    if(slowdown){
      SpeedTime.setMessage("SLOWED DOWN FOR:"+to_string(TimeUpForSpeed-levelTime)+ " s");
    }
    if(!speedup && !slowdown)SpeedTime.hide();


    sprintf(coinScoreStr, "%d", lasso.getNumCoins());

    coinScore.setMessage(coinScoreStr);

    if(lasso.getNumCoins()>=CoinRequirement){
      lasso.hideObject();
      for(size_t b = 0;b<coinRadar.size();b++){
        coinRadar[b]->hideObject();
      }
      coinRadar.clear();
      MagnetTime.hide();
      SpeedTime.hide();
      lasso.HideLassoBand();
      if(level==1){
        LivesChanged = true;
        LivesLabel.show();
      }
      LevelUp(++level,e);
      speedup = false;
      slowdown = false;
      attract = false;
      repel = false;
      if(level>=3){
        MagnetTime.show();
      }
      if(level>=4){
        SpeedTime.show();
      }
      lasso.ShowLassoBand();

      for(int u = 0;u<8;u++){CoinCount[u]=0;}
      while(!ToReleaseCoins.empty()){
        ToReleaseCoins.pop();
      }
      levelTime = 0;
      lasso.ShowObject();
      lasso.resetCoinNum();
      bool paused = true, rtheta = true;
      lasso.reset_all((PLAY_X_START+LASSO_X_OFFSET),(PLAY_Y_HEIGHT-LASSO_Y_HEIGHT),release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
      lasso.resetLoop();
      ToReleaseCoins.push(0);
      ToReleaseCoins.push(0);
    }

    stepCount++;
    currTime += stepTime;
    levelTime+=stepTime;
    if(leve==1)wait(0.25*(stepTime));
    else wait(0.1*(stepTime));
    if(endgame)break;
  } // End for(;;)

  wait(3);
} // End main_program
