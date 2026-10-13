.SUFFIXES : .tex .pdf .ht .md

#-------------------------------------------------------------------------------
# En cours de construction. Voir le LISEZ-MOI général
#-------------------------------------------------------------------------------

HTML_DIR = docs/html
DOXY_CFG = config.doxy

PANDOC_CSS = pandoc.css
PANDOC_CSL = csl/mla-notes.csl

PDFLATEX = pdflatex
DOXYGEN = doxygen
PANDOC = pandoc
DOCS_PDF = index.pdf \
           vue-globale-part.pdf \
           presentation-part.pdf \
           outils-part.pdf \
           boot-part.pdf \
           affichage-part.pdf \
           inter-part.pdf \
           appel-systeme-part.pdf \
           systeme-fichiers-part.pdf \
           memoire-part.pdf \
	   taches-part.pdf \
           synchronisation-part.pdf \
           registres-part.pdf \
           userland-part.pdf

DOCS_HT = $(DOCS_PDF:.pdf=.ht)
DOCS_MD = $(DOCS_PDF:.pdf=.md)

all : $(DOCS_PDF) dox md

dox :
	$(DOXYGEN) $(DOXY_CFG)

ht : $(DOCS_HT)

md : $(DOCS_MD)

livre : ManuX.pdf

.tex.pdf :
	$(PDFLATEX) $<

# Pour générer du html en fonction des sources LaTeX (pas utilisé a priori)
.tex.ht :
	$(PANDOC) --metadata pagetitle=\"$(basename $<)\" --css=$(PANDOC_CSS) $< -f latex -t html  -s -o $(HTML_DIR)/$(basename $<).html
	cp $(PANDOC_CSS) $(HTML_DIR)

# Pour générer du markdown (ensuite utilisé par mkdocs pour construire des pages html !)
# Il est placé dans le répertoire docs
.tex.md :
	$(PANDOC) --standalone --citeproc --metadata pagetitle=\"$(basename $<)\" --csl=$(PANDOC_CSL) --css=$(PANDOC_CSS) $< -f latex -t markdown_strict -s -o docs/$(basename $<).md

# Pour créer le site web
build :
	mkdocs build -d ManuX

serve :
	mkdocs serve

install :
	scp -r ManuX root@www.manu-chaput.net:/var/www/manux.manu-chaput.net/

clean :
	rm -f *.aux *.log *.toc *.idx *.out *.bcf *.bbl *.blg $(DOCS_PDF) docs/*.md  docs/*.html



