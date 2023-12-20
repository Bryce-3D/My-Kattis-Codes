#include <bits/stdc++.h>
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

/* IDEA

*/

/* DEBUG LOG

*/

struct Homu {
    int s;   // Number of solves
    int p;   // Penalty
    int f;   // Final submission time
    int o;   // Onsite flag
    int i;   // Index
};

bool operator <(const Homu& L, const Homu& R) {
    if (L.s > R.s) {
        return True;
    } elif (L.s < R.s) {
        return False;
    }

    if (L.p < R.p) {
        return True;
    } elif (L.p > R.p) {
        return False;
    }

    if (L.f < R.f) {
        return True;
    } elif (L.f > R.f) {
        return False;
    }

    return False;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    unordered_map<int,int> pts = {
        {1,100}, {2,75},  {3,60},  {4,50},  {5,45}, 
        {6,40},  {7,36},  {8,32},  {9,29},  {10,26},
        {11,24}, {12,22}, {13,20}, {14,18}, {15,16},
        {16,15}, {17,14}, {18,13}, {19,12}, {20,11}, 
        {21,10}, {22,9},  {23,8},  {24,7},  {25,6}, 
        {26,5},  {27,4},  {28,3},  {29,2},  {30,1}
    };


    int n;
    cin >> n;
    vector<Homu> v(0);
    int s_,p_,f_,o_;
    for (int Kumi = 0; Kumi < n; Kumi++) {
        cin >> s_ >> p_ >> f_ >> o_;
        v.push_back(Homu{s_,p_,f_,o_,Kumi});
    }

    vector<int> ans(n,0);
    sort(v.begin(),v.end());
    int ind = 0;

    // While there are more people to check
    while (ind < n) {
        // Get all the people who are tied
        int L = ind;
        ind += 1;
        while ((ind < n) && (v[L].s==v[ind].s) && 
                (v[L].p==v[ind].p) && (v[L].f==v[ind].f)) {
            ind += 1;
        }
        
        // Get the score of the tied people
        int score = 0;
        for (int i = L; i < ind; i++) {
            if (i < 30) {
                score += pts[i+1];
            }
        }
        if (score > 0) {
            score = (score-1)/(ind-L) + 1;
        }

        // Set their scores in ans
        for (int i = L; i < ind; i++) {
            ans[v[i].i] = score;
        }
    }

    // Add the remaining +1 pts
    for (int i = 0; i < n; i++) {
        ans[v[i].i] += v[i].o;
    }

    for (int i : ans) {
        cout << i << endl;
    }

    




    #ifdef DEBUG   // Check main variables
    cout << endl << endl << endl;
    cout << "~~~~~ MAIN VARIABLE CHECK ~~~~~" << endl;

    for (Homu homu : v) {
        cout << homu.s << " " << homu.p << " " << homu.f << " " 
             << homu.o << " " << homu.i << endl;
    }
    #endif
}










// RECYCLING BIN --------------------------------------------------------------
