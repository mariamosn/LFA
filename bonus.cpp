#include <iostream>
#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h>

#define EST 1
#define NORD 2
#define VEST 4
#define SUD 8

using namespace std;

int main() {
    // numărul de linii
    int l;
    // numărul de coloane
    int c;
    // numărul de roboți
    int r;
    // numărul de ieșiri
    int o;

    int **lab;
    vector<int> rob;
    vector<int> out;

    // preluare input

    scanf("%d%d%d%d", &l, &c, &r, &o);
    
    lab = (int **)malloc(l * sizeof(int *));
    for (int i = 0; i < l; i++) {
        lab[i] = (int *)malloc(c * sizeof(int));
    }

    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            scanf("%d", &lab[i][j]);
        }
    }

    for (int i = 0; i < r; i++) {
        int li, ci;
        scanf("%d%d", &li, &ci);
        rob.push_back(li * c + ci);
    }

    for (int i = 0; i < o; i++) {
        int lq, cq;
        scanf("%d%d", &lq, &cq);
        out.push_back(lq * c + cq);
    }

    // afișare codificare

    // n = l * c, m = 4(E, N, V, S), s = r, f = o
    printf("%d %d %d %d\n", l * c, 4, r, o);

    // tranz
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            int crt = i * c + j;
            // est
            if (lab[i][j] & EST || j == c - 1) {
                printf("%d ", crt);
            } else {
                printf("%d ", i * c + j + 1);
            }

            // nord
            if (lab[i][j] & NORD || i == 0) {
                printf("%d ", crt);
            } else {
                printf("%d ", (i - 1) * c + j);
            }

            // vest
            if (lab[i][j] & VEST || j == 0) {
                printf("%d ", crt);
            } else {
                printf("%d ", i * c + j - 1);
            }

            // sud
            if (lab[i][j] & SUD || i == l - 1) {
                printf("%d ", crt);
            } else {
                printf("%d ", (i + 1) * c + j);
            }

            printf("\n");
        }
    }

    // stari
    for (int i = 0; i < r; i++) {
        printf("%d ", rob[i]);
    }
    printf("\n");

    // fin
    for (int i = 0; i < o; i++) {
        printf("%d ", out[i]);
    }
    printf("\n");

    // eliberarea memoriei
    for (int i = 0; i < l; i++) {
        free(lab[i]);
    }
    free(lab);

    return 0;
}