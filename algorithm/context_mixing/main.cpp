#include <iostream>
#include <fstream>
#include<vector>
#include <iomanip>

using namespace std;

typedef long double ld;
string ress = "";


/// 2 - ppm last 64 bit
/// 3 - history ppm
/// history - cyclic
/// 64 bit - cyclic
class model1{
public:
    ld get(){
        return 0.5;
    }
};
class model2{
public:
    vector <ld> probaprobability;
    model2(){
        probaprobability.clear();
        probaprobability.resize(2);
    }
    ld get(){
        if(probaprobability[0] + probaprobability[1] <= 1) return .5;
        return probaprobability[0] / (probaprobability[0] + probaprobability[1]);
    }
    void add(int x){
        probaprobability[x]++;
    }
    void clear(){
        probaprobability.clear();
        probaprobability.resize(2);
    }

};
class model3{
public:
    vector <ld> probaprobability;
    model3(){
        probaprobability.clear();
        probaprobability.resize(2);
    }
    ld get(){
        if(probaprobability[0] + probaprobability[1] <= 1) return .5;
        return probaprobability[0] / (probaprobability[0] + probaprobability[1]);
    }
    void add(int x){
        if(probaprobability[x] < 255)probaprobability[x]++;
        if(probaprobability[1-x] > 2) (probaprobability[1-x] /= 2) += 1;
    }
    void pop(int x){
        probaprobability[x] = max(int(probaprobability[x] - 1), 1);
    }
    void clear(){
        probaprobability.clear();
        probaprobability.resize(2);
    }
};
class model4{
public:
    vector <ld> probaprobability;
    model4(){
        probaprobability.clear();
        probaprobability.resize(2);
    }
    void add(int x){
        if(probaprobability[x] < 255)probaprobability[x]++;
        if(probaprobability[1-x] > 2) (probaprobability[1-x] /= 2) += 1;
    }
    ld get(){
        if(probaprobability[0] + probaprobability[1] <= 1) return .5;
        return probaprobability[0] / (probaprobability[0] + probaprobability[1]);
    }
    void clear(){
        probaprobability.clear();
        probaprobability.resize(2);
    }
};
class model5{
public:
    vector <ld> cyclic_model[8];
    model5(){
        for(auto & i : cyclic_model) i.clear();
        for(auto & i : cyclic_model) i.resize(2);
    }
    void add(int x, int pos){
        pos %= 8;
        cyclic_model[pos][x]++;
    }
    ld get(int pos){
        pos %= 8;
        if(cyclic_model[pos][0] + cyclic_model[pos][1] == 0) return .5;
        return cyclic_model[pos][0] / (cyclic_model[pos][0] + cyclic_model[pos][1]);
    }
    void clear(){
        for(auto & i : cyclic_model) i.clear();
        for(auto & i : cyclic_model) i.resize(2);
    }
};
class model6{
public:
    vector <ld> cyclic_model_group[8];
    model6(){
        for(auto & i : cyclic_model_group) i.clear();
        for(auto & i : cyclic_model_group) i.resize(2);
    }
    void add(int x, int pos){
        pos %= 8;
        cyclic_model_group[pos][x]++;
    }
    void pop(int x, int pos){
        pos %= 8;
        cyclic_model_group[pos][x]--;
    }
    ld get(int pos){
        pos %= 8;
        if(cyclic_model_group[pos][0] + cyclic_model_group[pos][1] == 0) return .5;
        return cyclic_model_group[pos][0] / (cyclic_model_group[pos][0] + cyclic_model_group[pos][1]);
    }
    void clear(){
        for(auto & i : cyclic_model_group) i.clear();
        for(auto & i : cyclic_model_group) i.resize(2);
    }
};
class models{
public:
    model1 a1;
    model2 a2;
    model3 a3;
    model4 a4;
    model5 a5;
    model6 a6;
    void add(int x, int pos){
        a2.add(x);
        a3.add(x);
        a4.add(x);
        a5.add(x, pos);
        a6.add(x, pos);
    }


    void pop(int x, int pos){
        a3.pop(x);
        a6.pop(x, pos);
    }

    void clear(){
        a2.clear();
        a3.clear();
        a4.clear();
        a5.clear();
        a6.clear();
    }
    ld get(int pos){
    ///              0       1     2    3   4     5
        vector <ld> w = {0.05, 0.25, 0.1, 0.25, 0.25, 0.1};
        ld n0 = 0;
        n0 += a1.get() * w[0];
        n0 += a2.get() * w[1];
        n0 += a3.get() * w[2];
        n0 += a4.get() * w[3];
        n0 += a5.get(pos) * w[4];
        n0 += a6.get(pos) * w[5];
        return n0;
    }
};

vector<ld> calc(models p, int pos){
    ld n0 = p.get(pos);
    return {n0, 1 - n0};
}

string s2 = "";
vector <ld> a2[3], b2[3];
vector <ld> a3, b3;

class code {
public:
    ld arithmeticCoding(models p, string s) {
        ld left = 0;
        ld right = 1;
        for (int i = 0; i < s.size(); i++) {
            int symb = s[i] - '0';
            vector<ld> probability = calc(p, i);
            a3.push_back(probability[0]);
            ld newLeft = left;
            ld newRight = right;
            if (!symb) newRight = left + (right - left) * probability[0];
            else newLeft = left + (right - left) * probability[0];
            left = newLeft;
            right = newRight;
            s2 += s[i];
            p.add(symb, i);
            if (s2.size() >= 48) {
                p.pop(s2[s2.size() - 48] - '0', s2.size());
            }
        }
        return (left + right) / 2;
    }
};

class decode{
public:
    void arithmeticDecoding(models p, ld c, int n) {
        for(int i = 0; i < n; i++) {
            vector <ld> probability = calc(p, i);
            b3.push_back(probability[0]);
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
    string s = "";
    int n = 1010;
    for(int i = 0; i < n; i++){
        s += char(rand() % 2 + '0');
    }
    //cout << s << endl;
    models p;
    code a;

    int g = 48;
    ofstream fout_text("text.txt");
    fout_text << s << endl;
    ofstream fout("encode.txt");
    fout << n << endl;
    for(int i = 0; i < n; i += g){
        string cur_s = s.substr(i, min(g, int(s.size()) - i));
        ld cur = a.arithmeticCoding(p, cur_s);
        fout << fixed << setprecision(17) << cur << endl;
    }

    cout << "\nEncoded\n";
    p.clear();
    ifstream fin("encode.txt");
    fin >> n;
    decode b;
    for(int i = 0; i < (n + g - 1) / g; i++){
        ld c;
        fin >> c;
        b.arithmeticDecoding(p, c, min((i + 1) * g, int(s.size())) - i * g);
    }
    for(int i = 0; i < n; i++){

        if(a3[i] != b3[i]){
            cout << "unequal at " << i << "\n";
            cout << a3[i] << " " << b3[i] << endl;
        }
    }
    //cout << ress << endl;
    if(ress != s) {
        for(int i = 0; i < s.size(); i++){
            if(ress[i] != s[i]) {
                cout << "equal at " << i << " symb\n";
                break;
            }
        }
    } else cout << "equal";
    return 0;
}
