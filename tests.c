#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "dfa.h"
#include "nfa.h"

// DFA for multiples of 3
void mul3_delta(int state, char symbol, int *buf) {
  if (strcmp(symbol, "0") == 0) {
    switch (state) {
      case 0:
        buf[0] = 0;
        break;
      case 1:
        buf[0] = 2;
        break;
      case 2:
        buf[0] = 1;
        break;
      default:
        buf[0] = -1;
        break;
    }
  } else if (strcmp(symbol, "1") == 0) {
    switch (state) {
      case 0:
        buf[0] = 1;
        break;
      case 1:
        buf[0] = 0;
        break;
      case 2:
        buf[0] = 2;
        break;
      default:
        buf[0] = -1;
        break;
    } 
  } else buf[0] = -1;
}

// nfa - fourth from last symbol has to be 1 
void delta_nfa(int state, char symbol, int *buf) {
  if (strcmp(symbol, "0") == 0) {
    switch (state) {
      case 10:
        buf[0] = 10;
        break;
      case 0:
        buf[0] = 1;
        break;
      case 1:
        buf[0] = 2;
        break;
      case 2:
        buf[0] = 3;
      default:
        buf[0] = -1;
        break;
    }
  } else if (strcmp(symbol, "1") == 0) {
    switch (state) {
      case 10:
        buf[0] = 10;
        buf[1] = 0;
        break;
      case 0:
        buf[0] = 1;
        break;
      case 1:
        buf[0] = 2;
        break;
      case 2:
        buf[0] = 3;
        break;
      default:
        buf[0] = -1;
        break;
    }
  } else buf[0] = -1;
}

// nfa with epsilon transitions - even number of 1s or 0s
void delta_eps(int state, char symbol, int *buf) {
  if (strcmp(symbol, "") == 0) {
    switch (state) {
      case 0:
        buf[0] = 1;
        buf[1] = 3;
        break;
      default:
        buf[0] = -1;
        break;
    }
  } else if (strcmp(symbol, "0") == 0) {
    switch (state) {
      case 1:
        buf[0] = 2;
        break;
      case 2:
        buf[0] = 1;
        break;
      case 3:
      case 4:
        buf[0] = state;
        break;
      default:
        buf[0] = -1;
        break;
    }
  } else if (strcmp(symbol, "1") == 0) {
    switch (state) {
      case 1:
      case 2:
        buf[0] = state;
        break;
      case 3:
        buf[0] = 4;
        break;
      case 4:
        buf[0] = 3;
        break;
      default:
        buf[0] = -1;
        break;
    }
  } else buf[0] = -1;
}

