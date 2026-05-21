#include <Servo.h>
#include <LiquidCrystal.h>

// LCD: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

// LED-ek
const int greenLED[3] = {13, 10, 7};
const int redLED[3]   = {12, 9, 6};

// Gombok
const int buttonPin[3] = {11, 8, 5};

// Szervók
const int servoPin[3] = {4, 3, 2};
Servo servos[3];

// Rekeszek állapota
// true = foglalt, van csomag
// false = üres
bool occupied[3] = {true, true, true};

const int CLOSED_POS = 0;
const int OPEN_POS   = 90;

void updateLEDs() {
  for (int i = 0; i < 3; i++) {
    if (occupied[i]) {
      digitalWrite(redLED[i], HIGH);
      digitalWrite(greenLED[i], LOW);
    } else {
      digitalWrite(redLED[i], LOW);
      digitalWrite(greenLED[i], HIGH);
    }
  }
}

void printLine(byte row, const char* text) {
  lcd.setCursor(0, row);
  lcd.print("                ");
  lcd.setCursor(0, row);
  lcd.print(text);
}

void showStartupScreen() {
  lcd.clear();
  printLine(0, "Udvozoljuk!");
  printLine(1, "Csomagautomata");
  delay(2000);
  lcd.clear();
}

void showWelcome() {
  lcd.clear();
  printLine(0, "Valassz rekeszt");
  printLine(1, "1  2  3 gomb");
}

void showEmptyBox(int boxIndex) {
  lcd.clear();
  char line[17];
  snprintf(line, sizeof(line), "%d. rekesz ures", boxIndex + 1);
  printLine(0, line);
  printLine(1, "Nincs csomag");
  delay(1500);
  showWelcome();
}

void openBox(int boxIndex) {
  lcd.clear();
  char line[17];
  snprintf(line, sizeof(line), "%d. rekesz nyit", boxIndex + 1);
  printLine(0, line);
  printLine(1, "Ajto nyitasa");

  servos[boxIndex].write(OPEN_POS);
  delay(3000);

  lcd.clear();
  snprintf(line, sizeof(line), "%d. rekesz zar", boxIndex + 1);
  printLine(0, line);
  printLine(1, "Ajto zarasa");

  servos[boxIndex].write(CLOSED_POS);

  occupied[boxIndex] = false;
  updateLEDs();

  delay(1200);

  lcd.clear();
  snprintf(line, sizeof(line), "%d. rekesz ures", boxIndex + 1);
  printLine(0, line);
  printLine(1, "Atvetel kesz");

  delay(1500);
  showWelcome();
}

void waitButtonRelease(int pin) {
  while (digitalRead(pin) == LOW) {
    delay(10);
  }
  delay(50);
}

void setup() {
  delay(300);
  lcd.begin(16, 2);
  lcd.clear();

  for (int i = 0; i < 3; i++) {
    pinMode(greenLED[i], OUTPUT);
    pinMode(redLED[i], OUTPUT);
    pinMode(buttonPin[i], INPUT_PULLUP);
  }

  for (int i = 0; i < 3; i++) {
    servos[i].attach(servoPin[i]);
    servos[i].write(CLOSED_POS);
  }

  updateLEDs();
  showStartupScreen();
  showWelcome();
}

void loop() {
  for (int i = 0; i < 3; i++) {
    if (digitalRead(buttonPin[i]) == LOW) {
      waitButtonRelease(buttonPin[i]);

      if (occupied[i]) {
        openBox(i);
      } else {
        showEmptyBox(i);
      }
    }
  }
}