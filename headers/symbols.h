/**
 * Includes definitions which represent symbols and their characteristics.
 */
#ifndef SYMBOLS_H
#define SYMBOLS_H

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

/**
 * Represents a list of an external symbol's appearances in a file.
 * 
 * The additional typedef is used to avoid issues of types being not yet defined when used in structs.
 */
typedef struct LinkedList AppearancesList;

/**
 * Represents a symbol's content - its value, location, type, and list of appearances (only used for external symbols).
 */
typedef struct SymbolContent {
    int value;
    SymbolLocation location;
    SymbolType type;
    /* a list of addresses of memory words in which the symbol appears - only used for external symbols */
    AppearancesList *appearances;
} SymbolContent;

#endif 
