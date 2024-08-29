#ifndef BLOCK_STORE_H
#define BLOCK_STORE_H

#include <tkrzw_dbm_hash.h>
#include <string>
#include <iostream>
#include "Logger.h"

class BlockStore {
public:

    explicit BlockStore(const std::string& file_path) {
        Logger::log("BlockStore", "Opening up the BlockStore database...");
        // Open the tkrzw skip database file
        if (const auto status = dbm.Open(file_path, true, tkrzw::File::OPEN_TRUNCATE); status != tkrzw::Status::SUCCESS) {
            std::cerr << "Error opening the skip table database: " << status << std::endl;
        } else {
            std::cout << "Skip table database opened successfully at: " << file_path << std::endl;
        }
    }

    ~BlockStore() {
        // Explicitly flush changes before closing the database
        auto status = dbm.Synchronize(true, nullptr);
        if (status != tkrzw::Status::SUCCESS) {
            std::cerr << "Error synchronizing the database: " << status << std::endl;
        } else {
            std::cout << "Database synchronized successfully." << std::endl;
        }

        // Close the database
        status = dbm.Close();
        if (status != tkrzw::Status::SUCCESS) {
            std::cerr << "Error closing the database: " << status << std::endl;
        } else {
            std::cout << "Database closed successfully." << std::endl;
        }
    }

    // Store a block in the database
    bool storeBlock(std::string& block_key, const std::string& block_data) {
        if (const auto status = dbm.PushLast(block_data, -1, &block_key); status != tkrzw::Status::SUCCESS) {
            std::cerr << "Error storing block: " << status << std::endl;
            return false;
        } else {
            std::cout << "Block stored successfully with key: " << block_key << std::endl;
        }
        return true;
    }

    // Retrieve a block from the database
    std::string retrieveBlock(const std::string& block_key) {
        std::string block_data;
        if (auto status = dbm.Get(block_key, &block_data); status != tkrzw::Status::SUCCESS) {
            std::cerr << "Error retrieving block: " << status << std::endl;
            return "";
        } else {
            std::cout << "Block retrieved successfully with key: " << block_key << std::endl;
        }
        return block_data;
    }

    // Delete a block from the database
    bool deleteBlock(const std::string& block_key) {
        auto status = dbm.Remove(block_key);
        if (status != tkrzw::Status::SUCCESS) {
            std::cerr << "Error deleting block: " << status << std::endl;
            return false;
        }
        return true;
    }

private:
    tkrzw::HashDBM dbm;
};

#endif // BLOCK_STORE_H
