#include <iostream>
#include <string>

#include "SparseMatrix.hpp"

using namespace std;

int main() {
    SparseMatrix<int> m(0, 10, 10);

    while (true) {
        string s;
        cout << "op num | e" << endl;

        int x, y, val;
        char op;
        cin >> op;
        if (op == 'e')
            return 0;

        cin >> x >> y >> val;
        if (op == 'i')
            m.at(x, y) = val;
        else
            cout << "type in a valid operation" << endl;
        cout << m << endl;
    }
}