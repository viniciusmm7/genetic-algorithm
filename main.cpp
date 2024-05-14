#include "main.h"

class Knapsack {
    vector<Item_t> items;
    int total_weight = 0;
    int total_value = 0;
    int capacity;

public:
    explicit Knapsack(int capacity) : capacity(capacity) {}

    void add_item(Item_t item) {
        int weight, value;

        weight = item.weight;
        value = item.value;

        Item_t new_item;

        int index = items.size();

        new_item.index = index;
        new_item.weight = weight;
        new_item.value = value;

        items.push_back(new_item);
    }

    int get_capacity() {
        return capacity;
    }

    const vector<Item_t>& get_items() {
        return items;
    }

    int get_total_weight() {
        return total_weight;
    }

    void set_total_weight(int weight) {
        total_weight = weight;
    }

    int get_total_value() {
        return total_value;
    }

    void set_total_value(int value) {
        total_value = value;
    }

    vector_1d fitness(const ItemPool_t& item_pool, const vector_2d& population) {
        vector<Item_t> pool_items = item_pool.items;
        vector<int> weights, values;

        for (Item_t item : pool_items) {
            weights.push_back(item.weight);
            values.push_back(item.value);
        }

        vector_1d fitness_values;

        for (vector_1d individual : population) {
            int values_sum = 0, weights_sum = 0;

            for (unsigned long i = 0; i < individual.size(); i++) {
                int value_to_sum = individual[i] * values[i];
                int weight_to_sum = individual[i] * weights[i];

                if (weights_sum + weight_to_sum <= capacity) {
                    values_sum += value_to_sum;
                    weights_sum += weight_to_sum;
                }
            }

            if (weights_sum <= capacity) {
                fitness_values.push_back(values_sum);
            }
        }

        return fitness_values;
    }

    vector_2d selection(vector_1d& fitness, const vector_2d& population, int num_parents) {
        vector_2d parents(num_parents, vector<int>(population[0].size()));

        for (int i = 0; i < num_parents; i++) {
            auto max_fitness_idx = max_element(fitness.begin(), fitness.end()) - fitness.begin();
            parents[i] = population[max_fitness_idx];
            fitness[max_fitness_idx] = -9999;
        }

        return parents;
    }

    vector_2d crossover(const vector_2d& parents, int num_offsprings, double crossover_rate = CROSSOVER_RATE) {
        vector_2d offsprings(num_offsprings, vector<int>(parents[0].size()));
        int crossover_point = parents[0].size() / 2;

        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<double> dis(0, 1);

        for (int i = 0; i < num_offsprings; i++) {
            double x = dis(gen);

            if (x > crossover_rate) {
                return offsprings;
            }

            int parent1_index = i % parents.size();
            int parent2_index = (i + 1) % parents.size();

            for (int j = 0; j < crossover_point; j++) {
                offsprings[i][j] = parents[parent1_index][j];
            }

            for (unsigned long j = crossover_point; j < parents[0].size(); j++) {
                offsprings[i][j] = parents[parent2_index][j];
            }
        }

        return offsprings;
    }

    vector_2d mutation(const vector_2d& offsprings, double mutation_rate = MUTATION_RATE) {
        vector_2d mutants(offsprings.size(), vector<int>(offsprings[0].size()));

        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<double> dis(0, 1);
        uniform_int_distribution<int> int_dis(0, offsprings[0].size() - 1);

        for (unsigned long i = 0; i < mutants.size(); i++) {
            double random_value = dis(gen);
            mutants[i] = offsprings[i];

            if (random_value <= mutation_rate) {
                int int_random_value = int_dis(gen);
                // Concise and efficient way to toggle a boolean (1 | 0) value
                mutants[i][int_random_value] = !mutants[i][int_random_value];
            }
        }

        return mutants;
    }

