# APRfinder

Program searching for a-phased repeats. The apr stands for a-phased repeats.

# Installation

aprfinder can be easily installed within conda enviroment.

# Usage
```
aprfinder - tool for a-phased repeat search.
    Usage:
    aprfinder [--help] [--input=<string>] [--output=<string>] [--upper-bound=<int>] [--lower-bound=<int>]
              [--max-AT=<int>] [--min-AT=<int>] [--max-tracks=<int>] [--min-tracks=<int>] [--memory-size=<int>]
              [--exact-bound=<int>] [--exact-tracks=<int>] [--exact-AT=<int>]
options
    --help                    Prints information about the software.
    --input <string>          Input FASTA or MULTIFASTA file.
    --output <string>         Defines output file name. Note that <string> should be in format <name>.gff.
                              If not defined, output is set to result.gff. (Be carefull - running multiple times
                              without this option can overwrite the previous result).
    --upper-bound <int>       Maximum size of spacer.
    --lower-bound <int>       Minimum size of spacer.
    --exact-bound <int>       Sets lower-bound and upper-bound to the same value.
    --max-AT <int>            Maximum number of consecutive A/T nucleotides.
    --min-AT <int>            Minimum number of consecutive A/T nucleotides.
    --exact-AT <int>          Sets max-AT and min-AT to the same value.
    --min-tracks <int>        Minimum number of tracks to consider a sequence to be an a-phased repeat
    --max-tracks <int>        Minimum number of tracks to consider a sequence to be an a-phased repeat
    --exact-tracks <int>      Sets min-tracks and max-tracks to the same value

```