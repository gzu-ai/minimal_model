

#ifndef MINISAT_MMSOLVER_H
#define MINISAT_MMSOLVER_H

#include "SpaceDefine.h"
#include "BaseSolver.h"
namespace Minimal {
    class MMSolver : public BaseSolver {

    public:
        bool solve();

        void printStats();

    private:
        int compute_model_count = 0;
        bool result = false;
    };
}

#endif //MINISAT_MMSOLVER_H
