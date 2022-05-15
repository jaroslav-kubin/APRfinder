#include "conversion.h"
/* Those function are just for visualization. */


void red () {
  printf("\033[1;31m");
}


void reset () {
  printf("\033[0m");
}

char getNucleotide(uint8_t value) {
    return value == 1 ? 'y': 'n';
}

uint8_t getFromPosition(uint8_t value, int position){
    uint8_t shift = !position? position :(8-position);
    uint8_t final =  (value & (1 << shift)) >> shift;
    return final;
}

void printValues(struct seq seq, FILE *f) {
    uint32_t count = 0;
    int position = 0;
    uint8_t value;
    while (count < seq.length) {
        count++;
        value = seq.values[position];
        uint8_t val = getFromPosition(value, count%8);
        char a = getNucleotide(val);
        putc(a, f);
        if (!(count%8)) {
            position++;
        }
    }
    putc('\n', f);
}

/* ========================================== */
/* ================= SEARCHING ALGORITHMS =========================== */

int calculateMid(aTrack track) { 
    return (track.from + track.to)/2; 
}


void addToWindow(uint8_t *array, uint32_t *window, uint32_t *current, int* count) {
    uint32_t value =  array[*current];
    (*current)++;
    int shift = *count - 8;
    value <<= shift;
    *window += value;
    *count = shift;
}

uint32_t createMask(int min_at) {
    int i = 0;
    uint32_t mask = 0;
    uint32_t shift;
    while (i < min_at) {
        shift = 1 << (31-i);
        mask += shift;
        i++;
    }
    return mask;
    
}


uint32_t initializeWindow(seq seq, int position) {
    int i = 0;
    uint32_t shift;
    uint8_t value;
    uint32_t window = 0;
    
    while (i < 4) {
        if (position < seq.length) {
            value = seq.values[i];
            shift = value << (3-i)*8;
            window += shift;
        } else { 
            printf("This is not enough!\n");
            break;
        }
        i++;
        position++;
    }
    return window;   
}

uint32_t calculateStartingPoint(int total_size) {
    if (total_size < 1000) {
        return 0;
    }
    return total_size - 1000;
}





bool findTrack(uint32_t *window, seq *seq, int *count, int *position, aTrack *track, parameters param) {
    
    uint32_t mask = 0x80000000;
    //printf("********************************\n");
    int from = *position;
    //printf("from is %d\n", from);
    //printf("Position in findAtrack is %d\n", *position);
    *position += param.min_AT;
    *window <<= param.min_AT;
    *count += param.min_AT;
    int starting_point = calculateStartingPoint(seq->total_size);
    while ((*window & mask) == mask) {
        if (*position >= seq->length) {
            red();
            printf("I AM BRAKING!\n");
            reset();
            break;

        }
        (*position)++;
        
        *window <<= 1;
        *count += 1;
        if ((*position - from) == param.max_AT) {
            return true;
        }
    }
    track->from = starting_point + from+1;
    track->to = starting_point + *position;
    //printf("starting point is %d, (%d, %d)\n", starting_point, track.from, track.to);
    //printf("********************************\n");

    return false;
}


bounds satisfiesBoundaries(int x, int upper, int lower){
    if (x < lower) {
        return WITHIN;
    } else if (x > upper) {
        return OUTSIDE;
    } 
    return INSIDE;
}


bool writeResult(FILE *f, FILE *result, printer printer, repeats repeats) {
    int current = ftell(f);    
    int offset = printer.seek_start +repeats.from;
    int checker = fseek(f, offset, SEEK_SET);
    if (checker) {
        return false;
    }
    
    int length = repeats.to - repeats.from+1;
    char res[length+1];
    
    if ((fgets(res, sizeof(res), f)) == NULL) {
        printf("Something is horribly wrong. Report this bug please.\n");
        return false;
    }
    for (size_t i = 0; i < length;i++) {
        res[i] = tolower(res[i]);
        if (res[i] == 'a' || res[i] == 't') {
            res[i] = tolower(res[i]);
        } else {
            res[i] = toupper(res[i]);
        }
            
        
    }
    fprintf(result, "%s\t%s\t%s\t%d\t%d\t%c\t%c\t%c\ttracks=%d;seq=%s\n",  printer.id,
                                                    printer.source,
                                                    printer.type,
                                                    repeats.from,
                                                    repeats.to,
                                                    printer.score,
                                                    printer.strand,
                                                    printer.phased,
                                                    repeats.track_count,
                                                    res);                                     
    if (fseek(f, current, SEEK_SET)) {
        printf("Some error message\n");
        return true;
    }
    return false;


}

bool initialRepeats(repeats *rep, int mid_now, int from) {
    rep->mid_to_beat = mid_now;
    rep->from = from;
    rep->to = 0;
    rep->track_count = 0;
    return true;
}



void moveInWindow(uint32_t *window, int *position, int *count, int step) {
    *position += step;
    *window <<= step;
    *count += step;
}

