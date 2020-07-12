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


int main() {
	
	List *lIn = NULL;
	List *lOut = NULL;

	int n;
	scanf("%d", &n);

	for (int i = 0; i < n; i++) {

		char sign;
		scanf("\n");
		scanf("%c", &sign);

		if (sign == '+') {
			int value;
			scanf("%d", &value);
			push(&lIn, value);
		} else if (sign == '-') {
			if (lOut == NULL) {
				while (lIn != NULL)
					push(&lOut, pop(&lIn));
			}
			printf("%d\n", pop(&lOut));
		}
	}

	return 0;
} 