
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
    void printStats();
    Minisat::vec<Minisat::lbool>&  getModel()  {
        return model;
    };
    void addClause(Minisat::vec<Minisat::Lit>&lits){
        Minisat::sort(lits);
        Minisat::CRef  crf=ca.alloc(lits);
        clauses.push(crf);
    }
private:
    Minisat::vec<Minisat::lbool> model;
    int compute_model_count=0;
    int check_model_count=0;
    int compute_mini_model_count=0;
    bool result = false;
    bool check();
    Minisat::vec<Minisat::CRef> clauses;
    bool createGraph(Minisat::vec<Minisat::CRef>& clauses,StronglyConnectedGraph &graph);
    inline void computeTS(Minisat::vec<Minisat::CRef> &clauses, Minisat::vec<int> & source,Minisat::vec<Minisat::CRef> & ts);

    bool compute( Minisat::vec<Minisat::CRef>& ts,Minisat::vec<int> & S);
    template < class T>
    bool isDifferent(T&S, Minisat::Clause &clause, bool sign= false) {
        int clauseSize=clause.size();
        int clauseIndex=0;
        int sIndex=0;
        int sSize=S.size();
        int s;
        Minisat::Lit lis{};
        int var;
        while (sIndex<sSize&&clauseIndex<clauseSize){
            lis=(clause)[clauseIndex];
            var= Minisat::var(lis);
            s=S[sIndex];
            if (Minisat::sign(lis)!=sign){
                ++clauseIndex;
                continue;
            }
            if (var==s){
                ++sIndex;
                ++clauseIndex;
                continue;
            }
            if(var>s){
                ++sIndex;
            } else{
                return true;
            }
        }
        return false;
    }
    template < class T>
    bool isIntersection(T&S, Minisat::Clause &clause, bool sign= false) {
        int clauseSize=clause.size();
        int clauseIndex=0;
        int sIndex=0;
        int sSize=S.size();
        int s;
        Minisat::Lit lis{};
        int var;
        while (sIndex<sSize&&clauseIndex<clauseSize){
            lis=(clause)[clauseIndex];
            var= Minisat::var(lis);
            s=S[sIndex];
            if (Minisat::sign(lis)!=sign){
                ++clauseIndex;
                continue;
            }
            if (var==s){
                return true;
            }
            if(var>s){
                ++sIndex;
            } else{
                ++clauseIndex;
            }
        }
        return false;
    }

    int copyModel(Minisat::vec<Minisat::lbool>&model);

    void clear(Minisat::Clause &clause,Minisat::vec<int>& S);

    void reduce(Minisat::vec<Minisat::CRef> &clauses, Minisat::vec<int>& S) ;


    inline bool computeS(std::vector<int > & component,Minisat::vec<int> & S){
        S.clear();
        int limit=solver.nVars();
        for (auto it: component){
            if (it<limit){
                S.push(it);
            }
        }
        Minisat::sort(S);
        return true;
    }

    bool mr(Minisat::vec<Minisat::CRef> &clauses,Minisat::vec<Minisat::lbool> &model);
    void copyToClauses(Minisat::vec<Minisat::CRef> &source,Minisat::vec<Minisat::CRef> &dist);
    Minisat::ClauseAllocator     ca;
};


#endif //MINISAT_MRSOLVER_H
