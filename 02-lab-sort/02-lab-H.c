#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int* begin;
    int size;
    int capacity;
} Vector;

Vector newVector() {
    Vector v;
    v.begin = malloc(sizeof(int));
    v.size = 0;
    v.capacity = 1;
    return v;
}

void deleteVecor(Vector *v) {
	free(v -> begin);
}

void extend(Vector *v) {
    int* newBegin = malloc(2 * (v -> capacity) * sizeof(int));
    for (int i = 0; i < (v -> size); i++)
        *(newBegin + i) = *((v -> begin) + i);
    free(v -> begin);
    (v -> begin) = newBegin;
    (v -> capacity) = 2 * (v -> capacity);
}

void shrink(Vector *v) {
	int* newBegin = malloc(((v -> capacity) / 2) * sizeof(int));
	for (int i = 0; i < (v -> size); i++)
		*(newBegin + i) = *((v -> begin) + i);
	free(v -> begin);
	(v -> begin) = newBegin;
	(v -> capacity) = (v -> capacity) / 2;
}

void push_back(Vector *v, int value) {
    *((v -> begin) + (v -> size)) = value;
    (v -> size)++;

    if (v -> size == v -> capacity) {
        extend(v);
    }
}

int pop_back(Vector *v) {
	(v -> size)--;

	int deleted = *((v -> begin) + (v -> size));

	if (4 * v -> size == v -> capacity) {
		shrink(v);
	}

	return deleted;
}

int get(Vector *v, int i) {
	return *(v -> begin + i);
}

void set(Vector *v, int i, int value) {
	*(v -> begin + i) = value;
}

void swap(Vector *v, int i, int j) {
	int t = get(v, i);
	set(v, i, get(v, j));
	set(v, j, t);
}

void print(Vector *v) {
	for (int i = 0; i < v -> size; i++) {
		printf("%d ", get(v, i));
	}
	printf("\n");
}

int a, b;
unsigned int cur = 0;
unsigned int nextRand24() {
	cur = cur * a + b;
	return cur >> 8;
}

long long countInversions(Vector *v, int l, int r) {
	if (l + 1 == r)
		return 0;

	int m = (l + r)/2;

	long long result = 0;

	result += countInversions(v, l, m);
	result += countInversions(v, m, r);

	Vector mergedV = newVector();
	int i = l;
	int j = m;
	while (i < m && j < r) {
		if (get(v, i) <= get(v, j)) {
			push_back(&mergedV, get(v, i));
			i++;
		} else {
			push_back(&mergedV, get(v, j));
			j++;
			result += m - i;
		}
	}
	while (i < m) {
		push_back(&mergedV, get(v, i));
		i++;
	}
	while (j < r) {
		push_back(&mergedV, get(v, j));
		j++;
	}
	for (int k = 0; k < r - l; k++) {
		set(v, k + l, get(&mergedV, k));
	}

	//printf("%d, %d, %d: ", l, m, r);
	//print(&mergedV);

	deleteVecor(&mergedV);

	return result;
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	scanf("%d %d", &a, &b);
	Vector v = newVector();
	for (int i = 0; i < n; i++) {
		push_back(&v, nextRand24() % m);
		//push_back(&v, n - i);
	}

	printf("%lli", countInversions(&v, 0, n));
	return 0;
} 