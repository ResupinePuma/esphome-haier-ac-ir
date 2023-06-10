#pragma once

#include "esphome/core/automation.h"

namespace esphome
{
    namespace haier_acyrw02
    {

        template <typename... Ts>
        class VerticalAirflowAction : public Action<Ts...>
        {
        public:
            VerticalAirflowAction(HaierClimate *parent) : parent_(parent) {}
            TEMPLATABLE_VALUE(AirflowVerticalDirection, direction)
            void play(Ts... x) { this->parent_->set_vertical_airflow(this->direction_.value(x...)); }

        protected:
            HaierClimate *parent_;
        };
    }
}
