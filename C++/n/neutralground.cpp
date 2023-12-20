// SOURCE
// https://github.com/stevenhalim/cpbook-code/blob/master/ch8/maxflow.cpp

// Disclaimer: This code is a hybrid between old CP1-2-3 implementation of
// Edmonds Karp's algorithm -- re-written in OOP fashion and the fast
// Dinic's algorithm implementation by
// https://github.com/jaehyunp/stanfordacm/blob/master/code/Dinic.cc
// This code is written in modern C++17 standard

// #include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <bitset>
#include <algorithm>
#include <tuple>

using namespace std;

#define True  true
#define False false
#define MAX_INT 2147483647   // Max val for int

typedef long long ll;
typedef tuple<int, ll, ll> edge;
typedef vector<int> vi;
typedef pair<int, int> ii;

const ll INF = 1e18;                             // large enough

class max_flow {
private:
    int V;
    vector<edge> EL;
    vector<vi> AL;
    vi d, last;
    vector<ii> p;

    bool BFS(int s, int t) {                       // find augmenting path
        d.assign(V, -1); d[s] = 0;
        queue<int> q({s});
        p.assign(V, {-1, -1});                       // record BFS sp tree
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (u == t) break;                         // stop as sink t reached
            for (auto &idx : AL[u]) {                  // explore neighbors of u
                auto &[v, cap, flow] = EL[idx];          // stored in EL[idx]
                if ((cap-flow > 0) && (d[v] == -1))      // positive residual edge
                    d[v] = d[u]+1, q.push(v), p[v] = {u, idx}; // 3 lines in one!
            }
        }
        return d[t] != -1;                           // has an augmenting path
    }

    ll send_one_flow(int s, int t, ll f = INF) {   // send one flow from s->t
        if (s == t) return f;                        // bottleneck edge f found
        auto &[u, idx] = p[t];
        auto &cap = get<1>(EL[idx]), &flow = get<2>(EL[idx]);
        ll pushed = send_one_flow(s, u, min(f, cap-flow));
        flow += pushed;
        auto &rflow = get<2>(EL[idx^1]);             // back edge
        rflow -= pushed;                             // back flow
        return pushed;
    }

    ll DFS(int u, int t, ll f = INF) {             // traverse from s->t
        if ((u == t) || (f == 0)) return f;
        for (int &i = last[u]; i < (int)AL[u].size(); ++i) { // from last edge
            auto &[v, cap, flow] = EL[AL[u][i]];
            if (d[v] != d[u]+1) continue;              // not part of layer graph
            if (ll pushed = DFS(v, t, min(f, cap-flow))) {
                flow += pushed;
                auto &rflow = get<2>(EL[AL[u][i]^1]);     // back edge
                rflow -= pushed;
                return pushed;
            }
        }
        return 0;
    }

public:
    max_flow(int initialV) : V(initialV) {
        EL.clear();
        AL.assign(V, vi());
    }

    // if you are adding a bidirectional edge u<->v with weight w into your
    // flow graph, set directed = false (default value is directed = true)
    void add_edge(int u, int v, ll w, bool directed = true) {
        if (u == v) return;                          // safeguard: no self loop
        EL.emplace_back(v, w, 0);                    // u->v, cap w, flow 0
        AL[u].push_back(EL.size()-1);                // remember this index
        EL.emplace_back(u, directed ? 0 : w, 0);     // back edge
        AL[v].push_back(EL.size()-1);                // remember this index
    }

    ll edmonds_karp(int s, int t) {
        ll mf = 0;                                   // mf stands for max_flow
        while (BFS(s, t)) {                          // an O(V*E^2) algorithm
            ll f = send_one_flow(s, t);                // find and send 1 flow f
            if (f == 0) break;                         // if f == 0, stop
            mf += f;                                   // if f > 0, add to mf
        }
        return mf;
    }

    ll dinic(int s, int t) {
        ll mf = 0;                                   // mf stands for max_flow
        while (BFS(s, t)) {                          // an O(V^2*E) algorithm
            last.assign(V, 0);                         // important speedup
            while (ll f = DFS(s, t))                   // exhaust blocking flow
                mf += f;
        }
        return mf;
    }
};

