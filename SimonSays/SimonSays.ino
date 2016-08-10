#include "pitches.h"

int LED1 = 0;
int BTN1 = 1;

int LED2 = 3;
int BTN2 = 4;

int LED3 = 6;
int BTN3 = 7;

int PIN_SOUND = 23;

int btn_state_last = ~0;     // set all state to 1
int btn_game_target = 0;
int streak = 0;

// Melodies
// note durations: 4 = quarter note, 8 = eighth note, etc.:

// win
int melodyWin[] = { NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4 };
int noteDurationsWin[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

// loose
int melodyLoose[] = { NOTE_C4, NOTE_B3, NOTE_E3 };
int noteDurationsLoose[] = { 8, 8, 2 };

// // start
int melodyStart[] = { NOTE_E3, NOTE_C4 };
int noteDurationsStart[] = { 8, 4 };

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(9600);
    randomSeed(analogRead(0));

    // LED Output
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);

    // BTN Input
    pinMode(BTN1, INPUT);           // set pin to input
    digitalWrite(BTN1, HIGH);       // turn on pullup resistors

    pinMode(BTN2, INPUT);           // set pin to input
    digitalWrite(BTN2, HIGH);       // turn on pullup resistors

    pinMode(BTN3, INPUT);           // set pin to input
    digitalWrite(BTN3, HIGH);       // turn on pullup resistors

    btn_state_last = readButtons();
    playSoundStart();
    startRound();
}

int readButtons() {
    int btn_state = digitalRead(BTN1);
    btn_state |= digitalRead(BTN2) << 1;
    btn_state |= digitalRead(BTN3) << 2;
    // Serial.print("btn state: ");
    // Serial.println(btn_state);
    return btn_state;
}

void blinkNow(int pin, int times) {
    for (int i=0; i<times; i++) {
        digitalWrite(pin, HIGH);
        delay(300);
        digitalWrite(pin, LOW);
        delay(300);
    }
}

void ledsAllOff() {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
}

// the loop function runs over and over again forever
void loop() {
    delay(10);        // delay [ms] between reads for stability

    // if (btn_game_target == 0) {
    // }
    //
    int btn_state = readButtons();
    if (btn_state != btn_state_last) {
        // State change
        Serial.print("state last: ");
        Serial.print(btn_state_last);
        Serial.print(" state: ");
        Serial.println(btn_state);

        btn_state_last = btn_state;

        // btn1?
        int btn1_state = btn_state & 1;
        int btn2_state = btn_state & 2;
        int btn3_state = btn_state & 4;

        if (btn1_state == LOW) {
            Serial.println("btn1 pressed");
            answer(1);
        }

        if (btn2_state == LOW) {
            Serial.println("btn2 pressed");
            answer(2);
        }

        if (btn3_state == LOW) {
            Serial.println("btn3 pressed");
            answer(3);
        }
    }
}

void startRound() {
    btn_game_target = random(1, 4);  // 1, 2 or 3
    Serial.print("new random: ");
    Serial.println(btn_game_target);
    int pin = 0;
    if (btn_game_target == 1) {
        pin = LED1;
    } else if (btn_game_target == 2) {
        pin = LED2;
    } else if (btn_game_target == 3) {
        pin = LED3;
    }

    ledsAllOff();
    // blinkNow(pin, 2);
    digitalWrite(pin, HIGH);
}

void answer(int btn) {
    blinkNow(pin, 2);
    if (btn == btn_game_target) {
        Serial.print("YEAH! Streak: ");
        Serial.println(streak);
        streak++;
        playSoundWin();
    } else {
        Serial.println(":(");
        streak = 0;
        playSoundLoose();
    }
    startRound();
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
  }
}

void playSoundWin() {
    playSound(melodyWin, noteDurationsWin, 8);
}

void playSoundLoose() {
    playSound(melodyLoose, noteDurationsLoose, 3);
}

void playSoundStart() {
    playSound(melodyStart, noteDurationsStart, 2);
}
