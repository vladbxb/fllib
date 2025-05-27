
# Formal Language Library
FLLib is an open source library for modelling various objects, principles and operations in the Theory of Computation. 
## Why?
This project is more of a humble attempt at making a library, designed more as a coding exercise in mind, and not production use (efficiency was not at all a primary goal).
## How to use?
There are various classes modelling the behavior of theoretical models.
### DFA (Deterministic Finite Automaton)
The definition for this class can be found in `include/DFA.hpp`. The most convenient way to use it is by passing **initializer lists** as parameters.
#### Example:
In order to create a DFA which accepts the language `L = { w | w has an even number of 0's and an odd number of 1's }`, we need to construct a transition table. A transition table for this language looks like this:
| DFA | 0  | 1  |
| --- | -- | -- |
| q0  | q2 | q1 |
| q1  | q3 | q0 |
| q2  | q0 | q3 |
| q3  | q1 | q2 |

After including `DFA.hpp`, one may construct a DFA object for this transition table like so:
```
DFA dfa({
		{"q0", '0', "q2"},
		{"q0", '1', "q1"},
		{"q1", '0', "q3"},
		{"q1", '1', "q0"},
		{"q2", '0', "q0"},
		{"q2", '1', "q3"},
		{"q3", '0', "q1"},
		{"q3", '1', "q2"}
	},
	"q0",
	{"q1"}
);
```
Not bad, right? The good news is, the ctor can also take **sets** instead of initializer lists, so making a graphical interface should be easy enough.

#### How to use?
For testing strings, one may write:
```
dfa.test("00111"); // Accepted
dfa.test("110"); // Denied
```
## NFA (Non-deterministic Finite Automaton)
The definition for this class can be found in `include/NFA.hpp`. The most convenient way to use it is also by passing **initializer lists** as parameters.
### Example:
In order to create an NFA which accepts the language `L = { w | w ends in the substring "01" or "10" }`, we need to construct a transition table. A transition table for this language looks like this:
| NFA        | 0          | 1          |
| ---------- | ---------- | ---------- |
| q0 init    | `{q0, q1}` | `{q0, q2}` |
| q1         |            | q3         |
| q2         | q4         |            |
| q3 accept  |            |            |
| q4 accept  |            |            |

After including `NFA.hpp`, one may construct an NFA object for this transition table like so:
```
NFA nfa({
			{"q0", '0', {"q0", "q1"}},
			{"q0", '1', {"q0", "q2"}},
			{"q1", '1', {"q3"}},
			{"q2", '0', {"q4"}}
	},
	"q0",
	{"q3", "q4"}
);
```
#### How to use?
For testing strings, one may write:
```
nfa.test("0001"); // Accepted
nfa.test("11101010"); // Accepted
nfa.test("000011000111"); // Denied
nfa.test("11100100"); // Denied
```
