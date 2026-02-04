#include "vehicle_config.hpp"
#include "auto_logger.hpp"
#include <fstream>

std::shared_ptr<VehicleConfigManager> VehicleConfigManager::instance_ = nullptr;

std::once_flag VehicleConfigManager::init_flag_;

VehicleConfigManager::VehicleConfigManager() {
    AUTO_LOG_INFO("VehicleConfigManager initalized");
}

std::shared_ptr<VehicleConfigManager> VehicleConfigManager::getInstance(){
    std::call_once(init_flag_, []() {
        instance_ = std::shared_ptr<VehicleConfigManager>(new VehicleConfigManager());
    });
    
    return instance_;
};

void VehicleConfigManager::setCurrentVin(const std::string& vin){
    std::lock_guard<std::mutex> lock(mutex_);
    current_vin_ = vin;
    AUTO_LOG_INFO("VIN set to: {}");
};

std::string VehicleConfigManager::getCurrentVin() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return current_vin_;
}

void VehicleConfigManager::addVehicleModel(VehicleModel&& model){
    std::lock_guard<std::mutex> lock(mutex_);
    auto model_ptr = std::make_shared<VehicleModel>(std::move(model));
    vehicle_model_[model_ptr->model_id] = model_ptr;
    AUTO_LOG_INFO("Added vehicle model: {} ({})", model_ptr->model_name, model_ptr->model_id);
}

std::shared_ptr<VehicleModel> VehicleConfigManager::getVehicleModel(const std::string& model_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = vehicle_model_.find(model_id);

    if (it != vehicle_model_.end()){
        return it->second;
    }
    return nullptr;
}

std::vector<std::shared_ptr<VehicleModel>> VehicleConfigManager::getAllModels() const {
    std::lock_guard<std::mutex> lock(mutex_);

    std::vector<std::shared_ptr<VehicleModel>> models;

    for (const auto & [id, model] : vehicle_model_){
        models.push_back(model);
    }
    return models;
}

bool VehicleConfigManager::setCurrentModel(const std::string& model_id) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = vehicle_model_.find(model_id);

    if(it != vehicle_model_.end()) {
        current_model_ = it->second;
        AUTO_LOG_INFO("Current model set to : {}", model_id);
        return true;
    }

    AUTO_LOG_ERROR("Model not found: {}", model_id);
    return false;
}

std::shared_ptr<VehicleModel> VehicleConfigManager::getCurrentModel() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return current_model_;
}

void VehicleConfigManager::updateVehicleFeature(const std::string& model_id, std::vector<std::string>&& features){
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = vehicle_model_.find(model_id);

    if(it == vehicle_model_.end()){
        it->second->features = std::move(features);
        AUTO_LOG_INFO("Updated features for model: {}", model_id);
    } else {
        AUTO_LOG_WARN("Model not found for feature update: {}", model_id);
    }
}





































