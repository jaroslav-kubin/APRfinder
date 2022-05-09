
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "switches.h"


/* =============== MAIN FUNCTIONS, DATA STRUCTURES =============== */

/**
 * @brief Data structure for holding all information needed.
 * @param values - Binary coded nucleotide array
 * @param length - Lenght of the array
 * @param position - Current position in the Binary array
 */
typedef struct seq {
    uint8_t * values;
    int length;
    int total_size;
} seq;

typedef struct filePTR {
    FILE *input;
    FILE *output;
} fPTR;


/**
 * @brief Holds information about the founded aTrack.
 */
typedef struct aTrack {
    uint32_t from;
    uint32_t to;    
} aTrack;


typedef struct repeats {
    int from;
    int to;
    int track_count;
    int mid_to_beat;
} repeats;


/**
 * @brief Just enum for nucleotides.
 */
typedef enum nucleotide {
    n, 
    y
} nuc;

typedef enum bounds {
    INSIDE = 1,
    OUTSIDE = -1,
    WITHIN = 0
} bounds;

bool findTrack(uint32_t *window, seq *seq, int *count, int *position, aTrack *track, parameters param);
bool initialRepeats(repeats *rep, int mid_now, int from);
uint32_t calculateStartingPoint(int total_size);
uint32_t createMask(int min_at);
bounds satisfiesBoundaries(int x, int upper, int lower);
bool writeResult(FILE *f, FILE *result, printer printer, repeats repeats);
bool getHeader(char * header, char *fname);
void red();
void reset();


/**
 * @brief Seaches for 3-6 a/t nucleotides in a row.
 * 
 * @param seq - sequence for searching
 * @param value - actual value of sequence coding
 * @param track - track to create the aTrack
 * @return true 
 * @return false 
 */
bool searchAtrack(seq *seq, uint8_t *value, aTrack *track);


/**
 * @brief Get character representation of nucleotide.
 * @param seq - sequence for searching
 */
bool linearSearch(struct seq *seq, FILE *result, printer printer, FILE *f, aTrack *prev, parameters parametr, repeats *rep, bool *at_overflow, uint32_t mask);


/**
 * @brief Get the position in nucleotide coding.
 * 
 * @param value - coding of nucleotides
 * @param position - position to get the nuc from
 * @return uint8_t 
 */
uint8_t getFromPosition(uint8_t value, int position);


/**
 * @brief Converts string sequence of nucleotids into suitable form.
 * @param sequence - sequence of nucleotids
 * @return converted version
 */
bool converter(FILE* f, seq *seq, int *count);

/* ========================== END ========================== */
/* =============== DECOMPOSITION OF FUNCTIONS ================= */
 
/**
 * @brief Get binary coded nucleotide sequence from @param f.
 * 
 * @param f - file in format of fasta
 * @return binary coded sequence of nucleotide 
 */
bool getMemory(FILE* f, FILE *result, printer *printer, parameters parametr);


/**
 * @brief Informs user about allocation failure and frees the @param pointer
 * 
 * @param pointer - pointer to be freed.
 * @return nothing
 */
void * allocationFailed(uint8_t * pointer);


/**
 * @brief Raises value of @param value by certain amount and move the value by two bits to the left. 
 * 
 * - For adenin raise by 0
 * - For thymin raise by
 * - For cytosin raise by
 * - For guanin raise by
 * 
 * @param value - value
 * @param c - name of nucleotide
 * @param count - position in data structure
 * @return value raised by certain amount and moved two bits to the left.
 */
bool raiseValue(uint8_t * value, char c, int count);

/* =========================== END ============================ */
