#include <stdio.h>
#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <math.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <iterator>
#include <map>
#include <array>
#include <list>
#include <set>
#include <cstdlib>
#include <numeric>
#include <random>

#include "neural.cpp"


using namespace std;

const map<int, string> int2word = {
    { 0, "SELECT"},
    { 1, "DELETE"},
    { 2, "*"},
    { 3, "FROM"},
    { 4, "db"},
    { 5, "database"},
    { 6, "WHERE"},
    { 7, "="},
    { 8, "'"},
    { 9, "users"},
    { 10, "passwords"},
    { 11, ";"},
    { 12, ","},
    { 13, " "},
    { 14, "&"},
    { 15, "LIMIT"},
    { 16, "admin"},
    { 17, "100"},
    { 18, "10"},
    { 19, "id"},
    { 20, "fred"},
    { 21, "eos"}
};

const map<string,int> word2int = {
    { "SELECT", 0},
    { "DELETE", 1},
    { "*", 2},
    { "FROM", 3},
    { "db", 4},
    { "database", 5},
    { "WHERE", 6},
    { "=", 7},
    { "'", 8},
    { "users", 9},
    { "passwords", 10},
    { ";", 11},
    { ",", 12},
    { " ", 13},
    { "&", 14},
    { "LIMIT", 15},
    { "admin", 16},
    { "100", 17},
    { "10", 18},
    { "id", 19},
    { "fred", 20},
    { "eos", 21}
    
};

const map<string, vector<int>> vocab = {
    { "SELECT", {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} },
    { "DELETE", {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} },
    { "*", {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} },
    { "FROM", {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} },
    { "db", {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} },
    { "database", {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} },
    { "WHERE", {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} },
    { "=", {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0} },
    { "'", {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0} },
    { "users", {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0} },
    { "passwords", {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0} },
    { ";", {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0} },
    { ",", {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0} },
    { " ", {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0} },
    { "&", {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0} },
    { "LIMIT", {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0} },
    { "admin", {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0} },
    { "100", {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0} },
    { "10", {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0} },
    { "id", {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0} },
    { "fred", {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0} },
    { "eos", {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1} }
};

const map<vector<int>, vector<double>> word2vec = {
    { {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {-1.5,1.5} },
    { {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {-1.5,0.5} },
    { {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {-1.5,0.0} },
    { {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {-0.75,1.0} },
    { {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {-0.75,-1.0} },
    { {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {-0.75,-0.5} },
    { {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0.0,2.0} },
    { {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0.0,-2.0} },
    { {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0.5,-2.0} },
    { {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0}, {-1.5,-0.5} },
    { {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0}, {-1.5,-1.0} },
    { {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0}, {1.5,0.5} },
    { {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0}, {-1.25,-1.0} },
    { {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0}, {0.1,0.1}},
    { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0}, {0.3,0.3} },
    { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0}, {1.0,1.0} },
    { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0}, {0.5,-0.5} },
    { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0}, {1.0,-0.5} },
    { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0}, {1.0,-1.0} },
    { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0}, {-1.5,-1.5} },
    { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0}, {0.5,-1.5} },
    { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, {1.5,-0.5} }
};

/*
// fist attempt of word embedding
const map<vector<int>, vector<double>> word2vec = {
    { {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {-1.0,1.0} },
    { {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {-1.0,0.5} },
    { {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0.0,0.5} },
    { {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0.5,1.0} },
    { {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0.5,0.7} },
    { {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0.5,0.5} },
    { {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {-1.0,-0.5} },
    { {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {-1.0,-0.8} },
    { {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0}, {-0.7,-0.8} },
    { {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0}, {0.0,-0.5} },
    { {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0}, {0.0,-0.7} },
    { {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0}, {0.5,-0.5} },
    { {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0}, {0.5,-0.7} },
    { {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0}, {0.1,0.1}},
    { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0}, {0.3,0.3} },
    { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0}, {-0.8,0.8} },
    { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0}, {0.1,-0.3} },
    { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0}, {-0.6,-1.0} },
    { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0}, {-0.6,-0.8} },
    { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0}, {0.0,-0.8} },
    { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0}, {0.1,-0.4} },
    { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, {0.7,-0.5} }
};
 */

// random generator to generate requested floors
class RNG2
{
public:
    RNG2() : gen(std::random_device()()) {} // Seeds the mt19937.

    double get_random(int b){
        std::uniform_real_distribution<double> dis(0, b);
        double random = dis(gen);
        return random;
    }
    
    int get_random_integer(int b) {
        std::uniform_int_distribution<int> dis(0,b);
        int random = dis(gen);
        return random;
    }
    
private:
    std::mt19937 gen;
};


class QueryGPT {
public:
    RNG2 rng;
    QueryNN network;
    double learning_rate = 0.01;
    double loss;
    
    vector<vector<double>> k_weights;   // embedding_dim x embedding_dim
    vector<vector<double>> q_weights;   // embedding_dim x embedding_dim
    vector<vector<double>> v_weights;   // embedding_dim x embedding_dim
    
    vector<vector<double>> ff1_w_weights;   // embedding_dim x 4 * embedding_dim
    vector<vector<double>> ff1_b_weights;   // query_length x 4 * embedding_dim
    
    vector<vector<double>> ff2_w_weights;   // 4 * embedding_dim x vocab_size
    vector<vector<double>> ff2_b_weights;   // query_length x vocab_size

