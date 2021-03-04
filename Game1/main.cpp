#include <simplecpp>
#include <string>
#include <random>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include "MovingObject.h"
#include "lasso.h"
#include "coin.h"



#define XSTART LASSO_X_OFFSET + 300        //Co-ordinates for Displaying Lives
#define YFIXED WINDOW_Y-50               //Co-ordinates for Displaying lives

using namespace simplecpp;


int CumulativeScore;         //This is a weighted sum of scores from all Levels
vector<int> Scores;           //The current Player's Scores
vector<vector<string> > OthersScores;       //Past players best scores
bool endgame;               //To decide when to end the game
string Player;              //Store the Player Name
int CoinRequirement;        //Number of Coins required to cross a particular level
int CoinCount[8];

//Function for comparing while sorting past scores
bool comp(pair<string, string> a, pair<string, string> b){
  return stoi(a.second)>stoi(b.second);
}

//Sending the top 5 highest CumulativeScores till that level
vector<pair<string, string> > GetScoresToDisplay(int Level){
  vector<pair<string, string> > res;
  for(size_t i = 0;i<OthersScores.size();i++){
    if(OthersScores[i].size()>=(size_t)Level+1){
      res.push_back(make_pair(OthersScores[i][0],OthersScores[i][Level]));
    }
  }
  sort(res.begin(),res.end(), comp);    //Sort scores in descending order
  while(res.size()>5)res.pop_back();     //Return only top 5
  return res;
}

//Displaying your Cumulative Score and other cumulative High Scores
void DisplayScoreAndHighScores(int Level, XEvent &E){
  vector<pair<string, string> > t = GetScoresToDisplay(Level);
  //Displaying the current Player's Cumulative Score
  Text tt(WINDOW_X/2+100,WINDOW_Y/2-300,"Your Cumulative Score is: "+ to_string(CumulativeScore));
  if(t.size()>0){
    //Displaying past high scores
    Text tt1(WINDOW_X/2+100,WINDOW_Y/2-250, "Other Highest Cumulative Scores till this Level are:");
    Text arr[2*t.size()];
    //2*t.size() in order to differntiate between the name and score
    for(size_t i = 0;i<t.size();i++){
      arr[2*i].reset(WINDOW_X/2+100,WINDOW_Y/2-250+((i+1)*50), to_string(i+1)+". "+t[i].first);
      arr[2*i+1].reset(WINDOW_X/2+150,WINDOW_Y/2-250+((i+1)*50), t[i].second);
    }
    wait(5);
    Text tt2(WINDOW_X/2+100,WINDOW_Y/2+80, "Press a key to proceed!");
    //Waiting for user to press any key
    while(true){
      if(checkEvent(E))break;
    }
    tt2.hide();
    tt1.hide();
    tt.hide();
    for(size_t i = 0;i<t.size();i++){
      arr[2*i].hide();arr[2*i+1].hide();
    }
  }else{
    wait(5);
    Text tt3(WINDOW_X/2+100,WINDOW_Y/2+80, "Press a key to proceed!");
    //Waiting for user to press any key
    while(true){
      if(checkEvent(E))break;
    }
    tt3.hide();
    tt.hide();
  }
}


//Calculating Score for every round and appending to the Scores vector
void CalculateScore(int numCoins,int timeTaken, int numLives, int Level){
  //Formula to calculate score for the current level
  int x = (int)(100*(sqrt(numLives)/((double)timeTaken)))*(((double)numCoins)/((double)CoinRequirement));
  CumulativeScore+=x*Level;       //Adding the past-level's score to cumulative with weights
  Scores.push_back(CumulativeScore);
}

// Read up all past scores at the start of the game for less file-I/O
void ReadUpPastScores(){
  fstream f;
  f.open("scores.csv",ios::in);
  string s;
  string tem;
  vector<string> record;
  while(f){
    record.clear();
    if(!getline(f,s))break;       //Getting the New Record
    stringstream ss(s);           //Making a stringstream out of it, to separate commas
    while(getline(ss,tem,',')){   //Separating commas
      record.push_back(tem);
    }
    OthersScores.push_back(record);//Adding it to the list of all records
  }
  f.close();
}


