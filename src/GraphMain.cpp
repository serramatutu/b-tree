#include <iostream>
#include <string>
#include <stdexcept>

#include "Graph.hpp"

using namespace std;

int main() {
    Graph g;
        while (true) {
            try {
                string s;
                cout << "op num | e" << endl;

                string a, b;
                float val;
                char op;
                cin >> op;
                if (op == 'e')
                    return 0;

                cin >> a >> b >> val;
                if (op == 'I')
                    g.addVertex(a);
                else if (op == 'R')
                    g.removeVertex(a);
                else if (op == 'i')
                    g.addEdge(a, b, val);
                else if (op == 'r')
                    g.removeEdge(a, b);
                else if (op == 'c')
                    cout << "cost: " << g.cost(a, b) << std::endl;
                else
                    cout << "type in a valid operation" << endl;
                cout << g << endl;
            }
            catch (exception e) {
                cout << e.what() << endl;
                cin.get();
            }
        }
};