#include <bits/stdc++.h>

// long long
#define ll LoveLive
#define LoveLive long long

// Python Style
#define True  true
#define False false
#define elif  else if

// Make using pairs bearable
#define p0 first
#define p1 second

// Useful "constants"
#define MAX_INT 2147483647   // Max val for int
#define FASTIO  ios_base::sync_with_stdio(False); cin.tie(0);

// For loop macro
#define FOR(x,a,b)   for (int x = (a); x < (b); x++)
#define FORll(x,a,b) for (ll  x = (a); x < (b); x++)

using namespace std;

// Pair acronyms
typedef pair<int,int> pii;
typedef pair<ll,ll> pllll;
typedef pair<int,bool> pib;
typedef pair<ll,bool> pllb;

// Vector acronyms
typedef vector<int> vi;
typedef vector<vector<int>> vvi;
typedef vector<vector<vector<int>>> vvvi;

typedef vector<bool> vb;
typedef vector<vector<bool>> vvb;
typedef vector<vector<vector<bool>>> vvvb;

typedef vector<ll> vll;
typedef vector<vector<ll>> vvll;
typedef vector<vector<vector<ll>>> vvvll;

typedef vector<pii> vpii;

// Dict acronyms
typedef unordered_map<int,int> dii;
typedef unordered_map<int,bool> dib;
typedef unordered_map<ll,ll> dllll;
typedef unordered_map<ll,bool> dllb;

// Set acronyms
typedef set<int> si;
typedef set<ll> sll;
typedef set<vi> svi;
typedef set<pii> spii;



/* IDEA
idea maybe flood fill and keep a list of sets
adversarial test case: checkerboard
O(qn^2) 
or rather
first of all as usual let (r,c) be denoted by C*r+c
make a list of length RC
call it comp
comp[i] = the component vertex i belongs to
start with comp number = 0
initialize comp to all -1s
ok then start with comp number 0
for i in range(RC)
if comp[i] == -1 aka not explored yet
flood fill with current comp number
incr comp number
per query:
just  check if they are the same comp number
if same comp number, refer back to the original grid to check if they were 
both 0 or both 1
O(1) per query
O(n^2) grid processing, each vertex gets checked once and is used in the 
flood fill exactly once too
question now
how to impl flood fill?
BFS?
how would that work
scuffed queue method?
perhaps scuffed queue method works
but what if loop
seen set again
if in seen then don't put baacck in
but when do you put into seen
wait
I don't need a seen
I am marking it in comp
if comp[v] = comp number then it has been seen in this current flood fill
otherwise it -1
can do this when pushing neighbors onto the scuffed queue I always use
ok impl time ig
*/

/* DEBUG LOG
ind += 1; put at the wrong place -> inf loop

grid read wrongly (it is not space separated) -> segfaults

TLE Test Case 25 -> add fast i/o, change endl to '/n', change some vvi to vpii
*/

// Comment this out to turn off debugging
// #define DEBUG

int main() {
    FASTIO

    int R,C;
    cin >> R >> C;

    // Read the grid
    vvi grid(R, vi(C,0));
    for (int r = 0; r < R; r++) {
        string s;
        cin >> s;
        for (int c = 0; c < C; c++) {
            grid[r][c] = s[c] - '0';
        }
    }

    // comp[r][c] = the component number (r,c) belongs to or -1 if not seen yet
    vvi comp(R, vi(C,-1));
    int comp_no = 0;   // Current component number

    #ifdef DEBUG
    cout << "First half of input reading done" << endl;
    #endif

    FOR(r,0,R) {
        FOR(c,0,C) {
            #ifdef DEBUG
            cout << "Cycle " << comp_no << endl;
            #endif

            // Skip if already flooded
            if (comp[r][c] != -1) {
                continue;
            }

            // FLood fill the component using BFS
            vpii BFS{pii{r,c}};    // BFS "queue"
            spii seen{pii{r,c}};   // Vertices seen in the BFS so far
            int ind = 0;
            int type = grid[r][c];

            while (ind < BFS.size()) {
                pii next = BFS[ind];
                int r0 = next.p0;
                int c0 = next.p1;
                // Mark this node
                comp[r0][c0] = comp_no;
                // Up
                if (r0 > 0) {
                    int r1 = r0-1;
                    int c1 = c0;
                    if (grid[r1][c1] == type && seen.count(pii{r1,c1}) == 0) {
                        BFS.push_back(pii{r1,c1});
                        seen.insert(pii{r1,c1});
                    }
                }
                // Down
                if (r0 < R-1) {
                    int r1 = r0+1;
                    int c1 = c0;
                    if (grid[r1][c1] == type && seen.count(pii{r1,c1}) == 0) {
                        BFS.push_back(pii{r1,c1});
                        seen.insert(pii{r1,c1});
                    }
                }
                // Left
                if (c0 > 0) {
                    int r1 = r0;
                    int c1 = c0-1;
                    if (grid[r1][c1] == type && seen.count(pii{r1,c1}) == 0) {
                        BFS.push_back(pii{r1,c1});
                        seen.insert(pii{r1,c1});
                    }
                }
                // Right
                if (c0 < C-1) {
                    int r1 = r0;
                    int c1 = c0+1;
                    if (grid[r1][c1] == type && seen.count(pii{r1,c1}) == 0) {
                        BFS.push_back(pii{r1,c1});
                        seen.insert(pii{r1,c1});
                    }
                }

                ind += 1;   // Next element of BFS
            }

            // Increment to next component
            comp_no += 1;
        }
    }

    #ifdef DEBUG
    cout << "Preprocessing done" << endl;
    cout << endl;
    // Grid check
    cout << "grid = " << endl;
    for (vi v : grid) {
        for (int i : v) {
            cout << i << " ";
        }
        cout << endl;
    }
    cout << endl;
    // Comp check
    cout << "comp = " << endl;
    for (vi v : comp) {
        for (int i : v) {
            cout << i << " ";
        }
        cout << endl;
    }
    cout << endl;
    #endif



    int Homu;
    cin >> Homu;
    for (int Kumi = 0; Kumi < Homu; Kumi++) {
        #ifdef DEBUG
        cout << "Kumi " << Kumi << endl;
        #endif

        int r0,c0,r1,c1;
        cin >> r0 >> c0 >> r1 >> c1;
        r0 -= 1;
        c0 -= 1;
        r1 -= 1;
        c1 -= 1;

        if (comp[r0][c0] != comp[r1][c1]) {
            cout << "neither";
        } elif (grid[r0][c0] == 0) {
            cout << "binary";
        } else {
            cout << "decimal";
        }
        cout << '\n';
    }



    #ifdef DEBUG   // Check main variables
    #endif
}










// RECYCLING BIN --------------------------------------------------------------
