
#include "MMSolver.h"
#include <zlib.h>
#include <minisat/core/Dimacs.h>


bool MMSolver::solve() {
    Minisat::vec<Minisat::Lit> litsT;
    if(compute_model_count>0){
        return result;
    }
    compute_model_count=1;
    while (solver.solve()){
        result =true;
        litsT.clear();
        model.clear();
        for (int i = 0; i < solver.nVars(); i++) {
            Minisat::lbool value=solver.model[i];
            model.push(value);
            if (value == Minisat::l_True) {
                litsT.push(~Minisat::mkLit(i));
            } else if(value==Minisat::l_False){
                solver.addClause(~Minisat::mkLit(i));
            }
        }
        solver.addClause(litsT);
        ++compute_model_count;
    }
    return result;
}

void MMSolver::printStats() {
    BaseSolver::_printStats();
    printf("ComputeModelCount     : %d \n", compute_model_count);
}
