#ifndef NEXT_DATA_STEP_TRANSACTION_H
#define NEXT_DATA_STEP_TRANSACTION_H

#include <string>
#include <vector>

// Class to represent the next data step transaction in the blockchain
class NextDataStepTransaction {
public:
    int previous_block_number;  // Block number referencing the previous data step transaction
    std::string previous_label_key;  // Decryption key for the previous label
    std::vector<float> next_input_activations;  // Input activations for the next data record
    std::string next_encrypted_label;  // Encrypted label for the next data record

    // Constructor
    NextDataStepTransaction(int prev_block_num, const std::string& prev_key,
                            const std::vector<float>& next_inputs, const std::string& next_enc_label)
        : previous_block_number(prev_block_num), previous_label_key(prev_key),
          next_input_activations(next_inputs), next_encrypted_label(next_enc_label) {}

    // Serialization method (placeholder)
    std::string serialize() const {
        // This method should serialize the transaction into a format that can be transmitted over the network
        // For simplicity, this is just a placeholder and would need to be properly implemented
        return "Serialized NextDataStepTransaction";
    }

    // Method to validate the transaction (placeholder)
    bool validate() const {
        // This method should perform validation on the transaction
        // For simplicity, this is just a placeholder and would need to be properly implemented
        return true;
    }
};

#endif // NEXT_DATA_STEP_TRANSACTION_H
