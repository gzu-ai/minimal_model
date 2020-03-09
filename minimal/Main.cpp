
#include <errno.h>
#include <zlib.h>

#include "minisat/utils/System.h"
#include "minisat/utils/ParseUtils.h"
#include "minisat/utils/Options.h"
#include "minisat/core/Dimacs.h"
#include "minisat/mtl/IntMap.h"
#include "MMSolver.h"
#include "MRSolver.h"
#include <unordered_set>
#include <signal.h>
using namespace Minisat;

//=================================================================================================


static BaseSolver *solver;

//static void SIGINT_interrupt(int) { solver->interrupt(); }
//
//
static void SIGINT_exit(int) {
    printf("\n"); printf("*** INTERRUPTED ***\n");
    solver->printStats();
    printf("\n"); printf("*** INTERRUPTED ***\n");
    _exit(1);
}



int main(int argc, char** argv)
{

    int data[] = {1,2,3,4,5,6};
    int index=0;
    int lastIndex=0;
    int size=6;
    for ( ;lastIndex < size; ++lastIndex) {
        data[index]=data[lastIndex];
        if (data[lastIndex]%2==0){
            continue;
        }
        ++index;
    }

//    data[index]=-1;
//    printf("sss");


    try {
        setUsageHelp("USAGE: %s [options] <input-file> <result-output-file>\n\n  where input may be either in plain or gzipped DIMACS.\n");
        setX86FPUPrecision();

        // Extra options:
        //
        IntOption    verb   ("MAIN", "verb",   "Verbosity level (0=silent, 1=some, 2=more).", 0, IntRange(0, 2));
        IntOption    cpu_lim("MAIN", "cpu-lim","Limit on CPU time allowed in seconds.\n", 0, IntRange(0, INT32_MAX));
        IntOption    mem_lim("MAIN", "mem-lim","Limit on memory usage in megabytes.\n", 0, IntRange(0, INT32_MAX));
        BoolOption   strictp("MAIN", "strict", "Validate DIMACS header during parsing.", false);
        StringOption mod("MAIN","mod","Select a mod to compute mini modal \n        MMSAT,MRSAT","MRSAT");
        parseOptions(argc, argv, true);
        if (cpu_lim != 0) limitTime(cpu_lim);
        if (mem_lim != 0) limitMemory(mem_lim);
        sigTerm(SIGINT_exit);
        signal(SIGQUIT,SIGINT_exit);
        double initial_time = cpuTime();
        if (strcmp(&*mod,"MMSAT")==0){
            solver=new MMSolver(argv[1],strictp,verb);
        } else{
            solver=new MRSolver(argv[1],strictp,verb);
        }
        bool  result=solver->solve();
        solver->printStats();
        if (!result){
            printf("UNSATISFIABLE\n");
            return 0;
        }
        printf("SATISFIABLE\n");
        vec<lbool>& model = solver->getModel();

        for (int i = 0; i < model.size(); ++i) {
            if(model[i]==l_True){
                printf("%d ",i+1);
            }
        }
        printf("\n");

    } catch (OutOfMemoryException&){
        printf("===============================================================================\n");
        printf("INDETERMINATE\n");
        exit(0);
    }
}
