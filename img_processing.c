#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define IMAGE_WIDTH 200
#define IMAGE_HEIGHT 200
#define N 50 
#define POPULATION_SIZE 100
#define MAX_GENERATIONS 1000
#define MUTATION_RATE 0.01
#define CROSSOVER_RATE 0.7

typedef struct {
    int x, y;      
    int size;      
    int color;     
} Square;

typedef struct {
    Square squares[N];
    double fitness;
} Image;

void initialize_population(Image population[POPULATION_SIZE]) {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < N; j++) {
            population[i].squares[j].x = rand() % IMAGE_WIDTH;
            population[i].squares[j].y = rand() % IMAGE_HEIGHT;
            population[i].squares[j].size = rand() % 20 + 10;
            population[i].squares[j].color = rand();
        }
    }
}

double color_difference(int color1, int color2) {
    int r1 = (color1 >> 16) & 0xFF;
    int g1 = (color1 >> 8) & 0xFF;
    int b1 = color1 & 0xFF;

    int r2 = (color2 >> 16) & 0xFF;
    int g2 = (color2 >> 8) & 0xFF;
    int b2 = color2 & 0xFF;

    double diff = sqrt((r1 - r2) * (r1 - r2) + (g1 - g2) * (g1 - g2) + (b1 - b2) * (b1 - b2));
    return diff;
}

double evaluate_fitness(Image *image, Image *target) {
    double fitness = 0.0;
    for (int i = 0; i < N; i++) {
        int x = image->squares[i].x;
        int y = image->squares[i].y;
        int color = image->squares[i].color;

        int target_color = target->squares[i].color;
        double color_diff = color_difference(color, target_color);

        fitness += 1000.0 / (1.0 + color_diff);
    }

    return fitness;
}

void selection(Image population[POPULATION_SIZE], Image selected[POPULATION_SIZE]) {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        int tournament_size = 5;
        int best_index = rand() % POPULATION_SIZE;
        for (int j = 1; j < tournament_size; j++) {
            int random_index = rand() % POPULATION_SIZE;
            if (population[random_index].fitness > population[best_index].fitness) {
                best_index = random_index;
            }
        }
        selected[i] = population[best_index];
    }
}

void crossover(Image parent1, Image parent2, Image *child) {
    int crossover_point = rand() % N;
    for (int i = 0; i < N; i++) {
        if (i < crossover_point) {
            child->squares[i] = parent1.squares[i];
        } else {
            child->squares[i] = parent2.squares[i];
        }
    }
}

void mutate(Image *image) {
    int index = rand() % N;
    int mutation_type = rand() % 2;
    if (mutation_type == 0) {
        image->squares[index].x = rand() % IMAGE_WIDTH;
        image->squares[index].y = rand() % IMAGE_HEIGHT;
    } else {
        image->squares[index].size = rand() % 20 + 10;
    }
}

int main() {
    srand(time(NULL));
    Image target_image;
    for (int i = 0; i < N; i++) {
        target_image.squares[i].x = rand() % IMAGE_WIDTH;
        target_image.squares[i].y = rand() % IMAGE_HEIGHT;
        target_image.squares[i].size = rand() % 20 + 10;
        target_image.squares[i].color = rand();
    }

    Image population[POPULATION_SIZE];
    initialize_population(population);
    for (int generation = 0; generation < MAX_GENERATIONS; generation++) {
        for (int i = 0; i < POPULATION_SIZE; i++) {
            population[i].fitness = evaluate_fitness(&population[i], &target_image);
        }

        Image selected[POPULATION_SIZE];
        selection(population, selected);

        Image new_population[POPULATION_SIZE];

        for (int i = 0; i < POPULATION_SIZE; i++) {
            if (rand() < CROSSOVER_RATE * RAND_MAX) {
                int parent1 = rand() % POPULATION_SIZE;
                int parent2 = rand() % POPULATION_SIZE;
                crossover(selected[parent1], selected[parent2], &new_population[i]);
            } else {
                new_population[i] = selected[i];
            }

            if (rand() < MUTATION_RATE * RAND_MAX) {
                mutate(&new_population[i]);
            }
        }

        memcpy(population, new_population, sizeof(new_population));
        printf("Generation %d: Best Fitness = %lf\n", generation, selected[0].fitness);
    }
    return 0;
}