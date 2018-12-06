#include <stdexcept>
#include "Graph.hpp"

Graph::Graph() : matrix(-1, 0, 0) 
{}

bool Graph::nameExists(std::string name) const {
    return names.count(name) > 0;
}

void Graph::addVertex(std::string name) {
    if (nameExists(name))
        throw std::invalid_argument("name already exists");

    names[name] = matrix.width();
    matrix.resize(matrix.width() + 1, matrix.height() + 1);
}

void Graph::removeVertex(std::string name) {
    if (!nameExists(name))
        throw std::invalid_argument("name does not exist");
    matrix.purgeRow(names[name]);
    matrix.purgeCol(names[name]);
    names.erase(name);
}

void Graph::addEdge(std::string from, std::string to, float weight) {
    if (!nameExists(from) || !nameExists(to))
        throw std::invalid_argument("name does not exist");

    if (from == to)
        throw std::invalid_argument("cannot go from a node to itself");

    if (weight < 0)
        throw std::invalid_argument("invalid weight");
        
    matrix[names[from]][names[to]] = weight;
}

void Graph::removeEdge(std::string from, std::string to) {
    if (!nameExists(from) || !nameExists(to))
        throw std::invalid_argument("name does not exist");

    matrix[names[from]][names[to]] = -1;
}

float Graph::cost(std::string from, std::string to) const {
    if (!nameExists(from) || !nameExists(to))
        throw std::invalid_argument("name does not exist");

    return matrix.at(names.at(from), names.at(to));
}