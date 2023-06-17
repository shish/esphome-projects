#include "hp303b.h"
#include "esphome/core/log.h"

namespace esphome {
namespace hp303b {

static const char *const TAG = "hp303b.sensor";

static const uint8_t HP303B_ADDRESS = 0x77;

float HP303BComponent::get_setup_priority() const {
	return setup_priority::DATA;
}
void HP303BComponent::setup() {
    ESP_LOGCONFIG(TAG, "Setting up HP303B...");
    hp.begin();
}
void HP303BComponent::dump_config() {
    ESP_LOGCONFIG(TAG, "HP303B:");
    ESP_LOGCONFIG(TAG, "  Custom Sensor Active");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Connection with HP303B failed!");
  }
  LOG_UPDATE_INTERVAL(this);

  LOG_SENSOR("  ", "Temperature", this->temperature_);
  LOG_SENSOR("  ", "Pressure", this->pressure_);
}

void HP303BComponent::update() {
  int32_t pressure;
  int32_t temperature;
    hp.measurePressureOnce(pressure); // library returns value in in Pa, which equals 1/100 hPa
    hp.measureTempOnce(temperature);

    ESP_LOGD(TAG, "Got pressure=%.1f hPa", pressure / 100.0);
    ESP_LOGD(TAG, "Got temperature=%.1f °C", temperature);
    pressure_->publish_state(pressure / 100.0); 
    temperature_->publish_state(temperature); 
}

}  // namespace hp303b
}  // namespace esphome
