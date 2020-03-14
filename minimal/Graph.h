
#ifndef MINISAT_GRAPH_H
#define MINISAT_GRAPH_H

#include "SpaceDefine.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string.h>
namespace Minimal {
    class Graph {
        friend class StronglyConnectedGraph;

    public:
        void addPoint(int form, int to);

        bool remove(int point);

        void done();

        void print();

        int getEmptyInDegree() {
            if (emptyInDegree.size() > 0) {
                return *emptyInDegree.begin();
            } else {
                return -1;
            }
        }

        Graph *reverse();

    public:
        std::unordered_set<int> &getAllEmptyInDegree() { return emptyInDegree; }

    protected:
        std::unordered_map<int, std::unordered_set<int> > content;
        std::unordered_map<int, int> indegree;
        std::unordered_set<int> emptyInDegree;
        int maxNode = 0;


    };

    class StronglyConnectedGraph : public Graph {

    public:
        bool remove(int point);

        std::vector<int> &getComponent(int node) { return weights[node]; }

        std::unordered_set<int> &getAllEmptyInDegree() { return emptyInDegree; }

        void compute(Graph *graph);

        void done();

        int getEmptyInDegree();

        void print();

    private:
        std::unordered_map<int, std::vector<int> > weights;

        void dfs(Graph *graph, int node, std::vector<int> *list, bool *visited);

        void loop_dfs(Graph *graph, bool *visited, std::vector<int> *list);

    };
}
#endif //MINISAT_GRAPH_H
