// CREDITS
// Referenced my previous code at
//     https://github.com/Bryce-3D/My-Codeforces-Codes/blob/main/
//     C%2B%2B/1801-1900/CF_1833B.cpp
// on how to do a custom comparator

// Referenced
//     https://stackoverflow.com/questions/30149697/defining-hash-function-as-
//     part-of-a-struct
// to give a user-defined hash to a user-defined struct

// https://stackoverflow.com/questions/30149697/defining-hash-function-as-part-
// of-a-struct
// https://stackoverflow.com/questions/16093413/comparison-operator-overloading
// -for-a-struct-symmetrically-comparing-my-struct
// https://www.geeksforgeeks.org/template-specialization-c/

// ChatGPT prompts used:
/*
<other previous CS4234 prompts>

How do I use gp_hash_table in C++?

How do I iterate over the elements of an unordered_map in C++?

In the first for loop in your sample code, what is the data type of pair?

How do I access a character of a string in C++?

How do I get the length of a string n C++?

Does C++ have any form of negative indexing similar to python?

Does C++ have elif like Python?

Does C++ have formatted strings like Python?

Is there a function in C++ to convert a binary string to an integer?

What is the 2nd argument in stoi there for?

What is the data type of charVector.begin() and charVector.end() in the 
first code excerpt there?

Cool thanks

Is there a function in C++ to convert an integer to a binary string?

How do I declare a namespace in C++?

What is the size_t data type in C++?

What is the smallest possible size for size_t in C++?

Do any computers run on systems less than 32 bits?
*/
// CREDITS



/* Idea
2023-08-08
2^20 ~ 10^6 states
Could a dumb search work?
Perform Dijkstra's on the graph of all possible states
Dijkstra is nlgn ~ 2x10^7
20 test cases and 20 items per test case
-> 8x10^9 -> very iffy? idk
If the frontier in Dijkstra's never gets too big (aka insertion into the 
priority queue never becomes too long) then the log n constant will 
significantly drop, perhaps enough to make this pass?

2023-08-17
Python is basically confirmed to die I need to learn C++
This is what I get for putting off learning C++ so much :skull:

2023-08-21
I heard that the time constraints are a bit tight so jic I need to do this
    N = or 0  (can also be xor 0)
    F = xor 1
    S = or 1
    C = and 0
Actually maybe I shud do the bitmask thing jic it TLEs cause Prof Halim 
does make constant factor matter sometimes, and it might be the case here.
Also practice? Idk ig I don't have any other assignments atm aside from 
studying more Japanese.

N = no change
F = slip
S = 1
C = 0

    and orr xor
N =  1   0   0
F =  1   0   1
S =  0   1   0
C =  0   0   0

2023-08-22
Also what am I doing I just need to print the min cost (if possible), no need 
to convert back from int to binary string lmao.
*/

/* DEBUG LOG (cause why not)
1.) Forgot to read L,n_op,n_w from the first line of each test case
*/

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <bitset>
#define ll long long
using namespace std;

namespace Helpers {
    /*
        Homu = [int i, int c]
    i = int value corresponding to the binary string of this node
    c = cost of getting to this node from the start
    */
    struct Homu {
        int i;
        int c;

        Homu(int i, int c) {
            this->i = i;
            this->c = c;
        }
    };
    bool operator < (const Homu& l, const Homu& r) {
        // C++ Priority Queue uses max heap, so I need to flip the  
        // sign to get the lowest to pop first.
        return l.c > r.c;
    }

    /*
        quantum = [i_and, i_or, i_xor]
    A bitmask representation of a quantum operation.
    Applying a quantum operation to a binary string `bs` is equivalent to doing 
        (((x & i_and) | i_or) ^ i_xor)
    where `x` is the integer equivalent of `bs`

    Each character of the quantum op is converted as follows
            and o_r xor
        N =  1   0   0
        F =  1   0   1
        S =  0   1   0
        C =  0   0   0
    */
    struct quantum {
        int i_and;
        int i_or;
        int i_xor;

