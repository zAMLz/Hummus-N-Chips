
# ____   ___   ____ ____    __  __       _         __ _ _      
#|  _ \ / _ \ / ___/ ___|  |  \/  | __ _| | _____ / _(_) | ___ 
#| | | | | | | |   \___ \  | |\/| |/ _` | |/ / _ \ |_| | |/ _ \
#| |_| | |_| | |___ ___) | | |  | | (_| |   <  __/  _| | |  __/
#|____/ \___/ \____|____/  |_|  |_|\__,_|_|\_\___|_| |_|_|\___|
#                                                              

########################################################

LATEX     = pdflatex --shell-escape
DOC_DIR   = ./.docfiles/

HP_SPEC   = hummusplus_spec.tex
HP_DEP    = ${addprefix ${DOC_DIR},${HP_SPEC}}

TEX_SRC   = ${HP_SPEC}
TEX_AUX   = ${TEX_SRC:.tex=.aux}
TEX_LOG   = ${TEX_SRC:.tex=.log}
TEX_OUT   = ${TEX_SRC:.tex=.out}

MINT_TEMP = ${addprefix _minted-,${TEX_SRC}}
MINT_DIR  = ${MINT_TEMP:.tex=/}

########################################################

# Build the doc files
docs: hummusplus_spec.pdf

hummusplus_spec.pdf: ${HP_DEP}
	cd ${DOC_DIR}
	${LATEX} ${DOC_DIR}${HP_SPEC}

clean:
	-rm ${TEX_AUX} ${TEX_LOG} ${TEX_OUT}
	-rm -rf ${MINT_DIR}

spotless: clean
	-rm ${TEX_SRC:.tex=.pdf}