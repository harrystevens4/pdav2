#include "encoder.h"

RotaryEncoder::RotaryEncoder(int clk, int sw, int dt) {
  
  this->clk = clk; this->sw = sw; this->dt = dt;
  
  pinMode(clk,INPUT);
  pinMode(sw,INPUT);
  pinMode(dt,INPUT_PULLUP);

}
int RotaryEncoder::get_direction() {
  static int prev_direction = 0;
  static int prev_state = 0; //00 for none on 01 for clk 10 for dt 11 for both
  int dt = !digitalRead(this->dt);
  int clk = !digitalRead(this->clk);
  //only return the direction once
  int no_change = (prev_direction != 0);
  //set new previous state
  int prev_dt = (prev_state & 2) >> 1;
  int prev_clk = prev_state & 1;
  prev_state = 0;
  prev_state |= dt << 1;
  prev_state |= clk;
  //figure out direction
  if (prev_clk == 1 && clk == 1 && dt == 1 && prev_dt == 0){
    return 1;
  }
  if (prev_dt == 1 && dt == 1 && prev_clk == 0 && clk == 1){
    return -1;
  }
  return 0;
}
int RotaryEncoder::get_button(){
  return 0;
}