//Writing the Current Players scores at the end of the game
void WriteTheCurrentPlayerScores(){
  fstream f;
  f.open("scores.csv",ios::out | ios::app);
  f<<Player<<',';
  for(size_t i = 0;i<Scores.size();i++){
    if(i!=Scores.size()-1)f<<Scores[i]<<',';
    else f<<Scores[i]<<'\n';
  }
  f.close();
}


/*Deciding what's the Next step based on the character we pressed
 When t is pressed, throw lasso
 If lasso within range, make coin stick
 When y is pressed, yank lasso
 When l is pressed, loop lasso
 When q is pressed, quit*/

int nextChar(Lasso &lasso, char c, vector<Coin*> &coins){
  int z = -1;     //-1 if, there's been nothing special like catching a special coin

  switch(c) {
  case 't':       //If we have to throw the lasso
    lasso.unpause();
    break;
  case 'y':       //If we have to yank the lasso
    z = lasso.yank();   //lasso.yank returns the type of coin which it has collected
    break;
  case 'l':       //Loop the lasso
    if(lasso.getParts().size()<3 && !lasso.isLooped()){
      int index = lasso.check_for_ClosestCoin(coins);   //Checking for the Closest coin to the lasso
      if(index==-1){}else{
        lasso.get_the_coin(coins[index]);
        CoinCount[coins[index]->getType()]--;
        coins.erase(coins.begin()+index);
      }
    }
    if(!lasso.isLooped())lasso.loopit();
    break;
  case '[':
    if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}   //Decreasing Throw Angle
    break;
  case ']':
    if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }   //Increasing Throw Angle
    break;
  case '-':
    if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }       //Decreasing Lasso Speed
    break;
  case '=':
    if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }       //Increasing Lasso Speed
    break;
  case 'q':                                                             //Exiting the game
    exit(0);
  default:
    break;
  }
  return z;
}

