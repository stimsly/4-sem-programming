#include <iostream>
#include <fstream>
#include<vector>

using namespace std;

typedef long double ld;
string ress = "";


// 2 - ppm last 64 bit
// 3 - history ppm
// history - cyclic
// 64 bit - cyclic
class models{
public:
    vector <ld> probability_models[4];
    vector <ld> cyclic_model[8];
    vector <ld> cyclic_model_group[8];

    models(){
        for(int i = 0; i < 4; i++) probability_models[i].clear();
        for(int i = 0; i < 8; i++) cyclic_model[i].clear();
        for(int i = 0; i < 8; i++) cyclic_model_group[i].clear();

        for(int i = 0; i < 4; i++) probability_models[i].resize(2);
        for(int i = 0; i < 8; i++) cyclic_model[i].resize(2);
        for(int i = 0; i < 8; i++) cyclic_model_group[i].resize(2);
        probability_models[0][0] = probability_models[0][1] = 1;
    }

    void add(int x, int pos){
        add_model1(x);
        add_model2(x);
        add_model3(x);
        add_model4(x, pos);
        add_model5(x, pos);
    }
    void add_model1(int x){
        probability_models[1][x]++;
    }
    void add_model2(int x){
        if(probability_models[2][x] < 255)probability_models[2][x]++;
        if(probability_models[2][1-x] > 2) (probability_models[2][1-x] /= 2) += 1;
    }
    void add_model3(int x){
        if(probability_models[3][x] < 255)probability_models[3][x]++;
        if(probability_models[3][1-x] > 2) (probability_models[3][1-x] /= 2) += 1;
    }
    void add_model4(int x, int pos){
        pos %= 8;
        cyclic_model[pos][x]++;
    }
    void add_model5(int x, int pos){
        pos %= 8;
        cyclic_model_group[pos][x]++;
    }

    void pop(int x, int pos){
        pop_model2(x);
        pop_model5(x, pos);
    }
    void pop_model2(int x){
        probability_models[2][x] = max(int(probability_models[2][x] - 1), 1);
    }
    void pop_model5(int x, int pos){
        pos %= 8;
        cyclic_model_group[pos][x]--;
    }


    ld get_model2(){
        if(probability_models[2][0] + probability_models[2][1] == 0) return .5;
        return probability_models[2][0] / (probability_models[2][0] + probability_models[2][1]);
    }
    ld get_model3(){
        if(probability_models[3][0] + probability_models[3][1] == 0) return .5;
        return probability_models[3][0] / (probability_models[3][0] + probability_models[3][1]);
    }
    ld get_model4(int pos){
        pos %= 8;
        if(cyclic_model[pos][0] + cyclic_model[pos][1] == 0) return .5;
        return cyclic_model[pos][0] / (cyclic_model[pos][0] + cyclic_model[pos][1]);
    }
    ld get_model5(int pos){
        pos %= 8;
        if(cyclic_model_group[pos][0] + cyclic_model_group[pos][1] == 0) return .5;
        return cyclic_model_group[pos][0] / (cyclic_model_group[pos][0] + cyclic_model_group[pos][1]);
    }

    void clear(){
        for(int i = 0; i < 4; i++) probability_models[i].clear();
        for(int i = 0; i < 8; i++) cyclic_model[i].clear();
        for(int i = 0; i < 8; i++) cyclic_model_group[i].clear();

        for(int i = 0; i < 4; i++) probability_models[i].resize(2);
        for(int i = 0; i < 8; i++) cyclic_model[i].resize(2);
        for(int i = 0; i < 8; i++) cyclic_model_group[i].resize(2);
        probability_models[0][0] = probability_models[0][1] = 1;
    }
};

vector <ld> calc(models p, ld n, int pos){
    ///              0       1     2    3   4   5
    vector <ld> w = {0.05, 0.25, 0.1, 0.25, 0.25, 0.1};
    ld n0 = 0;
    n0 += (p.probability_models[0][0] / 2.0) * w[0];
    n0 += (p.probability_models[1][0] / n) * w[1];
    n0 += p.get_model2() * w[2];
    n0 += p.get_model3() * w[3];
    n0 += p.get_model4(pos) * w[4];
    n0 += p.get_model5(pos) * w[5];
    return {n0, 1 - n0};
}
string s2 = "";
vector <ld> a2[3], b2[3];
vector <pair <int, int>> a3, b3;
class code {
public:
    ld arithmeticCoding(models p, string s) {
        ld left = 0;
        ld right = 1;
        for (int i = 0; i < s.size(); i++) {
            int symb = s[i] - '0';
            vector <ld> probability = calc(p, s.size(), i);
            ld newLeft = left;
            ld newRight = right;
            if(!symb) newRight = left + (right - left) * probability[0];
            else newLeft = left + (right - left) * probability[0];
            left = newLeft;
            right = newRight;
            s2 += s[i];
            p.add(symb, i);
            if(s2.size() >= 48){
                p.pop(s2[s2.size() - 48] - '0',s2.size());
            }
        }
        return (left + right) / 2;
    }
};

class decode{
public:
    void arithmeticDecoding(models p, ld c, int n) {
        for(int i = 0; i < n; i++) {
            vector <ld> probability = calc(p, n, i);
            if(c <= probability[0]){
                ress += '0';
                c = (c) / (probability[0]);
            } else {
                ress += '1';
                c = (c - probability[0]) / (1. - probability[0]);
            }
            p.add(ress.back() - '0', i);
            if(ress.size() >= 48){
                p.pop(ress[ress.size() - 48] - '0',ress.size());
            }
        }
    }
};


int main() {
    ofstream out("input.txt");
    const int len = 1e4;
    for(int i = 0; i < len; i++){
        out << rand() % 2;
    }
    out.close();
    ifstream cin("input.txt");

    string s = "";
    cin >> s;
    int n = s.size();
    int group = 48;
    models p;
    vector <ld> c;
    for(int i = 0; i < n; i += group){
        string ss = "";
        for(int j = i; j < min(i + group, n); j++) ss += s[j];
        code a;
        ld ans = a.arithmeticCoding(p, ss);
        long long t = ans * 10000000000;
        ld ans2 = t * 1.0 / 10000000000;
        c.push_back(ans2);
    }
    p.clear();
    int j = 0;
    for(int i = 0; i < c.size(); i++){
        j = min(n, j + group);
        decode b;
        b.arithmeticDecoding(p, c[i], j - i * group);
    }
    if(ress == s) cout << "Equal";
    else {
        for(int i = 0; i < n; i++){
            if(s[i] != ress[i]){
                cout << "Equal at first " << i << " symbols" << endl;
                break;
            }
        }
    }
    return 0;
}
