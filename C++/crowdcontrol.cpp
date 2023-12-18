// CREDITS
/* ChatGPT prompts
How do I instantiate a pair in C++?

How do I sort a vector of integers in C++?
*/
// CREDITS



/* Idea
2023-08-14
Path length does not matter, only capacity

Sort all edges in descending weights and add in largest weight until a path 
exists. But then how will you know when the path actually exists?
Perhaps you could do some connected component algorithm but I'm not familiar 
with that atm.

New idea, grow outwards from the starting point like Dijkstra's instead.
Always grow outwards along the heaviest unincluded edge.
Never include a cycle?

Suppose that during this alg we wish to add edge ab, where a and b are both 
already in the tree growing out from the start.
WLOG supposed that a (possibly the start) was added into the tree before b.
This means that when b was added, the weight of the edge that connects to b 
was <= the weight of the edge ab due to the order they were added.
Therefore adding this edge does not create a better way to access b.

On the other hand, a was added before b.
I claim that this implies that the max cap path to a >= max cap path to b.
More generally, I claim that the vertices are added precisely in the order of 
non-increasing max cap path?
Check by induction. Base case trivial.
Suppose we add an edge that causes a new vertex v to be included in the tree.
Suppose that the added edge is uv.
Suppose for the sake of contradiction that the max cap path of v is greater 
than the max cap path of some w already previously included in the tree.
Since w is already included in the path, then that means that we must've 
picked some edge that has strictly less weight than the max cap path to v.
But then this is a contradiction, because our alg always picks the heaviest 
edge incident to an already included vertex. And since there exists a path 
to v with a higher capacity, aka all edges along said path have a higher 
weight than the limiting edge that goes to w, then all of these edges 
should've been picked before w's limiting edge, a contradiction!

Therefore, a being added before b implies that a's max cap path is not less 
than b's max cap path, so going to a via b then ba will never be better than 
the best path excluding b.
Therefore, the edge ab will be useless.

Ok so we can slightly modify the alg and only grow out as a tree, no cycles.
Actually maybe we can record the "direction we grew out".
The argument about the vertices being included in non-increasing max cap path 
order seems to imply that the edges like grow out and the direction wait no 
it's a tree ofc the path between the start and any other vertex is unique 
POV: You're retarded ffs
Ok so yeh perhaps we can also record the directions and it'll become a 
directed acyclic tree growing out from the start.
Then once we hit the destination, we backtrack the edges to find the path.
Once we find the path, we can just manually bash to find which streets to 
close.
There are at most 1000 cities and 1000 roads just a simple linear scan will do.

Wait does C++ have a set/dict data structure like Python?
Set of vertices included in the tree.
Wait no I can just use a boolean array that works too, there's only at most 
1000 cities anyway.

Algorithm: (0-index everything as usual because 1-indexing is crinj)
- Boolean array `included` such that included[i] = True if vertex i is 
  included in the set of vertices currently reachable in the tree.
- Integer array `parent` such that parent[i] = the index of the parent of 
  vertex i if vertex i is included in the tree already and -1 otherwise (also 
  set to -1 for the root node)
- Priority Queue of
- While vertex n-1 is not included
  - 

WAIT BIGGER PROBLEM DOES C++ EVEN HAVE A PRIORITY QUEUE I HAVE NO IDEA
Ok worst case I can self-implement using a heap that's definitely doable.
Ok thankfully yes it does
    https://en.cppreference.com/w/cpp/container/priority_queue
Probably easier to figure out how to use this than to self-implement it took 
me like 3 hrs to implement a segtree in Python last time back to finalizing 
the alg



2023-08-17
The hints dropped from today's lec seems similar to the 2 idea I had, that's 
good I guess but I still suck at implementing



2023-08-22
Random side comment my ideas are apparently just prim's and kruskal's but for 
max spanning tree instead of min spanning tree


Algorithm: (0-index everything as usual because 1-indexing is crinj)
- Boolean array `included` such that included[i] = True if vertex i is 
  included in the set of vertices currently reachable in the tree.
- Integer array `parent` such that parent[i] = the index of the parent of 
  vertex i if vertex i is included in the tree already and -1 otherwise (also 
  set to -1 for the root node)
- Priority Queue `next` of the next edge to explore when trying to grow the 
  tree
- While vertex n-1 is not included
  - Pop next node from 
*/

/* DEBUG LOG
1.) Outputs
*/

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <bitset>
#include <algorithm>
#define ll long long
using namespace std;

/*
    Edge = [v_0, v_1, cap]
Represents a directed edge to be put in the priority queue during execution.
*/
struct Edge {
    int v_0;   // Starting vertex
    int v_1;   // Ending vertex
    int cap;   // Capacity of the edge

