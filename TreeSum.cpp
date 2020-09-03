#include <iostream> 
#include <list>
#include <vector>
#include <stack>
#include <iterator> 
#include <fstream>
#include <string>
#include <algorithm> 
using namespace std;

long long const MAXN = 100000;

struct treeNode {
	long long sum_even;
	long long sum_odd;
	long long even_num;
	long long odd_num;
	long long increment;
};

long long n, q;
struct treeNode t[4 * MAXN];
long long arr[MAXN];
vector<long long> out;

void build(long long a[], long long v, long long tl, long long tr) {
	if (tl == tr)
	{
		if (a[tl] % 2 == 0)
		{
			t[v].sum_even = a[tl];
			t[v].sum_odd = 0;
			t[v].even_num = 1;
			t[v].odd_num = 0;
		}
		else
		{
			t[v].sum_even = 0;
			t[v].sum_odd = a[tl];
			t[v].even_num = 0;
			t[v].odd_num = 1;
		}
		t[v].increment = 0;
	}
	else {
		long long tm = (tl + tr) / 2;
		build(a, v * 2, tl, tm);
		build(a, v * 2 + 1, tm + 1, tr);
		t[v].sum_even = t[v * 2].sum_even + t[v * 2 + 1].sum_even;
		t[v].even_num = t[v * 2].even_num + t[v * 2 + 1].even_num;
		t[v].sum_odd = t[v * 2].sum_odd + t[v * 2 + 1].sum_odd;
		t[v].odd_num = t[v * 2].odd_num + t[v * 2 + 1].odd_num;
		t[v].increment = 0;
	}
}

void push(long long v, long long tl, long long tr) {
	if (t[v].increment != 0) 
	{

		t[v * 2].increment += t[v].increment;
		t[v * 2 + 1].increment += t[v].increment;

		if (t[v].increment % 2 == 0)
		{
			t[v * 2].sum_even = t[v * 2].sum_even + t[v * 2].even_num * t[v].increment;
			t[v * 2].sum_odd = t[v * 2].sum_odd + t[v * 2].odd_num * t[v].increment;
			t[v * 2 + 1].sum_even = t[v * 2 + 1].sum_even + t[v * 2 + 1].even_num * t[v].increment;
			t[v * 2 + 1].sum_odd = t[v * 2 + 1].sum_odd + t[v * 2 + 1].odd_num * t[v].increment;
		}
		else
		{
			swap(t[v * 2].even_num, t[v * 2].odd_num);
			swap(t[v * 2].sum_even, t[v * 2].sum_odd);
			t[v * 2].sum_even = t[v * 2].sum_even + t[v * 2].even_num * t[v].increment;
			t[v * 2].sum_odd = t[v * 2].sum_odd + t[v * 2].odd_num * t[v].increment;
			swap(t[v * 2 + 1].even_num, t[v * 2 + 1].odd_num);
			swap(t[v * 2 + 1].sum_even, t[v * 2 + 1].sum_odd);
			t[v * 2 + 1].sum_even = t[v * 2 + 1].sum_even + t[v * 2 + 1].even_num * t[v].increment;
			t[v * 2 + 1].sum_odd = t[v * 2 + 1].sum_odd + t[v * 2 + 1].odd_num * t[v].increment;
		}
		t[v].increment = 0;
	}
}

long long sum_even(long long v, long long tl, long long tr, long long l, long long r) {
	if (l > r)
		return 0;
	if (l == tl && r == tr)
		return t[v].sum_even;
	long long tm = (tl + tr) / 2;
	push(v, tl, tr);
	return sum_even(v * 2, tl, tm, l, min(r, tm))
		+ sum_even(v * 2 + 1, tm + 1, tr, max(l, tm + 1), r);
}

long long sum_odd(long long v, long long tl, long long tr, long long l, long long r) {
	if (l > r)
		return 0;
	if (l == tl && r == tr)
		return t[v].sum_odd;
	long long tm = (tl + tr) / 2;
	push(v, tl, tr);
	return sum_odd(v * 2, tl, tm, l, min(r, tm))
		+ sum_odd(v * 2 + 1, tm + 1, tr, max(l, tm + 1), r);
}

void update(long long v, long long tl, long long tr, long long pos, long long new_val) {
	if (tl == tr)
	{
		if (new_val % 2 == 0)
		{
			t[v].sum_even = new_val;
			t[v].sum_odd = 0;
			t[v].even_num = 1;
			t[v].odd_num = 0;
		}
		else
		{
			t[v].sum_even = 0;
			t[v].sum_odd = new_val;
			t[v].even_num = 0;
			t[v].odd_num = 1;
		}
		t[v].increment = 0;
	}
	else {
		long long tm = (tl + tr) / 2;
		push(v, tl, tr);

		if (pos <= tm)
		{
			update(v * 2, tl, tm, pos, new_val);			
		}
		else
		{
			update(v * 2 + 1, tm + 1, tr, pos, new_val);
		}
		t[v].sum_even = t[v * 2].sum_even + t[v * 2 + 1].sum_even;
		t[v].sum_odd = t[v * 2].sum_odd + t[v * 2 + 1].sum_odd;
		t[v].even_num = t[v * 2].even_num + t[v * 2 + 1].even_num;
		t[v].odd_num = t[v * 2].odd_num + t[v * 2 + 1].odd_num;
	}
}

void update_add(long long v, long long tl, long long tr, long long l, long long r) {
	if (l > r)
		return;
	if (l == tl && tr == r)
	{
		if (l == r) t[v].increment = 0;
		else t[v].increment += 1;

		swap(t[v].odd_num, t[v].even_num);
		swap(t[v].sum_even, t[v].sum_odd);
		t[v].sum_even += t[v].even_num;
		t[v].sum_odd += t[v].odd_num;
	}
	else {
		long long tm = (tl + tr) / 2;
		push(v, tl, tr);

		update_add(v * 2, tl, tm, l, min(r, tm));
		update_add(v * 2 + 1, tm + 1, tr, max(l, tm + 1), r);

		t[v].sum_even = t[v * 2].sum_even + t[v * 2 + 1].sum_even;
		t[v].sum_odd = t[v * 2].sum_odd + t[v * 2 + 1].sum_odd;
		t[v].even_num = t[v * 2].even_num + t[v * 2 + 1].even_num;
		t[v].odd_num = t[v * 2].odd_num + t[v * 2 + 1].odd_num;
	}
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
	long long j = 0, type, l, r;
	cin >> n >> q;
	//cout << "n:" << n << "q:" << q << endl;
	for (long long i = 0; i < n; i++)
	{
		cin >> arr[i];
	}
	build(arr, 1, 0, n - 1);
	while (j < q) {
		cin >> type >> l >> r;
		//cout << "type: " << type << "l:" <<  l << "r:" << r << endl;	
		switch (type)
		{
			case 1:
			{
				l -= 1;
				update(1, 0, n - 1, l, r);
				break;
			}
			case 2:
			{
				l -= 1;
				r -= 1;
				update_add(1, 0, n - 1, l, r);
				break;
			}
			case 3:
			{
				l -= 1;
				r -= 1;
				out.push_back(sum_even(1, 0, n - 1, l, r));
				//cout << sum_even(1, 0, n - 1, l, r) << endl;
				break;
			}
			case 4:
			{
				l -= 1;
				r -= 1;
				out.push_back(sum_odd(1, 0, n - 1, l, r));
				//cout << sum_odd(1, 0, n - 1, l, r) << endl;
				break;
			}
		}
		j++;
	}
	copy(out.begin(), out.end(), ostream_iterator<long long>(cout, "\n"));
	return 0;

}