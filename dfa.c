#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "nfa.h"
#include "dfa.h"

DFA* create_dfa(int initial_state, int accepting_states[], size_t num_states, void (*delta)(int, char, int*)) {
  if (num_states > MAX_STATES) return NULL;
  DFA *dfa = calloc(1, sizeof(DFA));
  if (!dfa) return NULL;
  dfa->initial_state = initial_state;
  for (int i = 0; i < (int)num_states; i++) dfa->accepting_states[accepting_states[i]] = true;
  dfa->delta = delta;
  return dfa;
}

bool check_dfa(DFA *dfa, char *input) {
  int res[MAX_STATES];
  for (int i = 0; i < MAX_STATES; i++) res[i] = -1;
  run_dfa(dfa, input, res); 
  for (int i = 0; i < MAX_STATES; i++) {
    if (res[i] == -1) break;
    if (dfa->accepting_states[res[i]]) return true;
  }
  return false;
}

void run_dfa(DFA *dfa, char *input, int *buf) {
  int cur_state[MAX_STATES]; 
  cur_state[0] = dfa->initial_state;
  for (int i = 0; input[i] != '\0'; i++) {
    dfa->delta(cur_state[0], input[i], cur_state);  
    if (cur_state[0] == -1) break;
  }
  buf[0] = cur_state[0];
}
