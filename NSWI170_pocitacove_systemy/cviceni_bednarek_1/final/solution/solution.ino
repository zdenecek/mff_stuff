
#include "funshield.h"
#include "input.h"

// #include "cstring"


// map of letter glyphs
constexpr byte letter_glyphs[]{
    0b10001000, // A
    0b10000011, // b
    0b11000110, // C
    0b10100001, // d
    0b10000110, // E
    0b10001110, // F
    0b10000010, // G
    0b10001001, // H
    0b11111001, // I
    0b11100001, // J
    0b10000101, // K
    0b11000111, // L
    0b11001000, // M
    0b10101011, // n
    0b10100011, // o
    0b10001100, // P
    0b10011000, // q
    0b10101111, // r
    0b10010010, // S
    0b10000111, // t
    0b11000001, // U
    0b11100011, // v
    0b10000001, // W
    0b10110110, // ksi
    0b10010001, // Y
    0b10100100, // Z
};
constexpr byte empty_glyph = 0b11111111;

constexpr int pows[] = {1, 10, 100, 1000};

int getGlyphByOrder(int num, int order)
{
  if(num == 0) {
    return order == 0? digits[0] : empty_glyph;
  }
  if(num < 0) {
    return empty_glyph;
  }
  int val = (num / pows[order]) % 10;

  byte glyph = digits[val];

  if(val == 0 && num < pows[order]) {
    glyph = empty_glyph;
  }
  return glyph;
}


struct Dice
{
  String description;
  int maxValue;
};

Dice available_dice[] = {
    {"d4 ", 4},
    {"d6 ", 6},
    {"d8 ", 8},
    {"d10", 10},
    {"d12", 12},
    {"d20", 20},
    {"d00", 100}};

size_t available_dice_count = sizeof(available_dice) / sizeof(available_dice[0]);


class SevenSegmentDisplay
{
public:
  static constexpr int led_count = 4;

  SevenSegmentDisplay(int latch_pin, int clock_pin, int data_pin, int glyph_period_in_ms)
      : clock_pin(clock_pin),
        latch_pin(latch_pin),
        data_pin(data_pin),
        glyph_period(glyph_period_in_ms)

  {
    for (int i = 0; i < led_count; i++)
    {
      state[i] = empty_glyph;
    }
  }

  void setup()
  {
    pinMode(latch_pin, OUTPUT);
    pinMode(clock_pin, OUTPUT);
    pinMode(data_pin, OUTPUT);

    last_change = 0;
    led_shown = 0;

    show(empty_glyph, 0);
  }

  void displayChar(char ch, int pos)
  {
    byte glyph = empty_glyph;
    if (isAlpha(ch)) {
      glyph = letter_glyphs[ch - (isUpperCase(ch) ? 'A' : 'a')];
    } else  if (isDigit(ch)) {
      glyph = digits[ch - '0'];
    }

    setGlyph(glyph, pos);
  }

  void displayNum(int num) {

    for(int i = 0; i < led_count; i++) {
      setGlyph(getGlyphByOrder(num, i), led_count - 1 - i);
    }
  }

  void setGlyph(byte glyph, int pos)
  {

    if (pos < 0 || pos > led_count)
      return;

    if(state[pos] == glyph) 
      return;

    state[pos] = glyph;
    changed = true;
  }

  void update(unsigned long time)
  {

    bool passed = (time - last_change) >= glyph_period;
    if (changed || passed)
    {
      changed = false;
      led_shown++;
      if (led_shown == led_count)
      {
        led_shown = 0;
      }

      show(state[led_shown], led_shown);
      last_change += glyph_period;
    }
  }

private:
  int latch_pin, clock_pin, data_pin;
  unsigned int glyph_period;
  unsigned long last_change;
  int led_shown;
  bool changed;
  byte state[led_count];

  void show(byte value, int led)
  {
    digitalWrite(latch_pin, HIGH); // open - vystupy kopiruji stav posuvnych registru

    // led = -led + led_count - 1;

    shiftOut(data_pin, clock_pin, MSBFIRST, value);             // 1 volani definuje segmenty
    shiftOut(data_pin, clock_pin, MSBFIRST, digit_muxpos[led]); // 2 volani definuje cislice

    digitalWrite(latch_pin, LOW); // closed
  }
};

class Button
{

private:
  unsigned long last_evaluation_time;
  bool last_result;

protected:
  int button_pin;
  bool previous_status;
  unsigned long last_change;
  unsigned long last_press;
  bool current_status;

  virtual bool determinePressed(unsigned long time)
  {
    return false;
  }

public:
  Button(int buttonPin) : button_pin(buttonPin),
                          previous_status(OFF),
                          last_change(0),
                          last_press(0)
  {
  }

  void setup()
  {
    pinMode(button_pin, INPUT);
  }

  bool pressed(unsigned long time)
  {
    // avoid evaluating multiple times
    if (time == last_evaluation_time) {
      return last_result;
    }

    current_status = digitalRead(button_pin);


    if (current_status != previous_status) {

      last_change = time;
    }

    bool ret = determinePressed(time);

    if (ret == true) {
      last_press = time;
    }

    previous_status = current_status;

    last_evaluation_time = time - 1; // in case more updates with same time take place
    last_result = ret;

    return ret;
  }
};

class DebouncedButton : public Button
{
public:
  DebouncedButton(int buttonPin) : Button(buttonPin)
  {
    already_pressed = false;
  }

protected:
  static unsigned long constexpr debounce = 100;
  bool already_pressed;

