#ifndef VALIDATION_WORKER_H
#define VALIDATION_WORKER_H

#include <thread>
#include "BlockStore.h"
#include "ZeroMQBroadcastWrapper.h"
#include "ModelDefinitionTransaction.h"

class ValidationWorker {
public:
    ValidationWorker(BlockStore& store, const std::string& broadcast_address)
        : blockStore(store), zmq_wrapper(broadcast_address, false), stop_thread(false) {}

    void start() {
        validator_thread = std::thread(&ValidationWorker::run, this);
    }

    void stop() {
        stop_thread = true;
        if (validator_thread.joinable()) {
            validator_thread.join();
        }
    }

private:
    BlockStore& blockStore;
    ZeroMQBroadcastWrapper zmq_wrapper;
    std::thread validator_thread;
    bool stop_thread;

    void run() {
        while (!stop_thread) {
            // Listen for broadcasts from the Training Miner
            std::string message = zmq_wrapper.receiveBroadcast();
            std::cout << "Validation Worker: Received broadcast message: " << message << std::endl;

            if (message.find("WEIGHTS_INITIALIZED:") == 0) {
                // Extract the block key from the message
                std::string block_key = message.substr(std::string("WEIGHTS_INITIALIZED:").length());

                // Retrieve the model definition from the BlockStore
                ModelDefinitionTransaction transaction = ModelDefinitionTransaction::load(blockStore, block_key);
                std::cout << "Validation Worker: ModelDefinitionTransaction loaded with model name: " << transaction.model_name << std::endl;

                // Mock validation (e.g., simple loss calculation)
                bool is_valid = validateWeights(transaction);

                // Broadcast validation result
                std::string validation_result = is_valid ? "VALID" : "INVALID";
                zmq_wrapper.broadcastMessage("VALIDATION_RESULT:" + block_key + ":" + validation_result);
                std::cout << "Validation Worker: Broadcast message sent for validation result with block key: " << block_key << " as " << validation_result << std::endl;
            }

            // Sleep or wait for the next cycle
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    // Mock validation function
    static bool validateWeights(const ModelDefinitionTransaction& transaction) {
        // Simulate a validation process (e.g., calculate a mock loss function)
        // For simplicity, let's say we always return true for now
        return true;
    }
};

#endif // VALIDATION_WORKER_H
