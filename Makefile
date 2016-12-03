# Makefile for the smash program
CC = g++
CFLAGS = -g -Wall -Wno-write-strings
CCLINK = $(CC)
OBJS = smash.o commands.o signals.o Job.o
RM = rm -f
# Creating the  executable
smash: $(OBJS)
	$(CCLINK) -o smash $(OBJS)
# Creating the object files
Job.o: Job.cpp Job.h 
commands.o: commands.cpp commands.h
smash.o: smash.cpp commands.h signals.h
signals.o: signals.cpp signals.h
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.* smash

# DO NOT DELETE
