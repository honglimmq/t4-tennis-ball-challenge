// Control Box Software
// Team: T4CB
// Members: Hong Lim, Maisha Famida Rahman, Eric Huang, Syed Muneeb Ahmed Hashmi, Xuanzi Liu

#define DEBUG 1
#define BAUD_RATE 9600

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
unsigned long prevLCDMillis = 0;
unsigned long prevRecieveMillis = 0;

unsigned long btnUpdateInterval = 50;
unsigned long joyUpdateInterval = 50;
unsigned long BTUpdateInterval = 100;
unsigned long lcdUpdateInterval = 500;
unsigned long ReceiveUpdateInterval = 100;

Button s1(BTN_MOVEMENT_PIN); // movement
Button s2(BTN_DOOR_PIN);     // door
Button s3(BTN_ESTOP_PIN);    // emergency
// Button s4();                 // not assigned
Joystick j1(JOYL_X_PIN, JOYL_Y_PIN);
Joystick j2(JOYR_X_PIN, JOYR_Y_PIN);
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_CHAR, LCD_ROW);

void setup()
{
  // initalize serial monitor
  Serial.begin(BAUD_RATE);
  Serial.println("Welcome!");

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

  #if DEBUG
    Serial.println("Entered DEBUG mode");
  #endif

  #if DEBUG
    if(connectionStatus()){
      Serial.println("BT: The connection link is established");
    }
    else{
      Serial.println("BT: The connection link is down");
    }
  #endif
}

void loop()
{
  currMillis = millis();

  // Checks comms link
  if (connectionStatus())
  {
    // Event 1: Handle buttons
    if (currMillis - prevBtnMillis >= btnUpdateInterval)
    {
      // Refresh sensor data
      s1.refresh();
      s2.refresh();
      s3.refresh();
      // s4.refresh();

      // Movement button
      if (s1.hasChanged())
      {
        storageService.setUnoMovementState(s1.VALUE);
        #if DEBUG
                Serial.println("BUTTON: Movement button has been pressed");
        #endif
      }

      // Door button
      if (s2.hasChanged())
      {
        storageService.setUnoDoorState(s2.VALUE);
        #if DEBUG
                Serial.println("BUTTON: Door button has been pressed");
        #endif
      }

      // Emergency button
      if (s3.hasChanged())
      {
        storageService.setUnoEmergencyState(s3.VALUE);
        #if DEBUG
                Serial.println("BUTTON: Emergency button has been pressed");
        #endif
      }
      
      prevBtnMillis = currMillis;
    }

    // Event 2: Handle joysticks
    if (currMillis - prevJoyMillis >= joyUpdateInterval)
    {
      // refresh sensor data before use
      j1.Y.refresh();
      j2.Y.refresh();

      // Left joystick
      if (j1.Y.hasChanged())
      {
        storageService.setUnoJoys(j1.Y.VALUE, j2.Y.VALUE);
      }

      // Right joystick
      if (j2.Y.hasChanged())
      {
        storageService.setUnoJoys(j1.Y.VALUE, j2.Y.VALUE);
      }
      #if DEBUG
           Serial.print("JOYSTICK: Left Joy = ");
           Serial.print(storageService.getUnoJoy1);
           Serial.print(" Right Joy = ");
           Serial.print(storageService.getUnoJoy2);
           Serial.print("\n");
      #endif
      prevJoyMillis = currMillis;
    }

    // Event 3: Send payload to the robot
    if (currMillis - prevBTMillis >= BTUpdateInterval)
    {
      sendPayload();
      prevBTMillis = currMillis;
    }

    // Event 4: Receieve and store payload from the robot
    if (currMillis - prevReceiveMillis >= ReceiveUpdateInterval) {
      recievePayload();
      prevReceiveMillis = currMillis;
    }

    // Last event: Refresh LCD
    if (currMillis - prevLCDMillis >= lcdUpdateInterval)
    {
      lcd.clear();
      lcd.setCursor(0, 0); // going to start of the 1st line
      lcd.print("Hello World");
      lcd.setCursor(0, 1); // going to start of the 2nd line
      prevLCDMillis = currMillis;
    }
  } else {
    #if DEBUG
      Serial.println("BT: The connection link is down");
    #endif
  }
}

/*
    Delievery Controller
*/
void setupBT()
{
  // Initialising BT module (if the pin change, need to change directly in BT library)
  BLE_UNO_init();
}

void sendPayload()
{
  // Mapping of joystick analog values to Bluetooth target values
  byte joy1 = map(storageService.getUnoJoy1(), ANALOG_JOY_MIN, ANALOG_JOY_MAX, JOY_BT_MIN, JOY_BT_MAX);
  byte joy2 = map(storageService.getUnoJoy2(), ANALOG_JOY_MIN, ANALOG_JOY_MAX, JOY_BT_MIN, JOY_BT_MAX);

  // Retrieving the rest of the payload and send it off via Bluetooth
  sendData(joy1, joy2, storageService.getUnoEmergencyState(), storageService.getUnoMovementState(), storageService.getUnoDoorState());
}
/*
    Response Manager
*/
void recievePayload() {
  // Recieve payload using BT library functions and store it for later use
  storageService.setMegaPayload(rpm_l(), rpm_r(), door_state(), moving_status(), feedback_status());
}
