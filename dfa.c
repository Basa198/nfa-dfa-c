#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "nfa.h"
#include "dfa.h"

static void transition(void (*delta)(int, char, int*), int* cur_states, char symbol, int* buf) {
  delta(cur_states[0], symbol, buf);
}

DFA* create_dfa(int initial_state, int accepting_states[], size_t num_states, void (*delta)(int, char, int*)) {
  if (num_states > MAX_STATES) return NULL;
  DFA *dfa = calloc(1, sizeof(DFA));
  if (!dfa) return NULL;
  dfa->initial_state = initial_state;
  for (int i = 0; i < (int)num_states; i++) dfa->accepting_states[accepting_states[i]] = true;
  dfa->delta = delta;
  dfa->transition = transition;
  dfa->num_accepting_states = num_states;
  return dfa;
}

bool check_dfa(DFA *dfa, char *input) {
  if (!dfa) return false;
  int res[MAX_STATES];
  run_dfa(dfa, input, res); 
  for (int i = 0; i < MAX_STATES; i++) {
    if (res[i] == -1) break;
    if (dfa->accepting_states[res[i]]) return true;
  }
  return false;
}

void run_dfa(DFA *dfa, char *input, int *buf) {
  if (!dfa) return;
  
  int cur_state[MAX_STATES]; 
  cur_state[0] = dfa->initial_state;
  for (int i = 1; i < MAX_STATES; i++) cur_state[i] = -1;
  if (strlen(input) == 0) {
    int temp[MAX_STATES];
    for (int i = 0; i < MAX_STATES; i++) temp[i] = -1;
    dfa->transition(dfa->delta, cur_state, '\0', temp);
    if (temp[0] == -1) memcpy(buf, cur_state, MAX_STATES * sizeof(int));
    else memcpy(buf, temp, MAX_STATES * sizeof(int));
    return;
  }
  for (int i = 0; input[i] != '\0'; i++) {
    int new_state[MAX_STATES];
    dfa->transition(dfa->delta, cur_state, input[i], new_state);
    memcpy(cur_state, new_state, MAX_STATES * sizeof(int));
  }
  memcpy(buf, cur_state, MAX_STATES * sizeof(int));
}
