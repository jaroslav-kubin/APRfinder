#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

typedef enum switches {
    input,
    output,
    upper_bound, 
    lower_bound,
    exact_bound,
    help,
    max_AT,
    min_AT,
    exact_AT,
    max_tracks,
    min_tracks,
    exact_tracks,
    memory_size

} switches;

typedef struct parameters {
    char* input;
    char* output;
    int lower;
    int upper;
    int min_AT;
    int max_AT;
    int min_tracks;
    int max_tracks;
    int memory_size;
} parameters;


typedef struct printer {
    char *id;
    char* source;
    char* type;
    int start;
    int end;
    char score;
    char strand;
    char phased;
    char* attributes;
    int seek_start;
} printer;



int setMemorySize(char *arg, parameters *param);
int setMaxTracks(char *arg, parameters *param);
int setMinTracks(char *arg, parameters *param);
int setMaxAT(char *arg, parameters *param);
int setMinAT(char *arg, parameters *param);
int setInput(char *arg, parameters *param);
int setOutput(char *arg, parameters *param);
int setLower(char *arg, parameters *param);
int setUpper(char *arg, parameters *param);
int setSeqID(char *arg, printer *printer);
int setSource(char *arg, printer *printer);
void printParameters(parameters param);
void printPrinter(printer printer);
void initializeImplicit(parameters *param);
void initializePrinter(printer *p);
int setExactTracks(char *arg, parameters *param);
int setExactAT(char *arg, parameters *param);
int setExactBound(char *arg, parameters *param);
void printUsage();
void printHelp();
