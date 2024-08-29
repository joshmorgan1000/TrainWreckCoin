#ifndef SIMPLE_NEURAL_NETWORK_H
#define SIMPLE_NEURAL_NETWORK_H

#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

class SimpleNeuralNetwork {
public:
    // Constructor
    SimpleNeuralNetwork(int input_size, int hidden_size, int output_size)
        : input_size(input_size), hidden_size(hidden_size), output_size(output_size) {
        initializeWeights();
    }

    // Forward pass
    std::vector<float> forward(const std::vector<float>& input) {
        // Input to hidden layer
        hidden_layer.resize(hidden_size);
        for (int i = 0; i < hidden_size; ++i) {
            hidden_layer[i] = 0.0f;
            for (int j = 0; j < input_size; ++j) {
                hidden_layer[i] += input[j] * weights_input_hidden[i][j];
            }
            hidden_layer[i] = relu(hidden_layer[i]);
        }

        // Hidden to output layer
        output_layer.resize(output_size);
        for (int i = 0; i < output_size; ++i) {
            output_layer[i] = 0.0f;
            for (int j = 0; j < hidden_size; ++j) {
                output_layer[i] += hidden_layer[j] * weights_hidden_output[i][j];
            }
        }

        // Apply softmax
        return softmax(output_layer);
    }

    // Training function (one step of gradient descent)
    void train(const std::vector<float>& input, const std::vector<float>& target, float learning_rate) {
        // Forward pass
        std::vector<float> output = forward(input);

        // Compute loss (cross-entropy) and gradients
        std::vector<float> output_error(output_size);
        for (int i = 0; i < output_size; ++i) {
            output_error[i] = output[i] - target[i];
        }

        // Backpropagation from output to hidden layer
        for (int i = 0; i < hidden_size; ++i) {
            float hidden_error = 0.0f;
            for (int j = 0; j < output_size; ++j) {
                hidden_error += output_error[j] * weights_hidden_output[j][i];
                weights_hidden_output[j][i] -= learning_rate * output_error[j] * hidden_layer[i];
            }
            // Backpropagation from hidden to input layer
            for (int j = 0; j < input_size; ++j) {
                weights_input_hidden[i][j] -= learning_rate * hidden_error * relu_derivative(hidden_layer[i]) * input[j];
            }
        }
    }

private:
    int input_size, hidden_size, output_size;
    std::vector<std::vector<float>> weights_input_hidden;
    std::vector<std::vector<float>> weights_hidden_output;
    std::vector<float> hidden_layer, output_layer;

    // Initialize weights randomly
    void initializeWeights() {
        weights_input_hidden.resize(hidden_size, std::vector<float>(input_size));
        weights_hidden_output.resize(output_size, std::vector<float>(hidden_size));

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-0.5, 0.5);

        for (auto& row : weights_input_hidden)
            std::generate(row.begin(), row.end(), [&]() { return dis(gen); });

        for (auto& row : weights_hidden_output)
            std::generate(row.begin(), row.end(), [&]() { return dis(gen); });
    }

    // ReLU activation function
    float relu(float x) const {
        return std::max(0.0f, x);
    }

    // ReLU derivative
    float relu_derivative(float x) const {
        return x > 0.0f ? 1.0f : 0.0f;
    }

    // Softmax function for output layer
    std::vector<float> softmax(const std::vector<float>& input) const {
        std::vector<float> output(input.size());
        float sum = 0.0f;
        for (float val : input) {
            sum += std::exp(val);
        }
        for (size_t i = 0; i < input.size(); ++i) {
            output[i] = std::exp(input[i]) / sum;
        }
        return output;
    }
};

#endif // SIMPLE_NEURAL_NETWORK_H
