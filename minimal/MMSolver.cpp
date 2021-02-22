
#include "MMSolver.h"
#include <zlib.h>
#include <minisat/core/Dimacs.h>
using namespace Minimal;

bool MMSolver::solve() {
    SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::Lit> litsT;
    if(compute_model_count>0){
        return result;
    }
    compute_model_count=1;
    solver.simplify();
    while (solver.solve(false, true)){
        result =true;
        litsT.clear();
        model.clear();
        for (int i = 0; i < solver.nVars(); i++) {
            SOlVER_NAMESPACE::lbool value=solver.model[i];
            model.push(value);
            if (value == SOlVER_NAMESPACE::l_True) {
                litsT.push(~SOlVER_NAMESPACE::mkLit(i));
            } else if(value==SOlVER_NAMESPACE::l_False){
                solver.addClause(~SOlVER_NAMESPACE::mkLit(i));
            }
        }
        solver.addClause(litsT);
        solver.simplify();
        ++compute_model_count;
    }
    return result;
}

void MMSolver::printStats() {
    BaseSolver::_printStats();
    printf("ComputeModelCount     : %d \n", compute_model_count);
}