bool linearSearch(seq *seq, FILE *result, printer printer, FILE *f, aTrack *prev, parameters parameter, repeats *rep, bool *at_overflow,uint32_t mask ) {
    uint8_t *array = seq->values;
    uint32_t length = (seq->length % 8 == 0) ? seq->length/8 : (seq->length/8)+1;
    int position = 0;
    uint32_t window = initializeWindow(*seq, position);
    int count = 0;
    
    aTrack now = {0, 0};
    uint32_t at_mask = 0x80000000;
    bounds b = false;
    uint32_t current = 4;
    uint32_t number;
    
    //OK
    while (position < seq->length) {
        if (*at_overflow) {
            while ((at_mask & window) == at_mask) {
                moveInWindow(&window, &position, &count, 1);
            }
            if (count != 32) {
                *at_overflow = false;
            }
        }
        
        if (current < length) {
            while (count > 7) {
                addToWindow(array, &window, &current, &count);
            } 
        }
        number = mask & window;
        if (number == mask) {
            if ((*at_overflow = findTrack(&window, seq, &count, &position, &now, parameter))) {
                continue;
            }
            int mid_now = calculateMid(now);
            int spacer = mid_now - rep->mid_to_beat;
            
            if (!rep->track_count) {
                rep->from = now.from;
                rep->to = now.to;
                rep->mid_to_beat = mid_now;
                rep->track_count++;
                continue;
            } 
            
            bounds b = satisfiesBoundaries(spacer, parameter.upper, parameter.lower);
            if (b == INSIDE) {
                rep->mid_to_beat = mid_now;
                rep->track_count++;
                rep->to = now.to;
                if (rep->track_count >= parameter.max_tracks) {
                    writeResult(f, result, printer, *rep);
                    initialRepeats(rep, 0, 0);
                }
            } else if (b == OUTSIDE) {
                if (rep->track_count >= parameter.min_tracks) {
                    writeResult(f, result, printer, *rep); 
                    initialRepeats(rep, 0, 0);
                } else {
                    initialRepeats(rep, mid_now, now.from);
                }
            }                
        } else {
            moveInWindow(&window, &position, &count, 1);
        }
    }
    if (b) {
        
    }
    return true;
}
bool getHeader(char * header, char *fname) {
    strcpy(fname, header);
    strcat(fname, "_result.gff");
    return true;
}
/* ============================================================ */

/* =============== DECOMPOSITION OF FUNCTIONS ================= */

bool raiseValue(uint8_t *value, char c, int counter) {
    if (c == 'a' || c == 't') {
        //printf("Value before: %d\n", *value);
        uint8_t shift = counter != 0?  1 << (8-counter): 1;
        *value += shift;
        //printf("I shifted and now have value %d\n", *value);
        return true;
    } 
    return c == 'c' || c == 'g' || c == 'n';
    
}
void inicializeSearch(aTrack *prev, repeats *rep, bool *at_overflow, uint8_t *code) {
    prev->from = 0;
    prev->to = 0;
    rep->from = 0;
    rep->to = 0;
    rep->mid_to_beat= 0;
    rep->track_count = 0;
    *at_overflow = false;
    *code = 0;
}


void inicializeHeader(char *header, printer *p, int *count, int *position, seq *s, FILE *f) {
    fscanf(f, "%s%*[^\n]s", header);
    p->seek_start = ftell(f);
    fscanf(f, "%*c");
    p->id = header;
    *count = 0;
    *position = 0;
    s->length = 0;
    s->total_size = 0;
}

void unfinishedBussiness(int count, seq *s, int position, uint8_t code, printer p, FILE *result, FILE *f, aTrack *prev, parameters param, repeats rep, bool at_overflow, uint32_t mask) {
    if (count % 8) {
        s->values[position] = code;
    }
    s->length = count;
    s->total_size += count;
    linearSearch(s, result, p, f, prev, param, &rep, &at_overflow, mask);
    if (rep.track_count >= param.min_tracks && rep.track_count <= param.min_tracks) {
        writeResult(f, result, p, rep);
    }
}


bool getMemory(FILE* f, FILE* result, printer *printer, parameters parametr) {
    int count = 0;
    seq seq = {0, 0, 0};
    if ((seq.values = malloc(parametr.memory_size)) == NULL) {
        return allocationFailed(seq.values);
    }
    int position = 0;
    uint8_t code = 0;
    char c; 
    aTrack prev = { 0, 0 };
    repeats rep = {0, 0, 0, 0}; 
    uint32_t mask = createMask(parametr.min_AT);
    bool at_overflow = false;
    char *header;
    if (!(header = malloc(20*sizeof(char)))) {
        return false;
    }
    while ((c = getc(f)) != EOF){    
        
        if (c == '\n') { 
            (printer->seek_start)++;
            continue;
        }

        if (c == '>') {
            if (count) {
                unfinishedBussiness(count, &seq, position, code, *printer, result, f, &prev, parametr, rep, at_overflow, mask);
                inicializeSearch(&prev, &rep, &at_overflow, &code);
                
            }
            inicializeHeader(header, printer, &count, &position, &seq, f);
            continue;
        }
        
        c = tolower(c);
        
        if (count == parametr.memory_size) { 
            seq.length = count;
            seq.total_size += count;
            linearSearch(&seq, result, *printer, f, &prev, parametr, &rep, &at_overflow, mask);
            position = 0;
            count = 0;
        }
        count += 1;
        if (!raiseValue(&code, c, count%8)) {
            fprintf(stderr, "Wrong format of the file.\n");
            return false;
        }
        if (count % 8 == 0) {
            seq.values[position] = code;
            position++;
            code = 0;
        }
        
    }
    unfinishedBussiness(count, &seq, position, code, *printer, result, f, &prev, parametr, rep, at_overflow, mask);
    free(seq.values);
    return true;
}   


void * allocationFailed(uint8_t * pointer) {
    fprintf(stderr, "Alocation failed.\n");
    free(pointer);
    return NULL;
}




/* ========================== END ================================ */
/* =============== MAIN FUNCTIONS, DATA STRUCTURES =============== */






/* ============================ END =============================== */
