#ifndef __B_NFA__
#define __B_NFA__

#include <stdbool.h>

#include "dfa.h"
#include "config.h"

struct DFA;

typedef struct NFA {
  int initial_state;
  int accepting_states[MAX_STATES];
  void (*delta)(int, char, int *buf);
} NFA;

NFA* create_nfa(int initial_state, int accepting_states[], void (*delta)(int, char, int*));
bool check_nfa(NFA *nfa, char *input);
void epsilon_closure(NFA *nfa, int states[MAX_STATES], int count, int *buf);
void run_nfa(NFA *nfa, char *input, int *buf);
void print_nfa(NFA *nfa);
struct DFA* to_dfa(NFA *nfa);
void free_nfa(NFA *nfa);

#endif