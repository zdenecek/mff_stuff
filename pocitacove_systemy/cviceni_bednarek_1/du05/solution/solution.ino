

#include "funshield.h"

constexpr int pows[] = {1, 10, 100, 1000};
constexpr int digit_off = 10;

int getDigitByOrder(int num, int order)
{
  return (num / pows[order]) % 10;
}

class Button
{

private:
  int button_pin;
  bool previous_status;
  unsigned long last_change;
  static unsigned long constexpr period_inbetween = 0;
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

      if (since_change >= period_inbetween)
      {
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
  int led_count;
  unsigned int digit_period;
  int number;
  unsigned long last_change;
  int led_shown;
  bool changed;

  void setState(int value, int led)
  {
    digitalWrite(_latch_pin, HIGH); // open - vystupy kopiruji stav posuvnych registru

    led = -led + led_count - 1;
    auto to_display = digits[value];

    if (led == 2)
    {
      to_display ^= 1 << 7;
    }

    shiftOut(_data_pin, _clock_pin, MSBFIRST, to_display);        // 1 volani definuje segmenty
    shiftOut(_data_pin, _clock_pin, MSBFIRST, digit_muxpos[led]); // 2 volani definuje cislice

    digitalWrite(_latch_pin, LOW); // closed
  }

public:
  SevenSegmentDisplay(int latch_pin_, int clock_pin_, int data_pin_, int led_count_, int digit_period_in_ms)
  {
    _clock_pin = clock_pin_;
    _latch_pin = latch_pin_;
    _data_pin = data_pin_;
    led_count = led_count_;
    digit_period = digit_period_in_ms;
  }

  void setup()
  {
    pinMode(_latch_pin, OUTPUT);
    pinMode(_clock_pin, OUTPUT);
    pinMode(_data_pin, OUTPUT);

    last_change = 0;
    led_shown = 0;

    setNumber(0);
    setState(0, 0);
  }

  void setNumber(int value)
  {
    if (number == value)
      return;
    number = value;
    changed = true;
  }

  void update(unsigned long time)
  {

    bool passed = (time - last_change) >= digit_period;
    if (changed || passed)
    {
      changed = false;
      led_shown++;
      if (led_shown == led_count)
      {
        led_shown = 0;
      }

      int val = getDigitByOrder(number, led_shown);

      if(led_shown > 1 && number < pows[led_shown] ) {
        val = digit_off;
      }

      setState(val, led_shown);
      last_change += digit_period;
    }
  }
};

enum State
{
  Running,
  Stopped,
  Lapped
};

class MyStopwatch
{
private:
  unsigned long current_start;
  unsigned long memory;
  unsigned long lap_time;
  State state;

public:
  MyStopwatch()
  {
    memory = 0;
    current_start = 0;
    lap_time = 0;
    state = Stopped;
  }

  void start_stop(unsigned long time)
  {
    if (state == Running)
    {
      state = Stopped;
      memory = time - current_start;
    }
    else if (state == Stopped)
    {
      state = Running;
      current_start = time - memory;
    }
  }

  void lap(unsigned long time)
  {
    if (state == Running)
    {
      state = Lapped;
      lap_time = time - current_start;
    }
    else if (state == Lapped)
    {
      state = Running;
    }
  }

  void reset(unsigned long time)
  {
    if (state == Stopped)
    {
      memory = 0;
    }
  }

  unsigned long getTimeInMs(unsigned long time)
  {
    if (state == Running)
    {
      return time - current_start;
    }
    else if (state == Stopped)
    {
      return memory;
    }
    else if (state == Lapped)
    {
      return lap_time;
    }

    return 0; //shouldn't happen
  }

  int getTime(unsigned long time)
  {
    return (int)(getTimeInMs(time) / 100);
  }
};

Button start_stop_button = Button(button1_pin);
Button lap_button = Button(button2_pin);
Button reset_button = Button(button3_pin);
SevenSegmentDisplay display = SevenSegmentDisplay(latch_pin, clock_pin, data_pin, 4, 1);
MyStopwatch stopwatch = MyStopwatch();


void setup()
{
  start_stop_button.setup();
  lap_button.setup();
  reset_button.setup();

  display.setup();
}

void loop()
{
  unsigned long time = millis();

  if (start_stop_button.pressed(time))
  {
    stopwatch.start_stop(time);
  }
  if (lap_button.pressed(time))
  {
    stopwatch.lap(time);
  }
  if (reset_button.pressed(time))
  {
    stopwatch.reset(time);
  }

  display.setNumber(stopwatch.getTime(time));
  display.update(time);
}
