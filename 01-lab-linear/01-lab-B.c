#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct List {
	int value;
	struct List* previous;
} List;


void push(struct List **l, int value) {
	List *newL = malloc(sizeof(List));
	newL -> value = value;
	newL -> previous = *l;
	*l = newL;
}

int pop(List **l) {
	if (*l == NULL)
		return 0;
	List *last = *l;
	*l = (*l) -> previous;
	int value = last -> value;
	free(last);
	return value;
}


int main()
{
	List *l = NULL;

	int n;
	scanf("%d", &n);

	for (int i = 0; i < n; i++) {
		char sign;
		scanf("\n");
		scanf("%c", &sign);

		if (sign == '+') {
			int value;
			scanf("%d", &value);
			push(&l, value);
			//printf("(%c %d)\n", sign, value);
		} else if (sign == '-') {
			//printf("(%c)\n", sign);
			printf("%d\n", l -> value);
			pop(&l);
		}
		// printf("1)\n");
		// scanf("\n");
		// printf("2)\n");
	}

	return 0;
} 