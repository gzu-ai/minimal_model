
#ifndef MINISAT_MRSOLVER_H
#define MINISAT_MRSOLVER_H


#include <minisat/mtl/Vec.h>
#include <minisat/core/SolverTypes.h>
#include <minisat/core/Solver.h>
#include <vector>
#include <minisat/mtl/Sort.h>
#include <minisat/simp/SimpSolver.h>
#include "Utils.h"
#include "Graph.h"
#include "BaseSolver.h"
class MRSolver: public BaseSolver {

public:
    bool solve();
    MRSolver(const char *path, bool strictp,int verbosity):BaseSolver(path,strictp,verbosity){}
    void printStats();
    Minisat::vec<Minisat::lbool>&  getModel()  {
        return model;
    };
private:
    Minisat::vec<Minisat::lbool> model;
    int compute_model_count=0;
    int check_model_count=0;
    int compute_mini_model_count=0;
    bool result = false;
    bool check();
    Minisat::vec<Minisat::CRef> clauses;
    bool createGraph(Minisat::vec<Minisat::CRef>& clauses,StronglyConnectedGraph &graph);
    inline void computeTS(Minisat::vec<Minisat::CRef> &clauses, std::vector<int> & source,Minisat::vec<Minisat::CRef> & ts);

    bool compute( Minisat::vec<Minisat::CRef>& ts,Minisat::vec<int> & S);

    static bool isIntersection(Minisat::vec<int>& S,Minisat::Clause & clause);

    void clear(Minisat::Clause &clause,Minisat::vec<int>& S);

    void reduce(Minisat::vec<Minisat::CRef> &clauses, Minisat::vec<int>& S) ;


    inline bool computeS(StronglyConnectedGraph & graph,Minisat::vec<int> & S){
        S.clear();
        std::unordered_set<int>& empty= graph.getAllEmptyInDegree();
        int V=solver.nVars();
        for (auto it:empty) {
            std::vector<int> &w = graph.getComponent(it);
            for (auto itt : w) {
                if (V > itt) {
                    S.push(itt);
                }
            }
        }
        Minisat::sort(S);
        return true;
    }

    bool mr(Minisat::vec<Minisat::CRef> &clauses,Minisat::vec<Minisat::lbool> &model);
    void copyToClauses(Minisat::vec<Minisat::CRef> &clauses);
    void copyToClauses(Minisat::vec<Minisat::CRef> &source,Minisat::vec<Minisat::CRef> &dist);
    Minisat::ClauseAllocator     ca;
};


#endif //MINISAT_MRSOLVER_H
