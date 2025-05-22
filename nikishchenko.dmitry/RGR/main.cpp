#include "OrientedGraph.h"
#include "UnorientedGraph.h"
#include <sstream>
#include <utility>

const std::string INVALID_INPUT = "ERROR: nodes must be integers!\n";

void saveGraph(OrientedGraph& graph, const std::string& filename);
void test_1_insert_delete_node(OrientedGraph& graph);
void test_2_add_remove_path(OrientedGraph& graph);
void test_3_topological_sort(OrientedGraph& graph);
void test_4_unoriented_graph_insert_delete_node(UnorientedGraph& graph);
void test_5_unoriented_graph_add_remove_path(UnorientedGraph& graph);
void test_6_unoriented_and_oriented_graph_walk_in_depth(
    OrientedGraph& ograph,
    UnorientedGraph& ugraph
);


int main()
{
    OrientedGraph graph;
    UnorientedGraph ugraph;

    test_1_insert_delete_node(graph);
    test_2_add_remove_path(graph);
    test_3_topological_sort(graph);
    test_4_unoriented_graph_insert_delete_node(ugraph);
    test_5_unoriented_graph_add_remove_path(ugraph);
    test_6_unoriented_and_oriented_graph_walk_in_depth(graph, ugraph);

    return EXIT_SUCCESS;
}

void saveGraph(OrientedGraph& graph, const std::string& filename)
{
    graph.exportToDot(filename);
    std::cout << "Graph saved to " << filename << "!\n";
}

void test_1_insert_delete_node(OrientedGraph& graph)
{
    std::cout << "--------------------------------------------------------\n"
        << "Test 1: Insert/Delete Nodes (Oriented Graph)\n\n"
        << "Enter values to add: ";

    std::string input;
    std::getline(std::cin, input);
    std::istringstream iss_add(input);

    std::for_each(
        std::istream_iterator<int>(iss_add),
        std::istream_iterator<int>(),
        [&graph](int val) { graph.insert(val); }
    );

    if (iss_add.fail() && !iss_add.eof())
    {
        std::cerr << INVALID_INPUT;
        exit(EXIT_FAILURE);
    }

    std::cout << "\nCurrent Graph:\n" << graph << "\n";

    std::cout << "Enter values to delete: ";
    std::getline(std::cin, input);
    std::istringstream iss_del(input);

    std::for_each(
        std::istream_iterator<int>(iss_del),
        std::istream_iterator<int>(),
        [&graph](int val) { graph.deleteVal(val); }
    );

    if (iss_del.fail() && !iss_del.eof())
    {
        std::cerr << INVALID_INPUT;
        exit(EXIT_FAILURE);
    }

    std::cout << "\nGraph after deletion:\n" << graph
        << "\n--------------------------------------------------------\n";
}


void test_2_add_remove_path(OrientedGraph& graph)
{
    std::cout << "\n--------------------------------------------------------\n"
        << "Test 2: Add/Remove Paths (Oriented Graph)\n\n"
        << "Enter paths to add (start end): ";

    std::string input;
    std::getline(std::cin, input);
    std::istringstream iss_add(input);

    std::vector<int> temp;
    std::copy(
        std::istream_iterator<int>(iss_add),
        std::istream_iterator<int>(),
        std::back_inserter(temp)
    );

    if (temp.size() % 2 != 0)
    {
        std::cerr << "ERROR: Odd number of nodes!\n";
        exit(EXIT_FAILURE);
    }

    std::vector<std::pair<int, int>> paths_add;
    for (auto it = temp.begin(); it != temp.end();)
    {
        int start = *it++;
        int end = *it++;
        paths_add.emplace_back(start, end);
    }

    std::for_each(
        paths_add.begin(),
        paths_add.end(),
        [&graph](const auto& pair) {
            graph.addPath(pair.first, pair.second);
        }
    );

    std::cout << "\nGraph after adding paths:\n" << graph << "\n";

    std::cout << "Enter paths to remove (start end): ";
    std::getline(std::cin, input);
    std::istringstream iss_remove(input);

    temp.clear();
    std::copy(
        std::istream_iterator<int>(iss_remove),
        std::istream_iterator<int>(),
        std::back_inserter(temp)
    );

    if (temp.size() % 2 != 0)
    {
        std::cerr << "ERROR: Odd number of nodes!\n";
        exit(EXIT_FAILURE);
    }

    std::vector<std::pair<int, int>> paths_remove;
    for (auto it = temp.begin(); it != temp.end();)
    {
        int start = *it++;
        int end = *it++;
        paths_remove.emplace_back(start, end);
    }

    std::for_each(
        paths_remove.begin(),
        paths_remove.end(),
        [&graph](const auto& pair)
        {
            graph.removePath(pair.first, pair.second);
        }
    );

    std::cout << "\nGraph after removing paths:\n" << graph
        << "\n--------------------------------------------------------\n";
}


