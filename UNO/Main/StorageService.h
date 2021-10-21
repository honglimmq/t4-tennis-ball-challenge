#ifndef StorageService_h
#define StorageService_h
#include "UnoPayload.h"
#include "MegaPayload.h"


class StorageService {

  public:
    StorageService();
    void setUnoPayload(int yVal1, int yVal2, bool movement, bool door, bool emergency);
    void setUnoPayload(struct UnoPayload uno);
    void setUnoJoys(int yVal1, int yVal2);
    void setUnoBtns(bool movement, bool door, bool emergency);
    void setUnoJoy1(int yVal);
    void setUnoJoy2(int yVal);
    void setUnoMovementState(bool movement);
    void setUnoDoorState(bool door);
    void setUnoEmergencyState(bool emergency);
    struct UnoPayload getUnoPayload();
    int getUnoJoy1();
    int getUnoJoy2();
    bool getUnoMovementState();
    bool getUnoDoorState();
    bool getUnoEmergencyState();
    void setMegaPayload(int leftRPM, int rightRPM, bool movement, bool door, bool emergency, int feedback);
    struct MegaPayload getMegaPayload();
    int getMegaLRPM();
    int getMegaRRPM();
    bool getMegaMovementState();
    bool getMegaDoorState();
    bool getMegaEmergencyState();
    int getMegaRobotFeedback();
    void setBTErrorCode(int n);
    int getBTErrorCode();
};
#endif