int example() {
    freopen("maxflow_in.txt", "r", stdin);

    int V, s, t; scanf("%d %d %d", &V, &s, &t);
    max_flow mf(V);
    for (int u = 0; u < V; ++u) {
        int k; scanf("%d", &k);
        while (k--) {
            int v, w; scanf("%d %d", &v, &w);

            // Adds directed edge (u,v) with weight w
            // Add False as the 4th option to add an undirected edge
            mf.add_edge(u, v, w);                      // default: directed edge
        }
    }

    // Calculates the max flow from source s to target t
    // Saves the state, so a rerun would give the increment only
    // printf("%lld\n", mf.edmonds_karp(s, t));
    printf("%lld\n", mf.dinic(s, t));

    return 0;
}

/* IDEA
I got spoiled that this was max flow but then again I should've guessed it 
given that the PS title was literally NP-Hard 2 + Flows 1

Add one supernode that connects to all armies of A
Then another supernode that connects to all armies of B
Find the max flow from supernode of A to supernode of B
Since max-flow = min-cut, this gives our desired ans

Make all edges going from A-supernode to armies of A directed
Make all edges going from armies of B to B-supernode directed
Just to be safe
Every other edge can be directed ig

Wait the cost is at the vertices not the edge, how do you actually model it 
as max flow?

Ok after playing around using the whiteboard in COM3-01-21 at night I have 
discovered that the following seems to work:

A-supernode, A-node, B-supernode, B-node all get assigned infinity
Every other vertex gets assigned their cost
Create the graph with edges as described above
Set the weight of every edge to be the minimum of its neighbors
Makes sense, the max flow that can be pushed through an edge = min between 
its neighbors since you can't overflood a neighbor I think?

Must ensure that one "infinite" edge is always bigger than the actual min-cut
Max size involves 1600 vertices
Pick every vertex -> 1600 * 9 = 14400
Set 20000 to be "infinity"

Wait wait above thing doesn't work I had a special case where my min cut was 
a vertical line

New idea:
Our problem is that we want to "flow" through our vertices, but max flow 
handles flow through edges instead...
What if we did some sort of dual transformation where edges become vertices 
and vice versa?

https://stackoverflow.com/questions/8751327/edmonds-karp-algorithm-for-a-graph-which-has-nodes-with-flow-capacities
BRYCE YOU FUCKING RETARD
FUCKING HELL
IT WAS SO FUCKING OBVIOUS
WHY
FUCKING FAILURE
I swear to god I'm actually gonna get a C for this mod
"Algo is my specialty" FAILS TO SOLVE SUCH A FUCKING BASIC PROBLEM
FUCKING KILL YOURSELF
Actually dumber than an average y1 cs student
*/

/* DEBUG LOG
1.) WA on Public TC 2 (outputs 8)
*/

#define inf (ll)999999   // "Infinite" edge in the graph

