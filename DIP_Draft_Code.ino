#include <Arduino.h>
#include <TM1637Display.h>
#define CLK 12
#define DIO 13
#include <Thread.h>
Thread myThread = Thread();

TM1637Display display(CLK, DIO);
uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
uint8_t datanull[] = { 0x00, 0x00, 0x00, 0x00};
const uint8_t SEG_OFF[] = {
  0x00,
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_A | SEG_F | SEG_E | SEG_G,                   // F
  SEG_A | SEG_F | SEG_E | SEG_G                    // F
};
const uint8_t SEG_ON[] = {
  0x00,
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_E | SEG_G | SEG_C,                           // N
  0x00,
};
const uint8_t SEG_1[] = {
  SEG_C | SEG_D | SEG_E | SEG_F | SEG_G ,          // b
  SEG_A | SEG_F | SEG_E | SEG_D  ,                 // c
  SEG_G | SEG_D  ,                                 // =
  SEG_B | SEG_C                                    // 1
};
const uint8_t HINT[] = {
  SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,           // H
  SEG_B | SEG_C  ,                                 // 1
  SEG_E | SEG_G | SEG_C ,                          // n
  SEG_D | SEG_E | SEG_F | SEG_G                    // t
};

#define led1 2 //led(s),output [variable, pin.No]
#define led2 3
#define led3 4
#define led4 5
#define led5 6
#define led6 7
#define led7 8
#define led8 9
#define led9 10

#define switch1 A0 //toggle switches [variable,pin No.]
#define switch2 A1
#define switch3 A2
#define switch4 A3
#define switch5 A4
#define switch6 A5

//Array to light up  9 LEDs
int DEF[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int a1[] = {1, 1, 1, 0, 0, 1, 0, 0, 1};
int a2[] = {1, 1, 1, 1, 1, 0, 0, 0, 0};
int a3[] = {1, 1, 1, 0, 0, 0, 0, 1, 1};
int a4[] = {1, 0, 1, 1, 0, 1, 0, 1, 1};
int a5[] = {0, 1, 1, 1, 1, 1, 1, 1, 0};
int a6[] = {1, 1, 1, 1, 1, 0, 1, 0, 1};

//(a,b,c,d,e,f,g,1,1) 7 segment array
int no1[] = {0, 1, 1, 0, 0, 0, 0, 1, 1}; //1
int no2[] = {1, 1, 0, 1, 1, 0, 1, 1, 1}; //2
int no3[] = {1, 1, 1, 1, 0, 0, 1, 1, 1}; //3
int no4[] = {0, 1, 1, 0, 0, 1, 1, 1, 1}; //4
int no5[] = {1, 0, 1, 1, 0, 1, 1, 1, 1}; //5
int no6[] = {1, 0, 1, 1, 1, 1, 1, 1, 1}; //6
int no7[] = {1, 1, 1, 0, 0, 0, 0, 1, 1}; //7
int no8[] = {1, 1, 1, 1, 1, 1, 1, 1, 1}; //8
int no9[] = {1, 1, 1, 1, 0, 1, 1, 1, 1}; //9

int y1[9], y2[9], y3[9], y4[9], y5[9], y6[9], OUT[9];

void hint() {
  display.setSegments(HINT);
  delay (2000);
  display.setSegments(datanull);
  delay (1000);
  display.setSegments(SEG_1);
  delay (3500);
}
void copy (int array1[], int array2[], int array_size) { // copy array
  int i = 0;
  for (i; i < array_size; i++)
    array1[i] = array2[i];
}

void checkBtn() {
  {
    if (digitalRead(switch1) == LOW) {
      Serial.println("switch 1 toggled");
      copy(y1, a1, 9);
    }
    else
      copy(y1, DEF, 9);
  }
  {
    if (digitalRead(switch2) == LOW) {
      Serial.println("switch 2 toggled");
      copy(y2, a2, 9);
    }
    else
      copy(y2, DEF, 9);
  }
  {
    if (digitalRead(switch3) == LOW) {
      Serial.println("switch 3 toggled");
      copy(y3, a3, 9);
    }
    else
      copy(y3, DEF, 9);
  }
  {
    if (digitalRead(switch4) == LOW) {
      Serial.println("switch 4 toggled");
      copy(y4, a4, 9);
    }
    else
      copy(y4, DEF, 9);
  }
  {
    if (digitalRead(switch5) == LOW) {
      Serial.println("switch 5 toggled");
      copy(y5, a5, 9);
    }
    else
      copy(y5, DEF, 9);
  }
  {
    if (digitalRead(switch6) == LOW) {
      Serial.println("switch 6 toggled");
      copy(y6, a6, 9);
    }
    else
      copy(y6, DEF, 9);
  }
}

void XOR () { // operation for lighting the led by comparing various inputs using XOR
  int i = 0;
  for (i; i < 9; i++)
    OUT[i] = y1[i] ^ y2[i] ^ y3[i] ^ y4[i] ^ y5[i] ^ y6[i];
}
void celebrate() { //fancy lighting sequence to differentiate 4 passwords
  delay (600);
  for (int z = 2; z < 9; z++)
  {
    OUT[z - 2] = 0;
    digitalWrite(z, LOW);
  }
  delay (600);
  for (int x = 2; x < 9; x++)
  {
    OUT[x - 2] = 1;
    digitalWrite(x, HIGH);
    delay(100);
  }
  delay(600);
  for (int z = 2; z < 9; z++)
  {
    OUT[z - 2] = 0;
    digitalWrite(z, LOW);
  }
}
void displayNo(int no1[]) {
  int a = 2;
  for (a; a < 11; a++) {
    if (no1[a - 2] == 0)
      digitalWrite(a, LOW);
    else
      digitalWrite(a, HIGH);
  }
  delay (5000);
  for (int z = 2; z < 9; z++)
  {
    OUT[z - 2] = 0;
    digitalWrite(z, LOW);
  }
  delay (1000);
}

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);
  pinMode(led9, OUTPUT);
  pinMode(switch1, INPUT_PULLUP);
  pinMode(switch2, INPUT_PULLUP);
  pinMode(switch3, INPUT_PULLUP);
  pinMode(switch4, INPUT_PULLUP);
  pinMode(switch5, INPUT_PULLUP);
  pinMode(switch6, INPUT_PULLUP);
  display.setBrightness(0x0d);
  myThread.onRun(hint);
  display.setSegments(SEG_OFF);
  myThread.onRun(hint);
  myThread.setInterval(500);
}

int count = 0;
void loop() {
  checkBtn();
  XOR();
  int k = 2;
  for (k; k < 11; k++) {
    if (OUT[k - 2] == 0)
      digitalWrite(k, LOW);
    else
      digitalWrite(k, HIGH);
  }
  Serial.println(count);
  if (OUT[0] == 1 && OUT[1] == 1 && OUT[2] == 1 && OUT[3] == 1 &&  OUT[4] == 1 && OUT[5] == 1 && OUT[6] == 1 &&  OUT[7] == 1 && OUT[8] == 1) {
    if (count == 0) {
      display.setSegments(data);
      delay(600);
      display.setSegments(datanull);
      delay(600);
    }
    display.setSegments(SEG_ON);
    celebrate();
    delay (1000);
    displayNo(no2);
    displayNo(no3);
    displayNo(no9);
    displayNo(no8);
    if (count > 2) {
      myThread.run();
    }
    count ++;
  }
  else
    display.setSegments(SEG_OFF);
}
