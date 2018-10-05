#include <iostream>
#include <string>

#include "AVLTree.hpp"

using namespace std;

int main() {
    AVLTree<int> t;
    
    while (true) {
        string s;
        cout << "op num | e" << endl;

        int num;
        char op;
        cin >> op;
        if (op == 'e')
            return 0;

        cin >> num;
        if (op == 'i')
            t.insert(num);
        else if (op == 'r')
            t.remove(num);
        else if (op == 'p') {
            for (auto it = t.begin(); it!=t.end(); it++)
                cout << *it << " ";
            cout << endl;
        }
        else
            cout << "type in a valid operation" << endl;
        cout << t << endl;
        cout << "height: " << t.height() << endl;
    }
}