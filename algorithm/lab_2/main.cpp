#include <bits/stdc++.h>

using namespace std;

const int N = 600;
const int MAX_M = (N) * (N - 1) / 2;
const int MAX_W = 5000;
int d[N][N], d2[N][N];

class brg{ // build_random_graph
public:
    class DSU{ // disjoint sets union
    public:
      int p[N];
      DSU(){
          for(int i = 0; i < N; i++) p[i] = i;
      }
      int get(int a){
          if(p[a] != a) p[a] = get(p[a]);
          return p[a];
      }
      void un(int a, int b){
          a = get(a);
          b = get(b);
          if(a > b) swap(a, b);
          p[b] = a;
      }


    };
    DSU s;
    vector <vector <pair <int, int>>> e;
    int e2[N][N]{};
    brg(int n, int m) { // build graph with m edges and (n>=) connectivity components
        if(m < N - n || m > (N - n + 1) * (N - n) / 2){
            cout << "[-] Graph with " << n << " connectivity components, " << N << " verticles, " << m << " edges does not exist!\n";
            return;
        }
        for (int i = 0; i < N; i++) {
            vector<pair<int, int>> t;
            e.push_back(t);
        }
        int sz = N;
        int step = 7;
        int a = 0;
        int b = step;
        for (int i = m; i > 0;) {
            b += step;
            if(b >= N){
                a += 3;
                b -= N;
                if(a > N) a-= N;
            }

            if(a == b) continue; // не петля
            if (e2[a][b]) continue; // не кратное ребро
            if (s.get(a) != s.get(b) && sz == n) continue; // не уменьшат кол - во компонент так как уже минимум
            //cout << i << " " << sz << endl;
            if (s.get(a) != s.get(b)) sz--;

            int w = rand() % MAX_W + 1;
            s.un(a, b);
            e[a].push_back({b, w});
            e[b].push_back({a, w});
            e2[a][b] = w;
            e2[b][a] = w;
            i--;
        }
    }
};

class heap{
public:
    pair <int, int> q[50000];
    int n = 0;
    bool empty(){
        return (n == 0);
    }
    void push(pair <int, int> a){
        q[n++] = a;
        int i = n - 1;
        while(i > 0 && q[i].first < q[(i - 1) / 2].first){
            swap(q[i],q[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }
    pair <int, int> top(){
        return q[0];
    }
    void pop(){
        if(n == 1){
            n = 0;
            return;
        }
        swap(q[0], q[--n]);
        int i = 0;
        while(i * 2 + 1 < n){
            int j = i * 2 + 1;
            if(i * 2 + 2 < n && q[j].first > q[i * 2 + 2].first){
                j = i * 2 + 2;
            }
            if(q[i].first > q[j].first) {
                swap(q[i], q[j]);
                i = j;
            } else break;
        }
    }
};

class solution{
public:
    void djikstra_queue(brg &a){ // O(nlogn)
        for(int i = 0; i < N; i++) {
            priority_queue <pair <int, int>> q;
            vector <int> v(N, 0);
            vector <int> dist(N, N * MAX_W + 1);
            dist[i] = 0;
            // cout << "new start node " << i << endl;
            q.push({0, i});
            while(!q.empty()){

                int u = q.top().second;
                // cout << u << endl;
                q.pop();
                if(v[u]) continue;
                v[u] = 1;
                //   cout << "find from " << u << endl;
                for(auto [to, w] : a.e[u]){
                    if(v[to]) continue;
                    dist[to] = min(dist[to], dist[u] + w);
                    q.push({-dist[to], to});
                }
            }
            for(int k = 0; k < N; k++){
                d[i][k] = dist[k];
            }
        }

    };
    void djikstra_heap(brg &a){
        for(int i = 0; i < N; i++) {
            heap q;
            vector <int> v(N, 0);
            vector <int> dist(N, N * MAX_W + 1);
            dist[i] = 0;
            // cout << "new start node " << i << endl;
            q.push({0, i});
            while(!q.empty()){

                int u = q.top().second;
                // cout << u << endl;
                q.pop();
                if(v[u]) continue;
                v[u] = 1;
                //   cout << "find from " << u << endl;
                for(auto [to, w] : a.e[u]){
                    if(v[to]) continue;
                    dist[to] = min(dist[to], dist[u] + w);
                    q.push({dist[to], to});
                }
            }
            for(int k = 0; k < N; k++){
                d2[i][k] = dist[k];
            }
        }
    };
};


int main() {

    // константы
    const int exp = 100; // колво эксперементов
    const int edges_count = N * 4; // кол - во ребер
    const int n = 1; // кол - во компонент связности

    int all = 0;
    int nmin = 1e5;
    int nmax = 0;
    for(int i = 0; i < exp; i++){
        // создаем граф
        if(i % 10 == 0) cout << i << endl;
        brg a(n, edges_count);

        // запускаем
        int start =  clock();
        solution s;
        // выбрать через что считать деисктру
        //s.djikstra_queue(a);
        s.djikstra_heap(a);
        int end = clock();
        end -= start;
        all += end;
        nmin = min(end, nmin);
        nmax = max(nmax, end);
    }
    cout << "Min time is " << nmin << "ms.\n";
    cout << "Average time is " << all / exp << "ms.\n";
    cout << "Max time is " << nmax << "ms.";
    return 0;
}
