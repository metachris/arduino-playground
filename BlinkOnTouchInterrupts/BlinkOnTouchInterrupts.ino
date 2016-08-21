#include "TimerOne.h"
#include "pitches.h"

const int LED1 = 0;
const int BTN1 = 2;

const int LED2 = 3;
const int BTN2 = 4;

const int LED3 = 6;
const int BTN3 = 7;

const int PIN_SOUND = 23;

// Debounce memory
volatile unsigned long last_millis_btn1 = 0;
volatile unsigned long last_millis_btn2 = 0;
volatile unsigned long last_millis_btn3 = 0;

// Debouncing time in milliseconds
const long DEBOUNCING_TIME = 700;

// Blink variables
const int BLINK_TIMES = 3;      // how often it should blink after a button
volatile int blink_pin = LED1;  // will be overwritten depending on the button
volatile int blink_cnt = 0;     // number of state changes since button

// Melodies
int melodyStart[] = { NOTE_E4, NOTE_E5 };
int noteDurationsStart[] = { 8, 4 };

int melodyButton[] = { NOTE_E5, NOTE_E5 };
int noteDurationsButton[] = { 8, 8 };

// Signal from interrupt to main loop for playing sounds
volatile int soundState = 0;

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
    attachInterrupt(digitalPinToInterrupt(BTN1), btn1, LOW);
    attachInterrupt(digitalPinToInterrupt(BTN2), btn2, LOW);
    attachInterrupt(digitalPinToInterrupt(BTN3), btn3, LOW);

    // Startup sound
    playSoundStart();
}

void btn1() {
    if (millis() - last_millis_btn1 >= DEBOUNCING_TIME) {
        blink(LED1);
        last_millis_btn1 = millis();
    }
}

void btn2() {
    if (millis() - last_millis_btn2 >= DEBOUNCING_TIME) {
        blink(LED2);
        last_millis_btn2 = millis();
    }
}

void btn3() {
    if (millis() - last_millis_btn3 >= DEBOUNCING_TIME) {
        blink(LED3);
        last_millis_btn3 = millis();
    }
}

// the loop function runs over and over again forever
void loop() {
    if (soundState == 1) {
        soundState = 0;
        playSoundButton();
    }
    delay(100);
    // Serial.println("foo");
    // delay(1000);
    // Serial.println("bar");
    // delay(1000);
}

void blink(int led_pin) {
    stopBlink();
    blink_pin = led_pin;
    blink_cnt = 0;
    Timer1.attachInterrupt(isrBlink);
    soundState = 1;
}

void stopBlink() {
    Timer1.detachInterrupt();
    digitalWrite(blink_pin, LOW);
}

void isrBlink() {
    blink_cnt++;
    digitalWrite(blink_pin, blink_cnt % 2 == 1 ? HIGH : LOW);
    if (blink_cnt == BLINK_TIMES * 2) {
        stopBlink();
    }
}

void playSound(int melody[], int durations[], int length) {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < length; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/durations[thisNote];
    tone(PIN_SOUND, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(PIN_SOUND);

    if (soundState != 0) {
        // new playback should start. quit current.
        return;
    }
  }
}

void playSoundStart() {
    playSound(melodyStart, noteDurationsStart, 2);
}

void playSoundButton() {
    playSound(melodyButton, noteDurationsButton, 2);
}
