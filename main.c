#include "conversion.h"
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <time.h>
#define fileOpeningFailure "Cant open the file %s.Check if your file after -i/--input <file> exists and can be accessed,\n"

void red () {
  printf("\033[1;31m");
}


void reset () {
  printf("\033[0m");
}

static struct option long_options[] = {
    { "input", required_argument, NULL, input },
    { "output", required_argument, NULL, output},
    { "upper-bound", required_argument, NULL, upper_bound},
    { "lower-bound", required_argument, NULL, lower_bound},
    { "exact-bound", required_argument, NULL, exact_bound},
    { "help", no_argument, NULL, help},
    { "max-AT", required_argument, NULL, max_AT},
    { "min-AT", required_argument, NULL, min_AT},
    { "exact-AT", required_argument, NULL, exact_AT},
    { "max-tracks", required_argument, NULL, max_tracks},
    { "min-tracks", required_argument, NULL, min_tracks},
    { "exact-tracks", required_argument, NULL, exact_tracks},
    { "memory-size", required_argument, NULL, memory_size},
    { 0, 0, 0, 0 }
};



/* ============================= END ============================== */

int main(int argc, char *argv[]){
    clock_t start = clock();
    int index = 0;
    parameters param = {};
    char c = getopt_long(argc, argv,"", long_options, &index);
    
    printer printer = {};
    if (c == EOF) {
        printUsage();
        return EXIT_SUCCESS;
    } 
    while (c != EOF) {
        switch (c) {
        case help:
            printHelp();
            return EXIT_SUCCESS;
            break;
        case input:
            if (!setInput(optarg, &param)) {
                return EXIT_FAILURE;
            }       
            break;
        case output:
            if (!setOutput(optarg, &param)) {
                return EXIT_FAILURE;
            }
            break;
        case lower_bound:
            if (!setLower(optarg, &param)) {
                return EXIT_FAILURE;
            }

            break;
        case upper_bound:
            if (!setUpper(optarg, &param)) {
                return EXIT_FAILURE;
            }
            break;
        
        case min_AT:
            if (!setMinAT(optarg, &param)) {
                return EXIT_FAILURE;
            }
            break;
        case max_AT:
            if (!setMaxAT(optarg, &param)) {
                return EXIT_FAILURE;
            }
            break;
        case max_tracks:
            if (!setMaxTracks(optarg, &param)) {
                return EXIT_FAILURE;
            }
            break;
        case min_tracks:
            if (!setMinTracks(optarg, &param)) {
                return EXIT_FAILURE;
            }
            break;
        case memory_size:
            if (!setMemorySize(optarg, &param)) {
                return EXIT_FAILURE;
            }
            break;
        case exact_tracks:
            if (!setExactTracks(optarg, &param)) {
                return EXIT_FAILURE;
            }
            break;
        case exact_AT:
            if (!setExactAT(optarg, &param)) {
                return EXIT_FAILURE;
            }
            break;
        case exact_bound:
            if (!setExactBound(optarg, &param)) {
                return EXIT_FAILURE;
            }
            break;
        default:
            printf("Unknown argument or bad usage. Run APRfinder --help to print out switches options.\n");
            return 1;
        }
        c = getopt_long(argc, argv,"", long_options, &index);
    }
    
    if (!param.input) {
        printf("The option --input is obligatory.\n");
        return -1;
    }
    //Input file opening
    FILE *file;
    if (!(file = fopen(param.input, "r"))) {
        fprintf(stderr, fileOpeningFailure, param.input);
        return -1;
    }


    
    
    seq seq;
    //Output file opening
    if (!param.output) {
        char *fname = "result.gff";
        param.output = fname;
    }
    

    initializePrinter(&printer);
    initializeImplicit(&param);
    if (param.upper > 20) {
        fprintf(stderr, "The upper bound for spacer length is little too big, maximum is 19.\n");
        return 1;
    }

    if (param.lower < 0) {
        fprintf(stderr, "The lower bound for spacer length is little too small, minimum is 2.\n");
        return 1;
    }
    if (param.lower > param.upper || param.min_AT > param.max_AT || param.min_tracks > param.max_tracks) {
        fprintf(stderr, "It looks like you switched upper and lower bound. I cant work with upper bound smaller then lower bound, rerun with correct arguments.\n");
        return 1;
    }


    
    printParameters(param);
    FILE *result;
    if (!(result = fopen(param.output, "w"))) {
        free(seq.values);
        fclose(file);
        fprintf(stderr, fileOpeningFailure, param.output);
        return -1;
    }
    
    

    // fPTR files = { .input = file, .output = result };

    printf("The program is currently searching for a-phased repeats. It can take a while. Abort with CTRL+C.\n");
    if (!(getMemory(file, result, &printer, param))) {
        fprintf(stderr, "Cannot convert this format.\n");
        return -1;
    }
    fclose(file);
    fclose(result);
    clock_t end = clock();
    red();
    printf("CPU Time : %lf s\n", (double) (end - start) / CLOCKS_PER_SEC);
    reset();
    printf("Program ended successfully. The result of the search is found in %s\n", param.output);
    return 0;
    
}

