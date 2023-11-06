
#include <stdio.h>

#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <stack>
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
#include <assert.h>

const int query_length = 22;
const int embedding_dim = 2;
const int vocab_size = 22;

using namespace std;


class Node
{
        
protected:
    

 public:
    vector<Node*> myArguments;
    double myResult;
    double myAdjoint = 0.0;
    bool visited = false;
    
    virtual ~Node() {}

    //  Access result
    double result()
    {
        return myResult;
    }
    //  Access adjoint
    double& adjoint()
    {
        return myAdjoint;
    }
    void resetAdjoints()
    {
        for (auto argument : myArguments) argument->resetAdjoints();
        myAdjoint = 0.0;
    }
    virtual void propagateAdjoint() = 0;
    virtual void evaluate() = 0;
    

    
};
           
class PlusNode : public Node
{
public:
    PlusNode(Node* lhs, Node* rhs)
    {
        myArguments.resize(2);
        myArguments[0] = lhs;
        myArguments[1] = rhs;
        myResult = lhs->result() + rhs->result();
    }
    void propagateAdjoint() override
    {
        myArguments[0]->adjoint() += myAdjoint;
        myArguments[1]->adjoint() += myAdjoint;
    }
    
    void evaluate() override {
        myResult = myArguments[0]->result() + myArguments[1]->result();
    }
};

class TimesNode : public Node
{
public:
    TimesNode(Node* lhs, Node* rhs)
    {
        myArguments.resize(2);
        myArguments[0] = lhs;
        myArguments[1] = rhs;
        myResult = lhs->result() * rhs->result();
    }
    void propagateAdjoint() override
    {
        myArguments[0]->adjoint() += myAdjoint * myArguments[1]->result();
        myArguments[1]->adjoint() += myAdjoint * myArguments[0]->result();
    }
    
    void evaluate() override {
        myResult = myArguments[0]->result() * myArguments[1]->result();
    }
};

class DivisionNode : public Node
{
public:
    DivisionNode(Node* lhs, Node* rhs)
    {
        myArguments.resize(2);
        myArguments[0] = lhs;
        myArguments[1] = rhs;
        myResult = lhs->result() / rhs->result();
    }
    void propagateAdjoint() override
    {
        myArguments[0]->adjoint() += myAdjoint / myArguments[1]->result();
        myArguments[1]->adjoint() += myAdjoint * (-myArguments[0]->result()) / myArguments[1]->result() / myArguments[1]->result() ;
    }
    
    void evaluate() override {
        myResult = myArguments[0]->result() / myArguments[1]->result();
    }
};

class LogNode : public Node
{
public:
    LogNode(Node* arg)
    {
        myArguments.resize(1);
        myArguments[0] = arg;
        myResult = log(arg->result());
    }
    
    void propagateAdjoint() override
        {
            myArguments[0]->adjoint() += myAdjoint / myArguments[0]->result();
        }
    
    void evaluate() override {
        myResult = log(myArguments[0]->result());
    }
};


class SigmoidNode : public Node
{
public:
    SigmoidNode(Node* arg)
    {
        myArguments.resize(1);
        myArguments[0] = arg;
        myResult = 1.0 / (1.0 + exp(-arg->result()));
    }
    
    void propagateAdjoint() override
        {
            myArguments[0]->adjoint() += myAdjoint * (1.0 / (1.0 + exp(-myArguments[0]->result()))) * (1.0 - (1.0 / (1.0 + exp(-myArguments[0]->result()))));
        }
    
    void evaluate() override {
        myResult = 1.0 / (1.0 + exp(-myArguments[0]->result()));
    }
};

class ExpminusNode : public Node
{
public:
    ExpminusNode(Node* arg)
    {
        myArguments.resize(1);
        myArguments[0] = arg;
        myResult = exp(-arg->result());
    }
    
    void propagateAdjoint() override
        {
            myArguments[0]->adjoint() += myAdjoint * (-1.0) * exp(-myArguments[0]->result());
        }
    
    void evaluate() override {
        myResult = exp(-myArguments[0]->result());
    }
};

class ExpNode : public Node
{
public:
    ExpNode(Node* arg)
    {
        myArguments.resize(1);
        myArguments[0] = arg;
        myResult = exp(arg->result());
    }
    
    void propagateAdjoint() override
        {
            myArguments[0]->adjoint() += myAdjoint * exp(myArguments[0]->result());
        }
    
    void evaluate() override {
        myResult = exp(myArguments[0]->result());
    }
};

