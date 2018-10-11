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
            d[key] = value;
        else if (op == 'r')
            d.remove(key);
        // else if (op == 'p') {
        //     for (std::pair<int, int> p : d)
        //         cout << p.first << " " << p.second;
        //     cout << std::endl;
        // }
        else if (op = 'f')
            cout << d[key] << endl;
        else
            cout << "type in a valid operation" << endl;
        cout << d << endl;
    }
}