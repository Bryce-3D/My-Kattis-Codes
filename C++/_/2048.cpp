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

*/

/* DEBUG LOG

*/

// Comment this out to turn off debugging
// #define DEBUG

void rot_ccw(vvi& board) {
    // Copy the board
    vvi temp(4, vi(4));
    FOR(r,0,4) {
        FOR(c,0,4) {
            temp[r][c] = board[r][c];
        }
    }
    // Rotate the board
    FOR(r,0,4) {
        FOR(c,0,4) {
            board[r][c] = temp[c][3-r];
        }
    }
}

void left(vvi& board) {
    for (vi& v : board) {
        // Remove 0s
        vi v0(0);
        FOR(i,0,4) {
            if (v[i] != 0) {
                v0.push_back(v[i]);
            }
        }
        // Combine numbers
        vi v1(0);
        FOR(i,0,v0.size()) {
            if (i == v0.size()-1) {
                v1.push_back(v0[i]);
            } elif (v0[i] == v0[i+1]) {
                v1.push_back(2*v0[i]);
                i += 1;
            } else {
                v1.push_back(v0[i]);
            }
        }
        // Copy back onto the board
        FOR(i,0,4) {
            if (i < v1.size()) {
                v[i] = v1[i];
            } else {
                v[i] = 0;
            }
        }
    }
}

void print(vvi& board) {
    for (vi v : board) {
        for (int i : v) {
            cout << i << " ";
        }
        cout << endl;
    }
}

int main() {
    vvi board(4, vi(4));
    FOR(r,0,4) {
        FOR(c,0,4) {
            cin >> board[r][c];
        }
    }
    int rot;
    cin >> rot;

    FOR(Homu,0,rot) {
        rot_ccw(board);
    }
    left(board);
    FOR(Homu,0,4-rot) {
        rot_ccw(board);
    }
    print(board);



    #ifdef DEBUG   // Check main variables
    #endif
}










// RECYCLING BIN --------------------------------------------------------------
