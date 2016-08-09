int LED1 = 0;
int BTN1 = 1;
//int BTN1_state_last = 1;     // previous state of the button

int LED2 = 3;
int BTN2 = 4;

int LED3 = 6;
int BTN3 = 7;

int btn_state_last = ~0;     // set all state to 1
int btn_game_target = 0;
int streak = 0;

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

void blinkNow(int pin) {
    for (int i=0; i<3; i++) {
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
            // blinkNow(LED1);
            if (btn_game_target == 1) {
                Serial.println("YEAH");
            } else {
                Serial.println(":(");
            }
            startRound();
        }

        if (btn2_state == LOW) {
            Serial.println("btn2 pressed");
            // blinkNow(LED2);
            if (btn_game_target == 2) {
                Serial.println("YEAH");
            } else {
                Serial.println(":(");
            }
            startRound();
        }

        if (btn3_state == LOW) {
            Serial.println("btn3 pressed");
            // blinkNow(LED3);
            if (btn_game_target == 3) {
                Serial.println("YEAH");
            } else {
                Serial.println(":(");
            }
            startRound();
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
    blinkNow(pin);
    digitalWrite(pin, HIGH);
}

void answer(int btn) {

}

void answerWrong() {

}
