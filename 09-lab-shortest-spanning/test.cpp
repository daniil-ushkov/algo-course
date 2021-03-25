#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

struct edge {
    int from;
    int to;
    ll w;
};

struct node {
    node(edge e) : d(0), e(e), delta(0), l(nullptr), r(nullptr) {}

    inline static node *merge(node *h1, node *h2) {
        push(h1);
        push(h2);
        if (h1 == nullptr) {
            return h2;
        }
        if (h2 == nullptr) {
            return h1;
        }
        if (h1->e.w > h2->e.w) {
            swap(h1, h2);
        }
        h1->r = merge(h1->r, h2);
        if ((h1->l ? h1->l->d : 0) < (h1->r ? h1->r->d : 0)) {
            swap(h1->l, h1->r);
        }
        h1->d = (h1->r ? h1->r->d : 0) + 1;
        return h1;
    }
    static void add(node *t, ll delta) {
        if (t == nullptr) {
            return;
        }
        t->delta += delta;
    }

    static void push(node *t) {
        if (t == nullptr) {
            return;
        }
        t->e.w += t->delta;
        add(t->l, t->delta);
        add(t->r, t->delta);
        t->delta = 0;
    }

    static edge min(node *h) {
        push(h);
        return h->e;
    }

    static node *extract_min(node *t) {
        push(t);
        if (t == nullptr) {
            return nullptr;
        }
        return merge(t->l, t->r);
    }

    static node *insert(node *t, edge e) {
        push(t);
        return merge(t, new node(e));
    }

    int d;
    edge e;
    ll delta;
    node *l;
    node *r;
};

int main () {
    node* t = nullptr;
    for (int i = 0; i < 300000; ++i) {
        t = node::merge(t, new node({0, 0, rand() % 10000}));
    }
    cout << "Hi";
    return 0;
}