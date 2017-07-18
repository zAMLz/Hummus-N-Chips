
# _   _                                     ____  _           
#| | | |_   _ _ __ ___  _ __ ___  _   _ ___|  _ \| |_   _ ___ 
#| |_| | | | | '_ ` _ \| '_ ` _ \| | | / __| |_) | | | | / __|
#|  _  | |_| | | | | | | | | | | | |_| \__ \  __/| | |_| \__ \
#|_| |_|\__,_|_| |_| |_|_| |_| |_|\__,_|___/_|   |_|\__,_|___/
#                                                             
# ____  _                 _       _             
#/ ___|(_)_ __ ___  _   _| | __ _| |_ ___  _ __ 
#\___ \| | '_ ` _ \| | | | |/ _` | __/ _ \| '__|
# ___) | | | | | | | |_| | | (_| | || (_) | |   
#|____/|_|_| |_| |_|\__,_|_|\__,_|\__\___/|_|   
#                                              
# __  __       _         __ _ _      
#|  \/  | __ _| | _____ / _(_) | ___ 
#| |\/| |/ _` | |/ / _ \ |_| | |/ _ \
#| |  | | (_| |   <  __/  _| | |  __/
#|_|  |_|\__,_|_|\_\___|_| |_|_|\___|
#                                    

########################################################

CC       = gcc -g -O2 -Wall -Wextra -Wpedantic

README   = README
MKFILE   = Makefile
DEPFILE  = Makefile.dep

MAINFILE = main.c

define SUPPFILE
debug_util.c hplus_asm.c simmus.c
endef

SOURCES  = ${MAINFILE} ${SUPPFILE}

HEADERS  = ${SUPPFILE:.c=.h}
OBJECTS  = ${SOURCES:.c=.o}
EXECBIN  = simmus

########################################################

# Build the HummusPlus Simulator
${EXECBIN}: ${HEADERS} ${OBJECTS}
	${CC} -o ${EXECBIN} ${OBJECTS} 

%.o : %.c
	${CC} -c $<

clean:
	- rm ${OBJECTS}
	- rm vgcore.*
	- rm ./*.dump
	- rm ./*.log

spotless: clean
	- rm ${EXECBIN}          
