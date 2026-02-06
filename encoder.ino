#include "encoder.h"

RotaryEncoder::RotaryEncoder(int clk, int sw, int dt) {
  
  this->clk = clk; this->sw = sw; this->dt = dt;
  
  pinMode(clk,INPUT);
  pinMode(sw,INPUT);
  pinMode(dt,INPUT_PULLUP);

}
int RotaryEncoder::get_direction() {
  static int prev_state = 0;
  int dt = !digitalRead(this->dt);
  if (dt == 0){
    prev_state = 0;
    return 0;
  }
  int clk = !digitalRead(this->clk);
  //only return the direction once
  if (prev_state != 0) return 0;
  if (clk && dt) {
    prev_state = 1;
    return 1;
  }else {
    prev_state = -1;
    return -1;
  }
}
int RotaryEncoder::get_button(){
  return 0;
}