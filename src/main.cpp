#include "auto_logger.hpp"
#include "vehicle_config.hpp"
#include <iostream>
#include <thread>

void testLogger() {
    AutoLogger::AutoLogger::getInstance().init("./logs");

    //common log
    AUTO_LOG_INFO("System starting...");
    AUTO_LOG_WARN("Low battery warning");
    AUTO_LOG_ERROR("Sensor communication failed");
    
    // can id logger
    AUTO_LOG_INFO_CAN(0x123, "Engine temperature: {}.C", 85);
    AUTO_LOG_ERROR_CAN(0x456, "Engine temperature: {}.C");

    //setting current CAN ID
    AutoLogger::AutoLogger::getInstance().setCanId(0x789);
    AutoLogger::AutoLogger::getInstance().info("AutoLogger::AutoLogger::getInstance()");
    
}

void testVehicleConfig() {
    auto config = VehicleConfigManager::getInstance();

    config->setCurrentVin("1HGCM82633A123456");
    // 添加车型（使用移动语义）
    VehicleModel model1{
        "MODEL_S",
        "Model S Performance",
        2023,
        "Tesla",
        "Electric",
        0.0,  // 电动车无排量
        1020
    };
    model1.features = {"Autopilot", "Ludicrous Mode", "Smart Air Suspension"};

    config->addVehicleModel(std::move(model1));

    // 添加另一个车型
    VehicleModel model2{
        "MODEL_3",
        "Model 3 Long Range",
        2023,
        "Tesla",
        "Electric",
        0.0,
        450
    };
    model2.features = {"Autopilot", "Premium Interior", "Dual Motor AWD"};
    
    config->addVehicleModel(std::move(model2));

    // 设置当前车型
    config->setCurrentModel("MODEL_S");

    //
    auto current_model = config->getCurrentModel();
    if (current_model) {
        AUTO_LOG_INFO("Current mode: {}", current_model->model_name);
        AUTO_LOG_INFO("Max power: {} HP", current_model->max_power);
    }
}

int main() {
    testLogger();
    testVehicleConfig();
    AutoLogger::AutoLogger::getInstance().flush();
    return 0;
}
