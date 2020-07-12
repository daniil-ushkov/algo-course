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

void print(Vector *v) {
	for (int i = 0; i < v -> size; i++) {
		printf("%d ", get(v, i));
	}
	printf("\n");
}

void mergeSort(Vector *v, int l, int r) {
	if (l + 1 == r)
		return;

	int m = (r + l)/2;

	mergeSort(v, l, m);
	mergeSort(v, m, r);

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
}

int lower_bound(Vector *v, int value) {
	int l = -1;
	int r = v -> size;
	while (r - l > 1) {
		int m = (r + l)/2;
		if (get(v, m) < value) {
			l = m;
		} else {
			r = m;
		}
	}
	if (get(v, r) == value) {
		return r + 1;
	} else {
		return -1;
	}
}

int upper_bound(Vector *v, int value) {
	int l = -1;
	int r = v -> size;
	while (r - l > 1) {
		int m = (r + l)/2;
		if (get(v, m) <= value) {
			l = m;
		} else {
			r = m;
		}
	}
	if (get(v, l) == value) {
		return l + 1;
	} else {
		return -1;
	}
}

int main() {
    
	int n;
	scanf("%d", &n);

	Vector v = newVector();
	int in;
	for (int i = 0; i < n; i++) {
		scanf("%d", &in);
		push_back(&v, in);
	}


	scanf("%d", &n);

	for (int i = 0; i < n; i++) {
		scanf("%d", &in);
		printf("%d %d\n", lower_bound(&v, in), upper_bound(&v, in));
	}

	scanf(" ");

    return 0;
} 