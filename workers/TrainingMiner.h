#ifndef TRAINING_MINER_H
#define TRAINING_MINER_H

#include <thread>
#include <random>
#include "BlockStore.h"
#include "ZeroMQBroadcastWrapper.h"
#include "ModelDefinitionTransaction.h"

class TrainingMiner {
public:
    TrainingMiner(BlockStore& store, const std::string& broadcast_address)
        : blockStore(store), zmq_wrapper(broadcast_address, false), stop_thread(false) {}

    void start() {
        miner_thread = std::thread(&TrainingMiner::run, this);
    }

    void stop() {
        stop_thread = true;
        if (miner_thread.joinable()) {
            miner_thread.join();
        }
    }

private:
    BlockStore& blockStore;
    ZeroMQBroadcastWrapper zmq_wrapper;
    std::thread miner_thread;
    bool stop_thread;

    void run() {
        while (!stop_thread) {
            // Listen for broadcasts from the Benefactor
            std::string message = zmq_wrapper.receiveBroadcast();
            std::cout << "Training Miner: Received broadcast message: " << message << std::endl;

            if (message.find("MODEL_DEFINITION_WRITTEN:") == 0) {
                // Extract the block key from the message
                std::string block_key = message.substr(std::string("MODEL_DEFINITION_WRITTEN:").length());

                // Retrieve the transaction from the BlockStore
                ModelDefinitionTransaction transaction = ModelDefinitionTransaction::load(blockStore, block_key);
                std::cout << "Training Miner: ModelDefinitionTransaction loaded with model name: " << transaction.model_name << std::endl;

                // Initialize weights based on the model definition
                std::map<std::string, std::vector<float>> initialized_weights = initializeWeights(transaction);
                std::cout << "Training Miner: Weights initialized for the model." << std::endl;

                // Simulate sending the initialized weights off to the next task
                zmq_wrapper.broadcastMessage("WEIGHTS_INITIALIZED:" + block_key);
                std::cout << "Training Miner: Broadcast message sent for initialized weights with block key: " << block_key << std::endl;
            }

            // Sleep or wait for the next cycle
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    // Initialize weights based on the model definition
    std::map<std::string, std::vector<float>> initializeWeights(const ModelDefinitionTransaction& transaction) {
        std::map<std::string, std::vector<float>> weights;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-0.1, 0.1);

        int layer_count = 1;
        for (const auto& layer : transaction.layers) {
            std::vector<float> layer_weights;
            int num_weights = 1;
            for (int dim : layer.dimensions) {
                num_weights *= dim;
            }

            for (int i = 0; i < num_weights; ++i) {
                layer_weights.push_back(dis(gen));
            }

            std::string layer_name = "layer_" + std::to_string(layer_count);
            weights[layer_name] = layer_weights;
            layer_count++;
        }

        return weights;
    }
};

#endif // TRAINING_MINER_H