class ReLuNode : public Node
{
public:
    ReLuNode(Node* arg)
    {
        myArguments.resize(1);
        myArguments[0] = arg;
        myResult = max(0.0,arg->result());
    }
    
    void propagateAdjoint() override
        {
            if (this->myResult > 0.0) {
                myArguments[0]->adjoint() += myAdjoint;
            }
            else {
                myArguments[0]->adjoint() = 0.0;
            }
        }
    
    void evaluate() override {
        myResult = max(0.0, myArguments[0]->result());
    }
};


class Leaf: public Node
{
public:
    
    double myValue;
    Leaf(double val)
    {
        myValue = val;
    }
    double getVal()
    {
        return myValue;
    }
    void setVal(double val)
    {
        myValue = val;
    }
    void propagateAdjoint() override {}
    
    void evaluate() override {
        myResult = myValue;
    }
};

class Number
{
    
public:
    Node* myNode;
    Number(double val)
        : myNode(new Leaf(val))
    {
}
    Number(Node* node)
        : myNode(node) {}
    Node* node()
    {
        return myNode;
    }
    
    Number operator+(Number rhs)
    {
     // Create node: note eagerly computes result
     Node* n = new PlusNode(this->node(), rhs.node());
     // Return result
     return n;
    }
    
    Number operator*(Number rhs)
    {
        //  Create node: note eagerly computes result
        Node* n = new TimesNode(this->node(), rhs.node());
        // Return result
        return n;
    }
    
    Number operator/(Number rhs)
    {
        //  Create node: note eagerly computes result
        Node* n = new DivisionNode(this->node(), rhs.node());
        // Return result
        return n;
    }

    Number log()
    {
        // Create node: note eagerly computes result
        Node* n = new LogNode(this->node());
        // Return result
        return n;
    }
    
    Number sigmoid()
    {
        // Create node: note eagerly computes result
        Node* n = new SigmoidNode(this->node());
        // Return result
        return n;
    }
    
    Number expminus()
    {
        // Create node: note eagerly computes result
        Node* n = new ExpminusNode(this->node());
        // Return result
        return n;
    }
    
    Number exp()
    {
        // Create node: note eagerly computes result
        Node* n = new ExpNode(this->node());
        // Return result
        return n;
    }
    
    Number relu()
    {
        // Create node: note eagerly computes result
        Node* n = new ReLuNode(this->node());
        // Return result
        return n;
    }
};
        
class QueryNN {
public:
    stack<Node*> traversal;
    int size;
    
    vector<vector<Node*>> k_weight_nodes;   // embedding_dim x embedding_dim
    vector<vector<Node*>> q_weight_nodes;   // embedding_dim x embedding_dim
    vector<vector<Node*>> v_weight_nodes;   // embedding_dim x embedding_dim
    
    vector<vector<Node*>> ff1_w_weight_nodes;   // embedding_dim x 4 * embedding_dim
    vector<vector<Node*>> ff1_b_weight_nodes;   // query_length x 4 * embedding_dim
    
    vector<vector<Node*>> ff2_w_weight_nodes;   // 4 * embedding_dim x embedding_dim
    vector<vector<Node*>> ff2_b_weight_nodes;   // query_length x embedding_dim
    
    vector<vector<Node*>> output_nodes;
    vector<vector<Node*>> end_nodes;    // query_length * embedding_dim
    Node* end_node;
    
    
    // construct the neural network for state value function
    QueryNN (vector<vector<double>> inputs, vector<vector<double>> q_weights, vector<vector<double>> k_weights, vector<vector<double>> v_weights,
              vector<vector<double>> ff1_w_weights,
              vector<vector<double>> ff1_b_weights,
              vector<vector<double>> ff2_w_weights,
              vector<vector<double>> ff2_b_weights,
              vector<int> correct_words) {
        
        // input layer contains length of query input:
        // query length 10, embedding dimension: 2 => 10 x 2 matrix
        // attention layer: w_Q, w_K, w_V 2 x 2 matrices
        // Q = I * w_Q, K = I * w_K, V = I * w_v
        // scores = softmax(1/4 Q * K^T) * V + input
        
        // feed forward NN
        // first layer input (query_length x embedding_dim)
        // w_weights (embedding_dim x 4 * embedding_dim)
        // b_weights (query_length x 4 * embedding_dim)
        
        // relu
        
        // second layer input (query_length x 4 * embedding_dim)
        // w_weights (4 * embedding_dim x 14)
        // b_weights (query_length x 14)
        
        // output (query_length x embedding_dim)
        
        this->nn_function(inputs, k_weights, q_weights, v_weights, ff1_w_weights, ff1_b_weights, ff2_w_weights, ff2_b_weights, correct_words);
    }
    
