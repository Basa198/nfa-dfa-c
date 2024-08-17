# Usage

Epsilon transitions are supported for all `NFA`s and those `DFA`s that have been converted from an `NFA`

`MAX_STATES` is the maximum number of states supported by an automaton. It is defined in `config.h` file.

State labels must start with 0 and can go up to `MAX_STATES` for correct behavior.

## Run Tests

```Bash
$ make all
$ ./program
```

## DFA

### `DFA` struct:

```C
typedef struct DFA {
  int initial_state;
  bool accepting_states[MAX_STATES];
  void (*delta)(int, char, int *buf);
  void (*transition)(void (*delta)(int, char, int *buf), int*, char, int*);
} DFA;
```

`transition()` is a function used internally by `DFA`. Don't modify it.

### List of functions for `DFA`

```C
DFA* create_dfa(int initial_state, int accepting_states[], size_t num_states, void (*delta)(int, char, int*));
bool check_dfa(DFA *dfa, char *input);
void run_dfa(DFA *dfa, char *input, int *buf); // writes a list of end states to buf
struct NFA* to_nfa(DFA *dfa);
```

### To create a `DFA` use:

```C
DFA* create_dfa(int initial_state, int accepting_states[], size_t num_states, void (*delta)(int, char, int*));

DFA *dfa = create_dfa(0, (int[]){0}, 1, delta);
```

### You can check a given input via:

```C
bool check_dfa(DFA *dfa, char *input);

check(dfa, "1001");
```

## NFA

### `NFA` struct:

```C
typedef struct NFA {
  int initial_state;
  bool accepting_states[MAX_STATES];
  void (*delta)(int, char, int *buf);
} NFA;
```

### List of functions for an `NFA`

```C
NFA* create_nfa(int initial_state, int accepting_states[], size_t num_states, void (*delta)(int, char, int*));
bool check_nfa(NFA *nfa, char *input);
void epsilon_closure(void (*delta)(int, char, int *buf), int source_states[], int *buf);
void run_nfa(NFA *nfa, char *input, int *buf);
struct DFA* to_dfa(NFA *nfa);
```

### To create an `NFA` use:

```C
NFA* create_nfa(int initial_state, int accepting_states[], size_t num_states, void (*delta)(int, char, int*));

NFA *nfa = create_nfa(10, (int[]) {3}, 1, delta);
```

### You can check a given input via:

```C
bool check_nfa(NFA *nfa, char *input);

check_nfa(nfa, "1000");
```

## `DFA`-`NFA` Conversions

You can convert between `NFA` and `DFA`

```C
DFA *dfa = create_dfa(0, (int[]){0}, 1, delta);
NFA *nfa = create_nfa(10, (int[]) {3}, 1, delta);

DFA *dfa_from_nfa = to_dfa(nfa);
NFA *nfa_from_dfa = to_nfa(dfa);
```

## Example of a `delta` function

```C
void delta_eps(int state, char symbol, int *buf) {
  if (!symbol) { // epsilon transition
    switch (state) {
      case 0:
        buf[0] = 1;
        buf[1] = 3;
        break;
      default:
        buf[0] = -1;
        break;
    }
  } else if (symbol == '0') {
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
  } else if (symbol == '1') {
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
```
