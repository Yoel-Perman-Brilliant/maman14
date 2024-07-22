#ifndef MAMAN14_SYMBOLS_H
#define MAMAN14_SYMBOLS_H

/**
 * Represents the location of a symbol's value (code or data).
 * Undefined is used for external symbols whose location is not yet known.
 */
typedef enum SymbolLocation {
    CODE, DATA, UNDEFINED
} SymbolLocation;

/**
 * Represents the type of a symbol - external or entry if it is given as parameter for an appropriate directive,
 * and regular otherwise.
 */
typedef enum SymbolType {
    EXTERNAL, ENTRY, REGULAR
} SymbolType;

typedef struct LinkedList AppearancesList;

/**
 * Represents a symbol's content - its value, location, type, and list of appearances (only used for external symbols).
 */
typedef struct SymbolContent {
    int value;
    SymbolLocation location;
    SymbolType type;
    AppearancesList *appearances;
} SymbolContent;


#endif 
