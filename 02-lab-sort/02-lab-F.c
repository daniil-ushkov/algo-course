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

int partion(Vector *v, int l, int r) {

	swap(v, rand() % (r - l + 1) + l, r);

	int pivot = get(v, r);
	int i = l - 1;
	for (int j = l; j < r; j++) {
		int val = get(v, j);
		if (val < pivot) {
			swap(v, ++i, j);
		}
		if (val == pivot && rand() % 2 == 0) {
			swap(v, ++i, j);
		}
	}
	swap(v, ++i, r);
	return i;
}

void quickSort(Vector *v, int l, int r) {
	
	if (l < r) {
		int m = partion(v, l, r);
		quickSort(v, l, m - 1);
		quickSort(v, m + 1, r);
	}
}

int k;

void antiQuickSort(Vector *v) {
	for (int i = 1; i < v -> size; i++) {
		swap(v, i/2, i);
	}
}



int main() {
	int n;
	scanf("%d", &n);
	k = n;
	Vector v = newVector();
	for (int i = 1; i <= n; i++) {
		push_back(&v, i);
	}
	antiQuickSort(&v);
	print(&v);
	return 0;
} 