    void remake(vector<vector<double>> inputs, vector<vector<double>> q_weights, vector<vector<double>> k_weights, vector<vector<double>> v_weights,
                vector<vector<double>> ff1_w_weights,
                vector<vector<double>> ff1_b_weights,
                vector<vector<double>> ff2_w_weights,
                vector<vector<double>> ff2_b_weights,
                vector<int> correct_words) {
        // empty stack
        while (!this->traversal.empty()) {
            this->traversal.pop();
        }
        
        this->k_weight_nodes.clear();
        this->q_weight_nodes.clear();
        this->v_weight_nodes.clear();
        
        this->ff1_w_weight_nodes.clear();
        this->ff1_b_weight_nodes.clear();
        this->ff2_w_weight_nodes.clear();
        this->ff2_b_weight_nodes.clear();
        
        this->output_nodes.clear();
        
        this->end_nodes.clear();
        
        this->nn_function(inputs, k_weights, q_weights, v_weights,
                          ff1_w_weights, ff1_b_weights, ff2_w_weights, ff2_b_weights,
                          correct_words);
        
    }
    
    void nn_function(vector<vector<double>> inputs, vector<vector<double>> q_weights, vector<vector<double>> k_weights, vector<vector<double>> v_weights,
                     vector<vector<double>> ff1_w_weights,
                     vector<vector<double>> ff1_b_weights,
                     vector<vector<double>> ff2_w_weights,
                     vector<vector<double>> ff2_b_weights,
                     vector<int> correct_words) {
        
        vector<vector<Number>> input_nodes;
        vector<vector<Number>> k_weight_nodes;
        vector<vector<Number>> q_weight_nodes;
        vector<vector<Number>> v_weight_nodes;
        
        vector<vector<Number>> ff1_w_weight_nodes;
        vector<vector<Number>> ff1_b_weight_nodes;
        vector<vector<Number>> ff2_w_weight_nodes;
        vector<vector<Number>> ff2_b_weight_nodes;
        
        vector<Number> temp_vec;
        vector<Node*> temp_vec2;
        
        
        // query_length x embedding_dim inputs
        for (int i = 0; i < inputs.size(); i++) {
            temp_vec.clear();
            for (int j = 0; j < inputs[i].size(); j++) {
                temp_vec.push_back(Number(inputs[i][j]));
            }
            input_nodes.push_back(temp_vec);

        }
        
        // k_weights: embedding_dim x embedding_dim
        for (int i = 0; i < k_weights.size(); i++) {
            temp_vec.clear();
            temp_vec2.clear();
            for (int j = 0; j < k_weights[i].size(); j++) {
                temp_vec.push_back(Number(k_weights[i][j]));
                temp_vec2.push_back(temp_vec[j].myNode);
            }
            k_weight_nodes.push_back(temp_vec);
            this->k_weight_nodes.push_back(temp_vec2);
        }
        
        // q_weights: embedding_dim x embedding_dim
        for (int i = 0; i < q_weights.size(); i++) {
            temp_vec.clear();
            temp_vec2.clear();
            for (int j = 0; j < q_weights[i].size(); j++) {
                temp_vec.push_back(Number(q_weights[i][j]));
                temp_vec2.push_back(temp_vec[j].myNode);
            }
            q_weight_nodes.push_back(temp_vec);
            this->q_weight_nodes.push_back(temp_vec2);
        }
        
        // v_weights: embedding_dim x embedding_dim
        for (int i = 0; i < v_weights.size(); i++) {
            temp_vec.clear();
            temp_vec2.clear();
            for (int j = 0; j < v_weights[i].size(); j++) {
                temp_vec.push_back(Number(v_weights[i][j]));
                temp_vec2.push_back(temp_vec[j].myNode);
            }
            v_weight_nodes.push_back(temp_vec);
            this->v_weight_nodes.push_back(temp_vec2);
        }
        
        // ff1_w_weights: embedding_dim x 4 * embedding_dim
        for (int i = 0; i < ff1_w_weights.size(); i++) {
            temp_vec.clear();
            temp_vec2.clear();
            for (int j = 0; j < ff1_w_weights[i].size(); j++) {
                temp_vec.push_back(Number(ff1_w_weights[i][j]));
                temp_vec2.push_back(temp_vec[j].myNode);
            }
            ff1_w_weight_nodes.push_back(temp_vec);
            this->ff1_w_weight_nodes.push_back(temp_vec2);
        }
        
        // ff1_b_weights: query_length x 4 * embedding_dim
        for (int i = 0; i < ff1_b_weights.size(); i++) {
            temp_vec.clear();
            temp_vec2.clear();
            for (int j = 0; j < ff1_b_weights[i].size(); j++) {
                temp_vec.push_back(Number(ff1_b_weights[i][j]));
                temp_vec2.push_back(temp_vec[j].myNode);
            }
            ff1_b_weight_nodes.push_back(temp_vec);
            this->ff1_b_weight_nodes.push_back(temp_vec2);
        }
        
        // ff2_w_weights: 4 * embedding_dim x vocab_size
        for (int i = 0; i < ff2_w_weights.size(); i++) {
            temp_vec.clear();
            temp_vec2.clear();
            for (int j = 0; j < ff2_w_weights[i].size(); j++) {
                temp_vec.push_back(Number(ff2_w_weights[i][j]));
                temp_vec2.push_back(temp_vec[j].myNode);
            }
            ff2_w_weight_nodes.push_back(temp_vec);
            this->ff2_w_weight_nodes.push_back(temp_vec2);
        }
        
        // ff2_b_weights: query_length x vocab_size
        for (int i = 0; i < ff2_b_weights.size(); i++) {
            temp_vec.clear();
            temp_vec2.clear();
            for (int j = 0; j < ff2_b_weights[i].size(); j++) {
                temp_vec.push_back(Number(ff2_b_weights[i][j]));
                temp_vec2.push_back(temp_vec[j].myNode);
            }
            ff2_b_weight_nodes.push_back(temp_vec);
            this->ff2_b_weight_nodes.push_back(temp_vec2);
        }
        
        
        // K = I * w_k (query_length x embedding_dim)
        vector<vector<Number>> K = this->matrix_mul(input_nodes,k_weight_nodes);
        
        // Q = I * w_q (query_length x embedding_dim)
        vector<vector<Number>> Q = this->matrix_mul(input_nodes,q_weight_nodes);
        
        // V = I * w_v (query_length x embedding_dim)
        vector<vector<Number>> V = this->matrix_mul(input_nodes,v_weight_nodes);
        
        // dot product
        vector<vector<Number>> dot = this->matrix_mul(Q, this->transpose(K));
        
        // scalar product with 1/(embedding_dim)**2
        for (int i = 0; i < query_length; i++) {
            for (int j = 0; j < query_length; j++) {
                dot[i][j] = Number(1.0/embedding_dim/embedding_dim) * dot[i][j];
            }
        }
        
        // soft max dot product
        dot = this->matrix_softmax(dot);
        
        // scores  = dot * V (query_length x embedding_dim)
        
        vector<vector<Number>> scores = this->matrix_mul(dot, V);
        
        // add the original input
        
        scores = this->matrix_sum(scores, input_nodes);
        

        // Feed forward NN
        // first layer
        scores = this->matrix_sum(this->matrix_mul(scores,ff1_w_weight_nodes), ff1_b_weight_nodes);
        
        
        // relu
        for (int i = 0; i < scores.size(); i++) {
            for (int j = 0; j < scores[i].size(); j++) {
                scores[i][j] = scores[i][j].relu();
            }
        }
        
        // second layer
        scores = this->matrix_sum(this->matrix_mul(scores,ff2_w_weight_nodes), ff2_b_weight_nodes);

        vector<vector<Number>> probs;
        
        for (int i = 0; i < query_length; i++) {
            temp_vec.clear();
            temp_vec2.clear();
            temp_vec = this->row_softmax(scores[i]);
            probs.push_back(temp_vec);
            for (int j = 0; j < vocab_size; j++) {
                temp_vec2.push_back(probs[i][j].myNode);
            }
            this->end_nodes.push_back(temp_vec2);
        }
        
        vector<vector<Number>> outputs;
        
        // for every word in query, compare with correct query
        for (int i = 0; i < query_length; i++) {
            temp_vec.clear();
            temp_vec2.clear();
            for (int j = 0; j < vocab_size; j++) {
                if (j == correct_words[i]) {
                    temp_vec.push_back(Number(-1.0) * probs[i][correct_words[i]].log());
                }
                else {
                    temp_vec.push_back(Number(0.0) * probs[i][j].log());
                }
                temp_vec2.push_back(temp_vec[j].myNode);
            }
                                
            outputs.push_back(temp_vec);
            this->output_nodes.push_back(temp_vec2);
        }
        
        Number y = Number(0.0);
        for (int i = 0; i < query_length; i++) {
            for (int j = 0; j < vocab_size; j++) {
                y = y + outputs[i][j];
            }
        }
        
        this->end_node = y.myNode;
    }
    
