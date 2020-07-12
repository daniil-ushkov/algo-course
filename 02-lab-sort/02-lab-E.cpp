#include <iostream>
#include <vector>

using namespace std;

int n, m, k;
vector<string> v = {};


void mergeSort(int f, int l, int r) {
	if (l + 1 == r)
		return;
	int mid = (l + r)/2;
	mergeSort(f, l, mid);
	mergeSort(f, mid, r);
	vector<string> mergedV;
	int i = l;
	int j = mid;
	while (i < mid && j < r) {
		if (v[i][m - 1 - f] <= v[j][m - 1 - f]) {
			mergedV.push_back(v[i]);
			i++;
		} else {
			mergedV.push_back(v[j]);
			j++;
		}
	}
	while (i < mid) {
		mergedV.push_back(v[i]);
		i++;
	}
	while (j < r) {
		mergedV.push_back(v[j]);
		j++;
	}
	for (int k = 0; k < r - l; k++) {
		v[k + l] = mergedV[k];
	}
}

int main() {
	cin >> n >> m >> k;
	string in;
	for (int i = 0; i < n; i++) {
		cin >> in;
		v.push_back(in);
	}
	for (int i = 0; i < k; i++) {
		mergeSort(i, 0, n);
	}
	for (auto el : v) {
		cout << el << "\n";
	}
	return 0;
}
