#include <iostream>
#include <vector>

using namespace std;

int n, m, k;
vector<vector<pair<int, int>>> net;
int* toArray(int array) {
	int* answer = new int[n];
	for (int i = 0; i < n; i++) {
		if ((array & (1 << i)) != 0) {
			answer[i] = 1;
		} else {
			answer[i] = 0;
		}
	}
	return answer;
}
void sort(int* &array) {
	for (auto layer : net) {
		for (auto comp : layer) {
			if (array[comp.first] > array[comp.second]) {
				swap(array[comp.first], array[comp.second]);
			}
		}
	}
}
bool isItSorted(int* array) {
	for (int i = 1; i < n; i++) {
		if (array[i - 1] > array[i]) {
			return false;
		}
	}
	return true;
}
bool check() {
	for (int mask = 0; mask < (1 << n); mask++) {
		int* array = toArray(mask);
		sort(array);
		if (!isItSorted(array)) {
			return false;
		}
	}
	return true;
}
int main() {
	cin >> n >> m >> k;
	int size, f, s;
	for (int i = 0; i < k; i++) {
		net.push_back(vector<pair<int, int>>());
		cin >> size;
		for (int j = 0; j < size; j++) {
			cin >> f >> s;
			net.back().push_back({min(f - 1, s - 1), max(f - 1, s - 1)});
		}
	}
	if (check()) {
		cout << "Yes";
	} else {
		cout << "No";
	}
	return 0;
}
