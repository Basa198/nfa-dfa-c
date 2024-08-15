#ifndef __B_DFA__
#define __B_DFA__

#include <stdbool.h>

#include "nfa.h"
#include "config.h"

typedef struct {
  int initial_state;  
  int accepting_states[MAX_STATES];
  void (*delta)(int, char, int *buf);
} DFA;

bool check_dfa(DFA dfa, char *input);
void run_dfa(DFA dfa, char *input, int *buf);
void print_dfa(DFA dfa);
NFA to_nfa(DFA dfa);
void free_dfa(DFA dfa);

#endif