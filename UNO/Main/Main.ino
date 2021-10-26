// Control Box Software
// Team: T4CB
// Members: Hong Lim, Maisha Famida Rahman, Eric Huang, Syed Muneeb Ahmed Hashmi, Xuanzi Liu

#define ANALOG_JOY_MIN 203
#define ANALOG_JOY_MAX 797
#define JOY_BT_MIN 0
#define JOY_BT_MAX 255

#define BT_RX_PIN 9
#define BT_TX_PIN 8

#define BTN_MOVEMENT_PIN 4
#define BTN_DOOR_PIN 5
#define BTN_ESTOP_PIN 6

#define JOYL_X_PIN A2
#define JOYL_Y_PIN A3
#define JOYR_X_PIN A0
#define JOYR_Y_PIN A1

#define LCD_SCL 3
#define LCD_SDA 4
#define LCD_ADDR 0x27
#define LCD_CHAR 16
#define LCD_ROW 2

#include "StorageService.h"
#include "UnoPayload.h"
#include "MegaPayload.h"
#include "InputHandling.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

StorageService storageService;

unsigned long currMillis = 0;

unsigned long prevBtnMillis = 0;
unsigned long prevJoyMillis = 0;
unsigned long prevBTMillis = 0;

unsigned long btnUpdateInterval = 50;
unsigned long joyUpdateInterval = 50;
unsigned long BTUpdateInterval = 100;
unsigned long lcdUpdateInterval = 500;

Button s1(BTN_MOVEMENT_PIN); // movement
Button s2(BTN_DOOR_PIN);     // door
Button s3(BTN_ESTOP_PIN);    // emergency
// Button s4();                 // not assigned
Joystick j1(JOYL_X_PIN, JOYL_Y_PIN);
Joystick j2(JOYR_X_PIN, JOYR_Y_PIN);
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_CHAR, LCD_ROW);

void setup()
{
  // initialize LCD
  lcd.begin();
  lcd.clear();
  lcd.print("Welcome");
  lcd.backlight();

  // initialize Bluetooth
  setupBT();

  // initialize input sensors
  s1.refresh();
  s2.refresh();
  s3.refresh();
  // s4.refresh();
  j1.refresh();
  j2.refresh();
}

void loop()
{
  currMillis = millis();

  // if (BT.connection()) // checks comms link
  if (connectionStatus())
  {
    // event 1: Handle buttons
    if (currMillis - prevBtnMillis >= btnUpdateInterval)
    {
      // refresh sensor data before use
      s1.refresh();
      s2.refresh();
      s3.refresh();
      // s4.refresh();

      if (s1.hasChanged())
      {
        // StorageService.setUnoBtns(bool movement, bool door, bool emergency);
        // consider using <int> instead of <bool>
        // needs methods setting sensor values individually
        // e.g. StorageService.setEmergencyState(s1.VALUE);
        storageService.setUnoBtns(s1.VALUE, s2.VALUE, s3.VALUE);
      }

      if (s2.hasChanged())
      {
        storageService.setUnoBtns(s1.VALUE, s2.VALUE, s3.VALUE);
      }

      if (s3.hasChanged())
      {
        storageService.setUnoBtns(s1.VALUE, s2.VALUE, s3.VALUE);
      }

      if (s4.hasChanged())
      {
      }
    }

    // event 2: Handle joysticks
    // comment by Eric: consider integrating event 2 into event 1
    if (currMillis - prevJoyMillis >= joyUpdateInterval)
    {
      // refresh sensor data before use
      j1.Y.refresh();
      j2.Y.refresh();

      if (j1.Y.hasChanged())
      {
        storageService.setUnoJoys(j1.Y.VALUE, j2.Y.VALUE);
      }

      if (j2.Y.hasChanged())
      {
        storageService.setUnoJoys(j1.Y.VALUE, j2.Y.VALUE);
      }
    }

    // event 3: Send payload to the robot
    if (currMillis - prevBTMillis >= BTUpdateInterval)
    {
      sendPayload();
    }

    // last event: Refresh LCD
    if (currMillis - prevBTMillis >= lcdUpdateInterval)
    {
      // LCD needs StorageService being finalised before retrieving data
      lcd.clear();
      lcd.setCursor(0, 0); // going to start of the 1st line
      lcd.print("Hello World");
      lcd.setCursor(0, 1); // going to start of the 2nd line
    }
  }
}

/*
    Delievery Controller
*/
void setupBT()
{
  // Initialising BT module to allocated pins
  // BT.setup(BT_RX_PIN, BT_TX_PIN);
  BLE_UNO_init();
}

void sendPayload()
{
  // Mapping of joystick analog values to Bluetooth target values
  byte joy1 = map(storageService.getUnoJoy1, ANALOG_JOY_MIN, ANALOG_JOY_MAX, JOY_BT_MIN, JOY_BT_MAX);
  byte joy2 = map(storageService.getUnoJoy2, ANALOG_JOY_MIN, ANALOG_JOY_MAX, JOY_BT_MIN, JOY_BT_MAX);

  // Retrieving the rest of the payload and send it off via Bluetooth
  // BT.send(joy1, joy2, storageService.getUnoMovementState(), storageService.getUnoDoorState(), storageService.getUnoEmergencyState());
  sendData(left_joystick, right_joystick, emergency_stop, start_stop, open_close);
}
