

#ifndef MINISAT_BASESOLVER_H
#define MINISAT_BASESOLVER_H

#include "SpaceDefine.h"

namespace Minimal {
    class BaseSolver {
    public:
        void readCNF(const char *path, bool strictp, int verbosity);

        virtual bool solve() = 0;

        inline int nVars() const { return solver.nVars(); }

        virtual SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::lbool> &getModel() {
            return solver.model;
        };

        virtual void addClause(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::Lit> &lits) = 0;

        virtual void printStats() { _printStats(); }

    protected:
        void _printStats();

        SOlVER_NAMESPACE::Solver solver;

    };

}
#endif //MINISAT_BASESOLVER_H
