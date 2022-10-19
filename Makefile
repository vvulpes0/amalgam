.SUFFIXES :
.SUFFIXES : .c .o

.PATH : $(.CURDIR)/include/amalgam
.PATH : $(.CURDIR)/include/amalgam/io
.PATH : $(.CURDIR)/src/amclassify
.PATH : $(.CURDIR)/src/bmatrix
.PATH : $(.CURDIR)/src/finsa
.PATH : $(.CURDIR)/src/smonoid
.PATH : $(.CURDIR)/src/uilist
.PATH : $(.CURDIR)/src/io

CFLAGS+= -I$(.CURDIR)/include
CFLAGS+= -I$(.CURDIR)/include/amalgam
LDFLAGS+= -L$(.OBJDIR)

.c.o :
	$(CC) $(CFLAGS) -o $(.TARGET) -c $(.IMPSRC)

.MAIN : amclassify

.PHONY : doc
doc : Doxyfile
	cd $(.CURDIR) && doxygen

amclassify : amclassify.o libamalgam.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o ${.TARGET} ${.ALLSRC:M*.o} -lamalgam
	install -d $(.CURDIR)/bin
	install -m0755 $(.TARGET) $(.CURDIR)/bin

test : test.o libamalgam.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o ${.TARGET} ${.ALLSRC:M*.o} -lamalgam

libamalgam.a : libsmonoid.a libfinsa.a libbmatrix.a libuilist.a
	libtool -static ${LDFLAGS} -o ${.TARGET} ${.ALLSRC:M*.a}

libbmatrix.a : bx_reachability.o bx_mul.o bx_mvmul.o bx_vmmul.o
libbmatrix.a : bx_emul.o bx_add.o bx_copy.o
libbmatrix.a : bx_cycle.o bx_identity.o bx_transpose.o bx_free.o bx_eq.o
	libtool -static ${LDFLAGS} -o ${.TARGET} ${.ALLSRC:M*.o}

libfinsa.a : fi_todot.o fi_issl.o fi_project.o
libfinsa.a : fi_fromatt.o fi_rmeps.o fi_trim.o fi_smonoid.o fi_nerode.o
libfinsa.a : fi_powerset.o fi_restrict.o fi_copy.o fi_free.o
	libtool -static ${LDFLAGS} -o ${.TARGET} ${.ALLSRC:M*.[ao]}

libsmonoid.a : sm_todot.o
libsmonoid.a : sm_istlda.o sm_islda.o
libsmonoid.a : sm_istlltriv.o sm_islltriv.o
libsmonoid.a : sm_istlrtriv.o sm_islrtriv.o
libsmonoid.a : sm_istlj.o sm_islj.o
libsmonoid.a : sm_istlt.o sm_islt.o
libsmonoid.a : sm_ese.o sm_localsm.o sm_generate.o
libsmonoid.a : sm_isacom.o sm_iscom.o sm_istn.o sm_isn.o
libsmonoid.a : sm_istk.o sm_isk.o sm_istd.o sm_isd.o sm_istgd.o sm_isgd.o
libsmonoid.a : sm_istriv.o sm_issemilat.o sm_isband.o sm_isda.o
libsmonoid.a : sm_issf.o sm_ispt.o sm_isltriv.o sm_isrtriv.o
libsmonoid.a : sm_eggbox.o sm_lrel.o sm_rrel.o
libsmonoid.a : sm_freelist.o sm_free.o
	libtool -static ${LDFLAGS} -o ${.TARGET} ${.ALLSRC:M*.o}

libuilist.a : ui_merge.o ui_insert.o ui_intersect.o ui_has_intersect.o
libuilist.a : ui_copy.o ui_free.o ui_eq.o ui_find.o
	libtool -static ${LDFLAGS} -o ${.TARGET} ${.ALLSRC:M*.o}

amclassify.o : amclassify.c amalgam.h smonoid.h finsa.h bmatrix.h uilist.h

bx_add.o : bx_add.c bmatrix.h uilist.h
bx_copy.o : bx_copy.c bmatrix.h uilist.h
bx_cycle.o : bx_cycle.c bmatrix.h uilist.h
bx_emul.o : bx_emul.c bmatrix.h uilist.h
bx_eq.o : bx_eq.c bmatrix.h uilist.h
bx_free.o : bx_free.c bmatrix.h uilist.h
bx_identity.o : bx_identity.c bmatrix.h uilist.h
bx_mul.o : bx_mul.c bmatrix.h uilist.h
bx_mvmul.o : bx_mvmul.c bmatrix.h uilist.h
bx_reachability.o : bx_reachability.c bmatrix.h
bx_transpose.o : bx_transpose.c bmatrix.h uilist.h
bx_vmmul.o : bx_vmmul.c bmatrix.h uilist.h

