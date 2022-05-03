#include "conversion.h"
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <time.h>
#define fileOpeningFailure "Cant open the file %s.Check if your file after -i/--input <file> exists and can be accessed,\n"


static struct option long_options[] = {
    { "input", required_argument, NULL, 'i' },
    { "output", required_argument, NULL, 'o'},
    { "upper", required_argument, NULL, 'u'},
    { "lower", required_argument, NULL, 'l'},
    { "help", no_argument, NULL, 'h'},
    { "seq-id", required_argument, NULL, 'd'},
    { "source", required_argument, NULL, 's'},
    { "max-AT", required_argument, NULL, 'm'},
    { "min-AT", required_argument, NULL, 'n'},
    { "max-tracks", required_argument, NULL, 'q'},
    { "min-tracks", required_argument, NULL, 'p'},
    { "memory-size", required_argument, NULL, 'y'},

    { 0, 0, 0, 0 }
};



/* ============================= END ============================== */

int main(int argc, char *argv[]){
    clock_t start = clock();
    int index = 0;
    parameters param = {};
    char c = getopt_long(argc, argv,"i:o:l:u:hs:d:m:n:q:p:y:", long_options, &index);
    
    printer printer = {};
    if (c == EOF) {
        fprintf(stderr, "No input file. The program need atleast -i/--input <name> to run or see help using -h/--help.\n");
        return 1;
    } 
    while (c != EOF) {
        switch (c) {
        case 'i':
            if (!setInput(optarg, &param)) {
                return 1;
            }       
            break;
        case 'o':
            if (!setOutput(optarg, &param)) {
                return 1;
            }
            break;
        case 'l':
            if (!setLower(optarg, &param)) {
                return 1;
            }

            break;
        case 'u':
            if (!setUpper(optarg, &param)) {
                return 1;
            }
            break;
        case 'h':
            printf("Stilll working on it.\n");
            break;
        case 'd': // seqid
            if (!setSeqID(optarg, &printer)) {
                return 1;
            }
            break;
        case 's':
            if (!setSource(optarg, &printer)) {
                return 1;
            }
            break;
        case 'n':
            if (!setMinAT(optarg, &param)) {
                return 1;
            }
            break;
        case 'm':
            if (!setMaxAT(optarg, &param)) {
                return 1;
            }
            break;
        case 'q':
            if (!setMaxTracks(optarg, &param)) {
                return 1;
            }
            break;
        case 'p':
            if (!setMinTracks(optarg, &param)) {
                return 1;
            }
            break;
        case 'y':
            if (!setMemorySize(optarg, &param)) {
                return 1;
            }
            break;
        default:
            printf("Unknown argument or bad usage. Use -h/--help to print usage\n");
            return 1;
        }
        c = getopt_long(argc, argv,"i:o:l:u:hs:d:", long_options, &index);
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


    
    char header[20];
    fscanf(file, "%*c%s%*[^\n]s", header);
    long int stream_start = ftell(file)+1;
    seq seq;
    int l;
    char *fname;
    //Output file opening
    if (!param.output) {
        l = strlen(header) + strlen("_result.gff")+1;
        if (!(fname = malloc(l*sizeof(char)))) {
            fprintf(stderr, "Allocation has failed\n");
            return 1;
        }
        getHeader(header, fname);
        param.output = fname;
    }
    

    initializePrinter(&printer, stream_start, header);
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
        fprintf(stderr, fileOpeningFailure, fname);
        return -1;
    }
    
    

    // fPTR files = { .input = file, .output = result };

    
    if (!(getMemory(file, result, printer, param))) {
        fprintf(stderr, "Cannot convert this format.\n");
        return -1;
    }
    fclose(file);
    fclose(result);
    clock_t end = clock();
    printf("Time : %lf s\n", (double) (end - start) / CLOCKS_PER_SEC);
    return 0;
    
}