int main() {
  DFA *mul3_dfa = create_dfa(0, (int[]){0}, mul3_delta);
  assert(check_dfa(mul3_dfa, "") == true); 
  assert(check_dfa(mul3_dfa, "1") == false); 
  assert(check_dfa(mul3_dfa, "0000") == true); 
  assert(check_dfa(mul3_dfa, "101") == false); 
  assert(check_dfa(mul3_dfa, "1001") == true); 
  assert(check_dfa(mul3_dfa, "1111") == true); 

  NFA *mul3_nfa = to_nfa(mul3_dfa);
  assert(check_nfa(mul3_nfa, "") == true); 
  assert(check_nfa(mul3_nfa, "1") == false); 
  assert(check_nfa(mul3_nfa, "0000") == true); 
  assert(check_nfa(mul3_nfa, "101") == false); 
  assert(check_nfa(mul3_nfa, "1001") == true); 
  assert(check_nfa(mul3_nfa, "1111") == true); 

  free_dfa(mul3_dfa);
  mul3_dfa = to_dfa(mul3_nfa);
  assert(check_dfa(mul3_dfa, "") == true); 
  assert(check_dfa(mul3_dfa, "1") == false); 
  assert(check_dfa(mul3_dfa, "0000") == true); 
  assert(check_dfa(mul3_dfa, "101") == false); 
  assert(check_dfa(mul3_dfa, "1001") == true); 
  assert(check_dfa(mul3_dfa, "1111") == true); 

  free_nfa(mul3_nfa);
  mul3_nfa = to_nfa(mul3_dfa);
  assert(check_nfa(mul3_nfa, "") == true); 
  assert(check_nfa(mul3_nfa, "1") == false); 
  assert(check_nfa(mul3_nfa, "0000") == true); 
  assert(check_nfa(mul3_nfa, "101") == false); 
  assert(check_nfa(mul3_nfa, "1001") == true); 
  assert(check_nfa(mul3_nfa, "1111") == true); 

  free_dfa(mul3_dfa);
  free_nfa(mul3_nfa);

  NFA *nfa = create_nfa(10, (int[]) {3}, delta_nfa); 
  assert(check_nfa(nfa, "") == false);
  assert(check_nfa(nfa, "0000") == false);
  assert(check_nfa(nfa, "1000") == true);
  assert(check_nfa(nfa, "10001") == false);
  assert(check_nfa(nfa, "11111") == true);
  assert(check_nfa(nfa, "10111") == false);
  assert(check_nfa(nfa, "000000001000") == true);
  assert(check_nfa(nfa, "10010010") == false);
  assert(check_nfa(nfa, "111") == false);

  DFA *dfa = to_dfa(nfa);
  assert(check_dfa(dfa, "") == false);
  assert(check_dfa(dfa, "0000") == false);
  assert(check_dfa(dfa, "1000") == true);
  assert(check_dfa(dfa, "10001") == false);
  assert(check_dfa(dfa, "11111") == true);
  assert(check_dfa(dfa, "10111") == false);
  assert(check_dfa(dfa, "000000001000") == true);
  assert(check_dfa(dfa, "10010010") == false);
  assert(check_dfa(dfa, "111") == false);

  free_nfa(nfa);
  nfa = to_nfa(dfa);
  assert(check_nfa(nfa, "") == false);
  assert(check_nfa(nfa, "0000") == false);
  assert(check_nfa(nfa, "1000") == true);
  assert(check_nfa(nfa, "10001") == false);
  assert(check_nfa(nfa, "11111") == true);
  assert(check_nfa(nfa, "10111") == false);
  assert(check_nfa(nfa, "000000001000") == true);
  assert(check_nfa(nfa, "10010010") == false);
  assert(check_nfa(nfa, "111") == false);

  free_dfa(dfa);
  dfa = to_dfa(nfa);
  assert(check_dfa(dfa, "") == false);
  assert(check_dfa(dfa, "0000") == false);
  assert(check_dfa(dfa, "1000") == true);
  assert(check_dfa(dfa, "10001") == false);
  assert(check_dfa(dfa, "11111") == true);
  assert(check_dfa(dfa, "10111") == false);
  assert(check_dfa(dfa, "000000001000") == true);
  assert(check_dfa(dfa, "10010010") == false);
  assert(check_dfa(dfa, "111") == false);

  free_nfa(nfa);
  free_dfa(dfa);

  NFA *nfa_epsilon = create_nfa(0, (int[]){1,3}, delta_eps);
  assert(check_nfa(nfa_epsilon, "") == true);
  assert(check_nfa(nfa_epsilon, "1") == true);
  assert(check_nfa(nfa_epsilon, "0") == true);
  assert(check_nfa(nfa_epsilon, "10") == false);
  assert(check_nfa(nfa_epsilon, "1001") == true);
  assert(check_nfa(nfa_epsilon, "100") == true);
  assert(check_nfa(nfa_epsilon, "111000") == false);
  assert(check_nfa(nfa_epsilon, "11001101") == false);
  assert(check_nfa(nfa_epsilon, "1001") == true);
  assert(check_nfa(nfa_epsilon, "1111000") == true);
  assert(check_nfa(nfa_epsilon, "010101") == false);
  assert(check_nfa(nfa_epsilon, "111111") == true);
  assert(check_nfa(nfa_epsilon, "111110") == false);

  DFA *dfa_epsilon = to_dfa(nfa_epsilon);
  assert(check_dfa(dfa_epsilon, "") == true);
  assert(check_dfa(dfa_epsilon, "1") == true);
  assert(check_dfa(dfa_epsilon, "0") == true);
  assert(check_dfa(dfa_epsilon, "10") == false);
  assert(check_dfa(dfa_epsilon, "1001") == true);
  assert(check_dfa(dfa_epsilon, "100") == true);
  assert(check_dfa(dfa_epsilon, "111000") == false);
  assert(check_dfa(dfa_epsilon, "11001101") == false);
  assert(check_dfa(dfa_epsilon, "1001") == true);
  assert(check_dfa(dfa_epsilon, "1111000") == true);
  assert(check_dfa(dfa_epsilon, "010101") == false);
  assert(check_dfa(dfa_epsilon, "111111") == true);
  assert(check_dfa(dfa_epsilon, "111110") == false);

  free_nfa(nfa_epsilon);
  nfa_epsilon = to_nfa(dfa_epsilon);
  assert(check_nfa(nfa_epsilon, "") == true);
  assert(check_nfa(nfa_epsilon, "1") == true);
  assert(check_nfa(nfa_epsilon, "0") == true);
  assert(check_nfa(nfa_epsilon, "10") == false);
  assert(check_nfa(nfa_epsilon, "1001") == true);
  assert(check_nfa(nfa_epsilon, "100") == true);
  assert(check_nfa(nfa_epsilon, "111000") == false);
  assert(check_nfa(nfa_epsilon, "11001101") == false);
  assert(check_nfa(nfa_epsilon, "1001") == true);
  assert(check_nfa(nfa_epsilon, "1111000") == true);
  assert(check_nfa(nfa_epsilon, "010101") == false);
  assert(check_nfa(nfa_epsilon, "111111") == true);
  assert(check_nfa(nfa_epsilon, "111110") == false);

  free_dfa(dfa_epsilon);
  dfa_epsilon = to_dfa(nfa_epsilon);
  assert(check_dfa(dfa_epsilon, "") == true);
  assert(check_dfa(dfa_epsilon, "1") == true);
  assert(check_dfa(dfa_epsilon, "0") == true);
  assert(check_dfa(dfa_epsilon, "10") == false);
  assert(check_dfa(dfa_epsilon, "1001") == true);
  assert(check_dfa(dfa_epsilon, "100") == true);
  assert(check_dfa(dfa_epsilon, "111000") == false);
  assert(check_dfa(dfa_epsilon, "11001101") == false);
  assert(check_dfa(dfa_epsilon, "1001") == true);
  assert(check_dfa(dfa_epsilon, "1111000") == true);
  assert(check_dfa(dfa_epsilon, "010101") == false);
  assert(check_dfa(dfa_epsilon, "111111") == true);
  assert(check_dfa(dfa_epsilon, "111110") == false);

  free_nfa(nfa_epsilon);
  free_dfa(dfa_epsilon);

  printf("All tests pass\n");
}