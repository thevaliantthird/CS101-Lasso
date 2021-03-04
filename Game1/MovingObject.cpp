#include <simplecpp>
#include "MovingObject.h"

using namespace simplecpp;

//Hide All Parts
void MovingObject::hideObject(){
  for(size_t i= 0;i<parts.size();i++){
    parts[i]->hide();
  }
}

//Show all parts
void MovingObject::ShowObject(){
  for(size_t i = 0;i<parts.size();i++){
    parts[i]->show();
  }
}

/*whatis is true for lasso(lasso movement, everything happens here)
and false for coins(as their velocities are defined by the runTrajectory function)*/
void MovingObject::nextStep(double t, bool whatis) {
  if(paused) { return; }
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);
  }
  if(whatis){
    vx += ax*t;
    vy += ay*t;
  }
} // End MovingObject::nextStep()

double MovingObject::getXPos() {      //Getting the X-co-ordinate
  return (parts.size() > 0) ? parts[0]->getX() : -1;
}

double MovingObject::getYPos() {      //Getting the Y-co-ordinate
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}

//To reset any MovingObject
void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(argx, argy);
  }
  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
} // End MovingObject::reset_all()

//Get some MovingObject attached to someother MovingObject
void MovingObject::getAttachedTo(MovingObject *m) {
  double xpos = m->getXPos();
  double ypos = m->getYPos();
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(xpos,ypos);
    m->addPart(parts[i]);
  }
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}
