#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "nfa.h"
#include "dfa.h"

NFA* create_nfa(int initial_state, int accepting_states[], size_t num_states, void (*delta)(int, char, int*)) {
  if (num_states > MAX_STATES) return NULL;
  NFA *nfa = calloc(1, sizeof(NFA));
  if (!nfa) return NULL;
  nfa->initial_state = initial_state;
  for (int i = 0; i < (int)num_states; i++) nfa->accepting_states[accepting_states[i]] = true;
  nfa->delta = delta;
  return nfa;
}

bool check_nfa(NFA *nfa, char *input) {
  if (!nfa) return false;
  int res[MAX_STATES];
  run_nfa(nfa, input, res);
  for (int i = 0; i < MAX_STATES; i++) {
    if (res[i] == -1) break;
    if (nfa->accepting_states[res[i]]) return true;
  }
  return false;
}

static bool contains(int *arr, size_t size, int val) {
  for (int i = 0; i < (int)size; i++) {
    if (arr[i] == val) return true;
  }
  return false;
}

void epsilon_closure(NFA *nfa, int source_states[], size_t count, int *buf) {
  if (!nfa) return;
  if (count > MAX_STATES) return;
  int states[MAX_STATES];
  memcpy(states, source_states, count * sizeof(int));
  for (int i = (int) count; i < MAX_STATES; i++) states[i] = -1;
  
  for (int i = 0; count < MAX_STATES && i < MAX_STATES; i++) {
    if (states[i] == -1) break;
    int new_state[MAX_STATES];
    for (int j = 0; j < MAX_STATES; j++) new_state[j] = -1; 
    nfa->delta(states[i], '\0', new_state);
    for (int j = 0; count < MAX_STATES && j < MAX_STATES; j++) {
      if (new_state[j] == -1) break;
      if (!contains(states, count, new_state[j])) states[count++] = new_state[j];
    }
  }
  memcpy(buf, states, MAX_STATES * sizeof(int));
}

void run_nfa(NFA *nfa, char *input, int *buf) {
  if (!nfa) return;
  int cur_state[MAX_STATES];
  epsilon_closure(nfa, (int[]) {nfa->initial_state}, 1, cur_state);
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
    epsilon_closure(nfa, new_state, size, cur_state);
  }
  memcpy(buf, cur_state, MAX_STATES * sizeof(int)); 
}

struct DFA* to_dfa(NFA *nfa) {

}