#include <iostream> 
#include <list>
#include <vector>
#include <iterator> 
#include <fstream>
#include <string>
#include <algorithm> 
#include <cmath>
#include <bitset>
#include <map>
using namespace std;

struct Combo {
	long long num;
	long long w;
	long long c;
	long long best;
};

vector<Combo>::const_iterator binarySearch(const vector<Combo>& container, long long element)
{
	const vector<Combo>::const_iterator endIt = end(container) - 1;

	vector<Combo>::const_iterator left = begin(container);
	vector<Combo>::const_iterator right = endIt;

	if (container.size() == 0
		|| container.front().w < element)
	{
		return left;
	}

	if (container.back().w > element)
	{
		return right;
	}

	while (distance(left, right) > 0) {
		const vector<Combo>::const_iterator mid = left + distance(left, right) / 2;

		if (element >= (*mid).w) {
			right = mid;
		}
		else {
			left = mid + 1;
		}
	}

	if ((*right).w <= element) {
		return left;
	}

	return endIt;
}

long long const MAXN = 40;
long long n, W;
long long p[MAXN], w[MAXN];
vector<long long> out;

bool knapsack_sorter(const Combo& lhs, const Combo& rhs)
{
	return lhs.w < rhs.w;
}

bool knapsack_sorter_value(long long rhs, const Combo& lhs)
{
	return lhs.w > rhs;
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
	cin >> n >> W;
	for (long long i = 0; i < n; i++)
	{
		cin >> w[i] >> p[i];
	}

	long long sn = n / 2;
	long long fn = n - sn;
	long long first_size = pow(2, sn) - 1;
	long long second_size = pow(2, fn) - 1;
	vector<Combo> first_vec;
	//Combo *first = new Combo[first_size];
	bitset<MAXN> mask;
	long long i, j;
	for (i = 0; i <= first_size; i++)
	{
		bitset<MAXN> mask(i);
		Combo new_combo;
		new_combo.num = i;
		new_combo.w = 0;
		new_combo.c = 0;
		for (j = 0; j < sn; j++)
		{
			if (mask.test(j))
			{
				new_combo.w += w[j];
				new_combo.c += p[j];
			}
		}
		first_vec.push_back(new_combo);
	}

	sort(first_vec.begin(), first_vec.end(), &knapsack_sorter);
	//vector <Combo>::iterator mit1, mit2;

	long long current_first_part_best_index = 0;
	for (i = 0; i <= first_size; i++) {
		if (first_vec[i].w > W)
			break;

		if (first_vec[i].c > first_vec[current_first_part_best_index].c) {
			current_first_part_best_index = i;
		}

		first_vec[i].best = current_first_part_best_index;
	}
	/*
	for (mit1 = first_vec.begin(); mit1 != first_vec.end(); )
	{
		bool skip = false;
		for (mit2 = mit1 + 1; mit2 != first_vec.end(); mit2++)
		{
			if ((*mit2).w <= (*mit1).w && (*mit2).c >= (*mit1).c)
			{
				mit1 = first_vec.erase(mit1);
				skip = true;
				break;
			}
		}
		if (!skip) mit1++;
	}
	*/
	Combo best_combo;
	//Combo second_best;
	best_combo.num = 0;
	best_combo.w = 0;
	best_combo.c = 0;
	map <long long, long long> second_vec;
	//vector<long long> second_vec;
	//second_best.num = 0;
	//second_best.w = 0;
	//second_best.c = 0;
	for (i = 0; i <= second_size; i++)
	{
		bitset<MAXN> mask(i);
		Combo curr_combo;
		curr_combo.num = i;
		curr_combo.w = 0;
		curr_combo.c = 0;
		for (j = 0; j < fn; j++)
			if (mask.test(j))
			{
				curr_combo.w += w[j + sn];
				curr_combo.c += p[j + sn];
			}
		//vector <Combo>::const_iterator candidate;
		//candidate = binarySearch(first_vec, W - curr_combo.w);

		std::map<long long, long long>::iterator mit = second_vec.find(curr_combo.w);
		if (mit != second_vec.end() && curr_combo.c <= mit->second && curr_combo.w != 0)
			continue;
		auto first_best = upper_bound(first_vec.begin(), first_vec.end(), W - curr_combo.w, knapsack_sorter_value);
		if (first_best != first_vec.begin()) 
			first_best -= 1;
		Combo candidate = first_vec[(*first_best).best];
		if (candidate.w <= W - curr_combo.w && candidate.c + curr_combo.c > best_combo.c)
		{
			best_combo.num = candidate.num + i * pow(2, sn);
			best_combo.w = curr_combo.w + candidate.w;
			best_combo.c = curr_combo.c + candidate.c;
		}
		/*
		second_best.w = curr_combo.w;
		second_best.c = curr_combo.c;
		second_best.num = curr_combo.num;
		*/
		if(mit == second_vec.end())
			second_vec.insert(pair<long long, long long>(curr_combo.w, curr_combo.c));
		else
			mit->second = curr_combo.c;
	}
	bitset<MAXN> answer(best_combo.num);
	for (j = 0; j < MAXN; j++)
		if (answer.test(j))
		{
			out.push_back(j + 1);
		}
	cout << out.size() << endl;
	copy(out.begin(), out.end(), ostream_iterator<long long>(cout, " "));
	return 0;

}