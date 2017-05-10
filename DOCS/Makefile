
LATEX    = pdflatex --shell-escape
DOC_DIR  = ./.docfiles/
HP_SPEC  = hummusplus_spec.tex

TEX_SRC  = ${HP_SPEC}
TEX_AUX  = ${TEX_SRC:.tex=.aux}
TEX_LOG  = ${TEX_SRC:.tex=.log}
TEX_OUT  = ${TEX_SRC:.tex=.out}
MINT_DIR = ${TEX_SRC:.tex=/}

########################################################


all: hummusplus_spec clean

noclean: hummusplus_spec

hummusplus_spec:
	cd ${DOC_DIR}
	${LATEX} ${DOC_DIR}${HP_SPEC}

clean:
	-rm ${TEX_AUX} ${TEX_LOG} ${TEX_OUT}
	echo ${MINT_DIR}
