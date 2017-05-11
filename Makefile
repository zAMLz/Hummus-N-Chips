
# __  __       _         __ _ _      
#|  \/  | __ _| | _____ / _(_) | ___ 
#| |\/| |/ _` | |/ / _ \ |_| | |/ _ \
#| |  | | (_| |   <  __/  _| | |  __/
#|_|  |_|\__,_|_|\_\___|_| |_|_|\___|
#                                    

DOCS_DIR  = DOCS
CHIP_DIR  = CHIPS_COMPILER
HASM_DIR  = HUMMUS_ASSEMBLER
HSIM_DIR  = HUMMUS_SIMULATOR

MAKEARGS  =  --no-print-directory -C

#################################################

# Build everything!!!
all: beans simmus chips docs

# Builds the documentation and cleans it up
docs:
	+${MAKE} ${MAKEARGS} ${DOCS_DIR}

chips:
	+${MAKE} ${MAKEARGS} ${CHIP_DIR}

beans:
	+${MAKE} ${MAKEARGS} ${HASM_DIR}

simmus:
	+${MAKE} ${MAKEARGS} ${HSIM_DIR}

clean:
	+${MAKE} ${MAKEARGS} ${HASM_DIR} clean
	+${MAKE} ${MAKEARGS} ${HSIM_DIR} clean
	+${MAKE} ${MAKEARGS} ${CHIP_DIR} clean
	+${MAKE} ${MAKEARGS} ${DOCS_DIR} clean

spotless:
	+${MAKE} ${MAKEARGS} ${HASM_DIR} spotless
	+${MAKE} ${MAKEARGS} ${HSIM_DIR} spotless
	+${MAKE} ${MAKEARGS} ${CHIP_DIR} spotless
	#+${MAKE} ${MAKEARGS} ${DOCS_DIR} spotless
