#include <iostream>
#include <string>

#include "AVLTree.hpp"

using namespace std;

int main() {
    AVLTree<int> t(0);
    
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
        // else if (op == 'r')
        //     t.remove(num);
        else
            cout << "type in a valid operation" << endl;
        cout << t << endl;
        cout << t.height() << endl;
    }
}