#ifndef MegaPayload_h
#define MegaPayload_h


struct MegaPayload {
  int _rpmLeft;
  int _rpmRight;
  bool _movementState;
  bool _doorState;
  bool _emergencyState;
  int _robotFeedback;
};
#endif