    tuple_vector optimize(const ItemPool_t& item_pool, vector_2d& population, int sol_per_pop, int num_generations) {
        vector_2d parameters;
        vector_2d fitness_history;

        int num_parents = sol_per_pop / 2;
        int num_offsprings = sol_per_pop - num_parents;

        for (int i = 0; i < num_generations; i++) {
            vector_1d fitness_score = fitness(item_pool, population);
            fitness_history.push_back(fitness_score);

            vector_2d parents = selection(fitness_score, population, num_parents);
            vector_2d offsprings = crossover(parents, num_offsprings);
            vector_2d mutants = mutation(offsprings);

            for (unsigned long j = 0; j < parents.size(); j++) {
                population[j] = parents[j];
            }

            for (unsigned long j = parents.size(); j < population.size(); j++) {
                population[j] = mutants[j - parents.size()];
            }
        }

        vector_1d fitness_last_gen = fitness(item_pool, population);
        auto max_fitness_idx = max_element(fitness_last_gen.begin(), fitness_last_gen.end()) - fitness_last_gen.begin();

        parameters.push_back(population[max_fitness_idx]);

        return make_tuple(parameters, fitness_history);
    }

    void print_results() {
//        cout << "Items in the knapsack: " << endl;
//
//        for (Item_t item : items) {
//            cout << "Item " << item.index + 1 << ": Weight = " << item.weight << ", Value = " << item.value << endl;
//        }

        cout << "Total value: " << total_value << endl;
        cout << "Total weight: " << total_weight << endl;
    }
};

ItemPool_t init_item_pool() {
    ItemPool_t item_pool;
    int n;
//    cout << "Enter how many items exists: ";
    cin >> n;
    item_pool.size = n;

    for (int i = 0; i < n; i++) {
        Item_t item;
        item.index = i;
//        cout << "Enter the weight and value of item " << i + 1 << ": ";
        cin >> item.weight >> item.value;
//        cout << "Weight: " << item.weight << ", Value: " << item.value << endl;
        item_pool.items.push_back(item);
    }

    return item_pool;
}

Knapsack init_knapsack() {
    int capacity;
//    cout << "Enter the weight capacity of the knapsack: ";
    cin >> capacity;
//    cout << "Capacity: " << capacity << endl;

    Knapsack knapsack(capacity);

    return knapsack;
}

vector_2d init_population(int sol_per_pop, int num_of_items) {
    vector_2d initial_population;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 1);

    for (int i = 0; i < sol_per_pop; i++) {
        vector<int> individual;
        individual.reserve(num_of_items);

        for (int j = 0; j < num_of_items; j++) {
            individual.push_back(dis(gen));
        }

        initial_population.push_back(individual);
    }

    return initial_population;
}

int get_num_generations() {
    int num_generations;
//    cout << "Enter the number of generations: ";
    cin >> num_generations;
//    cout << "Number of generations: " << num_generations << endl;

    return num_generations;
}

tuple_int get_knapsack_result(vector_2d parameters, vector<Item_t> items, Knapsack& knapsack) {
    int new_weight = 0;
    int new_value = 0;

    for (unsigned long i = 0; i < parameters[0].size(); i++) {
        if (parameters[0][i]) {
            knapsack.add_item(items[i]);

            if (new_weight + items[i].weight > knapsack.get_capacity()) {
                break;
            }

            new_weight += items[i].weight;
            new_value += items[i].value;
        }
    }

    return make_tuple(new_weight, new_value);
}

int main() {
    ItemPool_t item_pool = init_item_pool();
    Knapsack knapsack = init_knapsack();

    int sol_per_pop = 16; // Number of lines in the initial population matrix
    int num_of_items = item_pool.size; // Number of columns in the initial population matrix

    vector_2d initial_population = init_population(sol_per_pop, num_of_items);

    int num_generations = get_num_generations();

    tuple_vector optimizer_tuple = knapsack.optimize(item_pool, initial_population, sol_per_pop, num_generations);
    vector_2d parameters = get<0>(optimizer_tuple);
    vector_2d fitness_history = get<1>(optimizer_tuple);

    tuple_int results = get_knapsack_result(parameters, item_pool.items, knapsack);
    int new_weight = get<0>(results);
    int new_value = get<1>(results);

    knapsack.set_total_weight(new_weight);
    knapsack.set_total_value(new_value);
    knapsack.print_results();

    return 0;
}