/**

  Notes about the source code structure etc.:

* Following the ISO C99 Standard
  This program should be quite portable, but no guarantees! Tested with GCC version 4.7

* Using K&R brace style
* Assuming a maximum of 96 columns for all source files (adjust your editor accordingly!)

* Files:
 • "definitions.h" contains all the globally useful definitions, and #INCLUDEs like <stdbool.h>
 • "list.c" (along with "list.h") contains our linked-list implementation (actually a cyclic
   graph!). It's the only file that uses dynamic memory allocation, so it also defines xmalloc()
 • "parser.c" has parse_file_to_list(), which constructs the list of ASM commands to execute
 • "simulator.c" consumes that list (graph!) and actually does all the real job
 • "error.c" handles error codes, defined in "error.h".
 • "main.c" only initialises the program and calls functions defined in the aforementioned files

* "definitions.h" gets #INCLUDEd by all other source files (*.c)
* All header files have a proper "guard" definition, to protect against multiple inclusions
* Environment is supposed to #DEFINE DEBUG if and only if we have a debug build
* We are using assertions (<assert.h>) and C99 booleans (<stdbool.h>)
* Using the "static" keyword wherever possible for implementation hiding

**/

#include "definitions.h"
#include "info_list.h"


int main(int argc, char *argv[])
{
#ifdef DEBUG
    printf("This is a debug build (assertions enabled).\n\n");
#else
    #ifdef NDEBUG
    printf("This is a release build (assertions and debugging symbols disabled).\n\n");
    #endif
#endif

	if (argc != 3)
        error(E_INSUF_ARGS, ACTION_PRINTMSG_AND_EXIT);

	//TODO Parse initialization file
	FILE *cfgfile = fopen(argv[1], "r");
	if (cfgfile == NULL)
        error(E_FILE_NOT_F, ACTION_PRINTMSG_AND_EXIT);
    fprintf(stderr, "Parsing given configuration file %s:\n", argv[1]);
    parse_cfg_file(cfgfile);
    print_cfg_options();
    fclose(cfgfile);

    fprintf(stdout, "\n\n");


	//assert(NUM_OF_FLO_REG % 2 == 0);
    fprintf(stdout, "Parsing instructions file %s:\n", argv[2]);
	FILE *asmfile = fopen(argv[2], "r");

	if (asmfile == NULL)
        error(E_FILE_NOT_F, ACTION_PRINTMSG_AND_EXIT);

    list *L = parse_file_to_list(asmfile);
    //fclose(asmfile);

    info_list *iL = execute_list_loop(L);// The simulation takes place here (emitting diagnostic
                                         // messages etc.) and its results are stored in iL for
                                         // proper display...

    //printf("\nA manifestation of our power:\n");
    //process_list(L);    // for now, this just prints the (cleaned-up) input to the console
    //printf("printed by simulator processing R-type commands from the list\n");


    // USE iL here!
    // TODO: make outfile path an option
    //FILE *outfile = fopen("/tmp/huge-and-totally-random-filename.txt", "wr");
    //print_all(outfile, iL, 100);

    //free(asmfile);
	//fclose(outfile);
	return 0;
}