fi_copy.o : fi_copy.c finsa.h bmatrix.h uilist.h
fi_free.o : fi_free.c finsa.h bmatrix.h uilist.h
fi_fromatt.o : fi_fromatt.c att.h finsa.h bmatrix.h uilist.h
fi_issl.o : fi_issl.c finsa.h bmatrix.h uilist.h
fi_powerset.o : fi_powerset.c finsa.h bmatrix.h uilist.h
fi_project.o : fi_project.c finsa.h bmatrix.h uilist.h
fi_restrict.o : fi_restrict.c finsa.h bmatrix.h uilist.h
fi_smonoid.o : fi_smonoid.c finsa.h bmatrix.h uilist.h
fi_todot.o : fi_todot.c dot.h finsa.h bmatrix.h uilist.h
fi_trim.o : fi_trim.c finsa.h bmatrix.h

sm_eggbox.o : sm_eggbox.c smonoid.h bmatrix.h uilist.h
sm_ese.o : sm_ese.c smonoid.h
sm_free.o : sm_free.c smonoid.h uilist.h
sm_freelist.o : sm_freelist.c smonoid.h
sm_generate.o : sm_generate.c smonoid.h finsa.h bmatrix.h uilist.h
sm_isacom.o : sm_isacom.c smonoid.h uilist.h
sm_isband.o : sm_isband.c smonoid.h uilist.h
sm_iscom.o : sm_iscom.c smonoid.h uilist.h
sm_isd.o : sm_isd.c smonoid.h uilist.h
sm_isda.o : sm_isda.c smonoid.h uilist.h
sm_isgd.o : sm_isgd.c smonoid.h uilist.h
sm_isk.o : sm_isk.c smonoid.h uilist.h
sm_islda.o : sm_islda.c smonoid.h
sm_islj.o : sm_islj.c smonoid.h
sm_islt.o : sm_islt.c smonoid.h
sm_islltriv.o : sm_islltriv.c smonoid.h
sm_islrtriv.o : sm_islrtriv.c smonoid.h
sm_isltriv.o : sm_isltriv.c smonoid.h uilist.h
sm_isn.o : sm_isn.c smonoid.h uilist.h
sm_ispt.o : sm_ispt.c smonoid.h
sm_isrtriv.o : sm_isrtriv.c smonoid.h uilist.h
sm_issemilat.o : sm_issemilat.c smonoid.h
sm_issf.o : sm_issf.c smonoid.h uilist.h
sm_istd.o : sm_istd.c smonoid.h uilist.h
sm_istgd.o : sm_istgd.c smonoid.h uilist.h
sm_istk.o : sm_istk.c smonoid.h uilist.h
sm_istlda.o : sm_istlda.c smonoid.h
sm_istlj.o : sm_istlj.c smonoid.h
sm_istlt.o : sm_istlt.c smonoid.h
sm_istlltriv.o : sm_istlltriv.c smonoid.h
sm_istlrtriv.o : sm_istlrtriv.c smonoid.h
sm_istn.o : sm_istn.c smonoid.h uilist.h
sm_istriv.o : sm_istriv.c smonoid.h uilist.h
sm_localsm.o : sm_localsm.c smonoid.h finsa.h bmatrix.h uilist.h
sm_lrel.o : sm_rrel.c smonoid.h finsa.h bmatrix.h uilist.h
sm_rrel.o : sm_rrel.c smonoid.h finsa.h bmatrix.h uilist.h
sm_todot.o : sm_todot.c dot.h smonoid.h uilist.h

ui_copy.o : ui_copy.c uilist.h
ui_eq.o : ui_eq.c uilist.h
ui_find.o : ui_find.c uilist.h
ui_free.o : ui_free.c uilist.h
ui_has_intersect.o : ui_has_intersect.c uilist.h
ui_insert.o : ui_insert.c uilist.h
ui_intersect.o : ui_intersect.c uilist.h
ui_merge.o : ui_merge.c uilist.h
