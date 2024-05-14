//
// Created by vinicius on 11/05/24.
//
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;
using vector_2d = vector<vector<int>>;
using vector_1d = vector<int>;
using tuple_int = tuple<int, int>;
using tuple_vector = tuple<vector_2d, vector_2d>;

#ifndef EXTRA_MAIN_H
#define EXTRA_MAIN_H

#endif //EXTRA_MAIN_H

#define CROSSOVER_RATE 0.8
#define MUTATION_RATE 0.05

struct Item_t {
    int index = 0;
    int weight = 0;
    int value = 0;
};

struct ItemPool_t {
    vector<Item_t> items;
    int size = 0;
};

//struct Knapsack_t {
//    vector<Item_t> items;
//    unsigned capacity = 0;
//};