#include <iostream>
#include <stdio.h>
#include <string.h>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <bits/stdc++.h>

using namespace std;

struct pair_hash {
    inline std::size_t operator()(const std::pair<int,int> & v) const {
        return v.first*31+v.second;
    }
};

unordered_map<int, unordered_map<int, unordered_set<int>>> go_back;

unordered_map<pair<int, int>, pair<int, pair<int, int>>, pair_hash> merge_aux;

void read_input(int &n, int &m, int &s, int &f, int** &tranz, int* &stari,
                int* &fin) {

    scanf("%d%d%d%d", &n, &m, &s, &f);
    tranz = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        tranz[i] = (int *)malloc(m * sizeof(int));
    }
    stari = (int *)malloc(s * sizeof(int));
    fin = (int *)malloc(f * sizeof(int));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%d", &tranz[i][j]);
        }
    }

    for (int i = 0; i < s; i++) {
        scanf("%d", &stari[i]);
    }

    for (int i = 0; i < f; i++) {
        scanf("%d", &fin[i]);
    }
}

vector<int> accessible(int n, int m, int **tranz, int s, int *stari,
                        bool print) {
    vector<int> acc(n, 0);
    queue<int> q;

    for (int i = 0; i < s; i++) {
        q.push(stari[i]);
        acc[stari[i]] = 1;
    }
    
    while (!q.empty()) {
        int crt = q.front();
        q.pop();
        for (int i = 0; i < m; i++) {
            int next = tranz[crt][i];
            if (acc[next] == 0) {
                acc[next] = 1;
                q.push(next);
            }
        }
    }

    if (print) {
        for (int i = 0; i < n; i++) {
            if (acc[i]) {
                printf("%d\n", i);
            }
        }
    }

    return acc;
}

vector<int> productive(int n, int m, int **tranz, int f, int *fin, bool print) {
    vector<int> prod(n, 0);
    queue<int> q;

    for (int i = 0; i < f; i++) {
        q.push(fin[i]);
        prod[fin[i]] = 1;
    }
    
    while (!q.empty()) {
        int crt = q.front();
        q.pop();
        for (int i = 0; i < n; i++) {
            if (prod[i] == 0) {
                for (int j = 0; j < m; j++) {
                    if (tranz[i][j] == crt) {
                        q.push(i);
                        prod[i] = 1;
                    }
                }
            }
        }
    }

    if (print) {
        for (int i = 0; i < n; i++) {
            if (prod[i]) {
                printf("%d\n", i);
            }
        }
    }

    return prod;
}

void useful(int n, int m, int **tranz, int s, int *stari, int f, int *fin) {
    vector<int> acc = accessible(n, m, tranz, s, stari, false);
    vector<int> prod = productive(n, m, tranz, f, fin, false);

    for (int i = 0; i < n; i++) {
        if (acc[i] == 1 && prod[i] == 1) {
            printf("%d\n", i);
        }
    }
}

void compute_go_back(int n, int m, int **tranz) {
    for (int crt_state = 0; crt_state < n; crt_state++) {
        unordered_map<int, unordered_set<int>> ht;
        for (int sym = 0; sym < m; sym++) {
            unordered_set<int> prev_states;
            for (int i = 0; i < n; i++) {
                if (tranz[i][sym] == crt_state) {
                    prev_states.insert(i);
                }
            }
            ht[sym] = prev_states;
            
        }
        go_back[crt_state] = ht;
    }
}

