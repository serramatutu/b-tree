#include <iostream>
#include <string>

#include "Tree.hpp"

using namespace std;

int main() {
    int n;
    cout << "digite o tamanho de cada nó da árvore" << endl;
    cin >> n;
    Tree<int> t(n);
    
    while (true) {
        string s;
        cout << "digite um valor inteiro a ser inserido na árvore ou 'sair'" << endl;
        cin >> s;

        int num;
        try {
            num = stoi(s);
        }
        catch (invalid_argument e) {
            if (s != "sair") {
                cout << "digite um valor inteiro válido" << endl;
                continue;
            }
            else
                return 0;
        }

        t.insert(num);
        cout << t << endl;
    }
}