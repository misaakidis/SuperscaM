CC=gcc
CFLAGS=-std=c99 -w -s -fomit-frame-pointer
LIBS=-lm
HEADERS=$(wildcard *.h)
OBJECTS= cdb.o config_parser.o dictionary.o errors.o functional_units.o info_list.o instruction_set.o list.o main.o output_list.o parser.o printer.o registerFile.o reservation_stations.o simulator.o

%.o: %.c $(HEADERS)
	@$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

SuperscaM: $(OBJECTS)
	@$(CC) $(CFLAGS) $(LIBS) $(OBJECTS) -o $@

	@echo -e "\e[01;34mBuild was successful.\e[00m"
	@make clean


clean:
	@rm -f *.o
	@echo -e "\e[01;33mObject files were removed.\e[00m"
