/* IDEA
2023-09-28
https://en.wikipedia.org/wiki/Assignment_problem#Unbalanced_assignment
Lmao

Ok I heard this is apparently MCMF
https://github.com/stevenhalim/cpbook-code/blob/master/ch9/mcmf.cpp

1.) Run MCBM to find the max number of matchings pairings
2.) Run MCMF to find the max suitability that uses the max number of matches
    - Flow = max number of matches
    - Cost = negative of suitability (or max suitability - suitability)
    - Problems = Left Set
    - Topics = Right set
    - Have a super source and super target node. 0 cost for all new edges
    - 

Left nodes      = vertices 0 to L-1
Right nodes     = vertices L to L+R-1
Left supernode  = vertex L+R
Right supernode = vertex L+R+1
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
#include <tuple>
#include <set>
// #define ll LoveLive
// #define LoveLive long long
#define True  true
#define False false
#define MAX_INT 2147483647   // Max val for int
using namespace std;

typedef long long ll;
typedef tuple<int, ll, ll, ll> edge;
typedef vector<int> vi;
typedef vector<ll> vll;

int L;   // Number of problems
int R;   // Number of Topics
// adj[i] = List of neighbors of vertex i
vector<vector<int>> adj;




// https://github.com/stevenhalim/cpbook-code/blob/master/ch9/mcmf.cpp
typedef long long ll;
typedef tuple<int, ll, ll, ll> edge;
typedef vector<int> vi;
typedef vector<ll> vll;
const ll INF = 1e18; // INF = 1e18, not 2^63-1 to avoid overflow
class min_cost_max_flow {
private:
    int V;
    ll total_cost;
    vector<edge> EL;
    vector<vi> AL;
    vll d;
    vi last, vis;

    bool SPFA(int s, int t) { // SPFA to find augmenting path in residual graph
        d.assign(V, INF); d[s] = 0; vis[s] = 1;
        queue<int> q({s});
        while (!q.empty()) {
            int u = q.front(); q.pop(); vis[u] = 0;
            for (auto &idx : AL[u]) {                  // explore neighbors of 
                auto &[v, cap, flow, cost] = EL[idx];          // stored in EL[
                if ((cap-flow > 0) && (d[v] > d[u] + cost)) {      // positive 
                    d[v] = d[u]+cost;
                    if(!vis[v]) q.push(v), vis[v] = 1;
                }
            }
        }
        return d[t] != INF;                           // has an augmenting path
    }

    ll DFS(int u, int t, ll f = INF) {             // traverse from s->t
        if ((u == t) || (f == 0)) return f;
        vis[u] = 1;
        for (int &i = last[u]; i < (int)AL[u].size(); ++i) { // from last edge
            auto &[v, cap, flow, cost] = EL[AL[u][i]];
            if (!vis[v] && d[v] == d[u]+cost) {                      // in curr
                if (ll pushed = DFS(v, t, min(f, cap-flow))) {
                    total_cost += pushed * cost;
                    flow += pushed;
                    auto &[rv, rcap, rflow, rcost] = EL[AL[u][i]^1]; // back ed
                    rflow -= pushed;
                    vis[u] = 0;
                    return pushed;
                }
            }
        }
        vis[u] = 0;
        return 0;
    }

public:
    min_cost_max_flow(int initialV) : V(initialV), total_cost(0) {
        EL.clear();
        AL.assign(V, vi());
        vis.assign(V, 0);
    }

    // if you are adding a bidirectional edge u<->v with weight w into your
    // flow graph, set directed = false (default value is directed = true)
    void add_edge(int u, int v, ll w, ll c, bool directed = true) {
        if (u == v) return;                          // safeguard: no self loop
        EL.emplace_back(v, w, 0, c);                 // u->v, cap w, flow 0, co
        AL[u].push_back(EL.size()-1);                // remember this index
        EL.emplace_back(u, 0, 0, -c);                // back edge
        AL[v].push_back(EL.size()-1);                // remember this index
        if (!directed) add_edge(v, u, w, c);         // add again in reverse
    }

    pair<ll, ll> mcmf(int s, int t) {
        ll mf = 0;                                   // mf stands for max_flow
        while (SPFA(s, t)) {                          // an O(V^2*E) algorithm
            last.assign(V, 0);                         // important speedup
            while (ll f = DFS(s, t))                   // exhaust blocking flow
                mf += f;
        }
        return {mf, total_cost};
    }
};


int main() {
    int L;   // Number of nodes on the left    (Problems)
    int R;   // Number of nodes on the right   (Topics)
    int E;   // Number of edges
    cin >> L >> R >> E;

    min_cost_max_flow mf(L+R+2);

    // Create edges from left to right
    for (int Homu = 0; Homu < E; Homu++) {
        int l,r,s;
        cin >> l >> r >> s;
        l -= 1;
        r -= 1;
        mf.add_edge(l,L+r,1,-s);
    }
    // Create edges out of left supernode
    for (int i = 0; i < L; i++) {
        mf.add_edge(L+R,i,1,0);
    }
    // Create edges into the right supernode
    for (int i = L; i < L+R; i++) {
        mf.add_edge(i,L+R+1,1,0);
    }
    
    pair<ll,ll> ans = mf.mcmf(L+R,L+R+1);
    int ans0 = L - ans.first;
    int ans1 = -ans.second;
    cout << ans0 << " " << ans1 << endl;
}










// RECYCLING BIN ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
Helper function for MCBM() from 
https://github.com/stevenhalim/cpbook-code/blob/master/ch4/mcbm.cpp
*/
int Aug(int L, vector<vector<int>>& adj, 
        vector<int>& vis, vector<int>& match) {
    if (vis[L]) return 0;       // L visited, return 0
        vis[L] = 1;
        for (auto &R: adj[L])   // for (auto &R : AL[L])
            if ((match[R] == -1) || Aug(match[R], adj, vis, match)) {
                match[R] = L;   // flip status
                return 1;       // found 1 matching
            }
    return 0;                   // no matching
}

/* Helper function
Given a bipartite graph with `L` left vertices, `R` right vertices, and 
adjacency list `adj` (such that any left vertex is indexed lower than a 
right vertex and all edges go from left to right), returns the MCBM
*/
int MCBM(int L, int R, vector<vector<int>> adj) {
    int ans = 0;

    int V = L+R;   // Number of vertices
    int Vleft = L;   // Number of left vertices
    // vis[i] = 1 if visited, 0 if not yet visited for left vertices
    vector<int> vis;
    // match[i] = partner of i or -1 if no partner
    vector<int> match(V,-1);

    vector<vector<int>>& adj_r = adj;
    vector<int>& vis_r = vis;
    vector<int>& match_r = match;

    for (int i = 0; i < Vleft; i++) {
        vis.assign(Vleft,0);
        ans += Aug(i, adj_r, vis_r, match_r);
    }

    return ans;
}
