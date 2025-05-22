#include "UnorientedGraph.h"

bool UnorientedGraph::addPath(int start, int end)
{
    if (OrientedGraph::addPath(start, end))
    {
        adjacencyList_[end].push_back(start);
        return true;
    }
    return false;
}

bool UnorientedGraph::removePath(int start, int end)
{
    if (OrientedGraph::removePath(start, end))
    {
        adjacencyList_[end].push_back(start);
        return true;
    }
    return false;
}

void UnorientedGraph::exportToDot(const std::string& filename) const
{
    std::ofstream file(filename);
    file << "graph G {\n";

    std::set<std::pair<int, int>> unique_edges;

    for (const auto& node_pair : adjacencyList_)
    {
        int from = node_pair.first;
        for (int to : node_pair.second)
        {
            auto edge = std::minmax(from, to);
            unique_edges.insert(edge);
        }
    }

    std::for_each(unique_edges.begin(), unique_edges.end(),
        [&file](const auto& edge)
        {
            file << "  " << edge.first << " -- " << edge.second << ";\n";
        }
    );

    file << "}\n";
}

std::ostream& operator<<(std::ostream& out, const UnorientedGraph& graph)
{
    for (const auto& pair : graph.adjacencyList_)
    {
        out << pair.first << " -> ";
        for (int val : pair.second)
        {
            out << val << " ";
        }
        std::cout << '\n';
    }

    return out;
}
