cmake_minimum_required(VERSION 3.26)
project(TrainWreckCoin)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH $ENV{CMAKE_PREFIX_PATH})
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/triplets/arm64-osx.cmake)
find_package(cppzmq REQUIRED)
include_directories(
        ${CMAKE_SOURCE_DIR}/cppzmq
        ${CMAKE_SOURCE_DIR}/communications
        ${CMAKE_SOURCE_DIR}/transactions
        ${CMAKE_SOURCE_DIR}/workers
        ${CMAKE_SOURCE_DIR}/util
        ${CMAKE_SOURCE_DIR}/include
)
link_directories(
        ${CMAKE_SOURCE_DIR}/cppzmq
        ${CMAKE_SOURCE_DIR}/communications
        ${CMAKE_SOURCE_DIR}/transactions
        ${CMAKE_SOURCE_DIR}/workers
        ${CMAKE_SOURCE_DIR}/util
        ${CMAKE_SOURCE_DIR}/include
)
add_executable(TrainWreckCoin
        main.cpp
        transactions/ModelDefinitionTransaction.h
        transactions/DataSampleTransaction.h
        transactions/ProposedWeightsTransaction.h
        transactions/ValidationTransaction.h
        transactions/NextDataStepTransaction.h
        communications/ZeroMQBroadcastWrapper.h
        workers/ValidationWorker.h
        workers/SimpleNeuralNetwork.h
        communications/BlockStore.h
        workers/Benefactor.h
        workers/TrainingMiner.h
        util/Logger.h
)
target_link_libraries(TrainWreckCoin
        cppzmq
        tkrzw
)