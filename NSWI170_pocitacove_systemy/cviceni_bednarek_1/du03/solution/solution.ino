#include "funshield.h"

class Button
{

private:
    int button_pin;
    bool previous_status;
    bool quick_mode;
    unsigned long last_change;
    static unsigned long constexpr period_before_quick_mode = 1000;
    static unsigned long constexpr period_inbetween = 300;

public:
    Button(int buttonPin)
    {
        button_pin = buttonPin;
        previous_status = OFF;
        last_change = 0;
    }

    void setup() {
        pinMode(button_pin, INPUT);
    }

    bool pressed()
    {
        bool current_status = (ON == digitalRead(button_pin)) ? ON : OFF;
        if (current_status == ON && previous_status == OFF)
        {
            last_change = millis();
            previous_status = current_status;
            return true;
        }
        else if (current_status == ON && previous_status == ON)
        {
            auto time = millis();
            auto since_change = time - last_change;

            if (!quick_mode && since_change >= period_before_quick_mode)
            {
                last_change += period_before_quick_mode;
                quick_mode = true;
                return true;
            }
            else if (quick_mode && since_change >= period_inbetween)
            {
                last_change += period_inbetween;
                return true;
            }
        }
        else if (current_status == OFF && previous_status == ON)
        {
            quick_mode = false;
            previous_status = current_status;
        }

        return false;
    }
};

int leds[] = {led4_pin, led3_pin, led2_pin, led1_pin};
constexpr size_t led_count = sizeof(leds) / sizeof(leds[0]);

Button inc_button = Button(button1_pin);
Button dec_button = Button(button2_pin);

int counter = 0;
constexpr int max_count = (1 << led_count) - 1;

void setup()
{
    for (auto led : leds)
    {
        pinMode(led, OUTPUT);
        digitalWrite(led, OFF);
    }
    inc_button.setup();
    dec_button.setup();
}

void loop()
{
    // avoiding modulo
    bool change = false;
    if (inc_button.pressed())
    {
        change = true;
        if (++counter > max_count)
            counter = 0;
    }
    if (dec_button.pressed())
    {
        change = true;
        if (--counter < 0)
            counter = max_count;
    }
    if (change)
    {
        for (size_t i = 0; i < led_count; i++)
        {
            digitalWrite(leds[i], !((counter >> i) & 1));
        }
    }
}
