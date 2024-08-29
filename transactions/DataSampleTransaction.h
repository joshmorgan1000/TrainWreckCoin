#ifndef DATA_SAMPLE_TRANSACTION_H
#define DATA_SAMPLE_TRANSACTION_H

#include <string>
#include <vector>

// Class to represent a data sample transaction in the blockchain
class DataSampleTransaction {
public:
    int block_number;  // Block number referencing the model definition
    std::vector<float> input_activations;  // Input activations (e.g., pixel values for MNIST)
    std::string encrypted_label;  // Encrypted label for the data sample

    // Constructor
    DataSampleTransaction(int block_num, const std::vector<float>& inputs, const std::string& enc_label)
        : block_number(block_num), input_activations(inputs), encrypted_label(enc_label) {}

    // Serialization method (placeholder)
    std::string serialize() const {
        // This method should serialize the transaction into a format that can be transmitted over the network
        // For simplicity, this is just a placeholder and would need to be properly implemented
        return "Serialized DataSampleTransaction";
    }

    // Method to validate the transaction (placeholder)
    bool validate() const {
        // This method should perform validation on the transaction
        // For simplicity, this is just a placeholder and would need to be properly implemented
        return true;
    }
};

#endif // DATA_SAMPLE_TRANSACTION_H