        quantum(string op) {
            this->i_and = 0;
            this->i_or  = 0;
            this->i_xor = 0;
            int L = op.size();

            for (int exp = 0; exp < L; exp++) {
                char c = op[L-1-exp];
                if (c == 'N') {
                    this->i_and += (1 << exp);
                } else if (c == 'F') {
                    this->i_and += (1 << exp);
                    this->i_xor += (1 << exp);
                } else if (c == 'S') {
                    this->i_or  += (1 << exp);
                } else if (c == 'C') {
                    // Do nothing
                } else {
                    // Bug
                    int 魔法少女まどかマギカ_ワルプルギスの廻天_update_on_Sept_10 = 1/0;
                }
            }
        }
    };
    bool operator == (const quantum& l, const quantum& r) {
        bool Homu = l.i_and == r.i_and;
        bool Kumi = l.i_or  == r.i_or ;
        bool Rina = l.i_xor == r.i_xor;
        return (Homu && Kumi && Rina);
    }
    
    // Helper function to apply a quantum op `op` on an int `i`
    int operate(int i, quantum op) {
        i = i & op.i_and;
        i = i | op.i_or;
        i = i ^ op.i_xor;
        return i;
    }

}

namespace std {
    template<>
    struct hash<Helpers::quantum> {
        inline size_t operator() (const Helpers::quantum& op) const {
            int ans = op.i_and ^ op.i_or ^ op.i_xor;
            return ans;
        }
    };
}

using namespace Helpers;



int main() {
    int Homura;
    cin >> Homura;
    for (int Kumiko = 0; Kumiko < Homura; Kumiko++) {
        
        int L;      // Length of binary words and quantum operations
        int n_op;   // Number of available quantum operations
        int n_w;    // Number of pairs of words to transform
        cin >> L >> n_op >> n_w;

        // op_costs[op] = cost of executing quantum op `op`
        unordered_map<quantum,int> op_costs;
        for (int i = 0; i < n_op; i++) {
            string s;
            int c;
            cin >> s >> c;
            quantum Rina = quantum(s);
            op_costs[Rina] = c;
        }

        // For each pair of words to transform
        for (int Mahiro = 0; Mahiro < n_w; Mahiro++) {
            // Read the pair of words
            string Kanon_0;   // Initial binary string
            string Kanon_1;   // Target  binary string
            cin >> Kanon_0 >> Kanon_1;

            int I_0 = stoi(Kanon_0,nullptr,2);   // Initial bs as an int
            int I_1 = stoi(Kanon_1,nullptr,2);   // Target  bs as an int

            // Create the dict of costs and pq of nodes to explore for Dij
            /*
            costs[i] = cost of getting to int `i` from `I_0`
            `i` in `costs` iff `i` has been explored
            */
            unordered_map<int,int> costs;
            // Frontier of nodes (Homu's) to explore
            priority_queue<Homu> front;



            // Initiate Dijkstra's
            front.push(Homu(I_0,0));
            // While the target int has not been reached 
            // and there are more nodes to explore
            while ((costs.count(I_1)==0) && (!front.empty())) {
                // Pop the next node
                Homu next = front.top();
                front.pop();
                int i_0 = next.i;
                int c_0 = next.c;

                // If already explored, then skip this node
                if (costs.count(i_0)!=0) {
                    continue;
                // Else, record the cost (also marks node as visited)
                } else {
                    costs[i_0] = c_0;
                }

                // Generate neighbors and toss them into the frontier
                for (const pair<quantum,int> p : op_costs) {
                    quantum op = p.first;
                    int op_cost = p.second;
                    int i_1 = operate(i_0,op);   // int after the op
                    int c_1 = c_0 + op_cost;     // cumulative cost after op

                    // If `i_1` has already been explored, skip
                    // (Lessen log factor of the frontier)
                    if (costs.count(i_1)!=0) {
                        continue;
                    // Else, toss into the frontier
                    } else {
                        front.push(Homu(i_1,c_1));
                    }
                }
            }



            // If possible, then print the distance
            if (costs.count(I_1)!=0) {
                cout << costs[I_1] << " ";
            // If not possible, then print 'NP '
            } else {
                cout << "NP ";
            }
        }

        // Print a newline to go to the next test case
        cout << endl;
    }

    return 0;
}





// RECYCLING BIN ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Helper function to convert a binary string to an integer
// int bs_int(string bs) {
//     return stoi(bs,nullptr,2);
// }
// Helper function to convert an integer to a bit set representing the 
// binary string (bitsets print out like binary strings)
// bitset int_bs(int n) {
//     // TODO
// }
// Bruh u need the <N> at compile time :skull:



    // bool operator > (const Homu& l, const Homu& r) {
    //     return l.c < r.c;
    // }
    // bool operator == (const Homu&l, const Homu& r) {
    //     return (l.i==r.i) && (l.c==r.c);
    // }
