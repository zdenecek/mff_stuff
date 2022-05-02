

#include "funshield.h"

class Button
{

private:
  int button_pin;
  bool previous_status;
  unsigned long last_change;
  static unsigned long constexpr period_inbetween = 250;
  static bool constexpr allow_long_press = false;

public:
  Button(int buttonPin)
  {
    button_pin = buttonPin;
    previous_status = OFF;
    last_change = 0;
  }

  void setup()
  {
    pinMode(button_pin, INPUT);
  }

  bool pressed(unsigned long time)
  {
    bool current_status = (ON == digitalRead(button_pin)) ? ON : OFF;
    if (current_status == ON && previous_status == OFF)
    {
      last_change = time;
      previous_status = current_status;
      return true;
    }
    else if (allow_long_press && current_status == ON && previous_status == ON)
    {
      auto since_change = time - last_change;

      if(since_change >= period_inbetween) {
        last_change += period_inbetween;
        return true;
      }
      
    }
    else if (current_status == OFF && previous_status == ON)
    {
      previous_status = current_status;
    }

    return false;
  }
};

class SevenSegmentDisplay
{

private:
  int _latch_pin, _clock_pin, _data_pin;

public:
  int led_count = 4;

  SevenSegmentDisplay(int latch_pin_, int clock_pin_, int data_pin_)
  {
    _clock_pin = clock_pin_;
    _latch_pin = latch_pin_;
    _data_pin = data_pin_;
  }

  void setup()
  {
    pinMode(_latch_pin, OUTPUT);
    pinMode(_clock_pin, OUTPUT);
    pinMode(_data_pin, OUTPUT);

    setState(0,0);
  }

  void setState(int value, int led_shown)
  {
    digitalWrite(_latch_pin, HIGH); // open - vystupy kopiruji stav posuvnych registru

    led_shown = -led_shown + led_count - 1;

    shiftOut(_data_pin, _clock_pin, MSBFIRST, digits[value]);           // 1 volani definuje segmenty
    shiftOut(_data_pin, _clock_pin, MSBFIRST, digit_muxpos[led_shown]); // 2 volani definuje cislice

    digitalWrite(_latch_pin, LOW); // closed
  }
};

class State
{
public:
  int counters[5] = {0, 0, 0, 0, 0};

  void increment(int order)
  {
    counters[order]++;
    update();
  }
  void decrement(int order)
  {
    counters[order]--;
    update();
  }

private:
  void update()
  {
    for (int i = 0; i < 4; i++)
    { // intentionally don't go through the last index
      if (counters[i] == 10)
      {
        counters[i] = 0;
        counters[i + 1]++;
      }
      if (counters[i] == -1)
      {
        counters[i] = 9;
        counters[i + 1]--;
      }
    }
    counters[4] = 0;
  }
};

Button start_stop_button = Button(button1_pin);
Button lap_button = Button(button2_pin);
Button reset_button = Button(button3_pin);
SevenSegmentDisplay display = SevenSegmentDisplay(latch_pin, clock_pin, data_pin);

State state = State();

int current_order = 0;

Button buttons[] = {start_stop_button, lap_button, reset_button};

void setup()
{
  for (auto button : buttons)
  {
    button.setup();
  }
  display.setup();
}

void loop()
{
//  unsigned long time = millis();
  unsigned long time = 0;

  if (lap_button.pressed(time))
  {
    state.decrement(current_order);
  }
  if (start_stop_button.pressed(time))
  {
    state.increment(current_order);
  }

  if (reset_button.pressed(time))
  {
    current_order++;
    if (current_order == display.led_count)
      current_order = 0;
  }
  
  display.setState(state.counters[current_order], current_order);
}
