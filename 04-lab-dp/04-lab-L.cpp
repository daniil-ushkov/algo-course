#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

const int INF = 1e9;

ifstream fin("matching.in");
ofstream fout("matching.out");

int n;
vector<vector<pair<int, int>>> graph;
vector<long long> f;
vector<long long> g;
vector<long long> h;

void dfs(int v, int p) {
  for (auto to : graph[v]) {
    if (to.first != p) {
      dfs(to.first, v);
      g[v] += h[to.first];
      f[v] = max(f[v], g[to.first] + to.second - h[to.first]);
    }
  }
  f[v] += g[v];
  h[v] = max(f[v], g[v]);
}

int main() {
  fin >> n;
  graph.resize(n, vector<pair<int, int>>());
  for (int i = 0; i < n - 1; i++) {
    int u, v, weight;
    fin >> u >> v >> weight;
    graph[u - 1].push_back({v - 1, weight});
    graph[v - 1].push_back({u - 1, weight});
  }
  f.resize(n, 0);
  g.resize(n, 0);
  h.resize(n, 0);
  dfs(0, -1);
  fout << h[0];
  return 0;
}
