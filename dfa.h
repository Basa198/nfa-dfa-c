#ifndef __B_DFA__
#define __B_DFA__

#include <stdbool.h>

#include "nfa.h"
#include "config.h"

struct NFA;

typedef struct DFA {
  void (*delta)(int, char, int *buf); 
  void (*transition)(void (*delta)(int, char, int *buf), int*, char, int*);
  bool accepting_states[MAX_STATES];
  int initial_state;  
} DFA;

DFA* create_dfa(int initial_state, int accepting_states[], size_t num_states, void (*delta)(int, char, int*));
bool check_dfa(DFA *dfa, char *input);
void run_dfa(DFA *dfa, char *input, int *buf);
struct NFA* to_nfa(DFA *dfa);

#endif