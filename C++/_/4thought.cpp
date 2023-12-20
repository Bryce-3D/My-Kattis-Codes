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
There are only 64 possible combinations of operations, just 
bash them all and precompute
*/

/* DEBUG LOG

*/

// Comment this out to turn off debugging
// #define DEBUG

/*
Let 
    * = 0
    / = 1
    + = 2
    - = 3
*/
dii precomp() {
    dii ans;
    for (int op = 0; op < 64; op++) {
        int op0 = (op>>4)%4;
        int op1 = (op>>2)%4;
        int op2 = op%4;

        vi stack(1,4);

        if (op0 == 0) {
            stack[stack.size()-1] *= 4;
        } elif (op0 == 1) {
            stack[stack.size()-1] /= 4;
        } else {
            stack.push_back(op0);
            stack.push_back(4);
        }

        if (op1 == 0) {
            stack[stack.size()-1] *= 4;
        } elif (op1 == 1) {
            stack[stack.size()-1] /= 4;
        } else {
            stack.push_back(op1);
            stack.push_back(4);
        }

        if (op2 == 0) {
            stack[stack.size()-1] *= 4;
        } elif (op2 == 1) {
            stack[stack.size()-1] /= 4;
        } else {
            stack.push_back(op2);
            stack.push_back(4);
        }

        int result = stack[0];
        FOR(i,0,stack.size()/2) {
            if (stack[2*i+1] == 2) {
                result += stack[2*i+2];
            } else {
                result -= stack[2*i+2];
            }
        }

        ans[result] = op;
    }
    return ans;
}

int main() {
    dii possible = precomp();

    unordered_map<int,char> int_to_op{
        {0,'*'}, {1,'/'}, {2,'+'}, {3,'-'}
    };

    int Homu;
    cin >> Homu;
    FOR(Kumi,0,Homu) {
        int n;
        cin >> n;
        if (possible.count(n) == 0) {
            cout << "no solution" << endl;
            continue;
        }
        int op = possible[n];
        char op0 = int_to_op[(op>>4)%4];
        char op1 = int_to_op[(op>>2)%4];
        char op2 = int_to_op[op%4];

        cout << "4 " << op0 << " 4 " << op1 << " 4 " 
             << op2 << " 4 = " << n << endl;

    }



    #ifdef DEBUG   // Check main variables
    #endif
}










// RECYCLING BIN --------------------------------------------------------------