//Displaying the Instructions for the next level
void LevelUp(int n, XEvent &E){
  switch(n){
    case 2:
    {
      Text T(WINDOW_X/2+100,WINDOW_Y/2-300,"LEVEL UP! Welcome to Level 2!");
      Text T1(WINDOW_X/2+100, WINDOW_Y/2,"Here's what this Level brings new:");
      Text T4(WINDOW_X/2+100,WINDOW_Y/2 + 30,"Now, On we introduce the concept of Lives!");
      Text T6(WINDOW_X/2+100,WINDOW_Y/2 + 60,"Look at the Lives bar in the bottom to keep track of your life!");
      Circle c(WINDOW_X/2-70,WINDOW_Y/2 +90,10);
      Text T3(WINDOW_X/2-70,WINDOW_Y/2 +90,"B");
      Text T2(WINDOW_X/2+100, WINDOW_Y/2+90,"Bomb: Catch this by mistake and you'll lose a life");
      wait(5);
      Text T7(WINDOW_X/2+100, WINDOW_Y/2+120,"Press any key to continue!");
      //Waiting for user to press any key
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
      CoinRequirement = 20;   //Setting the level's coin requirement to complete
      break;
    }
    case 3:
    {
      Text t0(WINDOW_X/2+100,WINDOW_Y/2-300,"LEVEL UP! Welcome to Level 3!");
      Text t01(WINDOW_X/2+100,WINDOW_Y/2-280,"Here's what this Level brings new:");
      Text t04(WINDOW_X/2+100,WINDOW_Y/2-260,"Now, We introduce Magnets:");
      Text t06(WINDOW_X/2+100,WINDOW_Y/2-230,"Look for a Timer at Bottom which keeps track of Magnet's Time Left");
      Circle c1(WINDOW_X/2-70,WINDOW_Y/2-200,10);
      c1.setFill(true);
      c1.setColor(COLOR(255,0,0));
      Text t03(WINDOW_X/2-70,WINDOW_Y/2-200,"M");
      Text t02(WINDOW_X/2+100,WINDOW_Y/2-170,"Magnet: Upon collection it'll be useable for a 10-second period!");
      Text t05(WINDOW_X/2+100,WINDOW_Y/2-140,"There are two types of Magnets: Ones which attract coins & repel Bombs and the other way round, But you can't distinguish between them!");
      wait(5);
      Text t07(WINDOW_X/2+100,WINDOW_Y/2-110,"Press any key to continue!");
      //Waiting for user to press any key
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
      CoinRequirement = 25;             //Setting the level's coin requirement to complete
      break;
    }
    case 4:
    {
      Text t(WINDOW_X/2+100,WINDOW_Y/2-300,"LEVEL UP! Welcome to Level 4!");
      Text t1(WINDOW_X/2+100,WINDOW_Y/2-280,"What's new in this level:");
      Text t4(WINDOW_X/2+100,WINDOW_Y/2-260,"Now, We introduce Speed Tokens!:");
      Text t6(WINDOW_X/2+100,WINDOW_Y/2-230,"Look out for a Timer at Bottom which keeps track of Speed Token's Time Left");
      Circle c2(WINDOW_X/2-70,WINDOW_Y/2-200,10);
      c2.setFill(true);
      c2.setColor(COLOR(0,0,255));
      Text t3(WINDOW_X/2-70,WINDOW_Y/2-200,"S");
      Text t2(WINDOW_X/2+100,WINDOW_Y/2-170,"Speed Token: Upon collection it'll be useable for a 10-second period!");
      Text t5(WINDOW_X/2+100,WINDOW_Y/2-140,"There are two types of Tokens: Ones which speed up and others which speed down!");
      wait(5);
      Text t7(WINDOW_X/2+100,WINDOW_Y/2-110,"Press any key to continue!");
      //Waiting for user to press any key
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
      CoinRequirement = 30;               //Setting the level's coin requirement to complete
      break;
    }
    case 5:
    {
      Text T0(WINDOW_X/2+100,WINDOW_Y/2-300,"LEVEL UP! Welcome to Level 5!");
      Text T01(WINDOW_X/2+100,WINDOW_Y/2-280,"What's new in this level:");
      Text T04(WINDOW_X/2+100,WINDOW_Y/2-260,"Now, We introduce Lives and Compulse Tokens!:");
      Text T06(WINDOW_X/2+100,WINDOW_Y/2-240,"Lives: Collect them to increase your lives");
      Circle C3(WINDOW_X/2-70,WINDOW_Y/2-240,10);
      Text T08(WINDOW_X/2-70,WINDOW_Y/2-240,"L");
      C3.setFill(true);
      C3.setColor(COLOR(0,255,0));
      Circle C2(WINDOW_X/2-70,WINDOW_Y/2-210,10);
      C2.setFill(true);
      C2.setColor(COLOR(255,140,0));
      Text T03(WINDOW_X/2-70,WINDOW_Y/2-210,"C");
      Text T02(WINDOW_X/2+100,WINDOW_Y/2-210,"Compulse: If you don't catch these you lose a life!");
      Text T05(WINDOW_X/2+100,WINDOW_Y/2-180,"Also, If you watch a compulse coin, you get two points/Equivalent Coin Number.");
      wait(5);
      Text T07(WINDOW_X/2+100,WINDOW_Y/2-150,"Press any key to continue!");
      //Waiting for user to press any key
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
      CoinRequirement = 30;             //Setting the level's coin requirement to complete
      break;
    }
    case 6:
    {
      Text t11(WINDOW_X/2+100,WINDOW_Y/2-300,"Congratulations!"+Player+", You've Completed the Game!");
      wait(5);
      Text t12(WINDOW_X/2+100,WINDOW_Y/2-200,"You're probably bored of hearing this by now, But Press a key to End!");
      //Waiting for user to press any key
      while(true){
        if(checkEvent(E))break;
      }
      t11.hide();
      t12.hide();
      endgame = true;
      WriteTheCurrentPlayerScores();        //Write Down the scores when the game is completed
      break;
    }
  }
  DisplayScoreAndHighScores(n-1,E);
}

