#include <iostream>
#include <stdlib.h>
#include <malloc.h>

using namespace std;

typedef struct Vector {
    int* begin;
    int size;
    int capacity;
} Vector;

Vector newVector() {
    Vector v;
    v.begin = (int*) malloc(sizeof(int*));
    v.size = 0;
    v.capacity = 1;
    return v;
}

void extend(Vector *v) {
    int* newBegin = (int*) malloc(2 * (v -> capacity) * sizeof(int*));
    for (int i = 0; i < (v -> size); i++)
        *(newBegin + i) = *((v -> begin) + i);
    free(v -> begin);
    (v -> begin) = newBegin;
    (v -> capacity) = 2 * (v -> capacity);
}
void shrink(Vector *v) {
    int* newBegin = (int*) malloc(((v -> capacity) / 2) * sizeof(int));
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
    if (v -> size == 0)
        return 0;
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

int back(Vector *v) {
    if (v -> size == 0)
        return 0;
    return *(v -> begin + v -> size - 1);
}

int main() {
    string str;
    while (cin >> str) {
        size_t l = str.size();
        int r = 0, s = 0;
        Vector minR = newVector();
        Vector minS = newVector();
        bool flag = true;
        for (size_t i = 0; i < l; i++) {
            if (str[i] == '(') {
                r++;
                push_back(&minS, s);
            }
            if (str[i] == ')') {
                r--;
                pop_back(&minS);
                if (r < back(&minR)) {
                    flag = false;
                }
            }
            if (str[i] == '[') {
                s++;
                push_back(&minR, r);
            }
            if (str[i] == ']') {
                s--;
                pop_back(&minR);
                if (s < back(&minS)) {
                    flag = false;
                }
            }
        }
        if (r != 0 || s != 0)
            flag = false;
        if (flag) {
            printf("YES\n");
        } else {
            printf("NO\n");
        }
    }
    return 0;
}
