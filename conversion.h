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


/**
 * @brief Holds information about the founded aTrack.
 */
typedef struct aTrack {
    uint32_t from;
    uint32_t to;    
} aTrack;


/**
 * @brief Hold informatioin about possible A-phased repeat.
 * 
 */
typedef struct repeats {
    uint32_t from;
    uint32_t to;
    int track_count;
    uint32_t mid_to_beat;
} repeats;


/**
 * @brief Just enum for nucleotides.
 */
typedef enum nucleotide {
    n, 
    y
} nuc;


/**
 * @brief Represent if the spacer is: 
 *  - lower then lower bound (WITHIN)
 *  - between the lower and upper boundary (INSIDE) 
 *  - greater the upper bound (OUTSIDE)  
 */
typedef enum bounds {
    INSIDE = 1,
    OUTSIDE = -1,
    WITHIN = 0
} bounds;


/**
 * @brief Find A-track. Note that if there is more A/T nucleotides then allowed value, the track is not considered.
 * 
 * @param window - searched window
 * @param seq - sequence for searching
 * @param count - count of nucleotides within the window
 * @param position - current position in array
 * @param track - variable that will store the output
 * @param param - input paramaters
 * @return true on succes
 * @return false otherwise
 */
bool findTrack(uint32_t *window, seq *seq, int *count, int *position, aTrack *track, parameters param);


/**
 * @brief Initialize repeats to certain values
 * 
 * @param rep 
 * @param mid_now 
 * @param from  
 */
void inicializeRepeats(repeats *rep, int mid_now, int from);


/**
 * @brief Calculate starting point for the A-tracks index
 * 
 * @param total_size 
 * @return starting point
 */
uint32_t calculateStartingPoint(int total_size);

/**
 * @brief Create a mask 
 * 
 * @param min_at - mask length
 * @return mask
 */
uint32_t createMask(int min_at);


/**
 * @brief Check if the spacer is inside the boundaries
 * 
 * @param x - spacer
 * @param upper - upper boundary
 * @param lower - lower boundary
 * @return bounds (see enum bounds)
 */
bounds satisfiesBoundaries(uint32_t x, uint32_t upper, uint32_t lower);


/**
 * @brief Writes result to output file.
 * 
 * @param f - input file 
 * @param result - output file
 * @param printer - print informations
 * @param repeats - A-phased repeat information
 * @return true on succes
 * @return false otherwise
 */
bool writeResult(FILE *f, FILE *result, printer printer, repeats repeats);


/**
 * @brief Set the color of prints to red
 * 
 */
void red();


/**
 * @brief Reset the color of prints to white
 * 
 */
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
bool linearSearch(struct seq *seq, FILE *result, printer printer, FILE *f, parameters parametr, repeats *rep, bool *at_overflow, uint32_t mask);


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
