
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

#################################################

# Build everything!!!
all: humbler simmus chips docs

# Builds the documentation and cleans it up
docs:
	+${MAKE} -C ${DOCS_DIR} docs

chips:
	+${MAKE} -C ${CHIP_DIR} chips

humbler:
	+${MAKE} -C ${HASM_DIR} humbler

simmus:
	+${MAKE} -C ${HSIM_DIR} simmus

clean:
	+${MAKE} -C ${HASM_DIR} clean
	+${MAKE} -C ${HSIM_DIR} clean
	+${MAKE} -C ${CHIP_DIR} clean
	+${MAKE} -C ${DOCS_DIR} clean

spotless:
	+${MAKE} -C ${HASM_DIR} spotless
	+${MAKE} -C ${HSIM_DIR} spotless
	+${MAKE} -C ${CHIP_DIR} spotless
	+${MAKE} -C ${DOCS_DIR} spotless
