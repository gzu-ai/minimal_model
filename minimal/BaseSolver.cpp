//
// Created by 谢仲涛 on 2020/3/8.
//

#include <minisat/core/Dimacs.h>
#include "BaseSolver.h"
#include "minisat/utils/System.h"
BaseSolver::BaseSolver(const char *path, bool strictp, int verbosity) {
    gzFile in = gzopen(path, "rb");
    solver.verbosity = verbosity;
    if (in == NULL)
        printf("ERROR! Could not open file: %s\n", path), exit(1);

    if (this->solver.verbosity > 0) {
        printf("============================[ Problem Statistics ]=============================\n");
        printf("|                                                                             |\n");
    }

    Minisat::parse_DIMACS(in, this->solver, (bool) strictp);
    gzclose(in);
}
 void BaseSolver::_printStats()
{
    double cpu_time = Minisat::cpuTime();
    double mem_used = Minisat::memUsedPeak();
    if (mem_used != 0) printf("Memory used           : %.2f MB\n", mem_used);
    printf("CPU time              : %g s\n", cpu_time);
}
