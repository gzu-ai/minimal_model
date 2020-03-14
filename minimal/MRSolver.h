
#ifndef MINISAT_MRSOLVER_H
#define MINISAT_MRSOLVER_H


#include "SpaceDefine.h"
#include <vector>
#include "Utils.h"
#include "Graph.h"
#include "BaseSolver.h"
namespace Minimal {
    class MRSolver : public BaseSolver {

    public:
        bool solve();

        void printStats();

        SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::lbool> &getModel() {
            return model;
        };

        void addClause(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::Lit> &lits) {
            SOlVER_NAMESPACE::sort(lits);
            SOlVER_NAMESPACE::CRef crf = ca.alloc(lits);
            clauses.push(crf);
        }

    private:
        SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::lbool> model;
        int compute_model_count = 0;
        int check_model_count = 0;
        int compute_mini_model_count = 0;
        bool result = false;

        bool check();

        SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> clauses;

        bool createGraph(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> &clauses, StronglyConnectedGraph &graph);

        inline void
        computeTS(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> &clauses, SOlVER_NAMESPACE::vec<int> &source, SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> &ts);

        bool compute(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> &ts, SOlVER_NAMESPACE::vec<int> &S);

        template<class T>
        bool isDifferent(T &S, SOlVER_NAMESPACE::Clause &clause, bool sign = false) {
            int clauseSize = clause.size();
            int clauseIndex = 0;
            int sIndex = 0;
            int sSize = S.size();
            int s;
            SOlVER_NAMESPACE::Lit lis{};
            int var;
            while (sIndex < sSize && clauseIndex < clauseSize) {
                lis = (clause)[clauseIndex];
                var = SOlVER_NAMESPACE::var(lis);
                s = S[sIndex];
                if (SOlVER_NAMESPACE::sign(lis) != sign) {
                    ++clauseIndex;
                    continue;
                }
                if (var == s) {
                    ++sIndex;
                    ++clauseIndex;
                    continue;
                }
                if (var > s) {
                    ++sIndex;
                } else {
                    return true;
                }
            }
            return false;
        }

        template<class T>
        bool isIntersection(T &S, SOlVER_NAMESPACE::Clause &clause, bool sign = false) {
            int clauseSize = clause.size();
            int clauseIndex = 0;
            int sIndex = 0;
            int sSize = S.size();
            int s;
            SOlVER_NAMESPACE::Lit lis{};
            int var;
            while (sIndex < sSize && clauseIndex < clauseSize) {
                lis = (clause)[clauseIndex];
                var = SOlVER_NAMESPACE::var(lis);
                s = S[sIndex];
                if (SOlVER_NAMESPACE::sign(lis) != sign) {
                    ++clauseIndex;
                    continue;
                }
                if (var == s) {
                    return true;
                }
                if (var > s) {
                    ++sIndex;
                } else {
                    ++clauseIndex;
                }
            }
            return false;
        }

        int copyModel(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::lbool> &model);

        void clear(SOlVER_NAMESPACE::Clause &clause, SOlVER_NAMESPACE::vec<int> &S);

        void reduce(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> &clauses, SOlVER_NAMESPACE::vec<int> &S);


        inline bool computeS(std::vector<int> &component, SOlVER_NAMESPACE::vec<int> &S) {
            S.clear();
            int limit = solver.nVars();
            for (auto it: component) {
                if (it < limit) {
                    S.push(it);
                }
            }
            SOlVER_NAMESPACE::sort(S);
            return true;
        }

        bool mr(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> &clauses, SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::lbool> &model);

        void copyToClauses(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> &source, SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> &dist);

        SOlVER_NAMESPACE::ClauseAllocator ca;
    };
}

#endif //MINISAT_MRSOLVER_H
