#ifndef _SAMPLE_DATA_MODEL_H
#define _SAMPLE_DATA_MODEL_H

#include <Arduino.h>
#include "Model.h"
#include "DeviceModel.h"

class SampleDataModel : public Model {

public:
  SampleDataModel(DeviceModel &dev);

  int send(uint32_t count);

private:
  DeviceModel &device;
  bool first_post;

  int processJson(String &json);
};

#endif


