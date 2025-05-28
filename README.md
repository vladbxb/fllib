
# Formal Language Library
FLLib is an open source library for modelling various objects, principles and operations in the Theory of Computation. 
## Why?
This project is more of a humble attempt at making a library, designed more as a coding exercise in mind, and not production use (efficiency was not at all a primary goal).
## What is implemented?
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
### NFA (Non-deterministic Finite Automaton)
The definition for this class can be found in `include/NFA.hpp`. The most convenient way to use it is also by passing **initializer lists** as parameters.
#### Example:
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

### RegEx (Regular Expression)
The definition for this class can be found in `include/RegEx.hpp`. This class implements a regular expression purely for conversion to NFA. The most convenient way to use is by passing **a regular string**, along with **a notation type** as parameters.
#### Example:
In order to create a regular expression such as `ab*ab*a`, after including `RegEx.hpp`, since this is **infix notation**, we can write:
```
RegEx example("ab*ab*a", RegExNotation::Infix);
```
in order to achieve this. Postfix notation is also supported by passing `RegExNotation::Postfix` as the second parameter.

#### How to use?
For testing strings, it is advised to create an NFA based on this regular expression, since making one-off tests meant creating an NFA at each request, which users may not want. In order to test, taking into account the above example, one may write:
```
NFA nfa = example.toNFA();
nfa.test("aaba"); // Accepted
nfa.test("abbbbabbba"); // Accepted
nfa.test("abaab"); // Denied
nfa.test("bbabaab"); // Denied
```
### Moore Machine
The definition for this class can be found in `include/MooreMachine.hpp`. This class implements a classic Moore Machine (transducer). Usage is similar to the other automatons. The most convenient way to use is by passing **initializer lists** as arguments.
#### Example
In order to create a Moore machine, the process is very similar to the other automata. After including `MooreMachine.hpp`, an example of creating a MooreMachine object is the following:
```
MooreMachine a({
                    {"q0", '0', "q1"}, // input transitions
                    {"q0", '1', "q2"},
                    {"q1", '0', "q1"},
                    {"q1", '1', "q3"},
                    {"q2", '0', "q2"},
                    {"q2", '1', "q3"},
                    {"q3", '0', "q3"},
                    {"q3", '1', "q2"}
                },
                {
                    {"q0", 'a'}, // output for each state
                    {"q1", 'c'},
                    {"q2", 'a'},
                    {"q3", 'b'}
                },
                "q0"
            );
```
#### How to use?
For testing, the user can write:
```
a.test("001101"); // This will output "ccbaab" to stdio.
```

### Mealy Machine
The definition for this class can be found in `include/MealyMachine.hpp`. This class implements a classic Mealy Machine (transducer). Usage is similar to the other automatons. The most convenient way to use is by passing **initializer lists** as arguments.
#### Example
In order to create a Mealy machine, the process is very similar to the other automata. After including `MealyMachine.hpp`, an example of creating a MealyMachine object is the following:
```
MealyMachine a({
                 {"q0", '0', "q1"}, // input transitions
                 {"q0", '1', "q2"},
                 {"q1", '0', "q1"},
                 {"q1", '1', "q3"},
                 {"q2", '0', "q3"},
                 {"q2", '1', "q2"},
                 {"q3", '0', "q3"},
                 {"q3", '1', "q3"}
               },
               {
                 {"q0", '0', 'a'}, // output symbols
                 {"q0", '1', 'a'},
                 {"q1", '0', 'b'},
                 {"q1", '1', 'c'},
                 {"q2", '0', 'c'},
                 {"q2", '1', 'a'},
                 {"q3", '0', 'c'},
                 {"q3", '1', 'b'}
               },
               "q0"
            );
```
#### How to use?
For testing, the user can write:
```
a.test("0111"); // This will output "acbb" to stdio.
```
### PDA (Pushdown Automata)
The definition for PDA can be found at `include/PDA.hpp`. This class implements a non-deterministic pushdown automaton. The only recommended way of creating PDA objects is with **initializer lists**, even though the syntax is a bit tricky.
#### Example
For example, in order to create a PDA which accepts the language `L = { a^nb^n | n > 0 }`, you would need to construct a transition table, and that's a bit tedious to do. After including `PDA.hpp`, one may construct a PDA object like this:
```
PDA pda({
            { {"q0", 'Z'}, 'a', { {"q0", "AZ"} } }, // the triplet is on the left hand side
                                                    // state, stack symbol and then input symbol
            { {"q0" ,'A'}, 'a', { {"q0", "AA"} } }, // on the right hand side, it's a set of pairs
                                                    // state and stack operation
            { {"q0", 'A'}, 'b', { {"q1", Symbol::lambda()} } },
            { {"q1", 'A'}, 'b', { {"q1", Symbol::lambda()} } },
            { {"q1", 'Z'}, Symbol::lambda(), { {"q1", Symbol::lambda()} } }
        },
        "q0",
        'Z',
        {}
    );
```
#### How to use?
In order to test the above PDA, one may write:
```
pda.test("aabb"); // Accepted
pda.test("aaaabbbb"); // Accepted
pda.test("aabaaabbbbb"); // Denied
```

## Additional information
The above examples can easily be accessed in the `demos/` folder from the git repository.

## Credits
Some of the examples above were taken from [GeeksForGeeks](https://www.geeksforgeeks.org) and [Tutorialspoint](https://www.tutorialspoint.com).
