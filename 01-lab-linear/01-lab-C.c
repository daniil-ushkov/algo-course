#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct Vector {
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

int main() {

	Vector vIn = newVector();
	Vector vOut = newVector();

	int n;
	scanf("%d", &n);

	for (int i = 0; i < n; i++) {

		char sign;
		scanf("\n");
		scanf("%c", &sign);

		if (sign == '+') {
			int value;
			scanf("%d", &value);
			push_back(&vIn, value);
		} else if (sign == '-') {
			if (vOut.size == 0) {
				while (vIn.size != 0)
					push_back(&vOut, pop_back(&vIn));
			}
			printf("%d\n", pop_back(&vOut));
		}
	}

	return 0;
} 