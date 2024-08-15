#ifndef __B_NFA__
#define __B_NFA__

#include <stdbool.h>

#include "dfa.h"
#include "config.h"

struct DFA;

typedef struct NFA {
  int initial_state;
  bool accepting_states[MAX_STATES];
  void (*delta)(int, char, int *buf);
} NFA;

NFA* create_nfa(int initial_state, int accepting_states[], size_t num_states, void (*delta)(int, char, int*));
bool check_nfa(NFA *nfa, char *input);
void epsilon_closure(NFA *nfa, int source_states[], size_t count, int *buf);
void run_nfa(NFA *nfa, char *input, int *buf);
void print_nfa(NFA *nfa);
struct DFA* to_dfa(NFA *nfa);

#endif