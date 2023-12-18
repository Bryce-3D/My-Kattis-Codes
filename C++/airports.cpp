/* IDEA
2023-09-28
Ok I finally understand this problem at least

Current idea:
Represent each route as a vertex
Draw a directed edge from u to v if a plane can service route u then 
immediately service route v after.
Essentially the idea is to decompose this DAG (obviously cannot time travel 
so this is acyclic) into a bunch of chains such that the number of chains 
is minimal.

2023-10-02
Apparently this is well known as MIN-PATH-COVER
8.6.8 Min-Path-Cover in CP4
Take original DAG
For each vertex v, create v_in and v_out
Left vertices = all out vertices
Right vertices = all in vertices
Edge from u_out to v_in if can do flight v after flight u

*/

/* DEBUG LOG
1.) Prints out 2 on TC2
BUG: Double counted inspection time when s0 == e1 when computing `dur`
Inspection only happens once in this case

2.) Went from printing 3 on TC3 to printing 2???
Changed
            if (e0 != s1) {
                dur = T[e0][s1] + insp[s1];
            }
to
            if (e0 != s1) {
                dur += T[e0][s1] + insp[s1];
            }

3.) WA on TC6
Didn't take into account that the shortest flight might not be the most direct
Renamed T to T_fake and added a new T

4.) WA on TC4
Changed
            // Duration needed from leaving s0 to leaving s1
            int dur = T[s0][e0];
to
            // Duration needed from leaving s0 to leaving s1
            int dur = T_fake[s0][e0] + insp[e0];

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
#define ll LoveLive
#define LoveLive long long
#define True  true
#define False false
#define MAX_INT 2147483647   // Max val for int
using namespace std;

// Help debug by printing matrices
void print_mat(vector<vector<int>> M) {
    for (vector<int> v : M) {
        for (int i : v) {
            cout << i << " ";
        }
        cout << endl;
    }
}

/*
Helper function from 
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

/*
Helper function
Given a bipartite graph with `L` left vertices, `R` right vertices, and 
adjacency list `adj`, returns the MCBM
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

int main() {
    // {INPUT READING ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    int N;   // Number of airports
    int V;   // Number of flights (vertices on each side of bipartite graph)
    cin >> N >> V;

    // insp[i] = inspection time needed at airport `i`
    vector<int> insp(N,0);
    for (int i = 0; i < N; i++) {
        cin >> insp[i];
    }

    // T[i][j] = time to travel from airport `i` to airport `j`
    vector<vector<int>> T_fake(N,vector<int>(N,0));
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            cin >> T_fake[r][c];
        }
    }

    // Flight[i] = [s,e,t]
    //     = [starting airport, ending airport, departure time]
    vector<vector<int>> flight(V,vector<int>(3,0));
    for (int i = 0; i < V; i++) {
        for (int Homu = 0; Homu < 3; Homu++) {
            cin >> flight[i][Homu];
            flight[i][Homu] -= 1;   // 0-index this
        }
    }
    // INPUT READING} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



    // {DIJKSTRA'S ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Change of plans this is Floyd Warshall's now
    // https://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm
    /*
    T[i][j] = minimum time to travel from airport `i` to airport `j` 
    taking into account non-direct flights and inspections upon arrival
    */
    vector<vector<int>> T(N,vector<int>(N,0));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            T[i][j] = T_fake[i][j] + insp[j];
        }
    }
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (T[i][j] > T[i][k]+T[k][j]) {
                    T[i][j] = T[i][k]+T[k][j];
                }
            }
        }
    }
    // DIJKSTRA'S} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



    // {MIN-PATH-COVER MCBM SETUP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    /*
    Initialize the bipartite graph to run MCBM on
        in-vertex  i = i
        out-vertex i = V+i
    */
    vector<vector<int>> adj(2*V,vector<int>(0));
    for (int l = 0; l < V; l++) {
        for (int r = 0; r < V; r++) {
            int s0 = flight[l][0];
            int e0 = flight[l][1];
            int dep0 = flight[l][2];
            int s1 = flight[r][0];
            int e1 = flight[r][1];
            int dep1 = flight[r][2];

            // Duration needed from leaving s0 to leaving s1
            int dur = T_fake[s0][e0] + insp[e0];
            if (e0 != s1) {
                dur += T[e0][s1];
            }
            int TL = dep1-dep0;   // Time limit

            // Add the edge if it can be made on time
            if (dur <= TL) {
                adj[l].push_back(V+r);
            }
        }
    }
    // MIN-PATH-COVER MCBM SETUP} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



    // Print the ans
    int ans = V;
    ans -= MCBM(V,V,adj);   // -1 flight per matching
    cout << ans << endl;
}










// RECYCLING BIN ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

            // {DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // cout << dur << " " << TL << endl;
            // DEBUG} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

                // {DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                // cout << "Flight " << l << " can be followed by the flight " 
                //      << r << endl;
                // cout << "dur = " << dur << " and TL = " << TL << endl;
                // DEBUG} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // {DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // for (int i = 0; i < V; i++) {
    //     cout << "Vertex " << i << ":" << endl;
    //     for (int j : adj[i]) {
    //         cout << j - V << " ";
    //     }
    //     cout << endl;
    // }
    // DEBUG} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
