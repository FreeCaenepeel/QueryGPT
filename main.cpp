#include <stdio.h>

#include "query_gpt.cpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <math.h>
#include <iterator>
#include <map>
#include <array>
#include <list>
#include <set>
#include <cstdlib>
#include <numeric>

using namespace std;


// convert query to input matrix
vector<vector<double>> query_to_input (vector<string> input) {
    vector<vector<double>> result;

    for (int i = 0; i < int(input.size()); i++) {
        if (vocab.find(input[i]) != vocab.end()){
            result.push_back(word2vec.at(vocab.at(input[i])));
        }
        // set to an ampersand value
        else {
            result.push_back({0.3,0.3});
        }
    }
    
    // add eos
    for (int i = int(input.size()); i < query_length; i++) {
        result.push_back({1.5,-0.5});
    }
    return result;
}

// specific for tool. Convert query string to input matrix
vector<vector<double>> string_to_input (string query) {
    vector<string> input;
    string temp = "";
    for (int i = 0; i < query.size(); i++) {
        temp += query[i];
        if (word2int.find(temp) != word2int.end()){
            input.push_back(temp);
            temp = "";
        }
        // correct for upper case
        if (temp == "select") {
            input.push_back("SELECT");
            temp = "";
        }
        if (temp == "where") {
            input.push_back("WHERE");
            temp = "";
        }
        if (temp == "from") {
            input.push_back("FROM");
            temp = "";
        }
        if (temp == "delete") {
            input.push_back("DELETE");
            temp = "";
        }
        if (temp == "limit") {
            input.push_back("LIMIT");
            temp = "";
        }
    }

    return query_to_input(input);
    
}

// get correct_word_ids for correct query
vector<int> get_answer(vector<string> input) {
    vector<int> result;
    for (int i = 0; i < input.size(); i++) {
        if (word2int.find(input[i]) != word2int.end()){
            result.push_back(word2int.at(input[i]));
        }
        // the ampersand if not found
        else {
            result.push_back(14);
        }
    }
    
    // add eos to the end
    for (int i = int(input.size()); i < query_length; i++) {
        result.push_back(21);
    }
    
    return result;
}

// helper function to split incorrect and correct query
vector<string> split_q_and_a(string str, char del){
    vector<string> result;
    string temp = "";
    for (int i = 0; i < int(str.size()); i++){
         if(str[i] != del){
            temp += str[i];
        }
        else{
            result.push_back(temp);
            temp = "";
        }
    }
    
    result.push_back(temp);
    return result;
}
    

// helper function to split a string based on delimiter
vector<string> split(string str, char del){
    vector<string> result;
    string temp = "";
    int j = 0;
    while (str[j] != '\'') {
        j += 1;
    }
    for (int i = j; i < int(str.size()); i++){
         if(str[i] != del){
            temp += str[i];
        }
        else{
            // remove the single quotes
            result.push_back(temp.substr(1,temp.size()-2));
            temp = "";
        }
    }
    
    result.push_back(temp.substr(1,temp.size()-3));
    return result;
}