    Edge(int v_0, int v_1, int cap) {
        this->v_0 = v_0;
        this->v_1 = v_1;
        this->cap = cap;
    }
};
bool operator < (const Edge& L, const Edge& R) {
    return L.cap < R.cap;
}





int main () {
    int V;   // Number of vertices
    int E;   // Number of edges
    cin >> V >> E;

    // adj[i] = [(j,w_ij,index_ij) for j in i's neighbors]
    vector<vector<vector<int>>> adj(V);
    for (int Homu = 0; Homu < E; Homu++) {
        int i,j,c;
        cin >> i >> j >> c;
        adj[i].push_back(vector<int>{j,c,Homu});   // Record in i's neighbors
        adj[j].push_back(vector<int>{i,c,Homu});   // Record in j's neighbors
    }

    // seen[i] = True if `i` is in the tree and False otherwise
    vector<bool> seen(V,false);
    // par[i] = parent of vertex i if it exists and -1 otherwise
    vector<int> par(V,-1);
    // Priority queue of edges to traverse while running the algo
    priority_queue<Edge> pq;



    // Initialize Anti-Prim's
    seen[0] = true;
    for (vector<int> e : adj[0]) {
        pq.push(Edge(0,e[0],e[1]));
    }
    // While destination not reached and there's more edges to explore
    while (!seen[V-1] && !pq.empty()) {
        Edge next = pq.top();
        pq.pop();
        int v_0 = next.v_0;   // Start of the edge
        int v_1 = next.v_1;   // End of the edge
        // Capacity is not actually used here, handled by the priority queue
        // int cap = next.cap;   // Capacity of the edge

        // If v_1 has already been seen, then skip
        if (seen[v_1]) {
            continue;
        }

        // If not, add this edge
        seen[v_1] = true;   // Mark as seen
        par[v_1] = v_0;     // Record v_1's parent

        // And toss in all edges incident to v_1 that lead to a new vertex
        for (vector<int> e : adj[v_1]) {
            int v_x = e[0];   // Next vertex
            int c_x = e[1];   // Next capacity

            // If v_x has not yet been seen, then toss into the pq
            if (!seen[v_x]) {
                pq.push(Edge(v_1,v_x,c_x));
            }
        }
    }


    
    // Backtrack to find the max cap path to `V-1`
    vector<int> path({V-1});
    int prev = -1;
    int next = V-1;
    while (true) {
        prev = next;
        next = par[prev];
        if (next == -1) {
            break;
        } else {
            path.push_back(next);
        }
    }

    // List of edges to close
    vector<int> ans;

    // Close edges around the endpoints
    for (vector<int> e : adj[0]) {
        if (e[0] != path[path.size()-2]) {   // If not along path
            ans.push_back(e[2]);             // Add to ans
        }
    }
    for (vector<int> e : adj[V-1]) {
        if (e[0] != path[1]) {     // If not along path
            ans.push_back(e[2]);   // Add to ans
        }
    }
    // Close edges around the middle
    for (int i = 1; i < path.size()-1; i++) {
        // {DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // cout << i << endl;
        // DEBUG} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        int v = path[i];
        for (vector<int> e : adj[v]) {
            // If not along path on both sides
            if ((e[0] != path[i-1]) && (e[0] != path[i+1])) {
                ans.push_back(e[2]);   // Add to ans
            }
        }
    }

    // Corner case of no road to close
    if (ans.size() == 0) {        // If no road to close
        cout << "none" << endl;   // Print `none`
        return 0;                 // And stop
    }

    // Print the ans
    sort(ans.begin(),ans.end());
    int prev_1 = -1;
    for (int Homu : ans) {
        // Prevent duplicates
        if (Homu != prev_1) {
            cout << Homu << " ";
            prev_1 = Homu;
        }
    }
    cout << endl;


    // {DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Check `adj` - Seems good
    // for (int i = 0; i < V; i++) {
    //  cout << "Edges incident to vertex " << i << endl;
    //  for (vector<int> e : adj[i]) {
    //      cout << "Neighbor = " << e[0];
    //      cout << ", cap = " << e[1];
    //      cout << ", index = " << e[2] << endl;
    //  }
    //  cout << endl;
    // }

    // Check `path` - Seems good
    // for (int Homu : path) {
    //  cout << Homu << " ";
    // }
    // DEBUG} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    
    return 0;
}










// RECYCLING BIN ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // // {DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // cout << seen[V-1] << " " << !seen[V-1] << endl;
    // // DEBUG} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // // If `V-1` isn't reachable, then print `none` and stop
    // if (!seen[V-1]) {
    //  cout << "none" << endl;
    //  return 0;
    // }
