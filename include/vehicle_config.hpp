#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>
#include <vector>
#include <optional>

struct VehicleModel {
    std::string model_id;
    std::string model_name;
    uint32_t year;
    std::string manufacturer;
    std::string engine_type;
    double engine_capacity;
    uint32_t max_power;
    std::vector<std::string> features;

VehicleModel() = default;
VehicleModel(std::string id, std::string name, uint32_t year, std::string manu, std::string engine, double cap, uint32_t power):
    model_id(std::move(id)), model_name(std::move(name)), year(year), manufacturer(manu), engine_type(engine), engine_capacity(cap), max_power(power){};

VehicleModel(VehicleModel&&) = default;
VehicleModel& operator =(VehicleModel &&) = default;

VehicleModel(const VehicleModel&) = delete;
VehicleModel& operator =(VehicleModel &) = delete;

};

class VehicleConfigManager{
public:
VehicleConfigManager();
~VehicleConfigManager() = default;

static std::shared_ptr<VehicleConfigManager> getInstance();

VehicleConfigManager(const VehicleConfigManager &) = delete;
VehicleConfigManager& operator = (const VehicleConfigManager &) = delete;

void setCurrentVin(const std::string& vin);
std::string getCurrentVin() const;

// get vehicle model by ID
void addVehicleModel(VehicleModel && model);
std::shared_ptr<VehicleModel> getVehicleModel(const std::string & model_id);

// get all vehicle model
std::vector<std::shared_ptr<VehicleModel>> getAllModels() const;

// set current vehicle model
bool setCurrentModel(const std::string& model_id);
// get current vehicle model infomation
std::shared_ptr<VehicleModel> getCurrentModel() const;

// save config to file
//bool saveConfig(const std::string& filepath);

// from file load config
//bool loadConfig(const std::string& filepath);

// update vehicle feature
void updateVehicleFeature(const std::string& model_id, std::vector<std::string>&& features);




private:
    mutable std::mutex mutex_;
    std::string current_vin_;
    std::shared_ptr<VehicleModel> current_model_;
    std::unordered_map<std::string, std::shared_ptr<VehicleModel>> vehicle_model_;
    
    static std::shared_ptr<VehicleConfigManager> instance_;
    static std::once_flag init_flag_;
};


