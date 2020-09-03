#include <iostream> 
#include <list>
#include <vector>
#include <iterator> 
#include <fstream>
#include <string>
#include <algorithm> 
#include <cmath>
using namespace std;

//long long const INF = 1000000001;
long long const MAXN = 200000;

long long n, q;
vector<long long> t[4 * MAXN];
long long arr[MAXN];
vector<long long> out;

/*
auto inrange(long long low, long long high) {
	// This captures a copy of threshold
	return [=](long long value) {
		return (value >= low && value <= high);
	};
};
*/

void build(long long a[], long long v, long long tl, long long tr) {
	if (tl == tr)
		t[v] = vector<long long>(1, a[tl]);
	else {
		long long tm = (tl + tr) / 2;
		build(a, v * 2, tl, tm);
		build(a, v * 2 + 1, tm + 1, tr);
		merge(t[v * 2].begin(), t[v * 2].end(), t[v * 2 + 1].begin(), t[v * 2 + 1].end(),
			back_inserter(t[v]));
	}
}

int query(long long v, long long tl, long long tr, long long l, long long r, long long x, long long y) {
	if (l > r)
		return 0;
	if (l == tl && tr == r) {
		vector<long long>::iterator lower = lower_bound(t[v].begin(), t[v].end(), x);
		vector<long long>::iterator upper = upper_bound(t[v].begin(), t[v].end(), y);
//		auto count = count_if(t[v].begin(), t[v].end(), inrange(x, y));
		return upper - lower;
	}
	long long tm = (tl + tr) / 2;
	return query(v * 2, tl, tm, l, min(r, tm), x, y) + query(v * 2 + 1, tm + 1, tr, max(l, tm + 1), r, x, y);
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
	long long j = 0, type, l, r, x, y;
	cin >> n >> q;
	//cout << "n:" << n << "q:" << q << endl;
	for (long long i = 0; i < n; i++)
	{
		cin >> arr[i];
	}
	build(arr, 1, 0, n - 1);
	while (j < q) {
		cin >> l >> r >> x >> y;
		l -= 1;
		r -= 1;
		out.push_back(query(1, 0, n - 1, l, r, x, y));
		j++;
	}
	copy(out.begin(), out.end(), ostream_iterator<long long>(cout, "\n"));
	return 0;

}