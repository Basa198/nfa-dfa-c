#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "nfa.h"
#include "dfa.h"

/*
This function checks whether an integer array contains an element or not and returns a boolean value.
Assumptions:
- `arr` is at least of length `size`
*/
static bool contains(int *arr, size_t size, int val) {
  for (int i = 0; i < (int)size; i++) {
    if (arr[i] == val) return true;
  }
  return false;
}

/*
This function serves as the transition function for a `DFA` type object when it's converted from an `NFA` type object.
Assumptions:
- `states` is `MAX_STATES` long and has `-1` at empty indices
- `buf` can store `MAX_STATES` number of integers
*/
static void nfa_transition(void (*delta)(int, char, int*), int* states, char symbol, int* buf) {
  size_t c = 0;
  int res[MAX_STATES];
  for (int i = 0; i < MAX_STATES; i++) res[i] = -1;
  int cur_states[MAX_STATES];
  epsilon_closure(delta, states, cur_states);
  for (int i = 0; c < MAX_STATES && i < MAX_STATES; i++) {
    if (cur_states[i] == -1) break;
    int new_state[MAX_STATES];
    for (int j = 0; j < MAX_STATES; j++) new_state[j] = -1;
    delta(cur_states[i], symbol, new_state);
    for (int j = 0; c < MAX_STATES && j < MAX_STATES; j++) {
      if (new_state[j] == -1) break;
      if (!contains(res, c, new_state[j])) res[c++] = new_state[j];
    }
  } 
  epsilon_closure(delta, res, buf); 
}

/*
This function creates an object of type `NFA` 
Assumptions:
- `num_states` is the size of `accepting_states[]`
- `num_states` is less than `MAX_STATES`
*/
NFA* create_nfa(int initial_state, int accepting_states[], size_t num_states, void (*delta)(int, char, int*)) {
  if (num_states > MAX_STATES) return NULL;
  NFA *nfa = calloc(1, sizeof(NFA));
  if (!nfa) return NULL;
  nfa->initial_state = initial_state;
  for (int i = 0; i < (int)num_states; i++) nfa->accepting_states[accepting_states[i]] = true;
  nfa->delta = delta;
  return nfa;
}

/*
This function determines whether the given non-deterministic automaton accepts an input
Returns true if it accepts, false otherwise
*/
bool check_nfa(NFA *nfa, char *input) {
  if (!nfa) return false;
  int res[MAX_STATES];
  run_nfa(nfa, input, res); // returned res must have -1 at empty indices
  for (int i = 0; i < MAX_STATES; i++) {
    if (res[i] == -1) break;
    if (nfa->accepting_states[res[i]]) return true;
  }
  return false;
}

/*
This function determines the epsilon closure based on the delta function provided.
The result will be written to `buf`.
Assumptions:
- `buf` can store `MAX_STATES` number of integers
- `source_states[]` is `MAX_STATES` long and has `-1` value at empty indices
*/
void epsilon_closure(void (*delta)(int, char, int *buf), int source_states[], int *buf) {
  if (!delta) return;
  int states[MAX_STATES];
  int count = 0;
  while (source_states[count] != -1) {
    states[count] = source_states[count]; 
    count++;
  }
  for (int i = count; i < MAX_STATES; i++) states[i] = -1;
  for (int i = 0; count < MAX_STATES && i < MAX_STATES; i++) {
    if (states[i] == -1) break;
    int new_state[MAX_STATES];
    for (int j = 0; j < MAX_STATES; j++) new_state[j] = -1; 
    delta(states[i], '\0', new_state);
    for (int j = 0; count < MAX_STATES && j < MAX_STATES; j++) {
      if (new_state[j] == -1) break;
      if (!contains(states, count, new_state[j])) states[count++] = new_state[j];
    }
  }
  memcpy(buf, states, MAX_STATES * sizeof(int));
}

/*
This function determines the possible end states for a non-deterministic automaton given an input
The result is written to `buf`
Assumptions:
- `buf` can store `MAX_STATES` number of integers
*/
void run_nfa(NFA *nfa, char *input, int *buf) {
  if (!nfa) return;
  int cur_state[MAX_STATES];
  cur_state[0] = nfa->initial_state;
  for (int i = 1; i < MAX_STATES; i++) cur_state[i] = -1;
  epsilon_closure(nfa->delta, cur_state, cur_state);
  for (int i = 0; input[i] != '\0'; i++) { // iterate over input
    int new_state[MAX_STATES];
    for (int j = 0; j < MAX_STATES; j++) new_state[j] = -1;
    size_t size = 0;
    for (int j = 0; j < MAX_STATES; j++) { // iterate over every current state
      if (cur_state[j] == -1) break;
      int temp[MAX_STATES];
      for (int k = 0; k < MAX_STATES; k++) temp[k] = -1;
      nfa->delta(cur_state[j], input[i], temp);
      for (int k = 0; k < MAX_STATES; k++) { // iterate over returned states
        if (temp[k] == -1) break;
        if (!contains(new_state, size, temp[k])) new_state[size++] = temp[k];
      }
    }
    epsilon_closure(nfa->delta, new_state, cur_state);
  }
  memcpy(buf, cur_state, MAX_STATES * sizeof(int)); 
}

/*
This function converts a non-deterministic automaton to a deterministic automaton.
*/
DFA* to_dfa(NFA *nfa) {
  if (!nfa) return NULL;
  DFA *dfa = calloc(1, sizeof(DFA));
  if (!dfa) return NULL;
  dfa->initial_state = nfa->initial_state;
  dfa->delta = nfa->delta;
  dfa->transition = nfa_transition;
  memcpy(dfa->accepting_states, nfa->accepting_states, MAX_STATES * sizeof(bool));
  return dfa;
}