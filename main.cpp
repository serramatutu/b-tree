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
        cout << "op num | s" << endl;

        int num;
        char op;
        cin >> op;
        if (op == 's')
            return 0;

        cin >> num;
        if (op == 'i')
            t.insert(num);
        else if (op == 'r')
            t.remove(num);
        else
            cout << "Digite uma operação válida" << endl;
        cout << t << endl;
    }
}