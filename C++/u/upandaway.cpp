// CREDITS
// ChatGPT was used to help me figure out how to use std::priority_queue
// Referenced my previous code at
//     https://github.com/Bryce-3D/My-Codeforces-Codes/blob/main/C%2B%2B/1801-1900/CF_1833B.cpp
// on how to do a custom comparator

/* Idea
2023-08-17
First thought - ????????
Rereading today - wait the numbers are small can I do the braindead "make 
a copy of the graph for each possible number of fireworks left"?

n = 100 bases
f = 100 fireworks
-> 100^2 vertices = 10^4
Even O(n^2) might pass amazing

Wait it's a complete graph ok sure.
Just use standard 2d array to store all distances ig
And then an array for the heights too.
Then can memoize distances using a 2d array too?
Yeh there's only 10^4 states just use an array
Then dijkstra's says hi.

max 1000 dist between 2 mountains is 1000
Traveling to the same vertex twice is completely useless, so we have at most 
n-1 <= 99 trips
Max time taken is 99000 < 10^9 < 2^30 < 2^32 so int is enough ig

Wait u need k+1 graphs not k
eh it shud be fine
*/

/* Cheatsheet
std::vector<T>
    push_back() - append()
    pop_back()  - pop()
    size()
    clear()
    at()        - a[i] but checks for out of bounds
    empty()     - returns True is empty, False otherwise

std::priority_queue<T>
    push()
    pop()   - Does not return the value, C++ is actually trolling
    top()   - peek()
    size()
    empty() - returns True if empty, False otherwise

Different way?
https://stackoverflow.com/questions/16111337/declaring-a-priority-queue-in-c-with-a-custom-comparator
Idk idr where I found how to custom comparator for CF last time
*/

#include <iostream>
#include <vector>
#include <queue>

#define ll long long

using namespace std;

/*
Homu = [int ind, int fire, int time]
Homu represents a state to explore
Created to allow a custom comparator so the pq can sort
*/
struct Homu {
    int ind;    // Index of the mountain
    int fire;   // Number of fireworks used so far
    int time;   // Time traveled so far

    Homu(int i, int f, int t) {
        this->ind = i;
        this->fire = f;
        this->time = t;
    }
};
// Define < operator so that I can use sort
bool operator < (const Homu& a, const Homu& b) {
    // C++ Priority Queue uses max heap, so I need to flip the sign to get 
    // the shortest distance to pop first.
    return a.time > b.time;
}





int main() {
    // Input parsing ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    int n;   // Number of bases
    int x;   // Target index (0-indexed)
    int k;   // Number of fireworks
    cin >> n >> x >> k;
    x -= 1;   // 0-index x
    // h[i] = height of ith mountain (0-indexed)
    vector<int> h(n);
    for (int i = 0; i < n; i++) {
        cin >> h[i];
    }
    // t[i][j] = time from mountain i to j (0-indexed)
    vector<vector<int>> t(n, vector<int>(n));
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            cin >> t[r][c];
        }
    }
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Handle corner case jic
    if (x == 0) {
        cout << 0;
        return 0;
    }



    /*
    time[i][j] = time using i fireworks from mountain 0 to mountain j
        initialized to -1 for unvisited vertices
        time[0][0] = 0 cause ofc
    frontier = priority queue of Homu's to explore
        Homu stores the mountain index, number of used fireworks, and the 
        time spent so far
    */
    // Initialize all (k+1)n states as -1
    vector<vector<int>> time (k+1, vector<int> (n,-1));
    // Initialize the frontier
    priority_queue<Homu> pq;

    // Initiate Dijkstra's
    pq.push(Homu(0,0,0));


    // {DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // cout << "Current size of `pq` is " << pq.size() << endl;
    // cout << "pq.empty() evaluates to " << pq.empty() << endl;
    // cout << "!pq.empty() evaluates to " << !pq.empty() << endl;
    // DEBUG} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



    // While there are more nodes to explore
    while (!pq.empty()) {   
        // Pop the next node
        Homu next = pq.top();
        pq.pop();
        int ind = next.ind;
        int f_0 = next.fire;
        int t_0 = next.time;

        // {DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // cout << "Next is " << endl;
        // cout << "[" << ind << ", " << f_0 << ", " << t_0 << "]" << endl;
        // DEBUG} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



        // If this node has been explored already, then skip
        if (time[f_0][ind] != -1) {
            // Extra if condition for sanity check
            if (t_0 < time[f_0][ind]) {
                cout << "Dijkstra's didn't run properly, ";
                cout << "a node has been re-explored with less time" << endl;
                cout << "Running 1/0 to terminate for the memes" << endl;
                int have_you_heard_of_the_hit_anime_Assault_Lily_Bouquet = 1/0;
            } else {
                // {DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                // cout << "A vertex does not fit" << endl;
                // DEBUG} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                continue;
            }
        }
        // Else, record the time taken (also marks node as visited)
        time[f_0][ind] = t_0;



        // Generate the neighbors and toss them into the priority queue
        for (int i = 0; i < n; i++) {   // For each mountain i
            // Skip if it's yourself
            if (i == ind) {
                continue;
            }
            // Skip if we don't have enough fireworks
            if (f_0 + max(h[i]-h[ind],0) > k) {
                // {DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                // cout << "Not enough fireworks from ";
                // cout << "[" << ind << "," << f_0 << "]";
                // cout << " to ";
                // cout << "[" << i << "," << f_0 << max(h[i]-h[ind],0) << "]";
                // DEBUG} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                continue;
            }
            // Create the corresponding state
            int f_1 = f_0 + max(h[i]-h[ind],0);
            int t_1 = t_0 + t[ind][i];
            Homu next = Homu(i,f_1,t_1);
            // Insert the state into the priority queue
            pq.push(next);
        }

        // {DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // cout << "Number of elements in pq now is " << pq.size() << endl;
        // DEBUG} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    }



    // Check what is the fastest time to get to mountain x, if any
    int ans = -1;
    for (int i = 0; i < k+1; i++) {
        if (time[i][x] != -1) {
            if (ans == -1) {
                ans = time[i][x];
            } else {
                ans = min(time[i][x], ans);
            }
        }
    }
    cout << ans << endl;

    // {DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // // Check t
    // cout << "t is " << endl;
    // for (int r = 0; r < t.size(); r++) {
    //     for (int c = 0; c < t[0].size(); c++) {
    //         cout << t[r][c] << " ";
    //     }
    //     cout << endl;
    // }
    // // Check time
    // cout << "time is " << endl;
    // for (int r = 0; r < time.size(); r++) {
    //     for (int c = 0; c < time[0].size(); c++) {
    //         cout << time[r][c] << " ";
    //     }
    //     cout << endl;
    // }
    // // Check h
    // cout << "h is " << endl;
    // for (int i = 0; i < h.size(); i++) {
    //     cout << h[i] << " ";
    // }
    // cout << endl;
    // DEBUG} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    return 0;
}
