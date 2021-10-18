/*
  Control Box Software
  Team: T4CB
  Members: Hong Lim, Maisha Famida Rahman, Eric Huang, Syed Muneeb Ahmed Hashmi, Xuanzi Liu
*/

#define ANALOG_JOY_MIN 203
#define ANALOG_JOY_MAX 797
#define JOY_BT_MIN 0
#define JOY_BT_MAX 255

#define BT_RX_PIN 9
#define BT_TX_PIN 8

#define BTN_MOVEMENT_PIN 4
#define BTN_DOOR_PIN 5
#define BTN_ESTOP_PIN 6

#define JOYL_X_PIN 2
#define JOYL_Y_PIN 3
#define JOYR_X_PIN 0
#define JOYR_Y_PIN 1

#include "StorageService.h";
#include "UnoPayload.h";
#include "MegaPayload.h";
#include "InputHandling.h";

StorageService storageService;

unsigned long currMillis = 0;

unsigned long prevBtnMillis = 0;
unsigned long prevJoyMillis = 0;
unsigned long prevBTMillis = 0;

unsigned long btnUpdateInterval = 50;
unsigned long joyUpdateInterval = 50;
unsigned long BTUpdateInterval = 100;

// instaniate sensors
Button s1(4); // movement
Button s2(5); // door
Button s3(6); // emergency
Button s4(7); // unassigned
Joystick j1(A2, A3);
Joystick j2(A0, A1);

void setup()
{
  setupBT();
}

void loop()
{
  currMillis = millis();

  // if (BT.connection()) // checks comms link
  // event 1: Handle buttons
  if (currMillis - prevBtnMillis >= btnUpdateInterval)
  {
    s1.refresh();
    s2.refresh();
    s3.refresh();
    // s4.refresh();

    if (s1.hasChanged())
    { // setUnoBtns(bool movement, bool door, bool emergency);
      // comment by Eric: StorageService consider storing <int> instead of <bool>
      // comment by Eric: StorageService needs methods setting sensor values individually
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
  }

  // event 2: Handle joysticks
  // comment by Eric: consider integrating event 2 into event 1
  if (currMillis - prevJoyMillis >= joyUpdateInterval)
  {
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
}

/*
    Delievery Controller
*/
void setupBT()
{
  // Initialising BT module to allocated pins
  // BT.setup(BT_RX_PIN, BT_TX_PIN);
}

void sendPayload()
{
  // Mapping of joystick analog values to Bluetooth target values
  byte joy1 = map(storageService.getUnoJoy1, ANALOG_JOY_MIN, ANALOG_JOY_MAX, JOY_BT_MIN, JOY_BT_MAX);
  byte joy2 = map(storageService.getUnoJoy2, ANALOG_JOY_MIN, ANALOG_JOY_MAX, JOY_BT_MIN, JOY_BT_MAX);

  // Retrieving the rest of the payload and send it off via Bluetooth
  // BT.send(joy1, joy2, storageService.getUnoMovementState(), storageService.getUnoDoorState(), storageService.getUnoEmergencyState());
}
