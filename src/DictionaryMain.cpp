#include <iostream>
#include <string>

#include "Dictionary.hpp"

using namespace std;

int main() {
    Dictionary<int, int> d;
    
    while (true) {
        string s;
        cout << "op num | e" << endl;

        int key, value;
        char op;
        cin >> op;
        if (op == 'e')
            return 0;

        cin >> key >> value;
        if (op == 'i')
            d.insert(key, value);
        else if (op == 'r')
            d.remove(key);
        else
            cout << "type in a valid operation" << endl;
        cout << d << endl;
    }
}