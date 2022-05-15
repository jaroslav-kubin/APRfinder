#include "switches.h"
#define duplicitOption "This option has been already given --%s (check if you are not using --exact switch)\n"            
#define fileOpeningFailure "Cant open the file %s.Check if your file after -i/--input <file> exists and can be accessed.\n"
#define convertionFail "Cant convert the number from %s.\n"
#define paramaterOverflow "The argument of --%s needs to be between %d and %d\n"


int setValue(char *arg, int *value, char* option) {
    if (*value) {
        fprintf(stderr, duplicitOption, option);
        return 0;
    }
    (*value) = strtol(arg, NULL, 10);
    if (errno) {
        printf("Cant convert it\n");
        return 0;
    }
    if (!(*value)) {
        printf("Upper boundary is 0 or blabla\n");
        return 0;
    }
    return 1;
}

bool parameterOutOfBounds(int x, int lower, int upper) {
    return (x >= lower) && (x <= upper);
}

int setMemorySize(char *arg, parameters *param) {
    if (!(setValue(arg, &(param->memory_size), "memory-size"))) {
        return 0;
    }
    if (!parameterOutOfBounds(param->memory_size, 1000, 10000)) {
        printf(paramaterOverflow, "memory-size", 1000, 10000);
        return 0;
    }
    return 1;
}

int setMaxTracks(char *arg, parameters *param) {
    if (!(setValue(arg, &(param->max_tracks), "max-tracks"))) {
        return 0;
    }
    if (!parameterOutOfBounds(param->max_tracks, 1, 10)) {
        printf(paramaterOverflow, "max-tracks", 1, 10);
        return 0;
    }
    return 1;
}

int setMinTracks(char *arg, parameters *param) {
    if (!(setValue(arg, &(param->min_tracks), "min-tracks"))) {
        return 0;
    }
    if (!parameterOutOfBounds(param->min_tracks, 1, 10)) {
        printf(paramaterOverflow, "min-tracks", 1, 10);
        return 0;
    }
    return 1;
}

int setExactTracks(char *arg, parameters *param) {
    if (param->min_tracks || param->max_tracks) {
        fprintf(stderr, "You cant combine exact-tracks together with min/max-tracks\n");
        return 0;
    }
    int value = strtol(arg, NULL, 10);
    if (errno) {
        printf("Cant convert it\n");
        return 0;
    }
    if (!value) {
        printf("Upper boundary is 0 or blabla\n");
        return 0;
    }
    param->min_tracks = value;
    param->max_tracks = value;
    if (!parameterOutOfBounds(param->min_tracks, 1, 10)) {
        printf(paramaterOverflow, "exact-tracks", 1, 10);
        return 0;
    }
    
    return 1;
}


int setExactAT(char *arg, parameters *param) {
    if (param->min_AT || param->max_AT) {
        fprintf(stderr, "You cant combine exact-AT together with min/max-AT\n");
        return 0;
    }
    int value = strtol(arg, NULL, 10);
    if (errno) {
        printf("Cant convert it\n");
        return 0;
    }
    if (!value) {
        printf("Upper boundary is 0 or blabla\n");
        return 0;
    }
    
    param->min_AT = value;
    param->max_AT = value;
    if (!parameterOutOfBounds(param->min_AT, 1, 10)) {
        printf(paramaterOverflow, "exact-AT", 1, 10);
        return 0;
    }
    
    return 1;
}

int setExactBound(char *arg, parameters *param) {
    if (param->lower || param->upper) {
        fprintf(stderr, "You cant combine exact-bound together with lower/upper-bound\n");
        return 0;
    }
    int value = strtol(arg, NULL, 10);
    if (errno) {
        printf("Cant convert it\n");
        return 0;
    }
    if (!value) {
        printf("Upper boundary is 0 or blabla\n");
        return 0;
    }
    param->lower = value;
    param->upper = value;
    if (!parameterOutOfBounds(param->lower, 1, 10)) {
        printf(paramaterOverflow, "exact-bound", 1, 10);
        return 0;
    }
    
    return 1;
}

int setMaxAT(char *arg, parameters *param) {
    if (!(setValue(arg, &(param->max_AT), "max-AT"))) {
        return 0;
    }
    if (!parameterOutOfBounds(param->max_AT, 2, 12)) {
        printf(paramaterOverflow, "max-AT", 2, 12);
        return 0;
    }
    return 1;
}

int setMinAT(char *arg, parameters *param) {
    if (!(setValue(arg, &(param->min_AT), "min-AT"))) {
        return 0;
    }
    if (!parameterOutOfBounds(param->min_AT, 2, 12)) {
        printf(paramaterOverflow, "min-AT", 2, 12);
        return 0;
    }
    return 1;
}

int setInput(char *arg, parameters *param) {
    if (param->input) {
        fprintf(stderr, duplicitOption, "input");
        return 0;
    }
    param->input = arg;
    return 1;
}


