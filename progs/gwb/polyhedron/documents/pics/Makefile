.PHONE:
	all
BASESK=base.sk
BASETEX=base.tex
BASEPDF=base.pdf

CUBESK=cube.sk
CUBETEX=cube.tex
CUBEPDF=cube.pdf

CUBEDERSK=tricube.sk equitricube.sk trapecube.sk\
parallelcube.sk rectcube.sk squarecube.sk pentacube.sk hexacube.sk\
f2ebox.sk f2vbox.sk f2nbox.sk\
f3vbox.sk f3ebox.sk\
fenbox.sk fe1ebox.sk fe2ebox.sk fe3ebox.sk\
e2nbox.sk
CUBEDERTEX=tricube.tex equitricube.tex trapecube.tex\
parallelcube.tex rectcube.tex squarecube.tex pentacube.tex hexacube.tex\
f2ebox.tex f2vbox.tex f2nbox.tex\
f3vbox.tex f3ebox.tex\
fenbox.tex fe1ebox.tex fe2ebox.tex fe3ebox.tex\
e2nbox.tex
CUBEDERPDF=tricube.pdf equitricube.pdf trapecube.pdf\
parallelcube.pdf rectcube.pdf squarecube.pdf pentacube.pdf hexacube.pdf\
f2ebox.pdf f2vbox.pdf f2nbox.pdf\
f3vbox.pdf f3ebox.pdf\
fenbox.pdf fe1ebox.pdf fe2ebox.pdf fe3ebox.pdf\
e2nbox.pdf

SOLIDSK=solidcube.sk
SOLIDSUBSK=firstsubcube.sk secondsubcube.sk
SOLIDTEX=solidcube.tex
SOLIDSUBTEX=firstsubcube.tex secondsubcube.tex
SOLIDPDF=solidcube.pdf
SOLIDSUBPDF=firstsubcube.pdf secondsubcube.pdf

SKFILES=${BASESK} ${CUBESK} ${CUBEDERSK} ${SOLIDSK} ${SOLIDSUBSK}
TEXFILES=${BASETEX} ${CUBETEX} ${CUBEDERTEX} ${SOLIDTEX} ${SOLIDSUBTEX}
PDFFILES=${CUBEPDF} ${CUBEDERPDF} ${SOLIDPDF} ${SOLIDSUBPDF}
#${CUBEDERSK}:${CUBESK}
	
#${SOLIDSUBTEX} ${SOLIDSUBSK}:${SOLIDSK}
	

default:all
all: ${TEXFILES}
	
%.tex: %.sk 
	sketch  $^ -T> $@
%.pdf: %.tex
	xelatex $^
clean:
	rm ${TEXFILES} ${PDFFILES}
tex:
	@echo ${TEXFILES}
sk:
	@echo ${SKFILES}