    vector<vector<Number>> matrix_sum(vector<vector<Number>> A, vector<vector<Number>> B) {
        assert(A.size() == B.size());
        assert(A[0].size() == B[0].size());
        vector<vector<Number>> C;
        
        vector<Number> temp_vec;
        for (int i = 0; i < A.size(); i++) {
            temp_vec.clear();
            for (int j = 0; j < A[0].size(); j++) {
                temp_vec.push_back(Number(0.0));
            }
            C.push_back(temp_vec);
        }
        
        for (int i = 0; i < A.size(); i++) {
            for (int j = 0; j < A[0].size(); j++) {
                C[i][j] = A[i][j] + B[i][j];
            }
        }
        
        return C;
    }
    
    vector<vector<Number>> matrix_mul(vector<vector<Number>> A, vector<vector<Number>> B) {
        assert(A[0].size() == B.size());
        vector<vector<Number>> C;
        
        vector<Number> temp_vec;
        for (int i = 0; i < A.size(); i++) {
            temp_vec.clear();
            for (int j = 0; j < B[0].size(); j++) {
                temp_vec.push_back(Number(0.0));
            }
            C.push_back(temp_vec);
        }
        
        for (int i = 0; i < A.size(); i++) {
            for (int j = 0; j < B[0].size(); j++) {
                for (int k = 0; k < B.size(); k++) {
                    C[i][j] = C[i][j] + A[i][k] * B[k][j];
                }
            }
        }
        
        return C;
    }
    
