
# __  __       _         __ _ _      
#|  \/  | __ _| | _____ / _(_) | ___ 
#| |\/| |/ _` | |/ / _ \ |_| | |/ _ \
#| |  | | (_| |   <  __/  _| | |  __/
#|_|  |_|\__,_|_|\_\___|_| |_|_|\___|
#                                    

DOCS_DIR  = DOCS

#################################################

# Build everything!!!
all: documentation

# Builds the documentation and cleans it up
documentation:
	+${MAKE} -C ${DOCS_DIR} docs
	+${MAKE} -C ${DOCS_DIR} clean

