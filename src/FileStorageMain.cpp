#include <iostream>
#include <string>

#include "FileStorage.hpp"

using namespace std;

int main() {
    FileStorage<int> t("C:/Temp/file_storage.dat");
    
    while (true) {
        string s;
        cout << "op num | e" << endl;

        int num, index;
        char op;
        cin >> op;
        if (op == 'e')
            return 0;

        cin >> num >> index;
        if (op == 'w')
            t.write(num, index);
        else if (op == 'r')
            t.read(num);
        else if (op == 'd')
            t.remove(num);
        else
            cout << "type in a valid operation" << endl;
        cout << t << endl;
    }

    return 0;
}