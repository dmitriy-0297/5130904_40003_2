#include "OrientedGraph.h"


bool OrientedGraph::depthFirstSearch(int current, int target, std::unordered_map<int, bool>& visited)
{
    if (current == target)
    {
        return true;
    }
    visited[current] = true;
    for (int neighbor : adjacencyList_[current])
    {
        if (!visited[neighbor] && depthFirstSearch(neighbor, target, visited))
        {
            return true;
        }
    }

    return false;
}

bool OrientedGraph::insert(int val)
{
    if (find(val))
    {
        return false;
    }
    adjacencyList_[val] = std::vector<int>();
    return true;
}

bool OrientedGraph::deleteVal(int val)
{
    if (!find(val))
    {
        std::cerr << "ERROR: Value not found.\n";
        return false;
    }

    for (auto& pair : adjacencyList_)
    {
        auto& vec = pair.second;
        vec.erase(remove(vec.begin(), vec.end(), val), vec.end());
    }
    adjacencyList_.erase(val);
    return true;
}

bool OrientedGraph::addPath(int start, int end)
{
    if (!find(start) || !find(end))
    {
        std::cerr << "ERROR: Node not found.\n";
        return false;
    }
    std::vector<int>& vec = adjacencyList_[start];
    if (std::find(vec.cbegin(), vec.cend(), end) != vec.end())
    {
        std::cerr << "ERROR: Path already exists.\n";
        return false;
    }

    std::unordered_map<int, bool> visited;
    if (depthFirstSearch(end, start, visited) || start == end)
    {
        std::cerr << "ERROR: Cycle detected.\n";
        return false;
    }
    vec.push_back(end);
    return true;
}

bool OrientedGraph::removePath(int start, int end)
{
    if (!find(start) || !find(end))
    {
        return false;
    }

    auto& vec = adjacencyList_[start];
    auto it = remove(vec.begin(), vec.end(), end);
    if (it == vec.end())
    {
        return false;
    }

    vec.erase(it, vec.end());
    return true;
}

bool OrientedGraph::find(int val) const
{
    return adjacencyList_.find(val) != adjacencyList_.end();
}

void OrientedGraph::exportToDot(const std::string& filename) const
{
    std::ofstream file(filename);
    file << "digraph G {\n";
    for (const auto& pair : adjacencyList_)
    {
        int from = pair.first;
        for (int to : pair.second)
        {
            file << "  " << from << " -> " << to << ";\n";
        }
    }
    file << "}\n";
}

void OrientedGraph::walkInDepth(int start)
{
    if (!find(start))
    {
        std::cerr << "ERROR: Start node not found.\n";
        return;
    }

    std::unordered_map<int, bool> visited;
    outDFS(start, visited);
    std::cout << '\n';
}

void OrientedGraph::outDFS(int current, std::unordered_map<int, bool>& visited)
{
    visited[current] = true;
    std::cout << current << " ";


    std::for_each(adjacencyList_[current].begin(), adjacencyList_[current].end(),
        [&visited, this](int neighbor) {
            if (!visited[neighbor])
            {
                outDFS(neighbor, visited);
            }
        });
}

std::vector<int> OrientedGraph::topologicalSort()
{
    std::stack<int> stack;
    std::unordered_map<int, bool> visited;
    for (const auto& pair : adjacencyList_)
    {
        if (!visited[pair.first])
        {
            topologicalSortUtil(pair.first, visited, stack);
        }
    }
    std::vector<int> result;
    while (!stack.empty())
    {
        result.push_back(stack.top());
        stack.pop();
    }
    return result;
}

void OrientedGraph::topologicalSortUtil(int node, std::unordered_map<int, bool>& visited, std::stack<int>& stack)
{
    visited[node] = true;
    std::for_each(adjacencyList_[node].begin(), adjacencyList_[node].end(),
        [&visited, &stack, this](int neighbour) {
            if (!visited[neighbour]) {
                topologicalSortUtil(neighbour, visited, stack);
            }
        });

    stack.push(node);
}

std::ostream& operator<<(std::ostream& out, const OrientedGraph& graph)
{
    for (const auto& pair : graph.adjacencyList_)
    {
        out << pair.first << " -> ";
        for (int val : pair.second)
        {
            out << val << " ";
        }
        out << '\n';
    }
    return out;
}
