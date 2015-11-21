#ifndef _DEVICE_MODEL_H
#define _DEVICE_MODEL_H

#include <Arduino.h>
#include "Model.h"

class DeviceModel : public Model {

public:
  DeviceModel(void);

  int getTimeInterval(void);

  int populate(void);

  int createAndPopulate(void);

  String getMacAddr(void);

  int getId(void);

private:
  int timeInterval;

  int id;

  int processJson(String &json);
};

#endif