/*Function to check if its feasible to add a coin of this type provided the
pre-existing number of such coins on screen*/
bool Feasible(int type){
  switch(type){
    case 0:
      return CoinCount[0]<4;
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

//Function to help us deal with when the game is over!
void GameOver(int Level,Lasso &L, vector<Coin*> &coins, XEvent &E){
  L.hideObject();
  for(size_t i = 0;i<coins.size();i++){
    coins[i]->hideObject();
  }
  Text t(WINDOW_X/2+100,WINDOW_Y/2-300,"Game Over!");
  Text t1(WINDOW_X/2+100,WINDOW_Y/2-250,"Better Luck next time," + Player+"!");
  wait(5);
  Text t3(WINDOW_X/2+100,WINDOW_Y/2-200,"Press any key to continue!");
  //Waiting for user to press any key
  while(true){
    if(checkEvent(E))break;
  }
  t.hide();
  t1.hide();
  t3.hide();
  DisplayScoreAndHighScores(Level, E);      //Displaying the past-Levels Score
  WriteTheCurrentPlayerScores();            //Writing the Score
  endgame = true;
}

//The fuction which starts the game!
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
  wait(4);
  Text t9(WINDOW_X/2+100, WINDOW_Y/2-60, "Press any key to continue!");
  //Waiting for user to press any key
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
  CumulativeScore = 0;            //Initializing Initial Score
  CoinRequirement = 15;           //Setting Coin Requirement to finish first level
  initCanvas("Lasso", WINDOW_X, WINDOW_Y);
  long long stepCount = 0;              //Keeping count of number of steps done
  long double stepTime = STEP_TIME;    //The smallest increment in time
  long double runTime = -1;            // sec; -ve means infinite
  long double currTime = 0;            //Initializing the current time
  int level = 1;                  // The Level count for the game
  long double levelTime = 0;           //This contains time elapsed in any particular level
  size_t lives = 5;               //Initializing the number of lives(to make meaning after level 2)
  vector<Circle> Lives;           //Maintaining different Circles for Lives
  vector<Coin*> coinRadar;        //To keep track of the on-screen coins
  int i = -1;                     //Buffer variables to be used later for different purposes

  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);  //Initialize Lasso
  endgame = false;                        //Initializing the global variable which decides when game ends
  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);           //The play-screen boundary
  b1.setColor(COLOR(0,0,255));
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);             //The play-screen boundary
  b2.setColor(COLOR(0,0,255));
  bool attract = false;             //Deciding if a coin-attracting magnet is active
  bool repel = false;               //Deciding if a coin-repelling magnet is active
  bool speedup = false;             //Deciding if we have to speed up
  bool slowdown = false;            //Deciding if we have to slow down
  int TimeUpForMagnet;              //To decide at what time, we'll end magnet
  int TimeUpForSpeed;               //To Decide at what time, we'll end Speed-Ups and Downs
  bool LivesChanged = false;        //To make sure that we aren't redundant in Drawing the Number of Lives
  queue<int> ToReleaseCoins;        //Maintaining a queue of coins which are to be released later
  int CoinTP = 20;                  //The time period at interval of which coin is released
  int BombTP = 20;                  //The time period at interval of which Bomb is released
  int MagnetTP = 25;                //The time period at interval of which AttractMagnet is released
  int SpeedTP = 30;                 //The time period at interval of which LassoSpeedDown is released
  int CompulsionTP = 25;            //The time period at interval of which a Compulsion coin will be released
  int LiveTP = 30;                  //The time period at interval of which Lives are released
  //Setting up the Message buffers for Coin Count, Presently pressed key
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
  //Setting up the time counter for Magnet, Speed and Lives to be used for consequent levels
  Text MagnetTime(WINDOW_X-200,PLAY_Y_HEIGHT+20,"ATTRACT/REPEL:");
  Text SpeedTime(WINDOW_X-200,PLAY_Y_HEIGHT+50,"SPEED:");
  Text LivesLabel(XSTART+100, YFIXED,"Lives:");
  LivesLabel.hide();
  MagnetTime.hide();
  SpeedTime.hide();
  XEvent e1;
  StartGame(e1);                        //Starting the Game
  ReadUpPastScores();                   //Reading the Past Scores for easy processing later
  for(int u = 0;u<8;u++){CoinCount[u]=0;}
  ToReleaseCoins.push(0);
  ToReleaseCoins.push(0);
  for(;;) {
    if((runTime > 0) && (currTime > runTime)) { break; }
    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {
      char c = charFromEvent(e);
      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);          //Setting up message for which key is pressed
      i = nextChar(lasso,c,coinRadar);      //i returns which coin has been caught (if any)
    }
    switch(i){
      case 0:         //if normal coin caught
          lasso.incrementCoins(1);
          i = -1;
          break;
      case 1:        //if bomb caught
          lives--;
          LivesChanged = true;
          i = -1;
          break;
      case 2:       //If coin-attracting,bomb-repelling magnet caught
          attract = true;
          MagnetTime.show();
          if(repel){
            TimeUpForMagnet = 10.0-TimeUpForMagnet+(2*levelTime);
            repel = false;
          }else TimeUpForMagnet = levelTime+10.0;
          i = -1;
          break;
      case 3:       //If bomb-attracting, coin-repelling magnet caught
          repel = true;
          MagnetTime.show();
          if(attract){
              TimeUpForMagnet = 10.0-TimeUpForMagnet+(2*levelTime);
              attract = false;
            }else TimeUpForMagnet = levelTime+10.0;
            i = -1;
            break;
      case 4:       //If SpeedUp-Token caught
          speedup = true;
          SpeedTime.show();
          if(slowdown){
            TimeUpForSpeed = 10.0-TimeUpForSpeed+(2*levelTime);
            slowdown = false;
          }else TimeUpForSpeed = levelTime + 10.0;
          i = -1;
          break;
      case 5:       //If SlowDown-Token caught
        slowdown = true;
        SpeedTime.show();
        if(speedup){
          TimeUpForSpeed = 10.0-TimeUpForSpeed+(2*levelTime);
          speedup = false;
        }else TimeUpForSpeed = levelTime + 10.0;
        i = -1;
        break;
      case 6:       //If Life Token caught
        lives++;
        LivesChanged = true;
        i = -1;
        break;
      case 7:       //If compulsory coin caught
        lasso.incrementCoins(2);               //2 points for the Compulsory Coin
        i = -1;
        break;
      default:
      break;

    }
    //Adding different types of coins which should be ejected, to the queue
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
    //Actually releasing the coins
    if(ToReleaseCoins.size()>0 && coinRadar.size()<7){
      int y = ToReleaseCoins.front();   //Picking the front of the queue
      ToReleaseCoins.pop();
      //Checking Various factors to see if its feasible to launch it and its not already there in large number
      if(Feasible(y)){
        Coin *newCoin = new Coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, false, rtheta);
        CoinCount[y]++;
        newCoin->initType(y);     //Initializing Type
        //Initializing its Trajectory depending upon its type
        if(y==0){
          newCoin->initTrajectory(rand()%4,levelTime);  //
        }else if(y==1){
          newCoin->initTrajectory((rand()%2)*2,levelTime);  //We don't want bombs to leave the screen early so no parabola/sinusoidal
        }else if(y==7){
          int l = rand()%3;
          if(l==2)l = 3;
          newCoin->initTrajectory(l,levelTime);       //We want the compulsory ones to leave screen early and be difficult to catch
        }else{
          newCoin->initTrajectory((rand()%2)*2+1,levelTime);    //Magnets and SpeedUps may also leave screen early
        }
        coinRadar.push_back(newCoin);
      }
    }
    //SpeedingUp or slowing down, if token is activated
    if(speedup){
      lasso.set_vx(lasso.get_vx()*2);
      lasso.set_vy(lasso.get_vy()*2);
    }
    if(slowdown){
      lasso.set_vx(lasso.get_vx()/2);
      lasso.set_vy(lasso.get_vy()/2);
    }
    lasso.nextStep(stepTime,true);
    //Compensating for the screw-up for speed due to those tokens
    if(speedup){
      lasso.set_vx(lasso.get_vx()/2);
      lasso.set_vy(lasso.get_vy()/2);
    }
    if(slowdown){
      lasso.set_vx(lasso.get_vx()*2);
      lasso.set_vy(lasso.get_vy()*2);
    }
    //Resetting the Speed/Magnet Property once the Time is up
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
    for(size_t o = 0;o<coinRadar.size();o++){     //running the coin on the trajectory
      double X = lasso.getXPos();
      double Y = lasso.getYPos();
      coinRadar[o]->nextStep(stepTime,false);     //Running according to velocities
      coinRadar[o]->runTrajectory(levelTime);   //Changing veloctiy values according to trajectory
      if(attract || repel){     //Taking care of magnets
        //Finding the directions in which they should move due to magnet
        double V_X, V_Y;
        V_X = X - coinRadar[o]->getXPos();
        V_Y = Y - coinRadar[o]->getYPos();
        double mod = sqrt(V_X*V_X + V_Y*V_Y);
        V_X/=mod;
        V_Y/=mod;
        V_X*=COIN_SPEED*2;
        V_Y*=COIN_SPEED*2;
        //Superposing magnet-caused velocity with actual velocity
        if((coinRadar[o]->getType()==1 && attract) || (coinRadar[o]->getType()!=1 && repel) ){
          coinRadar[o]->set_vx(coinRadar[o]->get_vx() - V_X);
          coinRadar[o]->set_vy(coinRadar[o]->get_vy() - V_Y);
        }else{
          coinRadar[o]->set_vx(coinRadar[o]->get_vx() + V_X);
          coinRadar[o]->set_vy(coinRadar[o]->get_vy() + V_Y);
        }
      }
    }
    //Removing Coins which went outside of the boundaries
    for(size_t h = 0;h<coinRadar.size();h++){
      double XI = coinRadar[h]->getXPos();
      double YI = coinRadar[h]->getYPos();
      if(XI<PLAY_X_START || XI>WINDOW_X || YI>PLAY_Y_HEIGHT || YI<0){
        coinRadar[h]->hideObject();
        //If compulsory coin leaves the arena, then deduct lives
        if(coinRadar[h]->getType()==7){
          lives--;
          LivesChanged = true;
        }
        //Delete the coins which leave
        CoinCount[coinRadar[h]->getType()]--;
        delete coinRadar[h];
        coinRadar.erase(coinRadar.begin()+h);
      }
    }
    //Resetting the Number of Lives counter at the end of the play-screen
    if(level>=2 && LivesChanged){
      if(lives>0){
        while(lives>Lives.size()){
          Circle c;
          Lives.push_back(c);
          Lives[Lives.size()-1].reset(XSTART+120+((Lives.size()-1)*25),YFIXED,10);
          Lives[Lives.size()-1].setColor(COLOR(0,255,0));
          Lives[Lives.size()-1].setFill(true);
        }
        while(lives<Lives.size()){
          Lives[Lives.size()-1].hide();
          Lives.pop_back();
        }
        LivesChanged = false;
      }else{
        XEvent f;
        CalculateScore(lasso.getNumCoins(),levelTime,lives, level);
        GameOver(level,lasso, coinRadar,f);
        endgame = true;
      }
    }
    //Setting the Time-Left counter for The different Tokens
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
    //Updating the Coin-count
    sprintf(coinScoreStr, "%d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);
    //Levelling-Up/Ending the game, when the coin Requirement is met
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
      CalculateScore(lasso.getNumCoins(),levelTime,lives, level);
      LevelUp(++level,e);
      //Resetting everything when level is changed
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
    //Incrementing StepCount/LevelCount and stuff as these give an idea of time
    stepCount++;
    currTime += stepTime;
    levelTime+=stepTime;
    if(level==1)wait(0.25*(stepTime));
    else wait(0.1*(stepTime));
    if(endgame)break;     //end if game is ended
  } // End for(;;)

  wait(3);
} // End main_program
