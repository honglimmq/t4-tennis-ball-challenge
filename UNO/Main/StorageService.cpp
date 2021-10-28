#include "Arduino.h"
#include "StorageService.h"
#include "UnoPayload.h"
#include "MegaPayload.h"

UnoPayload _unoPayload;
MegaPayload _megaPayload;
int _btErrorCode;

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

  // Bluetooth error code
  _btErrorCode = 0;
}

// UnoPayload setters
void StorageService::setUnoPayload(int yVal1, int yVal2, bool movement, bool door, bool emergency) {
  // Joystick values
  _unoPayload._rawValY1 = (yVal1 >= 0) ? yVal1 : 0;
  _unoPayload._rawValY2 = (yVal2 >= 0) ? yVal2 : 0;

  // Buttons values
  _unoPayload._movementState = movement;
  _unoPayload._doorState = door;
  _unoPayload._emergencyState = emergency;
}

void StorageService::setUnoJoy1(int yVal) {
  _unoPayload._rawValY1 = (yVal >= 0) ? yVal : 0;
}
void StorageService::setUnoJoy2(int yVal) {
  _unoPayload._rawValY2 = (yVal >= 0) ? yVal : 0;
}
void StorageService::setUnoMovementState(bool movement) {
  _unoPayload._movementState = movement;
}
void StorageService::setUnoDoorState(bool door) {
  _unoPayload._doorState = door;
}
void StorageService::setUnoEmergencyState(bool emergency) {
  _unoPayload._emergencyState = emergency;
}

void StorageService::setUnoPayload(struct UnoPayload uno) {
  setUnoPayload(uno._rawValY1, uno._rawValY2,
                _unoPayload._movementState, _unoPayload._doorState,
                _unoPayload._emergencyState);
}

void StorageService::setUnoJoys(int yVal1, int yVal2) {
  setUnoPayload(yVal1, yVal2,
                _unoPayload._movementState, _unoPayload._doorState,
                _unoPayload._emergencyState);
}

void StorageService::setUnoBtns(bool movement, bool door, bool emergency) {
  setUnoPayload(_unoPayload._rawValY1, _unoPayload._rawValY2,
                movement, door,
                emergency);
}

// UnoPayload getters
struct UnoPayload StorageService::getUnoPayload() {
  struct UnoPayload temp = {_unoPayload._rawValY1, _unoPayload._rawValY2,
           _unoPayload._movementState, _unoPayload._doorState,
           _unoPayload._emergencyState
  };
  return temp;
}

int StorageService::getUnoJoy1() {
  return _unoPayload._rawValY1;
}
int StorageService::getUnoJoy2() {
  return _unoPayload._rawValY2;
}
bool StorageService::getUnoMovementState() {
  return _unoPayload._movementState;
}
bool StorageService::getUnoDoorState() {
  return _unoPayload._doorState;
}
bool StorageService::getUnoEmergencyState() {
  return _unoPayload._emergencyState;
}

// MegaPayload setters and getters
void StorageService::setMegaPayload(int leftRPM, int rightRPM, bool movement, bool door, bool emergency, int feedback) {
  _megaPayload._rpmLeft = leftRPM;
  _megaPayload._rpmRight = rightRPM;
  _megaPayload._movementState = movement;
  _megaPayload._doorState = door;
  _megaPayload._emergencyState = emergency;
  _megaPayload._robotFeedback = feedback;
}


void StorageService::setMegaLRPM(int leftRPM) {
  _megaPayload._rpmLeft = leftRPM;
}

void StorageService::setMegaRRPM(int rightRPM) {
  _megaPayload._rpmRight = rightRPM;
}
void StorageService::setMegaMovementState(bool movement) {
  _megaPayload._movementState = movement;
}

void StorageService::setMegaDoorState(bool door) {
  _megaPayload._doorState = door;
}

void StorageService::setMegaEmergencyState(bool emergency) {
  _megaPayload._emergencyState = emergency;
}

void StorageService::setMegaRobotFeedback(int feedback) {
  _megaPayload._robotFeedback = feedback;
}


struct MegaPayload StorageService::getMegaPayload() {
  struct MegaPayload temp = {_megaPayload._rpmLeft, _megaPayload._rpmRight,
           _megaPayload._movementState, _megaPayload._doorState,
           _megaPayload._emergencyState, _megaPayload._robotFeedback
  };
  return temp;
}

int StorageService::getMegaLRPM() {
  return _megaPayload._rpmLeft;
}

int StorageService::getMegaRRPM() {
  return _megaPayload._rpmRight;
}
bool StorageService::getMegaMovementState() {
  return _megaPayload._movementState;
}
bool StorageService::getMegaDoorState() {
  return _megaPayload._doorState;
}
bool StorageService::getMegaEmergencyState() {
  return _megaPayload._emergencyState;
}
int StorageService::getMegaRobotFeedback() {
  return _megaPayload._robotFeedback;
}

// Bluetooth error code

void StorageService::setBTErrorCode(int n) {
  _btErrorCode = n;
}

int StorageService::getBTErrorCode() {
  return _btErrorCode;
}