int main(int argc, char* argv[]) {
    
    vector<vector<double>> q_weights;  // 2 * 2
    vector<vector<double>> k_weights;  // 2 * 2
    vector<vector<double>> v_weights;  // 2 * 2
    vector<vector<double>> ff1_w_weights;  // 2 * 8
    vector<vector<double>> ff1_b_weights;  // 22 * 8
    vector<vector<double>> ff2_w_weights;  // 8 * 22
    vector<vector<double>> ff2_b_weights;  // 22 * 22
    
    double q_weightss[embedding_dim][embedding_dim] = {1.71631, 1.71631, -0.812487, -0.812487};  // 2 * 2
    
    double k_weightss[embedding_dim][embedding_dim] = {  0.377074, 0.377074, -1.80125, -1.80125 };  // 2 * 2
    
    double v_weightss[embedding_dim][embedding_dim] = {    2.00466, 0.910564, -0.160152, -1.02154};  // 2 * 2
    
    double ff1_w_weightss[embedding_dim][4 * embedding_dim] = {-1.36439, 2.20278, -1.26935, 2.85113, -0.569728, 0.170308, 4.52362, 1.62538, -1.92178, -4.50923, -2.76996, -3.55029, 2.66461, 3.01759, -0.999209, 1.33364
};  // 2 * 8
    
    double ff1_b_weightss[query_length][4 * embedding_dim] = { -3.38335, -6.26667, -4.9557, -6.02154, -2.55221, -4.57527, -7.00655, -2.54401, 1.77318, -1.50159, -1.41302, 2.9762, 0.266354, 0.606537, -1.61038, 6.01747, -3.24546, -7.91396, -4.7728, -5.80605, -4.2911, -4.51302, -8.01361, -3.92615, -3.38641, -0.190462, -3.3727, 1.3803, -0.0260585, -0.50356, -2.00851, 4.07905, -3.22832, -7.23318, -4.80348, -3.29875, 2.26313, -2.04828, -7.3413, -4.20552, 3.05979, -1.78479, -2.42781, 3.11769, 0.0416153, 0.720442, -0.132294, 6.12772, 1.74883, -0.66746, -1.67163, -1.99024, 2.18018, 1.85085, -4.04321, -2.27154, 2.51403, -1.51689, -2.32733, 2.55229, 0.0308127, 0.532138, 0.0592857, 6.35095, -3.7672, -0.772377, -3.94654, -3.00288, -5.85328, 1.46616, -0.995483, -1.59905, -2.77308, -5.06393, -1.75627, -4.00691, -3.88744, -3.84065, -3.75122, -4.33647, -3.07202, -0.268322, -2.32495, -4.65389, -5.64352, 0.816839, -1.43428, -1.17475, -2.63336, -2.25936, -3.11579, -4.04205, -5.23542, -0.624447, -2.92314, -4.29357, -3.16728, -1.03917, -1.73032, -4.60401, -5.88191, -3.40426, -1.53921, -0.875156, -3.23633, -1.55811, -1.03336, -3.32118, -3.13786, -3.42256, -1.32356, -4.09007, -2.61177, -2.3852, -2.18978, -2.3974, -2.83051, -0.561043, -3.78179, -1.95209, -3.21788, -1.04573, 0.23429, -3.18712, -4.31087, -1.58108, -1.1385, -4.09837, -2.88456, -1.31121, -3.14206, -3.68187, -0.944375, 0.515122, -2.54308, -1.50388, -0.0734685, -2.12389, -2.70254, -0.633059, -2.07368, -1.06051, -2.23192, -1.4383, -0.936263, -2.6197, 3.68555, -4.37181, -0.580754, -0.400222, -1.65168, -0.87494, -3.00884, -5.93448, -1.82879, -3.37445, -5.08181, -0.458408, -2.95312, -3.98633, -0.758545, -3.04363, 1.69516, -4.06329, -1.81437, -1.86443, -1.47111, -1.02779, -1.80399, -4.49503, -1.73192, -2.80155, -4.03156, -1.96776, 0.186461, -3.83004};  // 22 * 8
    
    double ff2_w_weightss[4 * embedding_dim][vocab_size] = {0.410803, 1.18041, -4.4916, 1.82667, 3.80833, 4.65751, -0.584905, 0.4303, -1.00336, -6.82191, 4.11464, -0.00353393, 0.0428991, 4.72722, 1.37456, -0.221377, 0.323173, 0.86154, 0.916184, 2.67683, -0.559764, -0.66463, 1.77044, 1.46968, 1.50483, 0.718223, 3.21691, -1.70186, 2.40835, 3.77684, 3.97882, -2.96631, 1.19353, -5.44654, 0.41196, -1.51393, 0.87837, -0.148937, 2.00345, -0.117996, -0.140525, 2.51885, 1.10485, -1.919, 1.67593, -0.772001, 2.12709, 1.96003, -2.08677, -3.15012, -0.26793, 1.24744, -0.700052, 3.22852, -4.80022, 3.20486, 0.604078, -1.55773, 1.39262, 3.68037, 1.27676, 2.16627, 2.15648, 1.13709, 1.75754, -1.28027, 1.99218, 0.0161683, 2.57276, 2.67156, -1.25702, 3.52717, 1.54219, 0.855638, 1.34498, 0.575039, -0.20297, -2.76609, 3.36876, 2.88317, 1.47325, -0.566253, -4.87169, -7.23387, 3.04691, -2.73905, 3.13904, 3.62807, 2.30681, -2.7658, 1.26166, 4.32303, 3.0923, 3.1396, -0.952534, 1.42733, 0.867703, -0.437576, -3.19317, -1.77828, 0.616776, 2.46456, 1.08655, 1.25065, 0.72264, 1.67984, 1.43976, -4.98009, 1.08911, 0.339107, 0.904755, 0.923843, 0.880941, 1.60033, 1.18239, -0.622878, 2.66631, 0.534227, 0.247456, -1.85673, 0.157477, 1.95861, 1.08829, -2.68657, 0.753386, 1.19001, 0.147031, 1.87394, 2.10881, 1.10302, 1.46111, -2.61573, 0.455495, 0.52774, 0.126861, -1.13261, -0.184232, 1.76783, -3.91322, -4.9297, -2.38611, 2.76272, -2.02018, 5.94891, 1.2768, -1.08186, 0.840333, 2.24836, 3.6518, 5.48739, 3.68692, -7.00978, 0.842137, 6.0344, 1.37119, 0.621359, 1.69441, -0.611323, 1.61866, 1.94123, 2.97084, -1.11131, -0.533497, -1.28222, -1.96741, -3.65798, 4.20872, 8.04909, 1.50593, 4.80973, 0.165065, -4.18425, -5.96657, 1.03875, -0.216026, 2.53564 };  // 8 * 22
    
    double ff2_b_weightss[query_length][vocab_size] = {10.2546, 11.0029, -0.718382, -2.01009, -0.845898, -0.874754, -2.48093, -1.41651, -0.811791, -0.612281, -0.550078, -0.768696, -0.76429, -1.44618, -0.702149, -1.58737, -0.681875, -0.809963, -0.760461, -1.10054, -0.668106, -1.64713, -0.00574958, -0.0110327, -0.0376761, -0.811975, -0.333738, -0.253575, -0.00487751, -0.000990962, -0.000257197, -0.000209876, -0.000111507, -6.01686e-05, -0.199515, 2.71551, -0.185233, -0.456956, -0.0658288, -0.0302312, -0.0121627, -0.0335872, -0.00242991, -0.269318, -1.5712, -1.3796, 8.45572, -1.82114, -2.07185, -2.45224, -1.49043, -1.43397, -1.50383, 9.25822, 7.05504, -1.46679, -1.49957, -2.1114, -1.56888, -1.65889, -1.50153, -2.19439, -1.59563, 5.68402, -1.4341, -1.69752, -0.39577, -0.317982, -0.530307, -1.55431, -1.04465, -0.649601, -0.474324, -0.74916, -0.298682, -0.156354, -0.110671, -0.0872943, 9.43563, 2.822, -0.630244, -1.31778, -0.498703, -0.41475, -0.472376, -0.862844, -0.425434, -1.26636, -2.10549, -0.898235, -1.31357, -0.0698032, -2.44937, -3.40284, -2.685, -1.67118, -1.23594, 10.049, 12.3345, -1.0132, -1.89329, -2.643, -1.32825, -2.91475, -1.12666, -2.04133, -1.66693, 11.202, -1.41349, -1.71321, -0.0135726, -0.0240652, -0.0619963, -0.886139, -0.461589, -0.338794, -0.0127855, -0.0142489, -0.00382441, -0.00320103, -0.00276511, -0.00247231, -0.100055, 3.05747, -0.187918, -0.498443, -0.115542, -0.0518116, -0.03662, -0.0927115, -0.0106104, -0.138299, -0.613798, -0.0660369, -0.346179, 2.33109, 3.46046, 5.47628, -0.368598, -0.282166, -0.0737595, -0.0438089, -0.0141983, -0.0164241, -0.371965, -3.06304, -0.623441, -1.45863, -0.333174, -0.468436, -0.536068, -2.17912, -0.144793, -0.264178, -0.0139906, -0.0242777, -0.0612643, -0.764701, -0.433515, -0.214589, -0.00794603, -0.0209838, -0.00509789, -0.0032147, -0.00277439, -0.00247719, -0.081444, 2.85613, -0.186059, -0.572255, -0.115412, -0.0515871, -0.0324894, -0.128328, -0.0121184, -0.121628, -1.57293, -0.869916, -0.905922, -2.32315, 6.8051, 10.5755, 1.77101, -2.66595, -1.8579, -0.716478, -0.761221, -1.37869, -1.25922, -1.30428, -0.878259, 6.46769, -0.921429, -1.43658, -1.84304, -2.29444, -1.35342, -1.27655, -0.712367, -0.684722, -0.715886, -0.941005, -0.717698, -0.71161, -0.767881, -0.845932, -0.714618, -1.0608, -0.677198, 6.27147, -0.709907, 10.8878, -0.716388, -1.14102, -0.704973, -0.797374, -0.758683, -0.805438, -0.717473, -2.25821, -1.63694, -1.20659, -1.33645, -2.26375, -2.57723, -1.7398, 1.8561, -2.70124, -1.89179, 9.26338, -0.980781, -2.42309, -1.64585, -2.50155, -1.30925, 4.91686, -2.28744, 5.24752, 5.01792, 5.26114, -1.5903, -3.47085, -0.894202, -0.849012, -1.01752, -1.96867, -2.09427, -2.33098, -0.926647, 6.45407, -1.05247, -1.15199, -0.652828, 6.18111, -0.910439, 9.61581, -1.13675, -1.53119, -1.15086, -1.26585, -1.35971, -1.53194, -0.90352, 0.477834, -1.28912, -1.03068, -1.26503, -2.02499, -2.31907, -1.49792, -1.33575, -2.52673, 8.04154, 7.19321, -0.878958, -1.88672, -1.26647, -2.0937, -1.20025, -2.28599, -2.17781, 5.13722, 5.10961, 6.3128, -1.60742, -5.10771, -1.28275, -1.08266, -1.14279, -2.01553, -2.07709, -1.21784, -1.3199, 7.08278, -2.62502, -1.06518, -1.0041, 5.68896, -1.20544, -1.17764, -1.11411, -1.32843, 6.91437, -1.53924, -2.0584, -1.558, 6.74834, -1.62032, -0.557575, -0.522984, -0.553748, -1.1275, -0.846273, -0.624455, -0.71418, -0.876469, 11.3351, -0.491472, -0.469505, -0.60749, -0.661865, -1.05993, -0.580979, -0.873905, -0.552318, -0.594979, -0.628477, -0.850943, -0.585774, 2.44572, -1.25408, -1.04837, -1.15157, -1.26389, -1.90961, -1.03284, -1.08892, -1.433, -2.15436, -1.45873, -0.96217, 4.14634, -1.19111, 10.426, -1.07457, -1.69412, 6.18547, -1.87944, -2.32504, -1.29053, 5.73509, -2.28058, -0.654464, -0.479935, -0.738637, -2.86239, -0.958287, -0.672143, -1.56192, -0.876339, 8.54493, -0.584386, -0.399512, -0.7558, -0.828794, -1.83427, -0.623584, 7.35202, -0.664468, -0.822996, -0.887136, -0.741809, -0.600564, 1.65048, -0.841146, -0.526542, -0.549948, -2.01088, -0.668282, -0.619163, -2.25082, -0.673644, -0.650816, -0.475267, -0.436944, -0.515419, -0.745345, 12.098, -0.594035, -1.33181, -0.563991, -0.647642, -0.745159, -0.64466, -0.569197, 3.96273, -0.748384, -0.443506, -1.50986, -2.95259, -1.16063, -0.857225, -0.52956, -0.39794, -0.321999, -1.61826, -0.300089, -0.965347, -1.02611, -1.06026, -0.892316, 2.45704, -0.889802, 5.81092, 5.14137, -0.4788, -0.427676, 3.17104, -0.807353, -0.777125, -0.797265, -0.824216, -0.829732, -0.789154, -1.09637, -1.00752, -0.832883, -0.833234, -0.761909, 6.09387, -0.788467, 10.4822, -0.797924, -0.902648, -0.855649, -1.09603, -1.04237, -0.949215, -0.830207, 0.0432265, -0.693187, -0.569462, -0.711117, -1.32225, -1.11462, -0.665277, -0.563353, -0.72893, -0.665511, -0.624654, -0.516752, -0.997984, -0.687683, -0.684328, -0.618578, -1.38654, -0.930467, 7.63195, 7.27805, -0.749822, -0.733776, 0.0542918, -0.617041, -0.545526, -0.565101, -0.703065, -0.692083, -0.584677, -0.643337, -0.646594, -0.620137, -0.531124, -0.529063, 9.46082, -0.567747, -0.56803, -0.570711, -0.651828, -0.583122, -0.651247, -0.889741, -0.664227, -0.583999, 2.94758 };  // 22 * 22
    
    // initialize weights
    q_weights.push_back({0.01,0.01});
    k_weights.push_back({0.01,0.01});
    v_weights.push_back({0.01,0.01});
    
    q_weights.push_back({-0.01,-0.01});
    k_weights.push_back({-0.01,-0.01});
    v_weights.push_back({-0.01,-0.01});
    
    
    for (int i = 0; i < query_length; i++) {
        ff1_b_weights.push_back({0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0});
        ff2_b_weights.push_back({0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0});
    }
    
    ff1_w_weights.push_back({{0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8}});
    ff1_w_weights.push_back({{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0}});
    
    for (int i = 0; i < 8; i++) {
        ff2_w_weights.push_back({0.5,1.0,1.5,2.0,2.5,3.0,-0.5,-1.0,-1.5,-2.0,-2.5,-3.0,1.0,2.0,3.0,2.5,2.0,1.5,1.0,0.5,0.0,-0.5});
    }
    
    
    
    for (int i = 0; i < embedding_dim; i++) {
        for (int j = 0; j < embedding_dim; j++) {
            q_weights[i][j] = q_weightss[i][j];
        }
    }
    for (int i = 0; i < embedding_dim; i++) {
        for (int j = 0; j < embedding_dim; j++) {
            k_weights[i][j] = k_weightss[i][j];
        }
    }

    for (int i = 0; i < embedding_dim; i++) {
        for (int j = 0; j < embedding_dim; j++) {
            v_weights[i][j] = v_weightss[i][j];
        }
    }

    for (int i = 0; i < embedding_dim; i++) {
        for (int j = 0; j < 4 * embedding_dim; j++) {
            ff1_w_weights[i][j] = ff1_w_weightss[i][j];
        }
    }

    for (int i = 0; i < query_length; i++) {
        for (int j = 0; j < 4 * embedding_dim; j++) {
            ff1_b_weights[i][j] = ff1_b_weightss[i][j];
        }
    }

    for (int i = 0; i < 4 * embedding_dim; i++) {
        for (int j = 0; j < vocab_size; j++) {
            ff2_w_weights[i][j] = ff2_w_weightss[i][j];
        }
    }

    for (int i = 0; i < query_length; i++) {
        for (int j = 0; j < vocab_size; j++) {
            ff2_b_weights[i][j] = ff2_b_weightss[i][j];
        }
    }
    
    
    // input for QueryGPT constructor
    vector<vector<double>> input = query_to_input({"SELECT", " ", "*", "FROM", "k", "db", ";"});
    
    QueryGPT* gpt = new QueryGPT(input, q_weights, k_weights, v_weights,
                             ff1_w_weights, ff1_b_weights,
                             ff2_w_weights, ff2_b_weights,
                                 {0, 13, 2, 13, 3, 13, 4, 11, 13, 13,13,13,13,13,13,13,13,13,13,13,13,13});
    // training code
    /*
    fstream file;
    file.open("queries_first.txt", ios::in);
    string line;
    vector<vector<double>> result;
    vector<string> query_with_answer;
    vector<string> line_splitted;
    vector<int> answer;
    vector<string> q;
    int counter = 0;
    int checker = 1;
    
    
    while (1) {
        if (file.eof()) {break;}
        counter += 1;
        if (counter == 100 * checker) {
            std::cout << counter << endl;
            checker += 1;
            std::cout << "loss " << gpt->loss << endl;
        }
        getline(file, line, '\n');
        query_with_answer = split_q_and_a(line,'%');
        line_splitted = split(query_with_answer[0].substr(2,line.size()-1),':');
        result = query_to_input(line_splitted);
        answer = get_answer(split(query_with_answer[1].substr(1,query_with_answer[1].size()-2),':'));
        
        gpt->run_training_sample(result, answer);

        
    }
    
    // print all weights
    std::cout << " q_weights " << endl;
    for (int i = 0; i < embedding_dim; i++) {
        for (int j = 0; j < embedding_dim; j++) {
            std::cout << " " << gpt->q_weights[i][j] << " ";
        }
    }
    std::cout << endl;
    
    std::cout << " k_weights " << endl;
    for (int i = 0; i < embedding_dim; i++) {
        for (int j = 0; j < embedding_dim; j++) {
            std::cout << " " << gpt->k_weights[i][j] << " ";
        }
    }
    std::cout << endl;
    
    std::cout << " v_weights " << endl;
    for (int i = 0; i < embedding_dim; i++) {
        for (int j = 0; j < embedding_dim; j++) {
            std::cout << " " << gpt->v_weights[i][j] << " ";
        }
    }
    std::cout << endl;
    
    std::cout << " ff1_w_weights " << endl;
    for (int i = 0; i < embedding_dim; i++) {
        for (int j = 0; j < 4 * embedding_dim; j++) {
            std::cout << " " << gpt->ff1_w_weights[i][j] << " ";
        }
    }
    std::cout << endl;
    
    std::cout << " ff1_b_weights " << endl;
    for (int i = 0; i < query_length; i++) {
        for (int j = 0; j < 4 * embedding_dim; j++) {
            std::cout << " " << gpt->ff1_b_weights[i][j] << " ";
        }
    }
    std::cout << endl;
    
    std::cout << " ff2_w_weights " << endl;
    for (int i = 0; i < 4 * embedding_dim; i++) {
        for (int j = 0; j < vocab_size; j++) {
            std::cout << " " << gpt->ff2_w_weights[i][j] << " ";
        }
    }
    std::cout << endl;
    
    std::cout << " ff2_b_weights " << endl;
    for (int i = 0; i < query_length; i++) {
        for (int j = 0; j < vocab_size; j++) {
            std::cout << " " << gpt->ff2_b_weights[i][j] << " ";
        }
    }
    std::cout << endl;
    */
     
    
    // test the model
    vector<vector<double>> test1 = query_to_input({"*", " ", "FROM", "SELECT", " ", " ", "db", ";"});
    
    vector<vector<double>> test2 = query_to_input({";","db"," ","FROM"," ","*","SELECT", " "});
    
    vector<vector<double>> test3 = query_to_input({"users", " ", "FROM"," ","SELECT"," ","db", ";"});
    
    vector<vector<double>> test4 = query_to_input({" ","passwords","FROM"," ","SELECT"," ","database"," ", "LIMIT","10"," ",";"});
    
    vector<vector<double>> test5 = query_to_input({"passwords",",", "users", " ", "FROM"," ","SELECT"," ","database"," ", "LIMIT", " ", "10",";"});
    
    vector<vector<double>> test6 = query_to_input({"*", " ", "FROM", "SELECT", " ", " ", "db", "WHERE", " ", "users", "=", "'", "admin",";"});
    
    vector<vector<double>> test7 = query_to_input({"id",",", "passwords", " ", " ", "FROM", "SELECT", " ", "db", "WHERE", " ", "id", "=", "fred","'",";"});
    
    vector<vector<double>> test8 = query_to_input({"DELETE"," ", "id",",", "passwords", " ", "FROM"," ", "database", "WHERE", " ", "users", "=", "admin","'",";"});
    
    vector<vector<double>> test9 = query_to_input({"DELETE"," ", "users",",", "passwords", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=", "'", "admin", " ", "LIMIT", " ", "100", ";"});
    
    vector<vector<double>> test10 = query_to_input({"SELECT", " ", "*", " ", "FROM", " ", "db", "LIMIT", " ", "5",";"});
    
    vector<vector<vector<double>>> tests = {test1,test2,test3,test4,test5,test6,test7,test8,test9,test10};
    
    for (int i = 0; i < tests.size(); i++) {
        std::cout << "suggested correction of test" << i << ":" << endl;
        gpt->correct_query(tests[i]);
        std::cout << endl;
        std::cout << "suggested correction of test" << i << ":" << endl;
        gpt->correct_query(tests[i]);
        std::cout << endl;
        std::cout << "suggested correction of test" << i << ":" << endl;
        gpt->correct_query(tests[i]);
        std::cout << endl;
    }
    
   
    // for online tool
    /*
    vector<vector<double>> requested = string_to_input(argv[1]);
    std::cout << " first suggestion: ";
    gpt->correct_query(requested);
    std::cout << endl;
    std::cout << " second suggestion: ";
    gpt->correct_query(requested);
    std::cout << endl;
    std::cout << " third suggestion:";
    gpt->correct_query(requested);
    */
    
}

