# $Id: Makefile,v 1.5 2015-01-30 15:23:45-08 - - $
# Karen Lee kglee

JAVASRC    = airport.java treemap.java misc.java
SOURCES    = ${JAVASRC} Makefile README
MAINCLASS  = airport
CLASSES    = ${JAVASRC:.java=.class}
JARCLASSES = ${CLASSES} treemap\$$tree.class
JARFILE    = airport
LISTING    = Listing.ps

all : ${JARFILE}

${JARFILE} : ${CLASSES}
	echo Main-class: ${MAINCLASS} >Manifest
	jar cvfm ${JARFILE} Manifest ${JARCLASSES}
	- rm Manifest
	chmod +x ${JARFILE}

%.class : %.java
	- cid $<
	javac $<

clean :
	- rm ${JARCLASSES}

spotless : clean
	- rm ${JARFILE}

ci : ${SOURCES}
	- checksource ${SOURCES}
	cid + ${SOURCES}

lis : ${SOURCES}
	mkpspdf ${LISTING} ${SOURCES}

submit : ${SOURCES}
	submit cmps012b-wm.w16 asg3 ${SOURCES}
	
again : ${SOURCES}
	gmake --no-print-directory spotless ci all lis

