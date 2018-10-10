#include <iostream>
#include <string>
#include <stdexcept>

#include "SparseMatrix.hpp"

using namespace std;

int main() {
    SparseMatrix<int> m(0, 10, 10);
    try {
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
                m[x][y] = val;
            else if (op == 'a')
                cout << m[x][y] << endl;
            else
                cout << "type in a valid operation" << endl;
            cout << m << endl;
        }
    }
    catch (exception e) {
        cout << e.what() << endl;
        cin.get();
    }
};