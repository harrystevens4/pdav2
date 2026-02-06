#ifndef _ENCODER_H
#define _ENCODER_H

class RotaryEncoder {
private:
  int clk, sw, dt;
public:
  RotaryEncoder(int clk, int sw, int dt);
  int get_direction();
  int get_button();
};

#endif