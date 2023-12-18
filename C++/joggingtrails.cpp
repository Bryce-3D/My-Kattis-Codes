/* IDEA
2023-10-24
Possible iff the graph is Eulerian
         iff every vertex has even degree
Just pair up odd degree vertices to make it Eulerian (number of such 
vertices is guaranteed to be even due to the handshaking lemma).

But vertices might not have a direct path?
Just run APSP algorithm (Floyd-Warshall)
O(n^3) and n is only 15, this costs basically nothing and can be ignored.



Only main part left is needing to run "min weight perfect matching" on 
the complete graph of all vertices with odd degree.
How do you do this?

n <= 15 -> max 7 pairs needed to match
There are
    13 * 11 * 9 * 7 * 5 * 3 * 1 = 135135
possible ways to form 7 pairs.
This is certainly a lot less than I expected just brute force LMAO



New question: How to actually generate these pairs?
Ig can do the same way you do it manually (135135 means I can have a 
constant factor of 1000 and it'll be fine).

while not empty:
    for not the 1st thing:
        pair with 1st thing and recurse

Something like that shud probably work ig
I can also be dumb and precompute all 7 possible cases (1,2,...,7 pairs 
needed to match) but probably best not to.
Last resort jic ig.



NOTE TO SELF: Don't forget to 0-index the input like a retard again
We believe in 0-indexing supremacy





2023-10-31
Above thing TLEd apparently need to do some dp thing
Consider all possible subsets of the k odd degree vertices with an even 
number of elements.
k is at most 14 here
There will be 2^(k-1) such subsets (Consider the binomial expansion of 
(1-1)^k moment).
Denote 1010 to mean like you include the 1st and 3rd vertices
    1        0         1        0
    ^3rd in  ^2nd out  ^1st in  ^0th out
Then you can have your recurrence be like
    11101 = 
        min(00101 + w(3,4),
            01001 + w(2,4),
            01101 + w(0,4))
Essentially min among pair the first 1 with another 1 + min way to pair 
the rest.
Doing so would make the calculation of each entry in the table take O(k).
The table size is 2^(k-1)
n <= 15 -> k <= 14 -> time to calculate the table is roughly 
    14 * 2^13 = 114688
Wait wasn't 13!! = 135135 so this would TLE too?
Ah checking each possible perfect matching was another O(n)
Ok so this should be roughly an improvement of a factor of n perhaps
Is this enough? Idk because the problem is trolling and doesn't say how 
many test cases there are but ok.
If this TLEs I'll hardcode this cause idk any faster way and the 
pdf said u shud use dp anyway.

This somehow WAed after 0.04s...




2023-10-31 (2)
New idea:
Precompute what subproblems have to be looked at for each possible problem (?)

Compare every pair of numbers representing a subset
    -> (2^14)^2/2 ~ 1.28 x 10^8
Too risky

Check every possible subproblem (14C2 of the bits)
    -> 2^14 * 14C2 * 14 ~ 2x10^4 * 10^2 * 1.5x10 ~ 3x10^7
Shud be a safe precomputation
*/

/* DEBUG LOG
1.) WA Test Case 2
???
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
#define elif  else if
#define MAX_INT 2147483647   // Max val for int
using namespace std;
typedef vector<int> vi;
typedef vector<vector<int>> vvi;
typedef vector<vector<vector<int>>> vvvi;
typedef vector<bool> vb;
typedef vector<vector<bool>> vvb;
typedef vector<vector<vector<bool>>> vvvb;
typedef vector<ll> vll;
typedef vector<vector<ll>> vvll;
typedef vector<vector<vector<ll>>> vvvll;

// Comment this out to turn off debugging
// #define DEBUG





// Helper function to count the number of 1s in the binary rep 
// of a positive integer `n`
int n_1s(int n) {
    int ans = 0;
    while (n > 0) {
        ans += n%2;
        n >>= 1;
    }
    return ans;
}

// Helper function to give the binary representation of a 
// positive integer `n` as a vi using `b` bits
vi int_to_bin(int n, int b) {
    vi ans(b);
    for (int i = 0; i < b; i++) {
        ans[i] = n%2;
        n >>= 1;
    }
    return ans;
}

// Helper function to convert the binary representation of a 
// positive integer to its value
int bin_to_int(vi& bin) {
    int n = bin.size();
    int ans = 0;
    for (int i = 0; i < n; i++) {
        ans += (1<<i) * bin[i];
    }
    return ans;
}



/* Precompute the subproblems of a given state for the dp
`i` is in `dps` iff i has an even number of bits and
    0 <= i < 2^14
Define
    dps[i] = [[j,x,y] for j in S(i)]
Where
    S(i) = Set of nonnegative integers such that the leftmost 1 bit 
            of `i` and another 1 bit of `i` are flipped to 0
    x,y  = The indices of the 2 bits in `j` that got flipped
*/
unordered_map<int,vvi> Homura() {
    unordered_map<int,vvi> dps;
    int Kumiko = 1 << 14;

    // Special case (no subproblems)
    dps[0] = vvi(0);

    for (int n = 1; n < Kumiko; n++) {
        // Skip numbers that have an odd number of 1 bits
        if (n_1s(n)%2 != 0) {
            continue;
        }
        // Toss `n` into `dps` with a blank list
        dps[n] = vvi(0);
        vi bin = int_to_bin(n,14);   // Bin rep of n

        // Find the leftmost 1 bit of n
        int L = 13;
        while (bin[L] != 1) {
            L -= 1;
        }

        // For each possible right 1 bit to remove
        for (int R = L-1; R >= 0; R--) {
            if (bin[R] == 0) {                  // If the bit is 0
                continue;                       // Skip it
            }
            vi sub = {n-(1<<L)-(1<<R), R, L};   // Get the subproblem
            dps[n].push_back(sub);              // Toss it into dps[n]
        }
    }

    return dps;
}





