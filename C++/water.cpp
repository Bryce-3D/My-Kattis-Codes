/* SOURCES
https://github.com/stevenhalim/cpbook-code/blob/master/ch8/maxflow.cpp
*/

/* IDEA
This is just.... max flow
Ok then

Either I decide to learn how to implement this or find library code online
I don't really remember the pushback algorithm thing too well anymore iirc 
it was cool but quite complicated.

Wait max flow uses directed edges hmm
https://stackoverflow.com/questions/29741691/maximum-flow-in-the-undirected-graph
Seems that it doesn't matter, nice
So it really is just max flow
Fair enough if you have flow going both ways just cancel them out so you'll 
always be going one way only.

Testing, Prof Halim's code seems to allow duplicated edges of repeated weights
Returns 17 when an edge of weight 1 is added to both (0,2) and (2,3)
Pogging

Wait Prof's code alr handles undirected case lmao yes Prof has documentation
Actually pogging
*/

/* DEBUG LOG
1.) WAing on test cases
Public TC 1
    1
    9
Public TC 2
    8
    1
    3
Apparently a run of the algo saves the state so it is printing out 
the increments.
*/

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

int main() {
    int V;   // Number of vertices
    int E;   // Number of edges
    int K;   // Number of upgrades
    cin >> V >> E >> K;

    ll ans;

    max_flow graph(V);
    for (int Homu = 0; Homu < E; Homu++) {
        int a,b;
        ll w;
        cin >> a >> b >> w;
        a -= 1;
        b -= 1;
        graph.add_edge(a,b,w,False);
    }

    ans = graph.dinic(0,1);
    cout << ans << endl;

    for (int Homu = 0; Homu < K; Homu++) {
        int a,b;
        ll w;
        cin >> a >> b >> w;
        a -= 1;
        b -= 1;
        graph.add_edge(a,b,w,False);
        ans += graph.dinic(0,1);
        cout << ans << endl;
    }
}

int example() {
  /*
  // Graph in Figure 8.11
  4 0 3
  2 1 8 2 8
  2 2 1 3 8
  1 3 8
  0
  // the max flow value of that graph should be 16
  */

    /*
    4 = number of vertices
    0 = source
    3 = target

    2 = number of edges going out of 0
        1,8 = (0,1) with cap 8
        2,8 = (0,2) with cap 8
    
    2 = number of edges going out of 1
        2,1 = (1,2) with cap 1
    
    ...

    Also this is 0 indexed (u=0 in loop)
    Based 0-indexing let's goooo
    */

  freopen("maxflow_in.txt", "r", stdin);

  int V, s, t; scanf("%d %d %d", &V, &s, &t);
  max_flow mf(V);
  for (int u = 0; u < V; ++u) {
    int k; scanf("%d", &k);
    while (k--) {
      int v, w; scanf("%d %d", &v, &w);

      // Adds the directed edge (u,v) with weight w
      mf.add_edge(u, v, w);                      // default: directed edge
    }
  }

    // Get the max flow from source s to target t
  // printf("%lld\n", mf.edmonds_karp(s, t));
  printf("%lld\n", mf.dinic(s, t));

  return 0;
}
