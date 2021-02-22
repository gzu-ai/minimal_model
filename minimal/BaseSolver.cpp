//
// Created by 谢仲涛 on 2020/3/8.
//

#include "Dimacs.h"
#include "BaseSolver.h"
#include "SpaceDefine.h"
using namespace Minimal;
void BaseSolver::readCNF(const char *path, bool strictp, int verbosity) {
    gzFile in = gzopen(path, "rb");
    solver.verbosity = verbosity;
    if (in == NULL)
        printf("ERROR! Could not open file: %s\n", path), exit(1);

    if (this->solver.verbosity > 0) {
        printf("============================[ Problem Statistics ]=============================\n");
        printf("|                                                                             |\n");
    }
    parse_DIMACS(in, this->solver, this,(bool) strictp);
    gzclose(in);
}
 void BaseSolver::_printStats()
{
    double cpu_time =SOlVER_NAMESPACE::cpuTime();
    double mem_used =SOlVER_NAMESPACE::memUsedPeak();
    if (mem_used != 0) printf("Memory used           : %.2f MB\n", mem_used);
    printf("CPU time              : %g s\n", cpu_time);
}
