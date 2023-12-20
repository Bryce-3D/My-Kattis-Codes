/* IDEA
Wow only at most 10 categories??? (a to j)
So at most 30 videos and at most 10 categories
Case 1: Pick first video
Case 2: Don't pick first video
Can you DP?
Case 1 -> Need every category
Case 2 -> Need every other category left
Hmm you'd have to DP on *every* possible subset of unwatched categories
Wait then can't you abuse the fact that 10 is small???
2^10 -> 1024 possible subsets
Grid will be 30(+1?) * 1024 size at most which is very very very doable
If this works it would be so dumb ngl

30 videos, each at most 900s, so max possible is 2700.
Can safely use arbitrarily large int `MAX` as a placeholder for impossible

CRUX OF THE IDEA
    DP[set of categories][i]
        The minimum amount of time needed to watch the specified set of 
        categories by only utilizing stuff from the first i videos.
        Set this to MAX if this is impossible.

Consider an arbitrary set of categories S, and let T be the set of categories 
satisfied by the ith video
    DP[S][i] = 
        min(DP[S][i-1], DP[S-T][i-1]+t[i])
where
    S-T represents set subtraction
    t[i] = duration (or time) of the ith video



Now time for implementation details

What I need for set subtraction on bits is
    a b -> output
    1 1 -> 0
    1 0 -> 1
    0 1 -> 0
    0 0 -> 0
This can be modeled by 
    a AND (NOT b)
So I can try representing the 2^10 possible subsets of categories as an int 
from 0 to 1023 and do bitwise operations to represent set subtraction.
Set addition is obviously union, aka just OR.
*/

/* DEBUG LOG
1.) Forgot to read in the first line for N and initialize S_req to 0
2.) Segfault
        // Perform the DP
        for (int c = 1; c <= N; c++) {
            int t = videos[c][0];     // Duration of the cth video
            int s_c = videos[c][1];   // Set of categories of the cth video
    Need to -1 the indexing of the videos oops
*/

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <bitset>
#include <algorithm>
#define ll LoveLive
#define LoveLive long long
#define True  true
#define False false
#define MAX 2147483647
using namespace std;

// Helper function to convert the string representation of a set of  
// categories to its corresponding integer representation.
// In this scheme, a = 2^0, b = 2^1, and so on until j = 2^9
int str_to_int(string cat) {
    int n = cat.size();   // Number of categories
    int ans = 0;
    for (int i = 0; i < n; i++) {
        char c = cat[i];
        int exp = (int)c - (int)'a';
        ans += 1 << exp;
    }
    return ans;
}

// Helper function to subtract two sets of categories represented as ints
int sub_cats(int L, int R) {
    return L & (~R);
}





int main() {
    int N;           // Number of videos
    int S_req = 0;   // Full set of categories that exist (as an int)
    cin >> N;
    // videos[i] = [duration of i, categories of i (as an int)]
    vector<vector<int>> videos;
    for (int Homu = 0; Homu < N; Homu++) {
        int d;
        string s_s;
        cin >> d >> s_s;
        int s_i = str_to_int(s_s);
        videos.push_back(vector<int>{d,s_i});
        S_req |= s_i;   // Also update the set of required categories
    }



    // DP[S][i] = min amount of time to watch categories S using the first 
    //            `i` videos only; set to `MAX` if this is not possible
    vector<vector<int>> DP(1024,vector<int>(N+1));
    // Initialize column 0
    DP[0][0] = 0;   // No videos needed for 0 categories
    for (int i = 1; i < 1024; i++) {
        DP[i][0] = MAX;   // Impossible to watch a non-empty set with 0 videos
    }

    // Perform the DP
    for (int c = 1; c <= N; c++) {
        int t = videos[c-1][0];     // Duration of the cth video
        int s_c = videos[c-1][1];   // Set of categories of the cth video

        for (int s = 0; s < 1024; s++) {   // s = set of categories needed
            // Case 1: You watch the cth video
            int s_0 = sub_cats(s,s_c);      // Remaining categories needed
            int case1 = MAX;                // Set to MAX if not possible
            if (DP[s_0][c-1] != MAX) {      // If possible
                case1 = DP[s_0][c-1] + t;   // Update case1's result
            }
            // Case 2: You don't watch the cth video
            int case2 = DP[s][c-1];         // Copy val of prev col
            // Record the better case
            DP[s][c] = min(case1,case2);
        }
    }



    // Print the ans
    cout << DP[S_req][N] << endl;
}
