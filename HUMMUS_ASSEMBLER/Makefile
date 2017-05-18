
# _   _                                     ____  _           
#| | | |_   _ _ __ ___  _ __ ___  _   _ ___|  _ \| |_   _ ___ 
#| |_| | | | | '_ ` _ \| '_ ` _ \| | | / __| |_) | | | | / __|
#|  _  | |_| | | | | | | | | | | | |_| \__ \  __/| | |_| \__ \
#|_| |_|\__,_|_| |_| |_|_| |_| |_|\__,_|___/_|   |_|\__,_|___/
#                                                             
#    _                           _     _           
#   / \   ___ ___  ___ _ __ ___ | |__ | | ___ _ __ 
#  / _ \ / __/ __|/ _ \ '_ ` _ \| '_ \| |/ _ \ '__|
# / ___ \\__ \__ \  __/ | | | | | |_) | |  __/ |   
#/_/   \_\___/___/\___|_| |_| |_|_.__/|_|\___|_|   
#                                                  
# __  __       _         __ _ _      
#|  \/  | __ _| | _____ / _(_) | ___ 
#| |\/| |/ _` | |/ / _ \ |_| | |/ _ \
#| |  | | (_| |   <  __/  _| | |  __/
#|_|  |_|\__,_|_|\_\___|_| |_|_|\___|
#                                    

########################################################

CC      = gcc -g -O2 -Wall -Wextra -Wpedantic

README   = README
MKFILE   = Makefile
DEPFILE  = Makefile.dep

MAINFILE = main.c

SUPP_01  = beans_assembler.c hummus_plus_assembly.c
SUPP_02  = ${SUPP_01} 
SUPPFILE = ${SUPP_02} debug_util.c prep_cmt_sp_dfa.c

SOURCES  = ${MAINFILE} ${SUPPFILE}

HEADERS  = ${SUPPFILE:.c=.h}
OBJECTS  = ${SOURCES:.c=.o}
EXECBIN  = beans

########################################################

# Build the humble HummusPlus Assembler
${EXECBIN}: ${HEADERS} ${OBJECTS} 
	${CC} -o ${EXECBIN} ${OBJECTS}

%.o : %.c
	${CC} -c $<

clean:
	- rm ${OBJECTS}
	- rm ./*.bin
	- rm ./*.log

spotless: clean
	- rm ${EXECBIN}                                               
