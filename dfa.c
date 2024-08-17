#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "nfa.h"
#include "dfa.h"

/*
This is a transition function for a pure `DFA`. Meaning it was not converted from an `NFA` type object
Assumptions:
- `cur_states` has `initial_state` at `0th` index
- `buf` can store `MAX_STATES` number of integers
- `buf` must have -1 at empty indices (this requirement comes from check_dfa)
*/
static void transition(void (*delta)(int, char, int*), int* cur_states, char symbol, int* buf) {
  delta(cur_states[0], symbol, buf);
}

/*
This function creates a `DFA` type object.
Assumptions:
- `num_states` is the size of `accepting_states[]`
- `num_states` is less than `MAX_STATES`
*/
DFA* create_dfa(int initial_state, int accepting_states[], size_t num_states, void (*delta)(int, char, int*)) {
  if (num_states > MAX_STATES) return NULL;
  DFA *dfa = calloc(1, sizeof(DFA));
  if (!dfa) return NULL;
  dfa->initial_state = initial_state;
  for (int i = 0; i < (int)num_states; i++) dfa->accepting_states[accepting_states[i]] = true;
  dfa->delta = delta;
  dfa->transition = transition;
  return dfa;
}

/*
This function determines whether the given deterministic automaton accepts an input
Returns true if it accepts, false otherwise
*/
bool check_dfa(DFA *dfa, char *input) {
  if (!dfa) return false;
  int res[MAX_STATES];
  run_dfa(dfa, input, res); // returned res must have -1 at empty indices
  for (int i = 0; i < MAX_STATES; i++) {
    if (res[i] == -1) break;
    if (dfa->accepting_states[res[i]]) return true;
  }
  return false;
}

/*
This function determines the end states of a deterministic automaton given an input
The result is written to `buf`
Assumptions:
- `buf` can store `MAX_STATES` number of integers
*/
void run_dfa(DFA *dfa, char *input, int *buf) {
  if (!dfa) return;
  int cur_state[MAX_STATES]; 
  cur_state[0] = dfa->initial_state;
  for (int i = 1; i < MAX_STATES; i++) cur_state[i] = -1;
  if (strlen(input) == 0) {
    int temp[MAX_STATES];
    for (int i = 0; i < MAX_STATES; i++) temp[i] = -1; // see comment in check_dfa when calling run_dfa
    dfa->transition(dfa->delta, cur_state, '\0', temp);
    if (temp[0] == -1) memcpy(buf, cur_state, MAX_STATES * sizeof(int));
    else memcpy(buf, temp, MAX_STATES * sizeof(int));
    return;
  }
  for (int i = 0; input[i] != '\0'; i++) {
    int new_state[MAX_STATES];
    for (int j = 0; j < MAX_STATES; j++) new_state[j] = -1;
    dfa->transition(dfa->delta, cur_state, input[i], new_state);
    memcpy(cur_state, new_state, MAX_STATES * sizeof(int));
  }
  memcpy(buf, cur_state, MAX_STATES * sizeof(int));
}

/*
This function converts a `DFA` type object to an `NFA` type object
*/
NFA* to_nfa(DFA *dfa) {
  if (!dfa) return NULL;
  NFA *nfa = calloc(1, sizeof(NFA));
  if (!nfa) return NULL;
  nfa->initial_state = dfa->initial_state;
  nfa->delta = dfa-> delta;
  memcpy(nfa->accepting_states, dfa->accepting_states, MAX_STATES * sizeof(bool));
  return nfa;
}