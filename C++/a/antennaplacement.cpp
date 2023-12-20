// Sources
// https://github.com/stevenhalim/cpbook-code/blob/master/ch4/mcbm.cpp

/* IDEA
2023-09-27
Essentially want to pair up as many *'s as possible
The graph is a grid graph -> It is bipartite
Use alg from the lec (need to go look again cause I forgot)

n <= 20 test cases
h <= 50
w <= 10
At most 250 per side

R by C grid
(r,c) -> C*r + c
WLOG let vertex (0,0) be on the right side
Right side will be the majority with ceil(RC/2)
Then (r,c) is left iff r+c is odd

Prof Halim's code stores all vertices together with left vertices in front 
and right vertices at the back.
Need a way to kinda convert this ig?

Use the standard (r,c) -> Cr + c conversion
Rank them using this (lower = first so (0,0) 1st, (0,1) 2nd, ...)

Converting (r,c) to index in Prof Halim's library code
    L vertex (r,c) aka r+c is odd
        (r,c) -> floor(Cr/2)      // Rows above
            + floor(c/2) + 1   // Row itself
            - 1                // 0-indexing
    R vertex (r,c) aka r+c is even
        (r,c) -> floor(RC/2)      // Left side goes first
            + ceil(Cr/2)       // Rows abvoe
            + floor(c/2) + 1   // Row itself
            - 1                // 0-indexing
Note: ceil(n/2) = (n+1)//2
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
#define ll LoveLive
#define LoveLive long long
#define True  true
#define False false
#define MAX_INT 2147483647   // Max val for int
using namespace std;

int R;   // Number of rows
int C;   // Number of cols
// M[r][c] = True if (r,c) needs to be covered, False otherwise
vector<vector<bool>> M;
// adj[i] = neighbors of `i`, where vertices are indexed using 
//          the convention specified by conv()
vector<vector<int>> adj;

// Extra variables to run Aug()
// vis[i] = 1 if visited, 0 if not yet visited for left vertices
vector<int> vis;
// match[i] = partner of i or -1 if no partner
vector<int> match;


/*
Helper function from 
https://github.com/stevenhalim/cpbook-code/blob/master/ch4/mcbm.cpp
*/
int Aug(int L) {
    if (vis[L]) return 0;       // L visited, return 0
        vis[L] = 1;
        for (auto &R: adj[L])   // for (auto &R : AL[L])
            if ((match[R] == -1) || Aug(match[R])) {
                match[R] = L;   // flip status
                return 1;       // found 1 matching
            }
    return 0;                   // no matching
}

/*
Helper function to convert the coordinate (r,c) in an R by C grid to 
its index such that all vertices with r+c odd (left) come before vertices 
with r+c even (right), and each half is ordered by row-major order
*/
int conv(int r, int c, int R, int C) {
    int ans = 0;

    if ((r+c)%2 == 1) {   // If Left vertex
        ans += (C*r)/2;
        ans += c/2;
    
    } else {   // If Right vertex
        ans += (R*C)/2;
        ans += (C*r+1)/2;
        ans += c/2;
    }

    return ans;
}

int main() {
    int Homura;
    cin >> Homura;
    for (int Kumiko = 0; Kumiko < Homura; Kumiko++) {
        // Read in number of rows and cols
        cin >> R >> C;
        // Read in the grid
        M = vector<vector<bool>>(R,vector<bool>(C,False));
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                char Mahiro;
                cin >> Mahiro;
                if (Mahiro == '*') {
                    M[r][c] = true;
                }
            }
        }
        // Create the adjacency list
        adj = vector<vector<int>>(R*C,vector<int>(0));
        // Handle horizontal edges
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C-1; c++) {
                // If both (r,c) and (r,c+1) are needed, add an edge
                if (M[r][c] && M[r][c+1]) {
                    if ((r+c)%2 == 1) {   // If (r,c) is left
                        adj[conv(r,c,R,C)].push_back(conv(r,c+1,R,C));
                    } else {              // If (r,c) is right
                        adj[conv(r,c+1,R,C)].push_back(conv(r,c,R,C));
                    }
                }
            }
        }
        // Handle vertical edges
        for (int r = 0; r < R-1; r++) {
            for (int c = 0; c < C; c++) {
                // If both (r,c) and (r+1,c) are needed, add an edge
                if (M[r][c] && M[r+1][c]) {
                    if ((r+c)%2 == 1) {   // If (r,c) is left
                        adj[conv(r,c,R,C)].push_back(conv(r+1,c,R,C));
                    } else {              // If (r,c) is right
                        adj[conv(r+1,c,R,C)].push_back(conv(r,c,R,C));
                    }
                }
            }
        }

        // Augmenting Path Algorithm
        // https://github.com/stevenhalim/cpbook-code/blob/master/ch4/mcbm.cpp
        int V = R*C;
        int Vleft = (R*C)/2;
        unordered_set<int> freeV;
        for (int i = 0; i < Vleft; ++i) {
            freeV.insert(i);
        }
        match.assign(V,-1);
        int MCBM = 0;
        for (auto &f : freeV) {
            vis.assign(Vleft,0);
            MCBM += Aug(f);
        }

        // Print the ans
        int ans = 0;
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                if (M[r][c]) {
                    ans += 1;
                }
            }
        }
        ans -= MCBM;
        cout << ans << endl;
    }
}