void compute_merge_aux(int n, int m, unordered_set<int> F) {
    queue<pair<int, int>> q;
    if (F.size() == 0) {
        for (int i = 0; i < n; i++) {
            q.push(make_pair(i, i));
            merge_aux[make_pair(i, i)] = make_pair(-1, make_pair(i, i));
        }
    } else {
        for (auto it = F.begin(); it != F.end(); it++) {
            int i = *it;
            q.push(make_pair(i, i));
            merge_aux[make_pair(i, i)] = make_pair(-1, make_pair(i, i));
        }
    }

    while (!q.empty()) {
        int s0 = q.front().first;
        int t0 = q.front().second;
        q.pop();

        for (int sym = 0; sym < m; sym++) {
            unordered_set<int> prev_s0 = go_back[s0][sym];
            unordered_set<int> prev_t0 = go_back[t0][sym];

            for (auto s = prev_s0.begin(); s != prev_s0.end(); s++) {
                for (auto t = prev_t0.begin(); t != prev_t0.end(); t++) {
                    int prev_s = *s, prev_t = *t;
                    if (prev_s > prev_t) {
                        int aux = prev_s;
                        prev_s = prev_t;
                        prev_t = aux;
                    }

                    if (merge_aux.find(make_pair(prev_s, prev_t)) ==
                            merge_aux.end()) {
                        q.push(make_pair(prev_s, prev_t));
                        merge_aux[make_pair(prev_s, prev_t)] =
                            make_pair(sym, make_pair(s0, t0));
                    }
                }
            }
        }
    }
}

vector<int> merging_seq3_0(int s0, int t0, int &merging_state) {
    if (s0 > t0) {
        int aux = s0;
        s0 = t0;
        t0 = aux;
    }
    vector<int> v;

    while (merge_aux[make_pair(s0, t0)].first != -1) {
        pair<int, int> p = make_pair(s0, t0);
        v.push_back(merge_aux[p].first);
        s0 = merge_aux[p].second.first;
        t0 = merge_aux[p].second.second;
    }

    merging_state = s0;
    return v;

}

void synchronize(int n, int m, int **tranz, int s, int *stari, int f,
                            int *fin) {
    vector<int> x;
    unordered_set<int> S;
    unordered_set<int> F;

    compute_go_back(n, m, tranz);

    for (int i = 0; i < f; i++) {
        F.insert(fin[i]);
    }

    if (s == 0) {
        for (int i = 0; i < n; i++) {
            S.insert(i);
        }
    } else {
        for (int i = 0; i < s; i++) {
            S.insert(stari[i]);
        }
    }

    compute_merge_aux(n, m, F);

    while (S.size() > 1) {
        int s0 = *(S.begin());
        S.erase(s0);
        int t0 = *(S.begin());
        S.erase(t0);

        int merging_state;
        vector<int> y = merging_seq3_0(s0, t0, merging_state);
        if (y.size() == 0) {
            exit(-1);
        }

        x.insert(x.end(), y.begin(), y.end());

        unordered_set<int> new_S;
        for (auto it = S.begin(); it != S.end(); it++) {
            int crt = *it;
            
            for (int i = 0; i < (int) y.size(); i++) {
                crt = tranz[crt][y[i]];
            }
            
            new_S.insert(crt);
        }
        S = new_S;

        S.insert(merging_state);
    }

    for (int i = 0; i < (int) x.size(); i++) {
        printf("%d ", x[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./main <problem>\n");
        return -1;
    }

    // preluare input

    // n = numărul de stări
    int n;
    // m = numărul de simboluri
    int m;
    // s = numărul de stări de pornire
    int s;
    // f = numărul de stări finale
    int f;
    // tranz[i][j] = starea în care se ajunge,
    //               pornind din starea i, cu simbolul j la intrare
    int **tranz;
    // stările de pornire
    int *stari;
    // strările finale
    int *fin;

    read_input(n, m, s, f, tranz, stari, fin);

    // rezolvarea problemei

    if (strcmp(argv[1], "accessible") == 0) {
        accessible(n, m, tranz, s, stari, true);

    } else if (strcmp(argv[1], "productive") == 0) {
        productive(n, m, tranz, f, fin, true);

    } else if (strcmp(argv[1], "useful") == 0) {
        useful(n, m, tranz, s, stari, f, fin);

    } else if (strcmp(argv[1], "synchronize") == 0) {
        synchronize(n, m, tranz, s, stari, f, fin);

    }

    // eliberarea memoriei
    for (int i = 0; i < n; i++) {
        free(tranz[i]);
    }
    free(tranz);
    free(stari);
    free(fin);

    return 0;
}