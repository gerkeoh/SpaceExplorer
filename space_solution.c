#include "space_explorer.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    unsigned int *visited_planets;
    double *distances;
    int size;
    int count;
} ShipState;

void ensure_capacity(ShipState *state, int required_capacity) {
    if (required_capacity > state->size) {
        int new_size = required_capacity * 2;
        state->visited_planets = realloc(state->visited_planets, new_size * sizeof(unsigned int));
        state->distances = realloc(state->distances, new_size * sizeof(double));
        if (!state->visited_planets || !state->distances) {
            fprintf(stderr, "Failed to allocate memory\n");
            exit(EXIT_FAILURE);
        }
        state->size = new_size;
    }
}

void initialize_state(ShipState **state) {
    *state = malloc(sizeof(ShipState));
    if (!*state) {
        fprintf(stderr, "Failed to allocate ShipState\n");
        exit(EXIT_FAILURE);
    }
    (*state)->size = 10;
    (*state)->count = 0;
    (*state)->visited_planets = malloc((*state)->size * sizeof(unsigned int));
    (*state)->distances = malloc((*state)->size * sizeof(double));
    if (!(*state)->visited_planets || !(*state)->distances) {
        fprintf(stderr, "Failed to allocate arrays\n");
        exit(EXIT_FAILURE);
    }
}

ShipAction space_hop(unsigned int crt_planet,
                     unsigned int *connections,
                     int num_connections,
                     double distance_from_mixer,
                     void *ship_state) {
    ShipState *state = (ShipState *)ship_state;
    ShipAction action;

    if (state == NULL) {
        initialize_state(&state);
    }

    ensure_capacity(state, state->count + 1);
    state->visited_planets[state->count] = crt_planet;
    state->distances[state->count] = distance_from_mixer;
    state->count++;

    printf("Current Planet ID: %u, Distance from Mixer: %.2f\n", crt_planet, distance_from_mixer);

    unsigned int next_planet = RAND_PLANET;
    for (int i = 0; i < num_connections; i++) {
        int already_visited = 0;
        for (int j = 0; j < state->count; j++) {
            if (connections[i] == state->visited_planets[j]) {
                already_visited = 1;
                break;
            }
        }
        if (!already_visited) {
            next_planet = connections[i];
        }
    }

    printf("Hopping to Planet ID: %u\n", next_planet);

    action.next_planet = next_planet;
    action.ship_state = state;
    return action;
}
