#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"
#include "LOLIN_HP303B.h"

namespace esphome {
namespace hp303b {

class HP303BComponent : public PollingComponent, public i2c::I2CDevice {
 public:
  LOLIN_HP303B hp;
  void set_temperature(sensor::Sensor *temperature) { temperature_ = temperature; }
  void set_pressure(sensor::Sensor *pressure) { pressure_ = pressure; }

  /// Schedule temperature+pressure readings.
  void update() override;
  /// Setup the sensor and test for a connection.
  void setup() override;
  void dump_config() override;

  float get_setup_priority() const override;

 protected:
  sensor::Sensor *temperature_{nullptr};
  sensor::Sensor *pressure_{nullptr};
};

}  // namespace hp303b
}  // namespace esphome
