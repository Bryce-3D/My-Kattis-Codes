/* IDEA
This looks like 4-SUM lmao

N <= 4000

Select each possible value for d (4000)
Solve 3-SUM on it?

How to do 3-SUM again
Need to find a,b,c such that a+b+c=d

Have a hash (or sort and bsearch) to find in O(1) (or O(logN)) whether 
a given value is in the list.
This was the O(N) (or O(NlogN)) idea for 2-SUM
3-SUM then made it O(N^2) by performing this on each element
Doing this again on 4-SUM would make it O(N^3)
N^3 = 4000^3 = 6.4 x 10^10, too big

Look at constraints again
    [-536 870 912, 536 870 911]
It's not like these numbers are even small tho

Try to reverse engineer time complexity
Suppose you first pick c and d
    O(N^2) = 4000^2 = 1.6 x 10^7
You'd then need to find a,b that work in less than O(N) time
Could some preprocessing make it O(1)?

What if you preprocess all possible sums a+b, sort it, then store it?
You'd have to also store which indices were used to create this sum to 
make sure that you don't pick the same c and d again
This preprocessing would again be O(N^2)
Wait and to sort this it'd take O(2N^2logN)
Searching this would be O(log(N^2)) = O(2logN)

Total time complexity: 2 * N^2 * 2logN ~ 8 x 10^8
This might be a bit too much and risky idk
Oh wait the time limit is 11s hmmm shud be fine???

2^31 = 2 147 483 648
Wait
536 870 912 * 4 = 2^31
Ok the constrains were made so that it would fit into int exactly LMAOOO

Testing using the following code I confirmed that vectors are sorted 
lexicographically so I can just put the sum in front and it'll auto sort
```
int main() {
    vector<int> v1 = {1,2,3};
    vector<int> v2 = {1,2,4};

    cout << "[1,2,3] < [1,2,4] = " << (v1 < v2) << endl;   // 1
    cout << "[1,2,4] < [1,2,3] = " << (v2 < v1) << endl;   // 0

    vector<int> v3 = {10,4,6};
    vector<int> v4 = {12,1,2,3,4};
    cout << "[10,4,6] < [12,1,2,3,4] = " << (v3 < v4) << endl;   // 1

    vector<vector<int>> v5 = {v3,v4,v2,v1};
    sort(v5.begin(),v5.end());
    cout << "v5[0] = " << v5[0][0] << " " << v5[0][1] << " " << v5[0][2] 
         << endl
         << "v5[1] = " << v5[1][0] << " " << v5[1][1] << " " << v5[1][2] 
         << endl
         << "v5[2] = " << v5[2][0] << " " << v5[2][1] << " " << v5[2][2] 
         << endl
         << "v5[3] = " << v5[3][0] << " " << v5[3][1] << " " << v5[3][2]
         << endl;

    pair<int,pair<int,int>> s0(1,pair<int,int>(100,200));
    pair<int,pair<int,int>> s1(2,pair<int,int>(1,2));
    cout << "s0 < s1 = " << (s0<s1) << endl;
}
```


Data Structures and variables
    N      = Number of integers
    v      = list of all the integers
    sum[i] = [ith smallest sum, [ind of summand 1, ind of summand 2]]

~~~~~~~~~~ Algo ~~~~~~~~~~
Initialize `N` and `v` by reading input
For each UNORDERED pair of distinct ints {a,b}, sum them and toss into `sum`
Sort `sum` in ascending order

For each ORDERED pair of distinct integers {c,d}
Iterate through the largest d's first
Check if d-c exists in sum st a,b,c,d distinct

*/

/* DEBUG LOG
1.) WA on TC6
    Fucking typed (L<N) instead of (L<sum.size()) in the last part ffs
*/

#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <bitset>
#include <algorithm>
#define ll LoveLive
#define LoveLive long long
#define True  true
#define False false
#define MAX_INT 2147483647   // Max val for int
using namespace std;

/*
Homu = [int a, int b, int s]
Homu represents a possible sum of a pair of distinct integers
Created to allow a custom 
*/

int main() {
    int N;              // Number of ints
    cin >> N;
    vector<int> v(N);   // List of ints
    for (int i = 0; i < N; i++) {
        cin >> v[i];
    }
    sort(v.begin(),v.end());

    // sum[i] = [ith smallest sum, [ind of summand 1, ind of summand 2]]
    vector<pair<int,pair<int,int>>> sum(N*(N-1)/2);
    int Homura = 0;
    for (int i = 0; i < N; i++) {
        for (int j = i+1; j < N; j++) {
            pair<int,pair<int,int>> Kumiko(v[i]+v[j],pair<int,int>(i,j));
            sum[Homura] = Kumiko;
            Homura++;
        }
    }
    sort(sum.begin(),sum.end());

    // {DEBUG -----------------------------------------------------------------
    // for (auto debug : sum) {
    //     cout << "[" << debug.first << ",[" << debug.second.first << "," 
    //          << debug.second.second << "]]" << endl;
    // }
    // DEBUG} -----------------------------------------------------------------

    // Iterate through all possible pairs (c,d)
    for (int d = N-1; d >= 0; d--) {    // Index of d
        for (int c = 0; c < N; c++) {   // Index of c
            if (d == c) { continue; }   // Ints must be distinct

            int t = v[d]-v[c];   // Target sum d-c

            if ((t < sum[0].first) || (t > sum[sum.size()-1].first)) {
                continue;
            }

            // Binary search the target sum in [L,R)
            // Try to find the LEFT end of the area with target sum since 
            // there could be multiple pairs that give the target sum.
            // Will terminate with the leftmost target sum at R
            int i = 0;
            if (t > sum[0].first) {
                int L = 0;
                int R = sum.size();
                while ((R-L) > 1) {
                    int M = L + (R-L)/2;
                    if (t <= sum[M].first) {   // If t is to the left
                        R = M;
                    } else {   // If target t is to the right
                        L = M;
                    }
                }
                i = R;
            }

            // Check if any of these sums works
            while ((i < sum.size()) && sum[i].first <= t) {
                if (sum[i].first != t) { i++; continue; }
                // Check distinctness
                int x = sum[i].second.first;    // Index of a
                int y = sum[i].second.second;   // Index of b
                if ((x!=d) && (x!=c) && (y!=d) && (y!=c) && (x!=y) && (d!=c)) {
                    cout << v[d] << endl;
                    return 0;
                }

                i++;
            }
        }
    }

    // If nothing works, print "no solution"
    cout << "no solution" << endl;
}
