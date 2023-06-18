import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate, sensor
from esphome.const import CONF_ID, CONF_SENSOR_ID, CONF_PIN, CONF_SENSORS
from esphome.core import CORE
from esphome import automation

AUTO_LOAD = ["climate"]

haier_ac160_ns = cg.esphome_ns.namespace("haier_ac160")
HaierClimate = haier_ac160_ns.class_("HaierClimate", climate.Climate, cg.Component)

CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(HaierClimate),
        cv.Required(CONF_SENSOR_ID): cv.use_id(sensor.Sensor),
        cv.Required(CONF_PIN): cv.int_
    }
)
CONF_VERTICAL_AIRFLOW = "vertical_airflow"
VerticalAirflowAction = haier_ac160_ns.class_("VerticalAirflowAction", automation.Action)
AirflowVerticalDirection = haier_ac160_ns.enum("AirflowVerticalDirection")
AIRFLOW_VERTICAL_DIRECTION_OPTIONS = {
    "UP": AirflowVerticalDirection.UP,
    "CENTER": AirflowVerticalDirection.CENTER,
    "DOWN": AirflowVerticalDirection.DOWN,
}


@automation.register_action(
    "climate.haier_ac160.set_vertical_airflow",
    VerticalAirflowAction,
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(HaierClimate),
            cv.Required(CONF_VERTICAL_AIRFLOW): cv.templatable(
                cv.enum(AIRFLOW_VERTICAL_DIRECTION_OPTIONS, upper=True)
            ),
        }
    ),
)
async def haier_set_vertical_airflow_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    template_ = await cg.templatable(
        config[CONF_VERTICAL_AIRFLOW], args, AirflowVerticalDirection
    )
    cg.add(var.set_direction(template_))
    return var

async def to_code(config):
    if CORE.is_esp8266 or CORE.is_esp32:
        cg.add_library("crankyoldgit/IRremoteESP8266", "2.8.4")

    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await climate.register_climate(var, config)
    
    sens = await cg.get_variable(config[CONF_SENSOR_ID])
    cg.add(var.init(sens, config[CONF_PIN]))


