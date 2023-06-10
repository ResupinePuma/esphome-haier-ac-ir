#pragma once

#include "esphome/core/log.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/automation.h"

#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "ir_Haier.h"

namespace esphome
{
  namespace haier_acyrw02
  {

    enum class VerticalSwingMode : uint8_t
    {
      MAX_UP = kHaierAc160SwingVHighest,
      UP = kHaierAc160SwingVTop,
      CENTER = kHaierAc160SwingVMiddle,
      DOWN = kHaierAc160SwingVLow,
      MAX_DOWN = kHaierAc160SwingVLowest,
      AUTO = kHaierAc160SwingVAuto
    };

    enum class AirflowVerticalDirection : uint8_t
    {
      MAX_UP = 0,
      UP = 1,
      CENTER = 2,
      DOWN = 3,
      MAX_DOWN = 4,
      AUTO = 5
    };

    const uint8_t HAIER_ACYRW02_TEMP_MIN = 16; // 16C
    const uint8_t HAIER_ACYRW02_TEMP_MAX = 30; // 32C

    class HaierClimate : public esphome::Component,
                         public climate::Climate
    {
    public:
      HaierClimate() : climate::Climate() {}
      HaierClimate(const HaierClimate &) = delete;
      HaierClimate &operator=(const HaierClimate &) = delete;
      ~HaierClimate();

    public:
      AirflowVerticalDirection get_vertical_airflow() const;
      void set_vertical_airflow(AirflowVerticalDirection direction);

      void set_sensor(sensor::Sensor *sensor);
      void init(sensor::Sensor *sensor, uint16_t pin);

    protected:
      IRHaierACYRW02 *ac_{nullptr};
      sensor::Sensor *sensor_{nullptr};

      AirflowVerticalDirection vertical_direction_;
      void setup_ir_cmd();

      climate::ClimateTraits traits() override;
      void control(const climate::ClimateCall &call) override;
    };

    template <typename... Ts>
    class VerticalAirflowAction : public Action<Ts...>
    {
    public:
      VerticalAirflowAction(
          : HaierClimate *parent) : parent_(parent) {}
      TEMPLATABLE_VALUE(AirflowVerticalDirection, direction)
      void play(Ts... x) { this->parent_->set_vertical_airflow(this->direction_.value(x...)); }

    protected:
      HaierClimate *parent_;
    };

  } // namespace haier_acyrw02
} // namespace esphome
