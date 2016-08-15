#include "TimerOne.h"

int LED1 = 0;
int BTN1 = 2;

int LED2 = 3;
int BTN2 = 4;

int LED3 = 6;
int BTN3 = 7;

// Debounce memory
volatile unsigned long last_micros_btn1 = 0;
volatile unsigned long last_micros_btn2 = 0;
volatile unsigned long last_micros_btn3 = 0;

// Debouncing time in milliseconds
long debouncing_time = 700;

// Blink variables
volatile int blink_pin = LED1;
volatile int blink_cnt = 0;
volatile int blink_times = 3;

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(9600);

    // LED Output
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);

    // BTN Input
    pinMode(BTN1, INPUT_PULLUP);
    pinMode(BTN2, INPUT_PULLUP);
    pinMode(BTN3, INPUT_PULLUP);

    // Timer for LED blinking
    Timer1.initialize(1000000);

    // Button interrupts
    attachInterrupt(digitalPinToInterrupt(BTN1), btn1, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BTN2), btn2, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BTN3), btn3, CHANGE);
}

void btn1() {
    if((long)(micros() - last_micros_btn1) >= debouncing_time * 1000) {
        blink(LED1);
        last_micros_btn1 = micros();
    }
}

void btn2() {
    if((long)(micros() - last_micros_btn2) >= debouncing_time * 1000) {
        blink(LED2);
        last_micros_btn2 = micros();
    }
}

void btn3() {
    if((long)(micros() - last_micros_btn3) >= debouncing_time * 1000) {
        blink(LED3);
        last_micros_btn3 = micros();
    }
}

// the loop function runs over and over again forever
void loop() {
    Serial.println("foo");
    delay(1000);
    Serial.println("bar");
    delay(1000);
}

void blink(int led_pin) {
    stopBlink();
    blink_pin = led_pin;
    blink_cnt = 0;
    Timer1.attachInterrupt(isrBlink);
}

void stopBlink() {
    Timer1.detachInterrupt();
    digitalWrite(blink_pin, LOW);
}

void isrBlink() {
    blink_cnt++;
    digitalWrite(blink_pin, blink_cnt % 2 == 1 ? HIGH : LOW);
    if (blink_cnt == blink_times * 2) {
        stopBlink();
    }
}
