/* IDEA
tl;dr delete edges to make the graph acyclic (aka a forest)
Essentially for each connected component, delete edges until it is a tree
This means that you essentially have to delete anything that doesn't 
disconnect components, and the nmber of edges to delete this is fixed.

This seems quite hard to actually do tho hmmm...

Perhaps I can iterate through the edges and just delete anything that forms 
a cycle during the process.
Wait yes isn't that all I have to do?
Idg why is this NP-Hard?

2023-08-31
I'm dumb, the edges are directed
Heard the idea is very dumb

BIG BRAIN INTUITION
Was looking at complete graph case
"Hmmm each pair got cycle, need to delete one of the pair"
"Then that's alr half, the remaining must be kept"
"Then it must become kinda like a poset"
"Wait this can be generalized WAIT YOOO"

Main idea:
Count number of edges from low to high
Count number of edges from high to low
Delete the set with a larger count
LMAO

*/

/* DEBUG LOG

*/

#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <bitset>
#include <algorithm>
#define ll LoveLive
#define LoveLive long long
#define True  true
#define False false
#define MAX_INT 2147483647   // Max val for int
using namespace std;

int main() {
    int V;   // Number of rooms/nodes
    int E;   // Number of corridors/edges
    cin >> V >> E;

    vector<int> Homu;   // Indices of edges that go from low to high
    vector<int> Mado;   // Indices of edges that go from high to low

    for (int Kumi = 1; Kumi <= E; Kumi++) {
        int v0,v1;
        cin >> v0 >> v1;
        if (v0 < v1) {
            Homu.push_back(Kumi);
        } else {
            Mado.push_back(Kumi);
        }
    }

    if (Homu.size() <= Mado.size()) {
        cout << Homu.size() << endl;
        for (int i : Homu) {
            cout << i << endl;
        }
    } else {
        cout << Mado.size() << endl;
        for (int i : Mado) {
            cout << i << endl;
        }
    }
}
