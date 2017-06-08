
# __  __       _         __ _ _      
#|  \/  | __ _| | _____ / _(_) | ___ 
#| |\/| |/ _` | |/ / _ \ |_| | |/ _ \
#| |  | | (_| |   <  __/  _| | |  __/
#|_|  |_|\__,_|_|\_\___|_| |_|_|\___|
#                                    

DOCS_DIR  = DOCS

CHIP_DIR  = CHIPS_COMPILER
CHIP      = chips

HASM_DIR  = HUMMUS_ASSEMBLER
HASM      = beans

HSIM_DIR  = HUMMUS_SIMULATOR
HSIM      = simmus

MAKEARGS  =  --no-print-directory -C

#################################################

# Build everything!!!
all: ${HASM} ${HSIM} ${CHIP} docs clean

# Builds the documentation and cleans it up
docs:
	+${MAKE} ${MAKEARGS} ${DOCS_DIR}

${CHIP}:
	+${MAKE} ${MAKEARGS} ${CHIP_DIR}

${HASM}:
	+${MAKE} ${MAKEARGS} ${HASM_DIR}
	mv ./${HASM_DIR}/${HASM} ./${HASM}

${HSIM}:
	+${MAKE} ${MAKEARGS} ${HSIM_DIR}

clean:
	+${MAKE} ${MAKEARGS} ${HASM_DIR} clean
	+${MAKE} ${MAKEARGS} ${HSIM_DIR} clean
	+${MAKE} ${MAKEARGS} ${CHIP_DIR} clean
	+${MAKE} ${MAKEARGS} ${DOCS_DIR} clean

spotless: clean
	- rm ./${HASM} ./${HSIM} ./${CHIP}
	# +${MAKE} ${MAKEARGS} ${HASM_DIR} spotless
	# +${MAKE} ${MAKEARGS} ${HSIM_DIR} spotless
	# +${MAKE} ${MAKEARGS} ${CHIP_DIR} spotless
	# +${MAKE} ${MAKEARGS} ${DOCS_DIR} spotless
