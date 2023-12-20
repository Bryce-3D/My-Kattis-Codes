/* IDEA
13! ~ 6.2 x 10^9
Yeh no way this is iterate through everything lmao
It seems quite close though, like even just 
eliminating a factor 10 would work.

Alternative formulation:
    Given a matrix, determine whether it is possible to pick one number 
    per row st no two lie in the same sol and none are on the main 
    diagonal st they sum to a desired target?
Wait no no this could cause a bunch of disjoint cycles instead...



2023-09-12 now
I thought of this vague idea I think yesterday idr when
A was MITM, does B also die to MITM (split vertices into half half)
Asking someone who finished this PS he said it was on the right track
So now I have to finish filling in the blanks I made for myself with 
that vague idea I suppose.

N <= 14 vertices
L <= 10^15 desired length

NOTE TO SELF USE LONG LONG FOR THE PATH
ラブライブ！スーパースター

We split the set of N vertices into floor(N/2) and ceil(N/2)?
Wait we can designate a "starting" point (that will obviously also be the 
end point)

Ok so
Fixed vertex X
Iterate through all partitionings the remaining vertices into two subsets 
S_0, S_1, where |S_0| and |S_1| are fixed values.
Suppose |S_0| = a, |S_1| = b
How many stuff will we have to iterate through?

(a+b)Cb * a! * b!

Note that a+b = n-1
So our worst case is a+b = 13
Best split is most likely a 6-7 split

Wait a minute this is just (a+b)!

Ok wait right we need to uh
Precompute all the subsets of size a then all the subsets of size b?
Perhaps yeh that was my original idea I thought hopwfully I could've 
used a simpler way but no that removes the precomputing part so it dies.
Idk :skull:

Suppose we wish to find all possible 
*wait random thought use int bitmasking to represent subsets then idk*
Ok back to my original train of thought

Suppose we wish to find all possible simple paths that pass through a 
specific subset of vertices for all subsets of a size k

nCk subsets of size k
k!  paths to permute through
k   to compute each path length
nCk * k! * k = n!/(n-k)! * k

When n=14 and we set k = 7...
    (14! * 7) / (7!) ~ 1.2 x 10^8
Ok this is definitely precomputable
Using the "fix a point" method n drops to 13 even
    (13! * 7) / (7!) ~ 8.6 x 10^6



General idea ~~~~~~~~~~
Pick a vertex to be the starting vertex
Let S be the set of non-starting vertices
    s = |S|
    a = floor(s/2)
    b = ceil(s/2)
The path will be 

Precompute all possible path lengths for all subsets of size a
Precompute all possible path lengths for all subsets of size b
For every partition of S into a subset A of size a and a subset B of size b
    Check if there is a pair such that 



Ok I took a piss and realized my mistake earlier
Ofc I always realize things when I take a piss
I was supposed to add a! and b!, not multiply them

So the actual runtime is

(a+b)Cb * (a! + b!)

Well I still have to compare them again each other
This can just be min(a!,b!) by comparing sets
ig just toss a factor of 2 cause why not

    2 * (a+b)Cb * (a! + b!)

Wait how do I handle the edge that jumps from set A to set B tho???
This is not constant
I can't pick a 2nd fixed point
Unless I iterate through all 2nd fixed points???

Pick a fixed starting point X
Vary the middle point M across the remaining n-1 points
Partition the remaining n-2 points into subsets A,B of size a,b, a+b=n-2
Iterate through all (a+b)Cb such partitions
For each partition
    (a!+b!)*(a+b) to precompute all possible lengths for both subsets
    (including the length of the segment connecting to the starting point and 
    the middle point, which is now fixed so this can be included)
    Compare the two sets (O(1) per check due to hashing). This takes a!+b!
    Each partition therefore takes
        2*(a+b)*(a!+b!)
Therefore, the total runtime will be
    (n-1) * (a+b)Cb * 2 * (a+b) * (a!+b!)
Try letting n = 14, a=b=6, this evaluates to ~4.15 x 10^8
THIS MIGHT PASS LET'S FUCKING GO PLS
I have like too many other assignments to do rn actually kill me





2023-09-13 now

---------- Algo High Level Idea ----------
Set vertex 0 to be the starting vertex
For middle vertex in range(1,N)
    Partition the remaining points into subsets A and B
    Compute the length for all a! + b! permutations within each of A and B
    Check via hash table whether any two of them sum up to L
    If there exists one
        print("possible")
        return
print("impossible")
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

// // Helper function that deletes an integer i from v not in place
// vector<int> del0(vector<int> v, int i) {
//     v.erase(remove(v.begin(),v.end(),i),v.end());
//     return v;
// }

// // Helper function that deletes an integer i from v in place
// void del1(vector<int> * v, int i) {
//     v->erase(remove(v->begin(),v->end(),i),v->end());
// }

// 

// Helper function that returns a list of all possible boolean lists of 
// length m such that exactly n elements are true.
vector<vector<bool>> choose(int m, int n) {
    // Prevent wrong calls
    if ((m < n) || (n < 0) || (m <= 0)) {
        cout << "Incorrectly called choose(" << m << "," << n << ")" << endl;
        exit(0);
    }

    // Base cases
    if (n == 0) {
        return vector<vector<bool>>(1,vector<bool>(m,False));
    } else if (m == n) {
        return vector<vector<bool>>(1,vector<bool>(m,True));
    }

    // If last element is True
    vector<vector<bool>> v0 = choose(m-1,n-1);
    for (vector<bool> &v : v0) {   // & so that it does not duplicate
        v.push_back(True);
    }
    // If last element if False
    vector<vector<bool>> v1 = choose(m-1,n);
    for (vector<bool> &v : v1) {   // & so that it does not duplicate
        v.push_back(False);
    }

    // Append together
    v0.insert(v0.end(), v1.begin(), v1.end());
    return v0;
}



int main() {
    int V;   // Number of vertices
    ll  L;   // Desired Hamiltonian Cycle length
    cin >> V >> L;

    // dist[i][j] = distance between i and j
    vector<vector<ll>> dist(V,vector<ll>(V));
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            cin >> dist[i][j];
        }
    }

    // Corner case where V = 2 or 3, since a=0 would break this
    if (V < 4) {
        // Homu is the permutation of vertices
        vector<int> Homu(V);
        for (int i = 0; i < V; i++) {
            Homu[i] = i;
        }
        // Check everything manually
        do {
            ll d = dist[Homu[V-1]][Homu[0]];
            for (int i = 0; i < V-1; i++) {
                d += dist[Homu[i]][Homu[i+1]];
            }
            if (d == L) {
                cout << "possible";
                return 0;
            }
        } while (next_permutation(Homu.begin(),Homu.end()));
        // If nothing worked, then impossible
        cout << "impossible";
        return 0;
    }

    int a = (V-2)/2;   // Number of vertices in set A
    int b = V-2-a;     // Number of vertices in set B

    // Representation for all possible partitions of a subset of size 
    // a+b into subsets of sizes a and b
    // True  = in set A
    // False = in set B
    vector<vector<bool>> ptts = choose(V-2,a);

    // Designate vertex 0 as the "starting vertex"
    // Iterate through vertex 1 to V-1 as the "middle vertex"
    for (int M = 1; M < V; M++) {
        // The other V-2 vertices to be used
        vector<int> vertz(0);
        for (int i = 1; i < V; i++) {
            if (i != M) { vertz.push_back(i); }
        }

        // For each possible partition
        for (vector<bool> ptt : ptts) {
            // Create the partition into sets A and B
            vector<int> A(0);
            vector<int> B(0);
            for (int i = 0; i < V-2; i++) {
                if (ptt[i] == True) {
                    A.push_back(vertz[i]);
                } else {
                    B.push_back(vertz[i]);
                }
            }
            sort(A.begin(),A.end());
            sort(B.begin(),B.end());

            // Set of possible distances in set A
            set<ll> A_d;
            // Set of possible distances in set B
            set<ll> B_d;

            // Iterate through all a! permutations of set A the path that 
            // goes 0 -> permutation of A -> M
            do {
                ll next = 0;
                next += dist[0][A[0]] + dist[A[a-1]][M];
                for (int i = 0; i < a-1; i++) {
                    next += dist[A[i]][A[i+1]];
                }
                A_d.insert(next);
            } while (next_permutation(A.begin(),A.end()));

            // Iterate through all b! permutations of set B the path that 
            // goes 0 -> permutation of B -> M
            do {
                ll next = 0;
                next += dist[M][B[0]] + dist[B[b-1]][0];
                for (int i = 0; i < b-1; i++) {
                    next += dist[B[i]][B[i+1]];
                }
                B_d.insert(next);
            } while (next_permutation(B.begin(),B.end()));

            // Check if there exists a pair that sums up to L
            for (ll d0 : A_d) {
                if (B_d.find(L-d0) != B_d.end()) {
                    cout << "possible" << endl;
                    return 0;
                }
            }

            // If no pair, continue to the next partition
        }
        // If this fails, check the next possible middle vertex
    }
    // If nothing is found, then it is impossible
    cout << "impossible" << endl;
    return 0;
}










// RECYCLING BIN --------------------------------------------------------------
// // Helper function that returns a list of all permutations of 
// // a list of integers.
// vector<vector<int>> permute(vector<int> V) {
//     // Base case
//     if (V.size() == 1) {
//         return vector<vector<int>>(1,V);
//     }

//     // Recursion
//     vector<vector<int>> ans(0);
//     for (int i : V) {
//         vector<vector<int>> next = permute(del0(V,i));
//         for (vector<int> v : next) {
//             v.push_back(i);
//         }
//         ans.insert(ans.end(), next.begin(), next.end());
//     }
//     return ans;
// }

// // Helper function that returns a list of all possible boolean lists of 
// // length m such that exactly n elements are true.
// vector<vector<bool>> choose(int m, int n) {
//     // Prevent wrong calls
//     if ((m < n) || (n < 0) || (m <= 0)) {
//         cout << "Incorrectly called choose(" << m << "," << n << ")" << endl;
//         int 暁美ほむら = 1/0;
//     }

//     // Base cases
//     if (n == 0) {
//         return vector<vector<bool>>(1,vector<bool>(m,False));
//     } else if (m == n) {
//         return vector<vector<bool>>(1,vector<bool>(m,True));
//     }

//     // If last element is True
//     vector<vector<bool>> v0 = choose(m-1,n-1);
//     for (vector<bool> v : v0) {
//         v.push_back(True);
//     }
//     // If last element if False
//     vector<vector<bool>> v1 = choose(m-1,n);
//     for (vector<bool> v : v1) {
//         v.push_back(False);
//     }

//     // Append together
//     v0.insert(v0.end(), v1.begin(), v1.end());
//     return v0;
// }
