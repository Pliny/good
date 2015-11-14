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

protected:

private:
  int timeInterval;

  String getMacAddr(void);

  int processJson(String &json);

};

#endif