void test_3_topological_sort(OrientedGraph& graph)
{
    std::cout << "\n--------------------------------------------------------\n"
        << "Test 3: Topological Sort\n\n"
        << "Graph before sort:\n" << graph << "\n";

    auto sorted = graph.topologicalSort();

    std::cout << "Topological order: ";
    std::copy(
        sorted.begin(),
        sorted.end(),
        std::ostream_iterator<int>(std::cout, " ")
    );

    std::cout << "\n\n--------------------------------------------------------\n";
}


void test_4_unoriented_graph_insert_delete_node(UnorientedGraph& graph)
{
    std::cout << "--------------------------------------------------------\n"
        << "Test 4: Insert/Delete Nodes (Unoriented Graph)\n\n"
        << "Enter values to add: ";

    std::string input;
    std::getline(std::cin, input);
    std::vector<int> nodes_to_add;

    std::copy(
        std::istream_iterator<int>(std::istringstream(input) >> std::ws),
        std::istream_iterator<int>(),
        std::back_inserter(nodes_to_add)
    );

    std::for_each(nodes_to_add.begin(), nodes_to_add.end(),
        [&graph](int val) { graph.insert(val); }
    );

    std::cout << "\nCurrent Graph:\n" << graph << "\n";

    std::cout << "Enter values to delete: ";
    std::getline(std::cin, input);

    std::vector<int> nodes_to_remove;
    std::remove_copy_if(
        std::istream_iterator<int>(std::istringstream(input) >> std::ws),
        std::istream_iterator<int>(),
        std::back_inserter(nodes_to_remove),
        [](int x) { return false; }
    );

    std::for_each(nodes_to_remove.begin(), nodes_to_remove.end(),
        [&graph](int val) { graph.deleteVal(val); }
    );

    std::cout << "\nGraph after deletion:\n" << graph
        << "\n--------------------------------------------------------\n";
}

void test_5_unoriented_graph_add_remove_path(UnorientedGraph& graph)
{
    std::cout << "\n--------------------------------------------------------\n"
        << "Test 5: Add/Remove Paths (Unoriented Graph)\n\n"
        << "Enter pairs to connect (start end): ";

    std::string input;
    std::getline(std::cin, input);
    std::vector<int> raw_values;

    std::copy(
        std::istream_iterator<int>(std::istringstream(input) >> std::ws),
        std::istream_iterator<int>(),
        std::back_inserter(raw_values)
    );

    if (raw_values.size() % 2 != 0) {
        std::cerr << "ERROR: Odd number of nodes!\n";
        exit(EXIT_FAILURE);
    }

    std::vector<std::pair<int, int>> connections;
    auto it = raw_values.begin();
    while (it != raw_values.end())
    {
        int start = *it++;
        int end = *it++;
        connections.emplace_back(start, end);
    }

    std::for_each(connections.begin(), connections.end(),
        [&graph](const auto& p) { graph.addPath(p.first, p.second); }
    );

    std::cout << "\nGraph after adding paths:\n" << graph << "\n";

    std::cout << "Enter pairs to disconnect: ";
    std::getline(std::cin, input);

    std::vector<int> raw_values_remove;
    std::copy(
        std::istream_iterator<int>(std::istringstream(input) >> std::ws),
        std::istream_iterator<int>(),
        std::back_inserter(raw_values_remove)
    );

    if (raw_values_remove.size() % 2 != 0)
    {
        std::cerr << "ERROR: Odd number of nodes!\n";
        exit(EXIT_FAILURE);
    }

    std::vector<std::pair<int, int>> disconnections;
    auto it_remove = raw_values_remove.begin();
    std::generate_n(
        std::back_inserter(disconnections),
        raw_values_remove.size() / 2,
        [&it_remove]() {
            return std::make_pair(*it_remove++, *it_remove++);
        }
    );

    std::for_each(disconnections.begin(), disconnections.end(),
        [&graph](const auto& p) { graph.removePath(p.first, p.second); }
    );

    std::cout << "\nGraph after removing paths:\n" << graph
        << "\n--------------------------------------------------------\n";
}

void test_6_unoriented_and_oriented_graph_walk_in_depth(
    OrientedGraph& ograph,
    UnorientedGraph& ugraph
)
{
    std::cout << "\n--------------------------------------------------------\n"
        << "Test 6: Depth-First Walk Comparison\n\n";

    int start_orient;
    std::cout << "Enter start node for ORIENTED graph: ";
    std::cin >> start_orient;
    std::cin.ignore();

    std::cout << "Oriented Graph DFS: ";
    ograph.walkInDepth(start_orient);

    int start_unorient;
    std::cout << "\nEnter start node for UNORIENTED graph: ";
    std::cin >> start_unorient;
    std::cin.ignore();

    std::cout << "Unoriented Graph DFS: ";
    ugraph.walkInDepth(start_unorient);

    std::cout << "\n--------------------------------------------------------\n";
}
