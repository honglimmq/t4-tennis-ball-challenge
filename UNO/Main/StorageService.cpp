#include "Arduino.h"
#include "StorageService.h"
#include "UnoPayload.h"
#include "MegaPayload.h"

UnoPayload _unoPayload;
MegaPayload _megaPayload;

StorageService::StorageService() {
  // Initalising the payloads:
  // Uno payload
  _unoPayload._rawValY1 =  0;
  _unoPayload._rawValY2 = 0;
  _unoPayload._movementState = false;
  _unoPayload._doorState = false;
  _unoPayload._emergencyState = false;

  // Mega payload
  _megaPayload._rpmLeft = 0;
  _megaPayload._rpmRight = 0;
  _megaPayload._movementState = false;
  _megaPayload._doorState = false;
  _megaPayload._emergencyState = false;
  _megaPayload._robotFeedback = 0;
}

// UnoPayload setters
void setUnoPayload(int yVal1, int yVal2, bool movement, bool door, bool emergency) {
  // Joystick values
  _unoPayload._rawValY1 = (yVal1 >= 0) ? yVal1 : 0;
  _unoPayload._rawValY2 = (yVal2 >= 0) ? yVal2 : 0;

  // Buttons values
  _unoPayload._movementState = movement;
  _unoPayload._doorState = door;
  _unoPayload._emergencyState = emergency;
}

void setUnoPayload(struct UnoPayload uno) {
  setUnoPayload(uno._rawValY1, uno._rawValY2,
                _unoPayload._movementState, _unoPayload._doorState,
                _unoPayload._emergencyState); 
}

void setUnoJoys(int yVal1, int yVal2) {
  setUnoPayload(yVal1, yVal2,
                _unoPayload._movementState, _unoPayload._doorState,
                _unoPayload._emergencyState);
}

void setUnoBtns(bool movement, bool door, bool emergency) {
  setUnoPayload(_unoPayload._rawValY1, _unoPayload._rawValY2,
                movement, door,
                emergency);
}

// UnoPayload getters
struct UnoPayload getUnoPayload() {
  struct UnoPayload temp = {_unoPayload._rawValY1, _unoPayload._rawValY2,
           _unoPayload._movementState, _unoPayload._doorState,
           _unoPayload._emergencyState
  };
  return temp;
}

int getUnoJoy1() {
  return _unoPayload._rawValY1;
}
int getUnoJoy2() {
  return _unoPayload._rawValY2;
}
bool getUnoMovementState() {
  return _unoPayload._movementState;
}
bool getUnoDoorState() {
  return _unoPayload._doorState;
}
bool getUnoEmergencyState() {
  return _unoPayload._emergencyState;
}

// MegaPayload setters and getters
void setMegaPayload(int leftRPM, int rightRPM, bool movement, bool door, bool emergency, int feedback) {
  _megaPayload._rpmLeft = leftRPM;
  _megaPayload._rpmRight = rightRPM;
  _megaPayload._movementState = movement;
  _megaPayload._doorState = door;
  _megaPayload._emergencyState = emergency;
  _megaPayload._robotFeedback = feedback;
}

struct MegaPayload getMegaPayload() {
  struct MegaPayload temp = {_megaPayload._rpmLeft, _megaPayload._rpmRight,
           _megaPayload._movementState, _megaPayload._doorState,
           _megaPayload._emergencyState, _megaPayload._robotFeedback
  };
  return temp;
}
