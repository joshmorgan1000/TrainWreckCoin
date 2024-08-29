#ifndef PROPOSED_WEIGHTS_TRANSACTION_H
#define PROPOSED_WEIGHTS_TRANSACTION_H

#include <string>
#include <vector>

// Class to represent a proposed weights transaction in the blockchain
class ProposedWeightsTransaction {
public:
    int block_number;  // Block number referencing the model definition
    std::string miner_address;  // Address of the training miner proposing the weights
    std::vector<float> weights;  // Proposed weights for the model layers

    // Constructor
    ProposedWeightsTransaction(int block_num, const std::string& miner, const std::vector<float>& initial_weights)
        : block_number(block_num), miner_address(miner), weights(initial_weights) {}

    // Serialization method (placeholder)
    std::string serialize() const {
        // This method should serialize the transaction into a format that can be transmitted over the network
        // For simplicity, this is just a placeholder and would need to be properly implemented
        return "Serialized ProposedWeightsTransaction";
    }

    // Method to validate the transaction (placeholder)
    bool validate() const {
        // This method should perform validation on the transaction
        // For simplicity, this is just a placeholder and would need to be properly implemented
        return true;
    }
};

#endif // PROPOSED_WEIGHTS_TRANSACTION_H
