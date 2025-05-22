#ifndef UNORIENTED_GRAPH
#define UNORIENTED_GRAPH

#include "OrientedGraph.h"
#include <set>

class UnorientedGraph : public OrientedGraph {
public:
    void topologicalSort() = delete;

    bool addPath(int start, int end) override;
    bool removePath(int start, int end) override;

    void exportToDot(const std::string& filename) const override;
    friend std::ostream& operator<<(std::ostream& out, const UnorientedGraph& graph);
};

#endif
