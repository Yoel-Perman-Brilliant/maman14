/**
 * Includes the prototype for create_files, which is responsible for creating the output files (.ob, .ext and .ent)
 * for an assembly file based on its filled requirements.
 */

#ifndef MAMAN14_OUTPUT_CREATOR_H
#define MAMAN14_OUTPUT_CREATOR_H

/**
 * Creates the output files for an assembly file based on its filled requirements.
 * Will only create .ext and .ent files if they will not be empty.
 * 
 * @param file_name    the extensionless file name
 * @param requirements a pointer to the requirements of the file
 * @return 1 if an error has occurred, 0 otherwise
 */
int create_files(char file_name[], Requirements *requirements);

#endif
