
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


class SevenSegmentDisplay
{

public:
  static constexpr int led_count = 4;

  SevenSegmentDisplay(int latch_pin_, int clock_pin_, int data_pin_, int glyph_period_in_ms)
  {
    _clock_pin = clock_pin_;
    _latch_pin = latch_pin_;
    _data_pin = data_pin_;
    glyph_period = glyph_period_in_ms;

    for(int i = 0; i < led_count; i++) {
      state[i] = empty_glyph;
    }
  }

  void setup()
  {
    pinMode(_latch_pin, OUTPUT);
    pinMode(_clock_pin, OUTPUT);
    pinMode(_data_pin, OUTPUT);

    last_change = 0;
    led_shown = 0;

    show(empty_glyph, 0);
  }

  void setGlyph(byte glyph, int pos) {

    if(pos < 0 || pos > led_count) return;
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
  int _latch_pin, _clock_pin, _data_pin;
  unsigned int glyph_period;
  unsigned long last_change;
  int led_shown;
  bool changed;
  byte state[led_count];

  void show(byte value, int led)
  {
    digitalWrite(_latch_pin, HIGH); // open - vystupy kopiruji stav posuvnych registru

    // led = -led + led_count - 1;

    shiftOut(_data_pin, _clock_pin, MSBFIRST, value);             // 1 volani definuje segmenty
    shiftOut(_data_pin, _clock_pin, MSBFIRST, digit_muxpos[led]); // 2 volani definuje cislice

    digitalWrite(_latch_pin, LOW); // closed
  }
};

class MessageDisplay
{
  static constexpr unsigned int scrollingInterval = 300;
  
  private:
    SevenSegmentDisplay *sevenSegmentDisplay;
    SerialInputHandler *handler;
    unsigned long last_change;
    int message_length;
    const char *message;
    int current_starting_index;

  public:

  MessageDisplay(SerialInputHandler *handler_, SevenSegmentDisplay *display_) 
  {
    sevenSegmentDisplay = display_;
    handler = handler_;
    last_change = 0;
    current_starting_index = -4;
  }

  void setup() {
    sevenSegmentDisplay->setup();
    message = handler->getMessage();
    message_length = strlen(message);
  }


  /**
   * Show chararcter on given position. If character is not letter, empty glyph is displayed instead.
   * @param ch character to be displayed
   * @param pos position (0 = leftmost)
   */
  void displayChar(char ch, byte pos)
  {
    byte glyph = empty_glyph;
    if (isAlpha(ch))
    {
      glyph = letter_glyphs[ch - (isUpperCase(ch) ? 'A' : 'a')];
    }

    sevenSegmentDisplay->setGlyph(glyph, pos);
  }

  void update (unsigned long time) {
    if(time - last_change >= scrollingInterval) {
      
      current_starting_index++;

      if(current_starting_index > message_length) {
        message = handler->getMessage();
        message_length = strlen(message);

        current_starting_index = -3;
      }

      for(int i = 0; i < SevenSegmentDisplay::led_count; i++) {
        
        int index = i + current_starting_index;
        char g = (index >= message_length || index < 0) ? empty_glyph : message[index];

        displayChar(g, i);
      }

      last_change += scrollingInterval;
    }

    sevenSegmentDisplay->update(time);
  }
};

SerialInputHandler input;
SevenSegmentDisplay sevenSegmentDisplay = SevenSegmentDisplay(latch_pin, clock_pin, data_pin, 1);
MessageDisplay display = MessageDisplay(&input, &sevenSegmentDisplay);

void setup()
{
  input.initialize();
  display.setup();
}

void loop()
{
  input.updateInLoop();
  unsigned long time = millis();

  display.update(time);
}
