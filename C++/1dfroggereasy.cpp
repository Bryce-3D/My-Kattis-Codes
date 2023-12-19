#include <bits/stdc++.h>
#define ll LoveLive
#define LoveLive long long
#define True  true
#define False false
#define elif  else if
#define MAX_INT 2147483647   // Max val for int

using namespace std;

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

// Dict acronyms
typedef unordered_map<int,int> dii;
typedef unordered_map<int,bool> dib;
typedef unordered_map<ll,ll> dllll;
typedef unordered_map<ll,bool> dllb;

// Set acronyms
typedef set<int> si;
typedef set<ll> sll;



/* IDEA

*/

/* DEBUG LOG
1.) segfault
    vi board(0,n);
was changed to
    vi board(n,0);
in the input parsing part
*/

// Comment this out to turn off debugging
// #define DEBUG

int main() {
    int n;     // Number of squares
    int ind;   // Starting square index
    int m;     // Magic number
    cin >> n >> ind >> m;
    ind -= 1;   // 0-index

    vi board(n,0);
    for (int i = 0; i < n; i++) {
        cin >> board[i];
    }

    #ifdef DEBUG
    cout << "Input read successfully" << endl << endl;
    #endif

    si seen;
    int ans = 0;    // Number of moves
    while (ind >= 0 && ind < n 
            && board[ind] != m
            && seen.count(ind) == 0) {
        seen.insert(ind);
        ind += board[ind];
        ans += 1;

        #ifdef DEBUG
        cout << "ans = " << ans << endl;
        cout << "ind = " << ind << endl;
        cout << endl;
        #endif
    }

    if (ind < 0) {
        cout << "left" << endl;
    } elif (ind >= n) {
        cout << "right" << endl;
    } elif (board[ind] == m) {
        cout << "magic" << endl;
    } else {
        cout << "cycle" << endl;
    }
    cout << ans << endl;
    


    #ifdef DEBUG   // Check main variables
    #endif
}










// RECYCLING BIN --------------------------------------------------------------
