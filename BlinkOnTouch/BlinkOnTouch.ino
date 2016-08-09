int LED1 = 0;
int BTN1 = 1;
//int BTN1_state_last = 1;     // previous state of the button

int LED2 = 3;
int BTN2 = 4;

int LED3 = 6;
int BTN3 = 7;
int btn_state_last = ~0;     // set all state to 1

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(9600);

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
}


// the loop function runs over and over again forever
void loop() {
    delay(10);        // delay [ms] between reads for stability

    int btn_state = digitalRead(BTN1);
    btn_state |= digitalRead(BTN2) << 1;
    btn_state |= digitalRead(BTN3) << 2;

    if (btn_state != btn_state_last) {
        // State change
        Serial.print("state last: ");
        Serial.print(btn_state_last);
        Serial.print(" state: ");
        Serial.println(btn_state);

        // btn1?
        int btn1_state = btn_state & 1;
        int btn2_state = btn_state & 2;
        int btn3_state = btn_state & 4;

        //    boolean btn1_is_low = btn_state & 1 == LOW;
        //    Serial.print(old);
        //    Serial.print(" => ");
        //    Serial.println(btn1_state);
        //    Serial.print("btn1 is low: ");
        //    Serial.println(btn1_state == LOW);

        if (btn1_state == LOW) {
            Serial.println("btn1 low");
            blinkNow(LED1);
        }

        if (btn2_state == LOW) {
            Serial.println("btn2 low");
            blinkNow(LED2);
        }

        if (btn3_state == LOW) {
            Serial.println("btn3 low");
            blinkNow(LED3);
        }
    }

    btn_state_last = btn_state;
}

void blinkNow(int pin) {
    for (int i=0; i<3; i++) {
        digitalWrite(pin, HIGH);
        delay(300);
        digitalWrite(pin, LOW);
        delay(300);
    }
}
