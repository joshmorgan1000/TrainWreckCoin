#include <iostream>
#include "BlockStore.h"
#include "Benefactor.h"
#include "TrainingMiner.h"
#include "ValidationWorker.h"
#include "Logger.h"

int main() {
    Logger::log("main", "Opening up the BlockStore database.");
    BlockStore blockStore("blockchain_skip.tkrzw");

    // Start the Benefactor
    Logger::log("main", "Starting the Benefactor.");
    Benefactor benefactor(blockStore, "tcp://*:5555");
    benefactor.start();

    // Start the Training Miner
    Logger::log("main", "Starting the Training Miner.");
    TrainingMiner miner(blockStore, "tcp://localhost:5555");
    miner.start();

    // Start the Validation Worker
    ValidationWorker validator(blockStore, "tcp://localhost:5555");
    validator.start();

    // Let the system run for a while
    std::this_thread::sleep_for(std::chrono::seconds(60));

    // Stop the threads
    benefactor.stop();
    miner.stop();
    validator.stop();

    return 0;
}