int inf = 1 << 29;

int main() {
    /* Precompute the subproblems of a given state for the dp
    `i` is in `dps` iff i has an even number of bits and
        0 <= i < 2^14
    Define
        dps[i] = [[j,x,y] for j in S(i)]
    Where
        S(i) = Set of nonnegative integers such that the leftmost 1 bit 
                of `i` and another 1 bit of `i` are flipped to 0
        x,y  = The indices of the 2 bits in `j` that got flipped
    */
    unordered_map<int,vvi> dps = Homura();

    

    // Loop until terminating 0
    while (True) {
        // Read the 1st line of input
        int V;   // Number of vertices
        int E;   // Number of edges
        cin >> V;
        if (V == 0) {
            break;
        }
        cin >> E;

        vi deg(V,0);   // deg[i] = degree of vertex i
        int s_w = 0;   // s_w = sum of the weights of all edges
        // M_fake[i][j] = length of shortest edge from i to j
        //                inf if no edge exists between i and j
        //                0 if i = j
        vvi M_fake(V,vi(V,inf));
        for (int i = 0; i < V; i++) {
            M_fake[i][i] = 0;
        }

        // Read the edge inputs
        for (int ほむら = 0; ほむら < E; ほむら++) {
            // Read the edge
            int v0,v1,d;
            cin >> v0 >> v1 >> d;
            // Change to 0-index cause 1-indexing is crinj
            v0 -= 1;
            v1 -= 1;
            // Update `M_fake` (min used in case multiple edges exist)
            M_fake[v0][v1] = min(d,M_fake[v0][v1]);
            M_fake[v1][v0] = min(d,M_fake[v1][v0]);
            // Update the degrees of both ends
            deg[v0] += 1;
            deg[v1] += 1;
            // Update `s_w`
            s_w += d;
        }



        // D_fake[i][j] = min dist from i to j
        vvi D_fake(V,vi(V));
        // Copy `M_fake` into `D_fake`
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                D_fake[i][j] = M_fake[i][j];
            }
        }
        // Run Floyd Warshall's
        for (int k = 0; k < V; k++) {
            for (int i = 0; i < V; i++) {
                for (int j = 0; j < V; j++) {
                    if (D_fake[i][j] > D_fake[i][k]+D_fake[k][j]) {
                        D_fake[i][j] = D_fake[i][k]+D_fake[k][j];
                    }
                }
            }
        }



        // odds = list of vertices with odd degree
        vi odds(0);
        for (int i = 0; i < V; i++) {
            if (deg[i]%2 == 1) {
                odds.push_back(i);
            }
        }
        int n = odds.size();   // Number of vertices with odd degree



        // M is M_fake trimmed down to odd degree vertices only
        // M[i][j] = length of edge from odds[i] to odds[j]
        //           inf if the edge doesn't exist
        //           0 if it is to itself
        vvi M(n,vi(n));
        // D is D_fake trimmed down to odd degree vertices only
        // D[i][j] = min dist from odds[i] to odds[j]
        vvi D(n,vi(n));
        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {
                M[r][c] = M_fake[odds[r]][odds[c]];
                D[r][c] = D_fake[odds[r]][odds[c]];
            }
        }



        // Perform dp
        int nn = 1<<n;   // Number of subsets of odd degree vertices
        vi dp(nn,0);
        for (int k = 1; k < nn; k++) {   // For each subset `k`
            // Skip `k` if it has an odd number of elements
            if (dps.find(k) == dps.end()) {
                continue;
            }
            int next = inf;   // Next value to put into dp[k]
            // For each possible subproblem of `k`
            for (vi sub : dps[k]) {
                int k0 = sub[0];   // Subset of `k` with 2 elements removed
                int u  = sub[1];   // 1st removed element
                int v  = sub[2];   // 2nd removed element
                int c = dp[k0] + D[u][v];   // Min cost if `u` and `v` pair up
                next = min(c,next);   // Update next
            }
            dp[k] = next;   // Record the min cost for subset `k`
        }



        // Print the ans
        int m_mtg_w = dp[nn-1];    // Min matching weight of odd deg vertices
        int ans = s_w + m_mtg_w;   // Edge weights + extra pairs to "Eulerize"
        cout << ans << endl;



        #ifdef DEBUG   // Check main variables
        cout << endl << endl << endl;
        cout << "~~~~~ MAIN VARIABLE CHECK ~~~~~" << endl;

        // Check `M_fake` ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        cout << "M_fake = " << endl;
        for (vi v : M_fake) {
            for (int i : v) {
                cout << i << " ";
            }
            cout << endl;
        }
        cout << endl;

        // Check `D_fake` ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        cout << "D_fake = " << endl;
        for (vi v : D_fake) {
            for (int i : v) {
                cout << i << " ";
            }
            cout << endl;
        }
        cout << endl;

        // Check `deg` ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        cout << "deg = ";
        for (int i : deg) {
            cout << i << " ";
        }
        cout << endl;

        // Check `M` ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        cout << "M = " << endl;
        for (vi v : M) {
            for (int i : v) {
                cout << i << " ";
            }
            cout << endl;
        }
        cout << endl;

        // Check `D` ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        cout << "D = " << endl;
        for (vi v : D) {
            for (int i : v) {
                cout << i << " ";
            }
            cout << endl;
        }
        cout << endl;

        // Check `dp` ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        cout << "dp = [";
        for (int i : dp) {
            cout << i << ", ";
        }
        cout << "]" << endl << endl;
        #endif
    }
}










// RECYCLING BIN --------------------------------------------------------------