  virtual bool determinePressed(unsigned long time)
  {

    if (current_status == ON && previous_status == ON && !already_pressed)
    {
      
      if (time - last_change >= debounce)
      {
        already_pressed = true;
        return true;
      }
    }
    else if (current_status == OFF && previous_status == ON) {
      already_pressed = false;
    }

    return false;
  }
};

class HoldableButton : public Button
{
public:
  HoldableButton(int buttonPin) : Button(buttonPin)
  {
  }

protected:

  virtual bool determinePressed(unsigned long time)
  {
    return current_status == ON;
  }
};


constexpr byte frames[][SevenSegmentDisplay::led_count] = {
    { 0b11111110, 0b11111110, 0b11111110, 0b11111110 },
    { 0b11111101, 0b11111101, 0b11111101, 0b11111101 },
    { 0b11111011, 0b11111011, 0b11111011, 0b11111011 },
    { 0b11110111, 0b11110111, 0b11110111, 0b11110111 },
    { 0b11101111, 0b11101111, 0b11101111, 0b11101111 },
    { 0b11011111, 0b11011111, 0b11011111, 0b11011111 }
  };
constexpr int animation_frame_length = sizeof(frames)/sizeof(frames[0]);

class RollingAnimation
{

  private:
  byte state[SevenSegmentDisplay::led_count] = { 0, 0, 0, 0 };
  int frame;
  static constexpr int period = 250;


  unsigned long lastChange = 0;

  void advanceCycle() {
    frame++;
    if(frame == animation_frame_length) {
      frame = 0;
    }
    for(int i = 0; i < SevenSegmentDisplay::led_count; i++) {
      state[i] = frames[frame][i];
    }
  }

  public:
  void resetAnimation(unsigned long time)
  {
    lastChange = time;
    frame = 0;
    for(int i = 0; i < SevenSegmentDisplay::led_count; i++) {
      state[i] = frames[0][i];
    }
  }

  byte *getAnimationFrame(unsigned long time)
  {
    if(time - lastChange >= period) {
      advanceCycle();
      lastChange += period;
    }
    return state;
  }
};

class DiceThrower {

  public:

  static int getResult(Dice* dice, unsigned long seed, int throws) {
    srand(seed);
    int cummulative = 0;
    for(int i = 0; i < throws; i++) {
      cummulative += (rand() % dice->maxValue) + 1;
    }
    return cummulative;
  }

};

enum DiceMachineState
{
  Rolling,
  ShowingResult,
  ShowingConfig
};


class DiceMachine
{

private:
  SevenSegmentDisplay *display;
  Button *rollButton;
  Button *throwsButton;
  Button *diceButton;
  RollingAnimation animation;

  DiceMachineState state;
  int throws;
  int die;
  unsigned long startRollTime;

  static constexpr int maxThrows = 9;

  void incrementThrows()
  {
    throws++;
    if (throws > maxThrows) {
      throws = 1; 
    }
  }

  void changeDie()
  {
    die++;
    if (die == available_dice_count)
      die = 0;
  }

  void startRolling(unsigned long time)
  {
    state = Rolling;
    startRollTime = time;
  }

  void stopRolling(unsigned long time)
  {
    state = ShowingResult;
    
    int result = DiceThrower::getResult(&available_dice[die], time - startRollTime, throws);
    
    display->displayNum(result);
  }

  void displayAnimation(unsigned long time)
  {
    byte* glyphs = animation.getAnimationFrame(time);
    for(int i = 0; i < SevenSegmentDisplay::led_count; i++) {
      display->setGlyph(glyphs[i], i);
    }
  }

  void goToConfigMode()
  {
    state = ShowingConfig;
    display->displayChar('0' + throws, 0);
    display->displayChar(available_dice[die].description[0], 1);
    display->displayChar(available_dice[die].description[1], 2);
    display->displayChar(available_dice[die].description[2], 3);
  }

public:
  DiceMachine(SevenSegmentDisplay *display, Button *rollButton, Button *throwsButton, Button *diceButton)
      : display(display), rollButton(rollButton), throwsButton(throwsButton), diceButton(diceButton), animation()
  {
    die = 0;
    throws = 1;
  }

  void setup()
  {
    display->setup();
    rollButton->setup();
    throwsButton->setup();
    diceButton->setup();
    goToConfigMode();
  }

  void update(unsigned long time)
  {

    if (state == Rolling)
    {
      if (!rollButton->pressed(time))
      {
        stopRolling(time);
      }
      else
      {
        displayAnimation(time);
      }
    }
    else
    {
      if (rollButton->pressed(time))
      {
        startRolling(time);
        displayAnimation(time);
      }
      else if (diceButton->pressed(time))
      {
        changeDie();
        goToConfigMode();

      }
      else if (throwsButton->pressed(time))
      {
        incrementThrows();
        goToConfigMode();
      }
    }

    display->update(time);
  }
};

SevenSegmentDisplay display = SevenSegmentDisplay(latch_pin, clock_pin, data_pin, 5);
HoldableButton b1 = HoldableButton(button1_pin);
DebouncedButton b2 = DebouncedButton(button2_pin);
DebouncedButton b3 = DebouncedButton(button3_pin);
DiceMachine diceMachine = DiceMachine(&display, &b1, &b2, &b3);

void setup()
{
  diceMachine.setup();
}

void loop()
{
  unsigned long time = millis();

  diceMachine.update(time);  
}
