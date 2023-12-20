/* Idea
Obviously loop back and forth on shortest edge possible
But determining when to migrate to a new edge (cause the global min might 
not yet be accessible so you have to loop on the current local min) is a bit 
of a ??? moment... maybe I'm brainfarting


2023-08-24
Idea from a bit of dicussion the other day:
    You don't need to know the exact time you leave the looping local min 
    cheap edge to go to the next, you just need to know how many extra times 
    you have to stay on it.
    Then you can just dp in terms of cost of prev node

(Everything is 0-indexed again cause 1-indexing is crinj)
You can only exist at even gates at even times

Wait need to check I heard the ans might overflo
2 <= N <= 10^5 gates
C_i <= 10^6 cost/road
T_i <= 10^6 opening time for gate i

Worst case would be around 10^6 * 10^6 = 10^12
int is 2^31 ~ 2x10^9
long long is 2^63 probably which is more than enough
Make everything long long then to be safe

Alg
- Process the wait times to be true wait times
  - Make sure it's nondecreasing
  - Make it st the parity of the gate and the parity of its opening time match
    (actually maybe you can instead think of it as "earliest possible passage" 
    rather than opening time)
- Prefix array of min weight of first i roads
- Haha dp go brr
    ans[n] = ans[n-1] + time_diff * cheapest among first n roads
  where ans[n] = answer for going through n roads/getting to gate n
*/

/* DEBUG LOG
1.) Did the parity check wrongly
        (next-i)%2 != 0
    instead of
        (next-i-1)%2 != 0
    since we're looking at gate i+1, not gate i
*/

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <bitset>
#include <algorithm>
#define ll LoveLive
#define LoveLive long long
using namespace std;

int main() {
    ll n;   // Number of gates
    cin >> n;
    // c[i] = cost of going from gate `i` to gate `i+1`
    vector<ll> c(n-1,0);
    // t_scam[i] = time when gate `i` opens
    vector<ll> t_scam(n,0);
    for (ll i = 0; i < n-1; i++) {
        cin >> c[i];
    }
    for (ll i = 0; i < n; i++) {
        cin >> t_scam[i];
    }

    // pre_c[i] = cheapest cost among first `i+1` roads
    // pre_c[0] = c[0] just as a placeholder
    vector<ll> pre_c(n,0);
    pre_c[0] = c[0];
    for (ll i = 0; i < n-1; i++) {
        pre_c[i+1] = min(c[i],pre_c[i]);
    }

    // t[i] = earliest possible time to go past gate `i`
    vector<ll> t(n,0);
    for (ll i = 0; i < n-1; i++) {
        ll next = t_scam[i+1];     // Get next opening time
        next = max(t[i]+1,next);   // Max it with prev passing time + 1
        // If the parity doesn't match, += 1 to make it match
        if ((next-i-1)%2 != 0) {
            next += 1;
        }
        t[i+1] = next;
    }

    // ans[i] = cheapest possible cost to exit beyond gate i
    vector<ll> ans(n,0);
    for (ll i = 1; i < n; i++) {
        // ans[i] = ans[i-1] + cost from gate i-1 to i
        ans[i] = ans[i-1] + c[i-1];
        // + ("time diff bet passing gate i-1 and i" - 1)
        // * cheapest road among first i roads
        ans[i] += (t[i]-t[i-1]-1) * pre_c[i];
    }

    // Print the ans
    cout << ans[n-1] << endl;



    // {DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // // Check `ans`
    // for (ll i : ans) {
    //     cout << i << " ";
    // }
    // cout << endl;
    // // Check `t`
    // for (ll i : t) {
    //     cout << i << " ";
    // }
    // cout << endl;
    // DEBUG} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    return 0;
}
