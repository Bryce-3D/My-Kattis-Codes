/* IDEA
Vertex is accessible iff it is at most S/2 meters away.
Since all distances are integers, this can be rounded down if S is odd.
Dijkstra's?
Wait no this is a tree the path is unique
Actually since this is a tree can't you just root it then turn on everything 
at an even depth/odd depth? (This must be what prof meant by bipartite is a 
special case also ig, trees are bipartite too now that I think of it)
Hmm but then you got some turned on already, so need to use the more general 
dp version since that can handle a sort of "stateful" initial state.
General idea is quite simple ig
ALso this is basically linear, shud pass the time limit comfortably



One *small* problem, how do I root a tree given an unsorted list of edges???
Probably build the edgelist first then work on that
Some sort of botched BFS???

queue = [0,_,_,_,_]
Toss in neighbors of 0, marking their parent as 0
Toss in neighbors of queue[1] except its parent, making their parent queue[1]
Toss in neighbors of queue[2] except its parent, making their parent queue[2]
...
Toss in neighbors of queue[n-1] except its parent,
    making their parent queue[n-1] (this will be an empty set but whatever)

This shud work I think, just need an extra variable to keep track of what 
index of the queue we're putting into now.
Can just initialize a blank vector with n spots and use it like an array and 
gradually fill it up.

Wait this "queue" can also be reused to dp, since I already did BFS here
Can do dp on the nodes from right to left and every subvalue will be 
calculated beforehand due to reverse BFS ordering



Do I need ll?
Yes we need to finish watching love live sunshine before genjitsu no yohane 
but not that, do we need a long long?
N <= 5 x 10^4
S <= 1 x 10^4
d <= 1 x 10^2
Max dist is Nd <= 5 x 10^6
No not really, int = 2^31 > 2 x 10^9 shud be good



Wait I forgot I need to calculate the distances first too hmmmm
Need to modify the "botched BFS" above a bit

queue = [0,_,_,_,_]
Toss in neighbors of 0, marking their parent as 0
    and their dist to root as dist[0] + dist from 0 to itself
Toss in neighbors of queue[1] except its parent, making their parent queue[1]
    and their dist to root as dist[queue[1]] + dist from queue[1] to itself
Toss in neighbors of queue[2] except its parent, making their parent queue[2]
    and their dist to root as dist[queue[2]] + dist from queue[2] to itself
...
Toss in neighbors of queue[n-1] except its parent,
    making their parent queue[n-1] (this will be an empty set but whatever)
    and their dist to root as dist[queue[n-1]] + dist from queue[n-1] to itself

Ok yes this *shud* work I believe yes? yes perhaps
How to access the edge weight during the process?
We'll be iterating through the adjacency list, just store the dist in the 
adjacency list too and it'll come together with the neighbor

Also let's rename `queue` to `BFS` cause yes



Wait we're framing this in terms of traversed edges hmmm
Ok so we can actually "reach" vertices whose distance is ceil(S/2)
I need to rigorize the definition of reachable vertex
Also how does the dp work for a vertex where you can't go any further (aka 
dist from root to vertex exactly equals S/2)
In that case you don't care about illuminating its children
In that case it's either cost=[0,0] if initially on and cost=[0,1] if 
initially off since its children don't matter just like me :MenheraAyyy:
Which means you kinda treat it like an unreachable vertex
On the other hand if the distance is strictly less than S/2 then you care 
about whether the children are illuminated.

We can now formally define a reachable vertex
A vertex `v` is `reachable` iff the distance of `v` from the root is 
STRICTLY less than S/2, i.e.
    dist[v] < S/2
or equivalently
    2*dist[v] < S
Because C++ is cursed and /2 is floor division rather than normal division
Another equivalent definition is a vertex v is reachable iff you can get onto 
the paths leading to its children.





As always, 0-index everything because 1-indexing is crinj
Also obviously let's root the tree at the school (node 0)

Data Structures
    adj[i]  = [`[j,w(i,j)]` for j in neighbors of i]
    on[i]   = True if node i is initially on and False otherwise
    par[i]  = parent of node i (set to -1 for the root)
    chi[i]  = list of children of node i
    dist[i] = dist of node i to the root
    BFS     = list of the nodes in BFS order

    If node i were initially off
    cost[i] = [min cost of subtree rooted at i if i is off,
               min cost of subtree rooted at i if i is on]
    If node i were initially on
    cost[i] = [min cost of subtree rooted at i,
               min cost of subtree rooted at i]
    *every entry of cost will be initialized to -1 to signify 
     uncalculated values



    Algo
Create `adj` and `on` by reading the inputs

Initialize `par`, `chi`, `dist`, `BFS`
BFS[0] = 0, dist[0] = 0
For i in range(n)
    For neighbor of BFS[i] that is not BFS[i]'s parent
        Set parent of neighbor as BFS[i]
        Append neighbor to children of BFS[i]
        Set its dist to dist[BFS[i]] + edge from BFS[i] to self
        Append to end of BFS

For v in BFS[::-1]
    If v is on and unreachable*
        Set both costs to 0
    Elif v is off and unreachable*
        Set off cost to 0 and on cost to 1
    Elif v is on and reachable*
        Set both costs to sum of min of costs of each child
    Elif v is off and reachable*
        cost[v][0] = sum of costs of each child when on
        cost[v][1] = 1 + sum of min of costs of each child

print(min(cost[0]))

*A vertex v is reachable iff 2*dist[v]<S
*/

