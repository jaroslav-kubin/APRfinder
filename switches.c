#include "switches.h"
#define duplicitOption "This option has been already given --%s\n"            
#define fileOpeningFailure "Cant open the file %s.Check if your file after -i/--input <file> exists and can be accessed,\n"
#define convertionFail "Cant convert the number from %s.\n"
#define paramaterOverflow "The argument of --%s needs to be between %d and %d\n"


int setValue(char *arg, int *value) {
    if (*value) {
        fprintf(stderr, duplicitOption, "max-AT");
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
    if (!(setValue(arg, &(param->memory_size)))) {
        return 0;
    }
    if (!parameterOutOfBounds(param->memory_size, 1000, 10000)) {
        printf(paramaterOverflow, "memory-size", 1000, 10000);
        return 0;
    }
    return 1;
}

int setMaxTracks(char *arg, parameters *param) {
    if (!(setValue(arg, &(param->max_tracks)))) {
        return 0;
    }
    if (!parameterOutOfBounds(param->max_tracks, 1, 10)) {
        printf(paramaterOverflow, "max-tracks", 1, 10);
        return 0;
    }
    return 1;
}

int setMinTracks(char *arg, parameters *param) {
    if (!(setValue(arg, &(param->min_tracks)))) {
        return 0;
    }
    if (!parameterOutOfBounds(param->min_tracks, 1, 10)) {
        printf(paramaterOverflow, "min-tracks", 1, 10);
        return 0;
    }
    return 1;
}

int setMaxAT(char *arg, parameters *param) {
    if (!(setValue(arg, &(param->max_AT)))) {
        return 0;
    }
    if (!parameterOutOfBounds(param->max_AT, 2, 12)) {
        printf(paramaterOverflow, "max-AT", 2, 12);
        return 0;
    }
    return 1;
}

int setMinAT(char *arg, parameters *param) {
    if (!(setValue(arg, &(param->min_AT)))) {
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
    if (!(setValue(arg, &(param->lower)))) {
        return 0;
    }
    if (!parameterOutOfBounds(param->lower, 2, 20)) {
        printf(paramaterOverflow, "lower", 2, 20);
        return 0;
    }
    return 1;
}


int setUpper(char* arg, parameters *param) {
    if (!(setValue(arg, &(param->upper)))) {
        return 0;
    }
    if (!parameterOutOfBounds(param->upper, 2, 20)) {
        printf(paramaterOverflow, "upper", 2, 20);
        return 0;
    }
    return 1;
}

int setSeqID(char* arg, printer *printer) {
    if (printer->id) {
        fprintf(stderr, duplicitOption, "source");
        return 0;
    }
    int length = strlen(optarg);
    char *seq_id;
    if (!(seq_id = malloc(length*sizeof(char)+1))) {
        fprintf(stderr, "Allocation has failed.\n");
        return 0;
    }
    strcpy(seq_id, optarg);
    printer->id = seq_id;
    return 1;
}


int setSource(char* arg, printer *printer) {
    if (printer->source) {
        fprintf(stderr, duplicitOption, "source");
        return 0;
    }
    int length = strlen(optarg);
    char* source;
    if (!(source = malloc(length*sizeof(char)+1))) {
        fprintf(stderr, "Allocation has failed.\n");
        return 0;
    }
    strcpy(source, optarg);
    printer->source = source;
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


void freePrinter(printer *printer) {
    free(printer->source);
    free(printer->id);
    free(printer->attributes);
    
}


void initializeImplicit(parameters *param) {
    if (!param->lower) {
        param->lower = 7; 
    }
    if (!param->upper) {
        param->upper = 12;
    }
    if (!param->min_AT) {
        param->min_AT = 3;
    }
    if (!param->max_AT) {
        param->max_AT = 8;
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

void initializePrinter(printer *p, int stream_start, char* header) {
    p->strand = '.';
    p->type = "A_phased_repeat";
    p->score = '.';
    p->phased = '.';
    p->seek_start = stream_start;
    if (!p->source) {
        p->source = "ABC_C";
    }
    if (!p->id) {
        p->id = header;
    }
}