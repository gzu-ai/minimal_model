

#ifndef MINISAT_BASESOLVER_H
#define MINISAT_BASESOLVER_H

#include <minisat/simp/SimpSolver.h>
#include "SpaceDefine.h"

namespace Minimal {
    class BaseSolver {

    public:
        BaseSolver(){
            this->solver =newSolver();
        }
        ~BaseSolver(){
           delete this->solver;
        }
        void readCNF(const char *path, bool strictp, int verbosity);

        virtual bool solve() = 0;

        inline int nVars() const { return solver->nVars(); }

        inline SOlVER_NAMESPACE::SimpSolver * newSolver(int nVar=0){
            SOlVER_NAMESPACE::SimpSolver *S=  new SOlVER_NAMESPACE::SimpSolver();

            for (int i = 0; i < nVar; ++i) {
                S->newVar();
            }
            return S;
        }

        virtual SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::lbool> &getModel() {
            return solver->model;
        };
        virtual void addClause(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::Lit> &lits) {
            SOlVER_NAMESPACE::sort(lits);
            SOlVER_NAMESPACE::CRef crf = ca.alloc(lits);
            clauses.push(crf);
        };

        void copyToSolver(){
            SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::Lit> lits;
            for (int i = 0; i < clauses.size(); ++i) {
                SOlVER_NAMESPACE::CRef crf = clauses[i];
                SOlVER_NAMESPACE::Clause &clause = ca[crf];
                lits.clear();
                for (int j = 0; j < clause.size(); ++j) {
                    lits.push(clause[j]);
                }
                solver->addClause_(lits);
            }
        }

        virtual void printStats() { _printStats(); }

    protected:
        void _printStats();
        SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> clauses;

        SOlVER_NAMESPACE::SimpSolver* solver;
        int verbosity;
        SOlVER_NAMESPACE::ClauseAllocator ca;

    };

}
#endif //MINISAT_BASESOLVER_H
