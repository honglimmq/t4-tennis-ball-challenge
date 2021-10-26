class Sensor
{
private:
public:
  bool INITIALIZED = false;
  int PIN = 0;
  int VALUE = 0;
  int LASTVALUE = 0;

  Sensor() {}

  Sensor(int pin, int mode)
  {
    initialize(pin, mode);
  }

  void initialize(int pin, int mode)
  {
    pinMode(pin, mode);
    PIN = pin;
    INITIALIZED = true;
  }

  void writeValue(int reading)
  {
    LASTVALUE = VALUE;
    VALUE = reading;
  }

  bool hasChanged()
  {
    return VALUE != LASTVALUE;
  }
};

class DigitalSensor : public Sensor
{
public:
  DigitalSensor() {}

  DigitalSensor(int pin)
  {
    initialize(pin);
  }

  void initialize(int pin)
  {
    Sensor::initialize(pin, INPUT_PULLUP);
  }

  void refresh()
  {
    Sensor::writeValue(digitalRead(PIN));
  }
};

class AnalogSensor : public Sensor
{
public:
  AnalogSensor()
  {
  }

  AnalogSensor(int pin)
  {
    initialize(pin);
  }

  void initialize(int pin)
  {
    Sensor::initialize(pin, INPUT);
  }

  void refresh()
  {
    Sensor::writeValue(analogRead(PIN));
  }
};

class Button : public DigitalSensor
{
public:
  Button() : DigitalSensor() {}

  Button(int pin)
  {
    initialize(pin);
  }

  void initialize(int pin)
  {
    DigitalSensor::initialize(pin);
  }

  bool isDown(int value)
  {
    return value == 0;
  }

  bool isDown()
  {
    return isDown(VALUE);
  }

  bool isUp(int value)
  {
    return value == 1;
  }

  bool isUp()
  {
    return isUp(VALUE);
  }

  bool isPressed()
  {
    return hasChanged() && isUp();
  }
};

class Potentiometer : public AnalogSensor
{
private:
  int map(int value, int low, int high)
  {
    mapped = true;
    lower = low;
    upper = high;
    int range = high - low;
    float rate = (float)range / 1024.0;
    float current = (float)value * rate + (float)low + 0.5;
    int round = (int)current;
    VALUE = round;
    return round;
  }

public:
  bool mapped = false;
  int lower = 0;
  int upper = 100;

  Potentiometer() : AnalogSensor() {}

  Potentiometer(int pin)
  {
    initialize(pin);
  }

  void initialize(int pin)
  {
    AnalogSensor::initialize(pin);
  }

  int map(int low, int high)
  {
    return map(VALUE, low, high);
  }

  int map(int high)
  {
    return map(0, high);
  }

  void disableMap()
  {
    mapped = false;
  }

  void refresh()
  {
    LASTVALUE = VALUE;
    if (mapped)
      VALUE = map(analogRead(PIN), lower, upper);
    else
      VALUE = analogRead(PIN);
  }
};

class Joystick
{
private:
public:
  Potentiometer X, Y;

  Joystick() {}

  Joystick(int pinX, int pinY)
  {
    initialize(pinY, pinX);
  }

  void initialize(int pinX, int pinY)
  {
    X.initialize(pinX);
    Y.initialize(pinY);
  }

  void refresh()
  {
    X.refresh();
    Y.refresh();
  }

  bool hasChanged()
  {
    return X.hasChanged() || Y.hasChanged();
  }
};
