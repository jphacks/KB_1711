#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>

// any pins can be used
#define SHARP_SCK  13
#define SHARP_MOSI 11
#define SHARP_SS   10

// Set the size of the display here, e.g. 144x168!
Adafruit_SharpMem display(SHARP_SCK, SHARP_MOSI, SHARP_SS, 96, 96);

#define BLACK 0
#define WHITE 1

#define NEUTRAL 1
#define HAPPY   2
#define SAD     3
#define ANGER   4
#define FEAR    5

// LCD
int minorHalfSize;

// ADC
int bt_in_pin = A0;
int signal_val = 0;
int bit_length = 5;

// servo
Servo myservo;
int servo_pin = 5;
int servo_angle = 0;
int servo_scaler = 0;

int category = 0;
int level = 0;

void setup() {
  // assign servo_pin for myServo
  myservo.attach(servo_pin);
  myservo.write(servo_angle); // position reset

  display.begin();
  display.clearDisplay();

  minorHalfSize = min(display.width(), display.height()) / 2;

  draw_jphacks();
  display.refresh();
  delay(1000);
  display.clearDisplay();
}

// nutral happy sad anger fear
void loop() {
  int i;
  int data_buffer[bit_length];

  // start up signal
  wait_for_rising();
  delay(40);
  wait_for_falling();
  delay(105); // 先頭ビットが取れないならこの辺を調整

  // read 5-bit
  for (i = 0; i < bit_length; i++) {
    data_buffer[i] = read_signal(20); // 値が踊るならこの値を調整(判定時のサンプル数)
    delay(120); // データが途中で乱れたらこの値を調整(1bitぶんのフレーム長)
  }

  // print result
  display.setTextSize(1);
  for (i = 0; i < bit_length; i++) {
    display.setCursor(10 * i, 80);
    //display.print(data_buffer[i]);////DEBUGDEBUGDEBUG
  }
  display.refresh();

  // calc category val
  category = 0;
  for (i = 0; i < 3; i++) {
    category += (data_buffer[i] << (2 - i));
  }
  display.setCursor(0, 0);
  //display.print(category);  //DEBUGDEBUGDEBUG

  // calc level val
  level = 0;
  level += data_buffer[3] << 1;
  level += data_buffer[4];

  display.setCursor(0, 15);
  display.print(level);

  // draw kao-moji
  switch (category) {
    case NEUTRAL: // n
      display.clearDisplay();
      display.setTextSize(3);
      display.setRotation(2);

      display.setCursor(15, 23);
      display.print(".");

      display.setCursor(64, 23);
      display.print(".");

      display.setCursor(41, 60);
      display.print("-");
      display.refresh();
      delay(2000);
      category = 2;
      break;
    case HAPPY: // h
      display.clearDisplay();
      display.setTextSize(3);
      display.setRotation(3);

      display.setCursor(20, 15);
      display.print("<");

      display.setCursor(20, 63);
      display.print("<");

      display.setCursor(60, 38);
      display.print("D");
      display.refresh();
      delay(2000);
      category = 3;
      break;
    case SAD: // s
      display.clearDisplay();
      display.setTextSize(3);
      display.setRotation(2);

      display.setCursor(15, 23);
      display.print("T");

      display.setCursor(64, 23);
      display.print("T");

      display.setRotation(3);

      display.setCursor(60, 38);
      display.print("(");
      display.refresh();
      delay(2000);
      category = 4;
      break;
    case ANGER: // a
      display.clearDisplay();
      display.setTextSize(3);
      display.setRotation(2);

      display.setCursor(15, 23);
      display.print(".");

      display.setCursor(64, 23);
      display.print(".");

      display.setRotation(3);

      display.setCursor(19, 13);
      display.print("\\");

      display.setCursor(19, 63);
      display.print("/");

      display.setCursor(60, 38);
      display.print("(");

      display.refresh();
      delay(2000);
      category = 5;
      break;
    case FEAR: // f
      display.clearDisplay();
      display.setTextSize(3);
      display.setRotation(2);

      display.setCursor(15, 23);
      display.print(".");

      display.setCursor(64, 23);
      display.print(".");

      display.setCursor(40, 60);
      display.print("o");

      display.setRotation(3);

      display.setCursor(19, 13);
      display.print("/");

      display.setCursor(19, 63);
      display.print("\\");

      display.refresh();

      category = 1;
      delay(2000);
      break;
  }

  // move servo
  switch (category) {
    case NEUTRAL:
//      myservo.write(0);
      break;
    case HAPPY:
//      myservo.write(30);
//      myservo.write(servo_scaler * level / 2);
//      delay(250);
//      myservo.write(servo_scaler * level / 4);
//      delay(250);
//      myservo.write(servo_scaler * level);
//      delay(250);
//      myservo.write(0);
      break;
    case SAD:
      //servo.write(servo_scaler * level);
      break;
  }
}


void wait_for_rising() {
  do {
    delay(1);
    signal_val = analogRead(bt_in_pin);
    //if(signal_val != 0) Serial.println(signal_val);
  } while (signal_val < 50);

  return;
}

void wait_for_falling() {
  do {
    delay(1);
    signal_val = analogRead(bt_in_pin);
    //if(signal_val != 0) Serial.println(signal_val);
  } while (signal_val >= 10);

  return;
}

int read_signal(int sample) {
  int count;
  long buf = 0;
  int ave = 0;

  for (count = 0; count < sample; count++) {
    buf += analogRead(bt_in_pin);
    delay(1);
  }

  ave = buf / sample;
  Serial.print("[Ave]");
  Serial.print(ave);
  Serial.print("\t:");

  if (ave < 5) {
    return 0;
  }
  else {
    return 1;
  }
}

void draw_jphacks() {
  display.clearDisplay();
  // text display tests
  display.setTextSize(2);
  display.setRotation(2);
  display.setTextColor(BLACK);
  display.setCursor(6, 30);
  display.println("JPHACKS");
  display.setCursor(25, 50);
  display.println("2017");
  display.refresh();
}

