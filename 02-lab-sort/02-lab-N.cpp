#include <iostream>
#include <vector>

using namespace std;

vector<vector<pair<int, int>>> net(20, vector<pair<int, int>>(0));
int currentLayer = 0;
void merger(int l, int r) {
	for (int i = l; i <= (l + r)/2; i++) {
		net[currentLayer].push_back({i, r + l - i});
	}
}
void bitonic(int l, int r) {
	for (int i = l; i <= (l + r)/2; i++) {
		net[currentLayer].push_back({i, i + (r - l + 1)/2});
	}
	if (l + 1 == r) {
		return;
	}
	currentLayer++;
	int m = (l + r)/2;
	bitonic(l, m);
	bitonic(m + 1, r);
	currentLayer--;
}
void createNet() {
	for (int i = 0; i <= 14; i += 2) {
		merger(i, i + 1);
	}
	while (net[currentLayer].size() != 0) {
		currentLayer++;
	}
	for (int i = 0; i <= 12; i += 4) {
		merger(i, i + 3);
		currentLayer++;
		bitonic(i, i + 1);
		bitonic(i + 2, i + 3);
		currentLayer--;
	}
	while (net[currentLayer].size() != 0) {
		currentLayer++;
	}
	for (int i = 0; i <= 8; i += 8) {
		merger(i, i + 7);
		currentLayer++;
		bitonic(i, i + 3);
		bitonic(i + 4, i + 7);
		currentLayer--;
	}
	while (net[currentLayer].size() != 0) {
		currentLayer++;
	}
	merger(0, 15);
	currentLayer++;
	bitonic(0, 7);
	bitonic(8, 15);
}
int main() {
	int n, m = 0; cin >> n;
	createNet();
	vector<vector<pair<int, int>>> myNet;
	for (auto layer : net) {
		myNet.push_back(vector<pair<int, int>>(0));
		for (auto comp : layer) {
			if (comp.first < n && comp.second < n) {
				myNet.back().push_back(comp);
				m++;
			}
		}
		if (myNet.back().size() == 0) {
			myNet.pop_back();
		}
	}
	cout << n << " " << m << " " << myNet.size() << "\n";
	for (auto layer : myNet) {
		cout << layer.size();
		for (auto comp : layer) {
			cout << " " << comp.first + 1 << " " << comp.second + 1;
		}
		cout << "\n";
	}
	return 0;
}
