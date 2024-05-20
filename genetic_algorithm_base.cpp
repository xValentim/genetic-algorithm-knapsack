// Problema da mochila binária

#include <bits/stdc++.h>
#include <vector>
#include <fstream>
#include <iostream>
// include random 
#include <random>
using namespace std;



struct item {
    int id;
    double peso;
    double valor;
};

int is_valid(vector<item> items, vector<int> put , int W) {
    int soma = 0;
    for (int i = 0; i < items.size(); i++) {
        soma += items[i].peso * put[i];
    }
    if (soma <= W) {
        return 1;
    }
    return 0;
}

int fitness(vector<item> items, vector<int> put) {
    int value = 0;
    for (int i = 0; i < items.size(); i++) {
        value += put[i] * items[i].valor;
    }
    return value;
}

void generate_instance(vector<int> &instance) {
    //vector<int> instance(n);
    // set seed 42
    srand(42);
    // random, 0 or 1 using random lib
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 1);

    for (int i = 0; i < instance.size(); i++) {
        instance[i] = dis(gen);
    }
    //return instance;

}


vector<int> crossover(vector<int> a, vector<int> b) {
    int n = a.size();
    vector<int> output(n);
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            output[i] = a[i];
        } else {
            output[i] = b[i];
        }
    }
    return output;
}

vector<int> mutation(vector<int> a) {
    int n = a.size();
    vector<int> output(n);
    int z;
    for (int i = 0; i < n; i++) {
        z = rand() % 100;
        if (z < 5) {
            if (a[i] == 0) {
                output[i] = 1;
            } else {
                output[i] = 0;
            }
        } 
    }
    return output;
}

void fill_random(vector<vector<int>> &output) {
    int m = output.size();
    for (int i = 0; i < m; i++) {
        generate_instance(output[i]);
    }
    //return output;
}

vector<int> heuristica(vector<item> items, int W) {
    
    // Hiperparâmetros
    int n_samples = items.size();
    int max_value = 0;
    int max_iter = 1000;
    int batch_len = 200;
    float epsilon_survivors = 0.2;

    vector<int> output(n_samples);
    vector<int> samples(n_samples);

    // batch size 20
    int n_survivors = epsilon_survivors * batch_len;
    vector<vector<int>> batch_survivors(n_survivors, vector<int>(n_samples, 0)); // Cria uma matriz 100x10 inicializada com zeros
    vector<vector<int>> batch(batch_len, vector<int>(n_samples, 0)); // Cria uma matriz 100x10 inicializada com zeros

    fill_random(batch);

    for (int j = 0; j < max_iter; j++) {
        for (int i = 0; i < batch_len; i++) {
            samples = mutation(batch[i]);
            int value = fitness(items, samples);
            if (is_valid(items, samples, W) && value > max_value){
                max_value = value;
                for (int k = 0; k < n_samples; k++) {
                    output[k] = samples[k];
                }
            }
        }

        // sort batch
        sort(batch.begin(), batch.end());

        // copy best 20 to survivors
        for (int i = 0; i < n_survivors; i++) {
            batch_survivors[i] = batch[i];
        }

        // crossover and create new batch
        for (int i = 0; i < batch_len; i++) {
            batch[i] = crossover(batch_survivors[i % n_survivors], batch_survivors[(i + 1) % n_survivors]);
        }
    }
    
    return output;
}

int main() {

    string filename = "./data/in02.txt";

    int N, W;
    ifstream arquivo(filename);

    arquivo >> N >> W;
    vector<item> items(N);

    for (int i = 0; i < N; i++) {
        arquivo >> items[i].peso >> items[i].valor;
        items[i].id = i;
    }


    arquivo.close();

    vector<int> new_items = heuristica(items, W);

    for (int i = 0; i < new_items.size(); i++) {
        cout << new_items[i] << endl;
    }

    for (int i = 0; i < items.size(); i++) {
        if (new_items[i] == 1) {
            cout << items[i].peso << " " << items[i].valor << endl;
        }
    }

    return 0;

}