int setOutput(char *arg, parameters *parameters) {
    if (parameters->output) {
        fprintf(stderr, duplicitOption, "output");
        return 0;
    }
    parameters->output = arg;
    return 1;
}


int setLower(char* arg, parameters *param) {
    if (!(setValue(arg, &(param->lower), "lower-bound"))) {
        return 0;
    }
    if (!parameterOutOfBounds(param->lower, 2, 20)) {
        printf(paramaterOverflow, "lower-bound", 2, 20);
        return 0;
    }
    return 1;
}


int setUpper(char* arg, parameters *param) {
    if (!(setValue(arg, &(param->upper), "upper-bound"))) {
        return 0;
    }
    if (!parameterOutOfBounds(param->upper, 2, 20)) {
        printf(paramaterOverflow, "upper-bound", 2, 20);
        return 0;
    }
    return 1;
}


void printParameters(parameters param) {
    printf("*** You are running the program with these atributes ***\n");
    printf("Running this with memory size %d\n", param.memory_size);
    printf("Input: %s\nOutput : %s\n", param.input, param.output);
    printf("Lower bound : %d\nUpper bound : %d\n", param.lower, param.upper);
    printf("Minimum for considering a-track: %d\n", param.min_AT);
    printf("Maximum for considering a-track: %d\n", param.max_AT);
    printf("If there is between %d and %d tracks with spacer between them of length %d to %d, I will write this as A-phased-repeat\n", param.min_tracks, 
                                                                                                                       param.max_tracks,
                                                                                                                       param.lower, param.upper);
}


void printPrinter(printer printer) {
    printf("Seq_id\tSource\tType        \tStart\tEnd\tScore\tStrand\tPhase\tAttributes\n");
    printf("%s\t%s\t%s\t%d\t%d\t%c\t%c\t%c\t%s\n",  printer.id,
                                                    printer.source,
                                                    printer.type,
                                                    printer.start,
                                                    printer.end,
                                                    printer.score,
                                                    printer.strand,
                                                    printer.phased,
                                                    printer.attributes);
}

void printUsage() {
    printf("APRfinder - tool for a-phased repeat search.\n");
    printf("Usage:\n");
    printf("APRfinder [--help] [--input=<string>] [--output=<string>] [--upper-bound=<int>] [--lower-bound=<int>]\n");
    printf("          [--max-AT=<int>] [--min-AT=<int>] [--max-tracks=<int>] [--min-tracks=<int>] [--memory-size=<int>]\n");
    printf("          [--exact-bound=<int>] [--exact-tracks=<int>] [--exact-AT=<int>]\n");
}


void printHelp() {
    printUsage();
    printf("options:\n");
    printf("--help              \tPrints information about the software.\n");
    printf("--input <string>    \tInput FASTA or MULTIFASTA file.\n");
    printf("--output <string>   \tDefines output file name. Note that <string> should be in format <name>.gff.\n");
    printf("                    \tIf not defined, output is set to result.gff. (Be carefull - running multiple times\n");
    printf("                    \twithout this option can overwrite the previous result)\n");
    printf("--upper-bound <int> \tMaximum size of spacer.\n");
    printf("--lower-bound <int> \tMinimum size of spacer.\n");
    printf("--exact-bound <int> \tSets lower-bound and upper-bound to the same value.\n");
    printf("--max-AT <int>      \tMaximum number of consecutive A/T nucleotides.\n");
    printf("--min-AT <int>      \tMinimum number of consecutive A/T nucleotides.\n");
    printf("--exact-AT <int>    \tSets max-AT and min-AT to the same value.\n");
    printf("--min-tracks <int>  \tMinimum number of tracks to consider a sequence to be an a-phased repeat.\n");
    printf("--max-tracks <int>  \tMinimum number of tracks to consider a sequence to be an a-phased repeat.\n");
    printf("--exact-tracks <int>\tSets min-tracks and max-tracks to the same value.\n");
}

void freePrinter(printer *printer) {
    free(printer->source);
    free(printer->id);
    free(printer->attributes);
    
}


void initializeImplicit(parameters *param) {
    if (!param->lower) {
        param->lower = 10; 
    }
    if (!param->upper) {
        param->upper = 12;
    }
    if (!param->min_AT) {
        param->min_AT = 3;
    }
    if (!param->max_AT) {
        param->max_AT = 9;
    }
    if (!param->min_tracks) {
        param->min_tracks = 3;
    }
    if (!param->max_tracks) {
        param->max_tracks = 5;
    }
    if (!param->memory_size) {
        param->memory_size = 1000;
    }
}

void initializePrinter(printer *p) {
    p->strand = '.';
    p->type = "A_phased_repeat";
    p->score = '.';
    p->phased = '.';
    p->seek_start = 0;
    if (!p->source) {
        p->source = "APRfinder";
    }
    if (!p->id) {
        p->id = NULL;
    }
}