    QueryGPT (vector<vector<double>> inputs, vector<vector<double>> q_weights, vector<vector<double>> k_weights, vector<vector<double>> v_weights,
              vector<vector<double>> ff1_w_weights,
              vector<vector<double>> ff1_b_weights,
              vector<vector<double>> ff2_w_weights,
              vector<vector<double>> ff2_b_weights,
              vector<int> answer):
        network(inputs, k_weights, q_weights, v_weights, ff1_w_weights, ff1_b_weights, ff2_w_weights, ff2_b_weights, answer)
    {
        this->k_weights = k_weights;
        this->q_weights = q_weights;
        this->v_weights = v_weights;
        
        this->ff1_w_weights = ff1_w_weights;
        this->ff1_b_weights = ff1_b_weights;
        this->ff2_w_weights = ff2_w_weights;
        this->ff2_b_weights = ff2_b_weights;
    }
    
    // given probability distribution, sample word
    int sample_word(vector<double> probs) {
        //  calculate cdf
        for (int i = 1; i < probs.size(); i ++) {
            probs[i] += probs[i-1];
        }

        double r = this->rng.get_random(1);
        int index = 0;
        while (index < probs.size() && r > probs[index]) {
            index += 1;
        }
        return index;
    }

    // run one training sample
    void run_training_sample (vector<vector<double>> inputs, vector<int> answer) {
        
        vector<string> words;
        this->network.remake(inputs, this->k_weights, this->q_weights, this->v_weights, this->ff1_w_weights, this->ff1_b_weights,
                             this->ff2_w_weights, this->ff2_b_weights,
                             answer);
        
        this->network.evaluate(this->network.end_node);
        this->loss = this->network.end_node->result();
        
        // sample the output query
        vector<double> probabilities;
        int index;
        for (int i = 0; i < query_length; i++) {
            probabilities.clear();

            for (int j = 0; j < vocab_size; j++) {
                probabilities.push_back(this->network.end_nodes[i][j]->result());
            }

            index = this->sample_word(probabilities);
            words.push_back(int2word.at(index));
        }
        
        // propagate adjoints
        
        this->network.propagate_adjoints(this->network.end_node);
        
        for (int i = 0; i < this->network.k_weight_nodes.size(); i++) {
            for (int j = 0; j< this->network.k_weight_nodes[0].size(); j++) {
                this->k_weights[i][j] -= this->learning_rate * this->network.k_weight_nodes[i][j]->myAdjoint;
                if (abs(this->k_weights[i][j]) > 100) {
                    std::cout << this->k_weights[i][j] << " ";
                }
            }
        }
        
        for (int i = 0; i < this->network.q_weight_nodes.size(); i++) {
            for (int j = 0; j< this->network.q_weight_nodes[0].size(); j++) {
                this->q_weights[i][j] -= this->learning_rate * this->network.q_weight_nodes[i][j]->myAdjoint;
            }
        }
        
        for (int i = 0; i < this->network.v_weight_nodes.size(); i++) {
            for (int j = 0; j< this->network.v_weight_nodes[0].size(); j++) {
                this->v_weights[i][j] -= this->learning_rate * this->network.v_weight_nodes[i][j]->myAdjoint;
            }
        }
        
        for (int i = 0; i < this->network.ff1_w_weight_nodes.size(); i++) {
            for (int j = 0; j< this->network.ff1_w_weight_nodes[0].size(); j++) {
                this->ff1_w_weights[i][j] -= this->learning_rate * this->network.ff1_w_weight_nodes[i][j]->myAdjoint;
            }
        }
        
        for (int i = 0; i < this->network.ff1_b_weight_nodes.size(); i++) {
            for (int j = 0; j< this->network.ff1_b_weight_nodes[0].size(); j++) {
                this->ff1_b_weights[i][j] -= this->learning_rate * this->network.ff1_b_weight_nodes[i][j]->myAdjoint;
            }
        }
        
        for (int i = 0; i < this->network.ff2_w_weight_nodes.size(); i++) {
            for (int j = 0; j< this->network.ff2_w_weight_nodes[0].size(); j++) {
                this->ff2_w_weights[i][j] -= this->learning_rate * this->network.ff2_w_weight_nodes[i][j]->myAdjoint;
            }
        }
        
        for (int i = 0; i < this->network.ff2_b_weight_nodes.size(); i++) {
            for (int j = 0; j< this->network.ff2_b_weight_nodes[0].size(); j++) {
                this->ff2_b_weights[i][j] -= this->learning_rate * this->network.ff2_b_weight_nodes[i][j]->myAdjoint;
            }
        }
          
    }
    
    void correct_query (vector<vector<double>> inputs) {
        vector<string> words;
        this->network.remake(inputs, this->k_weights, this->q_weights, this->v_weights, this->ff1_w_weights, this->ff1_b_weights,
                             this->ff2_w_weights, this->ff2_b_weights,
                             {0,0,0,0,0,0,0,0,0,0,0,0,0,0});
        
        this->network.evaluate(this->network.end_node);
        
        // sample the output query
        vector<double> probabilities;
        int index;
        for (int i = 0; i < query_length; i++) {
            probabilities.clear();
            for (int j = 0; j < vocab_size; j++) {
                probabilities.push_back(this->network.end_nodes[i][j]->result());
            }
            index = this->sample_word(probabilities);
            words.push_back(int2word.at(index));
            if (index == 21) {break;}
        }
        
        
        // print the sampled query
        for (int i = 0; i < words.size() - 1; i++) {
            std::cout << words[i];
        }
        std::cout << endl;
    }
    

};
