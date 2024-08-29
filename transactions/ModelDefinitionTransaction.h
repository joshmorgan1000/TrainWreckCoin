#ifndef MODEL_DEFINITION_TRANSACTION_H
#define MODEL_DEFINITION_TRANSACTION_H

#include <string>
#include <vector>
#include <map>
#include <atomic>
#include "BlockStore.h"
#include "nlohmann/json.hpp"
#include "Logger.h"

enum class LayerType {
    FullyConnected,
    Convolutional,
    ReLU,
    Softmax
};

struct LayerDefinition {
    LayerType layer_type;
    std::vector<int> dimensions;
    std::map<std::string, float> parameters;

    LayerDefinition(LayerType type, const std::vector<int>& dims)
        : layer_type(type), dimensions(dims) {}

    // Serialize LayerDefinition to JSON
    [[nodiscard]] nlohmann::json toJson() const {
        nlohmann::json json_layer;
        json_layer["layer_type"] = static_cast<int>(layer_type);
        json_layer["dimensions"] = dimensions;
        json_layer["parameters"] = parameters;
        return json_layer;
    }

    // Deserialize LayerDefinition from JSON
    static LayerDefinition fromJson(const nlohmann::json& json_layer) {
        const auto type = static_cast<LayerType>(json_layer["layer_type"].get<int>());
        const auto dims = json_layer["dimensions"].get<std::vector<int>>();
        auto params = json_layer["parameters"].get<std::map<std::string, float>>();
        return LayerDefinition(type, dims);
    }
};

class ModelDefinitionTransaction {
public:
    std::string model_name;
    std::string benefactor_address;
    std::vector<LayerDefinition> layers;
    std::string dataset_hash;
    std::string encrypted_label_key;
    static std::atomic_flag file_write_flag;
    static std::atomic_flag file_read_flag;

    ModelDefinitionTransaction(const std::string& name, const std::string& benefactor,
                               const std::string& dataset_hash, const std::string& encrypted_key)
        : model_name(name), benefactor_address(benefactor), dataset_hash(dataset_hash), encrypted_label_key(encrypted_key) {}

    void addLayer(const LayerDefinition& layer) {
        layers.push_back(layer);
    }

    // Serialize the transaction to a JSON string
    [[nodiscard]] std::string serialize() const {
        nlohmann::json json_transaction;
        json_transaction["model_name"] = model_name;
        json_transaction["benefactor_address"] = benefactor_address;
        json_transaction["dataset_hash"] = dataset_hash;
        json_transaction["encrypted_label_key"] = encrypted_label_key;

        for (const auto& layer : layers) {
            json_transaction["layers"].push_back(layer.toJson());
        }

        return json_transaction.dump();
    }

    // Deserialize the transaction from a JSON string
    static ModelDefinitionTransaction deserialize(const std::string& serialized_transaction) {
        nlohmann::json json_transaction = nlohmann::json::parse(serialized_transaction);

        std::string name = json_transaction["model_name"].get<std::string>();
        std::string benefactor = json_transaction["benefactor_address"].get<std::string>();
        std::string dataset_hash = json_transaction["dataset_hash"].get<std::string>();
        std::string encrypted_key = json_transaction["encrypted_label_key"].get<std::string>();

        ModelDefinitionTransaction transaction(name, benefactor, dataset_hash, encrypted_key);

        for (const auto& json_layer : json_transaction["layers"]) {
            transaction.addLayer(LayerDefinition::fromJson(json_layer));
        }

        return transaction;
    }

    // Store the transaction in BlockStore
    void store(BlockStore &block_store, std::string &block_key) const {
        Logger::log("ModelDefinitionTransaction", "Getting the thread locks to write a record...");
        while (file_write_flag.test_and_set(std::memory_order_acquire) || file_read_flag.test(std::memory_order_acquire)) {
            while (file_write_flag.test(std::memory_order_acquire) && file_read_flag.test(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
        }
        if (const std::string serialized_data = serialize(); !block_store.storeBlock(block_key, serialized_data)) {
            Logger::log("ModelDefinitionTransaction", "Failed to store the block in BlockStore");
        }
        file_write_flag.clear(std::memory_order_release);
        file_write_flag.notify_all();
        Logger::log("ModelDefinitionTransaction", "ModelDefinitionTransaction stored successfully with key: " + block_key);
    }

    // Load the transaction from BlockStore
    static ModelDefinitionTransaction load(BlockStore& block_store, const std::string& block_key) {
        Logger::log("ModelDefinitionTransaction", "Getting the thread locks to read a record...");
        if (file_write_flag.test(std::memory_order_acquire)) {
            file_write_flag.wait(true, std::memory_order_release);
        }
        file_read_flag.test_and_set(std::memory_order_relaxed);
        const std::string serialized_data = block_store.retrieveBlock(block_key);
        file_read_flag.clear(std::memory_order_release);
        if (serialized_data.empty()) {
            Logger::log("ModelDefinitionTransaction", "Failed to load block");
            throw std::runtime_error("Failed to load block");
        }
        Logger::log("ModelDefinitionTransaction", "Loaded ModelDefinitionTransaction from BlockStore with key: " + block_key);
        return deserialize(serialized_data);
    }

    static bool validate() {
        return true;
    }
};

std::atomic_flag ModelDefinitionTransaction::file_write_flag = false;
std::atomic_flag ModelDefinitionTransaction::file_read_flag = false;

#endif // MODEL_DEFINITION_TRANSACTION_H