/* DEBUG LOG
1.) Segfault happened
    The line
        vector<vector<vector<int>>> adj(N,vector<vector<int>>(2));
    was changed to just 
        vector<vector<vector<int>>> adj(N);
2.) Outputs 0 on public test case 1
    In the first public test case,
        cost[0] = [0,0]
        cost[1] = [0,0]
        cost[2] = [2,0]
        cost[3] = [0,1]
        cost[4] = [0,1]
    First error is cost[2][1] being 0 instead of 1
    I forgot to add 1 to `Kumi` in Case 4 (off and reachable)
*/

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <bitset>
#include <algorithm>
#define ll LoveLive
#define LoveLive long long
#define True  true
#define False false
using namespace std;

int main() {
    // Input Reading ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    int N;   // Number of intersections
    int S;   // Jogging distance
    cin >> N >> S;
    // adj[i] = [`[j,w(i,j)]` for j in neighbors of i]
    vector<vector<vector<int>>> adj(N);
    // on[i]   = True if node i is initially on and False otherwise
    vector<bool> on(N,False);
    for (int Homu = 0; Homu < N-1; Homu++) {
        int i,j,d;
        cin >> i >> j >> d;
        // Don't forget to 0-index
        i -= 1;
        j -= 1;
        adj[i].push_back(vector<int>{j,d});
        adj[j].push_back(vector<int>{i,d});
    }
    int SHIBUYA_KANON;
    cin >> SHIBUYA_KANON;
    for (int Kumi = 0; Kumi < SHIBUYA_KANON; Kumi++) {
        int i;
        cin >> i;
        // Don't forget to 0-index
        i -= 1;
        on[i] = True;
    }

    /* {DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Double check the collected data
    cout << "N = " << N << ", S = " << S << endl;
    for (int i = 0; i < N; i++) {
        cout << "The neighbors of " << i << " are:" << endl;
        for(vector<int> e : adj[i]) {
            cout << e[0] << " with weight " << e[1] << endl;
        }
    }
    cout << "The lamps that are turned on are:" << endl;
    for (int i = 0; i < N; i++) {
        if (on[i]) {
            cout << i << " ";
        }
    }
    cout << endl;
    DEBUG} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



    // BFS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // par[i] = parent of node i (set to -1 for the root)
    vector<int> par(N,-1);
    // chi[i] = list of children of node i
    vector<vector<int>> chi(N);
    // dist[i] = dist of node i to the root
    vector<int> dist(N,0);
    // BFS = list of the nodes in BFS order
    vector<int> BFS(N,0);
    // Perform the botched BFS
    // L = left pointer for next parent to explore through
    // R = right pointer for where to place the newborn children
    int R = 1;
    for (int L = 0; L < N; L++) {
        int v0 = BFS[L];   // Parent to look at next
        for (vector<int> e : adj[v0]) {
            int v1 = e[0];
            int d = e[1];
            // Do not re-explore v0's parent
            if (par[v0] == v1) {
                continue;
            }

            par[v1] = v0;            // Record v1's parent
            chi[v0].push_back(v1);   // Add to v0's children
            dist[v1] = dist[v0]+d;   // Calculate and store v1's dist
            BFS[R] = v1;             // Toss v1 into the BFS order
            R += 1;                  // Increment the right pointer
        }
    }



    // DP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    /*
    If node i were initially off
        cost[i] = [min cost of subtree rooted at i if i is off,
                   min cost of subtree rooted at i if i is on]
    If node i were initially on
        cost[i] = [min cost of subtree rooted at i,
                   min cost of subtree rooted at i]
    *every entry of cost will be initialized to -1 to signify 
     uncalculated values
    */
    vector<vector<int>> cost(N,vector<int>(2,-1));
    // Perform the dp
    for (int i = N-1; i > -1; i--) {
        int v = BFS[i];   // Next node to check
        
        // If v is on and unreachable
        if (on[v] && (2*dist[v] >= S)) {
            cost[v][0] = 0;
            cost[v][1] = 0;
        
        // Elif v is off and unreachable
        } else if (!on[v] && (2*dist[v] >= S)) {
            cost[v][0] = 0;
            cost[v][1] = 1;

        // Elif v is on and reachable
        } else if (on[v] && (2*dist[v] < S)) {
            // Set both costs to sum of min of costs of each child
            int Homu = 0;
            for (int c : chi[v]) {
                Homu += min(cost[c][0],cost[c][1]);
            }
            cost[v][0] = Homu;
            cost[v][1] = Homu;
        
        // Elif v is off and reachable
        } else if (!on[v] && (2*dist[v] < S)) {
            // cost[v][0] = sum of costs of each child when on
            int Homu = 0;
            for (int c : chi[v]) {
                Homu += cost[c][1];
            }
            cost[v][0] = Homu;
            // cost[v][1] = 1 + sum of min of costs of each child
            int Kumi = 1;
            for (int c : chi[v]) {
                Kumi += min(cost[c][0],cost[c][1]);
            }
            cost[v][1] = Kumi;
        
        // Bug???
        } else {
            int Aquas_cover_of_one_room_sugar_life_is_playing_on_loop = 1/0;
        }
    }

    // Print the ans
    cout << min(cost[0][0],cost[0][1]) << endl;

    // {DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // // Double check other values

    // // `par` looks good
    // for (int i = 0; i < N; i++) {
    //     cout << "The parent of " << i << " is " << par[i] << endl;
    // }
    // cout << endl;

    // // `chi` looks good
    // for (int i = 0; i < N; i++) {
    //     cout << "The children of " << i << " are ";
    //     for (int j : chi[i]) {
    //         cout << j << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    // // `dist` looks good
    // for (int i = 0; i < N; i++) {
    //     cout << "Dist from root to " << i << " is " << dist[i] << endl;
    // }
    // cout << endl;

    // // `BFS` looks good
    // cout << "The BFS traversal goes in the order ";
    // for (int i : BFS) {
    //     cout << i << " ";
    // }
    // cout << endl << endl;

    // // The error is probably in the DP then
    // // `cost`
    // for (int i = 0; i < N; i++) {
    //     cout << "cost[" << i << "] = ";
    //     cout << "[" << cost[i][0] << "," << cost[i][1] << "]" << endl;
    // }
    // cout << endl;
    // DEBUG} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



    return 0;
}
