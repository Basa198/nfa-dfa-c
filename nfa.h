#ifndef __B_NFA__
#define __B_NFA__

#include <stdbool.h>

#include "dfa.h"
#include "config.h"

struct DFA;

typedef struct NFA {
  void (*delta)(int, char, int *buf);
  bool accepting_states[MAX_STATES];
  int initial_state;
} NFA;

NFA* create_nfa(int initial_state, int accepting_states[], size_t num_states, void (*delta)(int, char, int*));
bool check_nfa(NFA *nfa, char *input);
void epsilon_closure(void (*delta)(int, char, int *buf), int source_states[], int *buf);
void run_nfa(NFA *nfa, char *input, int *buf);
struct DFA* to_dfa(NFA *nfa);

#endif