int main() {
    int R;   // Number of rows
    int C;   // Number of cols
    cin >> C >> R;

    // Throw away the newline at the end of the first line
    // char 暁美ほむら;
    // cin >> 暁美ほむら;

    // Matrix of grid spaces
    // -1 = A's army
    // -2 = B's army
    vector<vector<ll>> M(R,vector<ll>(C));
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            char L;
            cin >> L;

            if (L == 'A') {
                M[r][c] = (ll)(-1);
            } else if (L == 'B') {
                M[r][c] = (ll)(-2);
            } else {
                M[r][c] = (ll)(int(L)-int('0'));
            }

            // {DEBUG ---------------------------------------------------------
            // cout << L;
            // if (c == C-1) { cout << endl; }
            // DEBUG} ---------------------------------------------------------
        }
        // Throw away the newline
        // char 魔法少女まどかマギカ・ワルプルギスの回天・２０２４冬;
        // cin >> 魔法少女まどかマギカ・ワルプルギスの回天・２０２４冬;
    }

    // Create the graph
    /*
    A-supernode = vertex 2RC
    B-supernode = vertex 2RC+1
    in-vertex  at [r,c] = C*r + c
    out-vertex at [r,c] = RC + C*r + c
    */
    max_flow graph(2*R*C+2);

    // Create self edges
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            int v_in  = C*r + c;         // In-vertex  for [r,c]
            int v_out = R*C + C*r + c;   // Out-vertex for [r,c]
            ll w;                        // Weight from in to out-vertex
            // Infinity if an army, cost otherwise
            if (M[r][c] < 0) {
                w = inf;
            } else {
                w = M[r][c];
            }
            // Add the weighted edge to represent going through that vertex
            graph.add_edge(v_in,v_out,w);
        }
    }

    // Create horizontal edges
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C-1; c++) {
            int v0_in  = C*r + c;         // In-vertex  for [r,c]
            int v0_out = R*C + C*r + c;   // Out-vertex for [r,c]
            int v1_in  = v0_in  + 1;      // In-vertex  for [r,c+1]
            int v1_out = v0_out + 1;      // Out-vertex for [r,c+1]
            // Connect with an arbitrarily large capacity
            graph.add_edge(v0_out,v1_in,inf);
            graph.add_edge(v1_out,v0_in,inf);
        }
    }

    // Create vertical edges
    for (int r = 0; r < R-1; r++) {
        for (int c = 0; c < C; c++) {
            int v0_in  = C*r + c;         // In-vertex  for [r,c]
            int v0_out = R*C + C*r + c;   // Out-vertex for [r,c]
            int v1_in  = v0_in  + C;      // In-vertex  for [r+1,c]
            int v1_out = v0_out + C;      // Out-vertex for [r+1,c]
            // Connect with an arbitrarily large capacity
            graph.add_edge(v0_out,v1_in,inf);
            graph.add_edge(v1_out,v0_in,inf);
        }
    }

    // Create edges from supernode-A
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            // Do not connect if it is not an A-node
            if (M[r][c] != -1) {
                continue;
            }
            // Connect to the in-vertex of every A-node
            int v_in = C*r + c;   // In-vertex for [r,c]
            graph.add_edge(2*R*C,v_in,inf);
        }
    }

    // Create edges from supernode-B
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            // Do not connect if it is not a B-node
            if (M[r][c] != -2) {
                continue;
            }
            // Connect from the out-vertex of every B-node
            int v_out = R*C + C*r + c;   // Out-vertex for [r,c]
            graph.add_edge(v_out,2*R*C+1,inf);
        }
    }

    ll ans = graph.dinic(2*R*C,2*R*C+1);
    cout << ans << endl;

    // {DEBUG -----------------------------------------------------------------
    // Check input parsing
    // cout << "R = " << R << ", C = " << C << endl;
    // cout << "The grid M is " << endl;
    // for (int r = 0; r < R; r++) {
    //  for (int c = 0; c < C; c++) {
    //      int n = M[r][c];
    //      if (n >= 0) {
    //          cout << "  " << n;
    //      } else {
    //          cout << " "  << n;
    //      }
    //  }
    //  cout << endl;
    // }
    /*
    Eh why is the first column -1 -1 2 1 1 1???
    It should be A2AAAA = -1 2 -1 -1 -1 -1
    Ah maybe it's the newline after reading C and R?

    That char is now read
    Now the first column is 2 2 1 1 1 1?

    Tried printing out the characters being read and fsr characters are 
    just... completely wrong now

    ACTUAL:
    A211111321231111111111111
    2A2111110001111111BB11111
    AA211111000111111BBBB1111
    A2111114111411111BBBB1111
    AA2111110001111111BB11111
    AA21111110111111111111111

    READ:
    2111113212311111111111112
    2111110001111111BB11111AA
    11111000111111BBBB1111A21
    1114111411111BBBB1111AA21
    1110001111111BB11111AA211
    1110111111111111111111111

    Wait it doesn't get the newline char???

    Bruh my bug was actually from not being able to pasre the input :skull:
    */
    // DEBUG} -----------------------------------------------------------------
}
