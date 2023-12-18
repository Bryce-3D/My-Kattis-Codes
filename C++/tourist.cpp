/* IDEA
2023-09-28
Probably flow? Matchings doesn't make sense for this problem at all.
I guess the challenge is to figure out how to model it as such.
Flow has to be modeled as passing through an interesting location somehow.
Then again you can only pick one path where flow kinda pushes through 
multiple paths, which is kinda off...

2023-??-??
Idea is needs mcmf

2023-10-02
Hint I got:
```
you can store much less
    dp[diagonal][cell 1 on diagonal][cell 2 on diagonal]
updating is just moving down a diagonal
```

2023-10-03
🔶 Homura Akemi 🔶 — Today at 1:41 PM
Yo can I have a hint for C?
ik it's supposed to be mcmf
And that the "min cost" part is probably how to model the maximum number of 
*'s you pass through
But can't seem to figure out how to setup the flow
Chicken Tendies — Today at 1:42 PM
oh yea the flow isnt too intuitive at first glance
so you need to go from start the end
then back to start right
🔶 Homura Akemi 🔶 — Today at 1:43 PM
yeh
Chicken Tendies — Today at 1:43 PM
instead of finding a path from start to end then from end back to start, you 
can just find 2 paths from start to end
🔶 Homura Akemi 🔶 — Today at 1:43 PM
wait
oH
yOOO
ok
Chicken Tendies — Today at 1:43 PM
i.e. you want to find a flow of 2 from start to end and the min cost of this
🔶 Homura Akemi 🔶 — Today at 1:43 PM
ahhhhhh
hmm
So
you have a bottleneck pipe at the start/end to limit it to 2 pushed flows
Then for * stuff you have a capacity of 1 with cost 1
and cap of 1 with cost 2
But for .
You just have cap 2 with cost 2
So it'll be incentivized to go through *
Chicken Tendies — Today at 1:45 PM
yea you got it
LOL
🔶 Homura Akemi 🔶 — Today at 1:45 PM
But then it can only use it once
nice nice ahhh
Oke thanks for the nudge
Chicken Tendies — Today at 1:45 PM
dont mention it
🔶 Homura Akemi 🔶 — Today at 1:45 PM
I will implement after my LAJ tut then hopefully can get 暁美ほむら to stay in 
the leaderboard
Chicken Tendies — Today at 1:46 PM
i've seen this idea before... if i didnt, i prob wont be able to figure it out 
myself either LMAO
🔶 Homura Akemi 🔶 — Today at 1:46 PM
Me with my 0 competitive programming experience:
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
#define elif  else if
#define MAX_INT 2147483647   // Max val for int
using namespace std;



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
    int ほむら;
    cin >> ほむら;
    for (int くみこ = 0; くみこ < ほむら; くみこ++) {
        // INPUT READING ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        int R,C;   // Number of rows and cols
        cin >> C >> R;
        // 0 = ., 1 = *, 2 = #
        vector<vector<int>> map(R,vector<int>(C,0));
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                char chr;
                cin >> chr;
                if (chr == '.') {
                    map[r][c] = 0;
                } elif (chr == '*') {
                    map[r][c] = 1;
                } else {
                    map[r][c] = 2;
                }
            }
        }



        // SETTING UP MCMF ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        /* vertex at [r,c] = vertex C*r + c
        vertices 0 to RC-1   = in-vertices
        vertices RC to 2RC-1 = out-vertices
        vertex 2RC           = Source vertex
        vertex 2RC+1         = Target vertex
        */
        min_cost_max_flow mf(2*R*C+2);

        // mf.add_edge(u,v, capacity, cost)

        // in-vertex to corresponding out-vertex
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                int v_in  = C*r+c;
                int v_out = v_in + R*C;
                // If normal spot
                if (map[r][c] == 0) {
                    mf.add_edge(v_in,v_out,2,0);
                // If interesting spot
                } elif (map[r][c] == 1) {
                    mf.add_edge(v_in,v_out,1,-1);   // Incentive
                    mf.add_edge(v_in,v_out,1,0);
                }
                // If blocked, do nothing
            }
        }

        // Horizontal edges
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C-1; c++) {
                // Connect u to v along u_out to v_in
                int u_out = R*C + C*r + c;
                int v_in  = C*r + (c+1);
                mf.add_edge(u_out,v_in,2,0);
            }
        }

        // Vertical edges
        for (int r = 0; r < R-1; r++) {
            for (int c = 0; c < C; c++) {
                // Connect u to v along u_out to v_in
                int u_out = R*C + C*r + c;
                int v_in  = C*(r+1) + c;
                mf.add_edge(u_out,v_in,2,0);
            }
        }

        // Source and Target edges
        mf.add_edge(2*R*C,0,2,0);
        mf.add_edge(2*R*C-1,2*R*C+1,2,0);



        // RUNNING MCMF AND PRINTING THE ANS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // ans = [max flow, min cost]
        pair<ll,ll> ans0 = mf.mcmf(2*R*C,2*R*C+1);
        ll ans = -ans0.second;
        cout << ans << endl;
    }
}