    vector<vector<Number>> transpose(vector<vector<Number>> A) {
        vector<vector<Number>> B;
        vector<Number> temp_vec;
        for (int j = 0; j < A[0].size(); j++) {
            temp_vec.clear();
            for (int i = 0; i < A.size(); i++) {
                temp_vec.push_back(Number(0.0));
            }
            B.push_back(temp_vec);
        }
        for (int i = 0; i < B.size(); i++) {
            for (int j = 0; j < B[0].size(); j++) {
                B[i][j] = B[i][j] + A[j][i];
            }
        }
        
        return B;
    }
    
    vector<vector<Number>> matrix_softmax(vector<vector<Number>> A) {
        vector<vector<Number>> B;
        
        vector<Number> temp_vec;
        Number denominator = Number(0.0);
        for (int i = 0; i < A.size(); i++) {
            temp_vec.clear();
            for (int j = 0; j < A[0].size(); j++) {
                temp_vec.push_back(A[i][j].exp());
                denominator = denominator + temp_vec[j];
            }
            B.push_back(temp_vec);
        }
        
        for (int i = 0; i < B.size(); i++) {
            for (int j = 0; j < B[0].size(); j++) {
                B[i][j] = B[i][j] / denominator;
            }
        }
        
        return B;
    }
    
    vector<Number> row_softmax(vector<Number> A) {
        vector<Number> B;
        
        Number denominator = Number(0.0);
        for (int i = 0; i < A.size(); i++) {
            B.push_back(A[i].exp());
            denominator = denominator + B[i];
        }
        
        for (int i = 0; i < B.size(); i++) {
            B[i] = B[i] / denominator;
        }
        
        return B;
    }
    
    // evaluate using DFS and produce the stack
    double evaluate(Node* n) {
        if (n->visited == false) {
            for (auto arg : n->myArguments) {
                this->evaluate(arg);
            }
            n->evaluate();
            
            this->traversal.push(n);
            n->visited = true;
        }
        return this->end_node->result();
    }
    
    void propagate_adjoints(Node* start_node) {
        Node* n = this->traversal.top();
        while (n != start_node) {
            this->traversal.pop();
            n = traversal.top();
        }
        n->myAdjoint = 1.0;
        n->propagateAdjoint();
        this->traversal.pop();
        while ( !this->traversal.empty()) {
            n = this->traversal.top();
            this->traversal.pop();
            n->propagateAdjoint();
        }
        
    }

};
