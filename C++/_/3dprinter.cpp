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
Spam printers then spam the statue

Spam printers for n days -> 2^n printers
print statues for d days -> 2^n * d statues

Given n+d days can get 2^n*d statues

(d+1)/d <= 2 for all psoitive integers d

Hm
spam print for n days
have k=2^n printers

spam printer for 1 day + spam statue next day
vs
division + spam statue next day

let a+b = k
2(a+b) vs a + a+2b
same...

1 day  -> 2^0 * 1 = 1
2 days -> 2^1 * 1 = 2
3 days -> 2^2 * 1 = 4

ok yes just logarithm

1 -> 1
2 -> 2
3 -> 3
4 -> 3
5 -> 
*/

/* DEBUG LOG

*/

// Comment this out to turn off debugging
// #define DEBUG

int main() {
    int n,n0;
    cin >> n;
    n0 = n;

    int ans = 0;
    while (n0 != 0) {
        n0 >>= 1;
        ans += 1;
    }
    if (n != (1 << (ans-1))) {
        ans += 1;
    }
    cout << ans;



    #ifdef DEBUG   // Check main variables
    #endif
}










// RECYCLING BIN --------------------------------------------------------------
