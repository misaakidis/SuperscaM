#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED


/** Prototypes of non-static functions: */
list *parse_file_to_list(FILE *);
char *xstrtok(char *line, char *delims);
int xatoi(char token[]);
int strToRegNum(char* token);

//static bool isFirstLabel = true;

#endif // PARSER_H_INCLUDED
