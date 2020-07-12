#include <stdio.h>

using namespace std;

double A;
int n;

double next(double h1, double h2) {
	return 2*h2 - h1 + 2;
}

bool check(double h2) {
	double h1 = A;
	for (int i = 3; i <= n; i++) {
		double h3 = next(h1, h2);
		if (h3 < 0) {
			return 0;
		}
		h1 = h2;
		h2 = h3;
	}
	return 1;
}

double calc(double h2) {
	double h1 = A, h3 = 0;
	for (int i = 3; i <= n; i++) {
		h3 = next(h1, h2);
		h1 = h2;
		h2 = h3;
	}
	return h3;
}

int main() {
	scanf("%d %lf", &n, &A);
	double l = 0, r = A;
	while (r - l > 0.00001) {
		double m = (l + r)/2;
		if (check(m)) {
			r = m;
		} else {
			l = m;
		}
	}
	printf("%.2f", calc(r));
	return 0;
}
