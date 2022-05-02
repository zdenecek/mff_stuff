#include "funshield.h"

unsigned long last_time;

int program[] = {led1_pin, led2_pin, led3_pin, led4_pin, led3_pin, led2_pin};
constexpr size_t program_size = 6;

int program_pos;

int leds[] = {led1_pin, led2_pin, led3_pin, led4_pin};
constexpr size_t led_count = 4;

constexpr int period = 300;

void setup()
{
  program_pos = 0;
  
  for(size_t i = 0; i < led_count; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], OFF);
  }
  
  last_time = millis();
}

void loop()
{
  unsigned long time_now = millis();
  unsigned long delta = time_now - last_time;
  if (delta > period)
  {
    
    digitalWrite(program[program_pos], OFF);

    program_pos = (program_pos + 1) % program_size;

    digitalWrite(program[program_pos], ON);
  
    last_time = last_time + period;
  }
}
