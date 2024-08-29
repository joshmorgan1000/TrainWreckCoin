#ifndef VALIDATION_TRANSACTION_H
#define VALIDATION_TRANSACTION_H

#include <string>
#include <vector>

// Class to represent a validation transaction in the blockchain
class ValidationTransaction {
public:
    int block_number;  // Block number referencing the model definition
    std::string validator_address;  // Address of the validation miner
    std::string weights_hash;  // Hash of the proposed weights that were validated
    float validation_loss;  // Loss calculated by the validator
    bool is_valid;  // Whether the weights are deemed valid (improve the model)

    // Constructor
    ValidationTransaction(int block_num, const std::string& validator, const std::string& w_hash,
                          float loss, bool valid)
        : block_number(block_num), validator_address(validator), weights_hash(w_hash),
          validation_loss(loss), is_valid(valid) {}

    // Serialization method (placeholder)
    std::string serialize() const {
        // This method should serialize the transaction into a format that can be transmitted over the network
        // For simplicity, this is just a placeholder and would need to be properly implemented
        return "Serialized ValidationTransaction";
    }

    // Method to validate the transaction (placeholder)
    bool validate() const {
        // This method should perform validation on the transaction
        // For simplicity, this is just a placeholder and would need to be properly implemented
        return true;
    }
};

#endif // VALIDATION_TRANSACTION_H
