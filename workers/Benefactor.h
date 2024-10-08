#ifndef BENEFACTOR_H
#define BENEFACTOR_H

#include <thread>
#include "BlockStore.h"
#include "ModelDefinitionTransaction.h"
#include "ZeroMQBroadcastWrapper.h"
#include "Logger.h"

class Benefactor {
public:
    Benefactor(BlockStore& store, const std::string& broadcast_address)
        : blockStore(store), zmq_wrapper(broadcast_address, true), stop_thread(false) {}

    void start() {
        Logger::log("Benefactor", "Starting the Benefactor thread...");
        benefactor_thread = std::thread(&Benefactor::run, this);
    }

    void stop() {
        stop_thread = true;
        if (benefactor_thread.joinable()) {
            benefactor_thread.join();
        }
    }

private:
    BlockStore& blockStore;
    ZeroMQBroadcastWrapper zmq_wrapper;
    std::thread benefactor_thread;
    std::atomic<bool> stop_thread;

    void run() {
        int transaction_count = 0;
        while (!stop_thread) {
            // Create a ModelDefinitionTransaction
            Logger::log("Benefactor", "Creating a new ModelDefinitionTransaction...");
            ModelDefinitionTransaction transaction("MNIST_Model", "0x1234", "abc123def456", "encryptedKey");

            // Add layers to the model
            LayerDefinition layer1(LayerType::FullyConnected, {784, 128});
            LayerDefinition layer2(LayerType::ReLU, {});
            LayerDefinition layer3(LayerType::FullyConnected, {128, 10});
            LayerDefinition layer4(LayerType::Softmax, {});

            transaction.addLayer(layer1);
            transaction.addLayer(layer2);
            transaction.addLayer(layer3);
            transaction.addLayer(layer4);

            // Define a unique block key for storage
            std::string block_key = "block_" + std::to_string(transaction_count++);

            // Store the transaction in the BlockStore
            Logger::log("Benefactor", "Storing the ModelDefinitionTransaction in the BlockStore...");
            transaction.store(blockStore, block_key);
            Logger::log("Benefactor", "ModelDefinitionTransaction stored successfully with key: " + block_key);

            // Sleep for a couple seconds
            std::this_thread::sleep_for(std::chrono::seconds(2));

            // Broadcast that the transaction has been written
            Logger::log("Benefactor", "Broadcasting message for block key: " + block_key);
            zmq_wrapper.broadcastMessage("MODEL_DEFINITION_WRITTEN:" + block_key);
            Logger::log("Benefactor", "Broadcasted message for block key: " + block_key + ", waiting for validation (maybe I should be keeping myself busy somehow?");

            // Wait for the validation result
            std::string validation_message = waitForValidation(block_key);
            if (validation_message == "VALID") {
                Logger::log("Benefactor", "Validation succeeded for block key: " + block_key + ", proceeding to the next block...");
                // Proceed to the next step, like sending the next transaction
            } else {
                Logger::log("Benefactor", "Validation failed for block key: " + block_key + "... not sure what to do in this case...?");
                // Handle validation failure (e.g., retry, log, or halt)
            }

            // Sleep or wait for the next cycle (to simulate real-time operation)
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    }

    std::string waitForValidation(const std::string& expected_block_key) {
        while (!stop_thread) {
            // Listen for validation results
            std::string message = zmq_wrapper.receiveBroadcast();
            if (message.find("VALIDATION_RESULT:") == 0) {
                // Parse the validation message
                Logger::log("Benefactor", "Received validation message: " + message);
                std::string validation_info = message.substr(std::string("VALIDATION_RESULT:").length());
                size_t pos = validation_info.find(':');
                if (pos != std::string::npos) {
                    std::string block_key = validation_info.substr(0, pos);
                    std::string validation_result = validation_info.substr(pos + 1);

                    if (block_key == expected_block_key) {
                        return validation_result;
                    }
                }
            }

            // Sleep briefly to avoid busy-waiting
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        return "INVALID";  // Return invalid if the thread is stopped
    }
};

#endif // BENEFACTOR_H
