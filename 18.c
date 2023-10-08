
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct SF SF;
struct SF {
    char pair;
    union {
        struct { SF *a, *b; };
        int n;
    };
};

SF *new(void) {
    SF *result = (SF *)malloc(sizeof(SF));
    memset(result, 0, sizeof(SF));
    return result;
}

SF *pair(SF *a, SF *b) {
    SF *result = new();
    result->pair = 1;
    result->a = a;
    result->b = b;
    return result;
}

SF *regular(int n) {
    SF *result = new();
    result ->n = n;
    return result;
}

SF *parse_at(char *str, int *pat) {
    SF *result = new();
    int at = *pat;

    if (str[at] == '[') {
        result->pair = 1;
        at++;
        result->a = parse_at(str, &at);
        assert(str[at] == ',');
        at++;
        result->b = parse_at(str, &at);
        assert(str[at] == ']');
        at++;
    } else {
        assert(isdigit(str[at]));
        result->pair = 0;
        result->n = str[at++] - '0';
    }
    
    *pat = at;

    return result;
}

SF *parse(char *s) {
    int at = 0;
    return parse_at(s, &at);
}

void print(SF *sf) {
    if (sf->pair) {
        putchar('[');
        print(sf->a);
        putchar(',');
        print(sf->b);
        putchar(']');
    } else {
        printf("%d", sf->n);
    }
}

SF *should_explode(SF *sf, int depth) {
    if (!sf->pair) return 0;
    if (depth == 4) return sf;

    depth++;
    
    SF *a = should_explode(sf->a, depth);
    if (a) return a;
    
    return should_explode(sf->b, depth);
}

static SF *flattened[4096];

int flatten(SF *sf, int count) {
    if (!sf->pair) {
        assert(count < 4096);
        flattened[count++] = sf;
        return count;
    }

    count = flatten(sf->a, count);
    return flatten(sf->b, count);
}

void explode(SF *sf, SF *p) {
    assert(p->pair);
    int count = flatten(sf, 0);
    for (int i = 0; i < count; i++) {
        if (flattened[i] == p->a) {
            int left = i-1;
            if (left >= 0)
                flattened[left]->n += p->a->n;
            int right = i+2;
            if (right < count)
                flattened[right]->n += p->b->n;
            break;
        }
    }
    p->pair = 0;
    p->n = 0;
}

void split(SF *p) {
    assert(!p->pair);
    int n = p->n;
    
    p->pair = 1;
    p->a = regular(n /2);
    p->b = regular(n / 2 + (n % 2));
}

SF *should_split(SF *sf) {
    if (sf->pair) {
        SF *a = should_split(sf->a);
        if (a) return a;
        return should_split(sf->b);
    }
    if (sf->n >= 10) return sf;
    return 0;
}

void reduce(SF *sf) {
    for (;;) {
        SF *p;
        if (p = should_explode(sf, 0)) {
            explode(sf, p);
            continue;
        }
        if (p = should_split(sf)) {
            split(p);
            continue;
        }
        break;
    }
}

int magnitude(SF *sf) {
    if (!sf->pair) return sf->n;
    return 3*magnitude(sf->a) + 2*magnitude(sf->b);
}

SF *clone(SF *sf) {
    if (!sf->pair) return regular(sf->n);
    return pair(clone(sf->a), clone(sf->b));
}

SF *add(SF *a, SF *b) {
    SF *sum = pair(a, b);
    reduce(sum);
    return sum;
}

int main(void) {
    char line[4096];
    fgets(line, sizeof(line), stdin);

    SF *numbers[4096];
    int count = 0;
    
    SF *sum = parse(line);
    numbers[count++] = clone(sum);
    while (fgets(line, sizeof(line), stdin)) {
        SF *a = sum;
        SF *b = parse(line);
        
        sum = pair(a, b);
        
        numbers[count++] = clone(b);
        
        reduce(sum);
        //print(sum);
        //putchar('\n');
    }
    
    printf("%d\n", magnitude(sum));

    int max = 0;
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++) {
            if (i == j) continue;
            SF *sum = pair(clone(numbers[i]), clone(numbers[j]));
            reduce(sum);
            int mag = magnitude(sum);
            if (mag > max) max = mag;
        }
    }
    printf("%d\n", max);
}
