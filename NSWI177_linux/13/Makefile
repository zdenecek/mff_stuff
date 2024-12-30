COURSES = \
	NTVY017\
	NPRG062\
	NTVY016\
	NDMI011\
	NJAZ090\
	NTIN071\
	NDMI002\
	NJAZ072\
	NSWI120\
	NMAI069\
	NAIL062\
	NJAZ074\
	NMAI054\
	NSWI141\
	NMAI057\
	NPRG045\
	NMAI058\
	NMAI059\
	NSWI170\
	NJAZ070\
	NTIN061\
	NTIN060\
	NDBI025\
	NJAZ091\
	NSWI177\
	NPRG030\
	NTVY015\
	NTVY014\
	NPRG031\

.PHONY: all clean

ht = $(addprefix public_html/, $(addsuffix .html,$(COURSES)))
mt = $(addprefix src/, $(addsuffix .meta,$(COURSES)))

all: public_html/index.html public_html/main.css  $(ht)

clean: 
	rm -f _menu.html public_html/index.html public_html/main.css public_html/N*.html 

_menu.html: $(mt)
	bin/make_menu.sh $? >_menu.md
	pandoc _menu.md >_menu.html

public_html/main.css: main.css
	cp main.css public_html/main.css

public_html/index.html: _menu.html
	bin/build.sh index

public_html/N%.html: src/N%.md src/N%.meta _menu.html
	bin/build.sh "N$*" --metadata-file "src/N$*.meta"


