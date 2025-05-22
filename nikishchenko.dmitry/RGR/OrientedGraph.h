#ifndef ORIENTED_GRAPH
#define ORIENTED_GRAPH

#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <algorithm>
#include <stack>
#include <fstream>

class OrientedGraph {
protected:
    std::map<int, std::vector<int>> adjacencyList_;

    bool depthFirstSearch(int current, int target, std::unordered_map<int, bool>& visited);
    void outDFS(int current, std::unordered_map<int, bool>& visited);
    void topologicalSortUtil(int node, std::unordered_map<int, bool>& visited, std::stack<int>& stack);

public:
    OrientedGraph() = default;
    OrientedGraph(const OrientedGraph& graph) = default;
    OrientedGraph(OrientedGraph&& graph) noexcept = default;

    OrientedGraph& operator=(const OrientedGraph& graph) = default;
    OrientedGraph& operator=(OrientedGraph&& graph) noexcept = default;
    friend std::ostream& operator<<(std::ostream& out, const OrientedGraph& graph);

    bool insert(int val);
    bool deleteVal(int val);
    virtual bool addPath(int start, int end);
    virtual bool removePath(int start, int end);
    bool find(int val) const;
    virtual void exportToDot(const std::string& filename) const;
    void walkInDepth(int start);
    std::vector<int> topologicalSort();
};

#endif
