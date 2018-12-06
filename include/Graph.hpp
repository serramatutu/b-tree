#ifndef GRAPH_INCLUDED
#define GRAPH_INCLUDED

#include <map>
#include <string>
#include "SparseMatrix.hpp"

class Graph {
    private:
        std::map<std::string, int> names;
        SparseMatrix<float> matrix;

        bool nameExists(std::string name) const;

    public:
        Graph();

        void addVertex(std::string name);
        void removeVertex(std::string name);
        void addEdge(std::string from, std::string to, float weight);
        void removeEdge(std::string from, std::string to);
        float cost(std::string from, std::string to) const;

        friend std::ostream& operator<<(std::ostream& os, const Graph& g) {
            for (const auto& kv : g.names)
                os << kv.first << ": " << kv.second << std::endl;
            os << g.matrix;
        }
};

#endif