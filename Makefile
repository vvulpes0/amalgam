.SUFFIXES :
.SUFFIXES : .c .o

.PATH : $(.CURDIR)/src
.PATH : $(.CURDIR)/src/include
.PATH : $(.CURDIR)/src/bmatrix
.PATH : $(.CURDIR)/src/finsa
.PATH : $(.CURDIR)/src/smonoid
.PATH : $(.CURDIR)/src/uilist

CFLAGS+= -I $(.CURDIR)/src/include
LDFLAGS+= -L$(.OBJDIR)

.c.o :
	$(CC) $(CFLAGS) -o $(.TARGET) -c $(.IMPSRC)

.PHONY : all
all : doc test

.PHONY : doc
doc : Doxyfile
	cd $(.CURDIR) && doxygen

test : test.o libamalgam.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o ${.TARGET} ${.ALLSRC:M*.o} -lamalgam

libamalgam.a : libsmonoid.a libfinsa.a libbmatrix.a libuilist.a
	libtool -static ${LDFLAGS} -o ${.TARGET} ${.ALLSRC:M*.a}

libbmatrix.a : bx_reachability.o bx_mul.o bx_mvmul.o bx_vmmul.o
libbmatrix.a : bx_emul.o bx_add.o bx_copy.o
libbmatrix.a : bx_cycle.o bx_identity.o bx_transpose.o bx_free.o bx_eq.o
	libtool -static ${LDFLAGS} -o ${.TARGET} ${.ALLSRC:M*.o}

libfinsa.a : fi_smonoid.o fi_free.o
	libtool -static ${LDFLAGS} -o ${.TARGET} ${.ALLSRC:M*.[ao]}

libsmonoid.a : sm_istf.o sm_isf.o
libsmonoid.a : sm_istk.o sm_isk.o sm_istd.o sm_isd.o sm_istgd.o sm_isgd.o
libsmonoid.a : sm_istriv.o sm_issemilat.o sm_isband.o sm_isda.o
libsmonoid.a : sm_issf.o sm_ispt.o sm_isltriv.o sm_isrtriv.o
libsmonoid.a : sm_eggbox.o sm_lrel.o sm_rrel.o sm_free.o
	libtool -static ${LDFLAGS} -o ${.TARGET} ${.ALLSRC:M*.o}

libuilist.a : ui_merge.o ui_intersect.o ui_has_intersect.o
libuilist.a : ui_copy.o ui_free.o ui_eq.o
	libtool -static ${LDFLAGS} -o ${.TARGET} ${.ALLSRC:M*.o}

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

fi_free.o : fi_free.c finsa.h bmatrix.h uilist.h
fi_smonoid.o : fi_smonoid.c finsa.h bmatrix.h uilist.h

sm_eggbox.o : sm_eggbox.c smonoid.h bmatrix.h uilist.h
sm_free.o : sm_free.c smonoid.h uilist.h
sm_isband.o : sm_isband.c smonoid.h uilist.h
sm_isd.o : sm_isd.c smonoid.h uilist.h
sm_isda.o : sm_isda.c smonoid.h uilist.h
sm_isf.o : sm_isf.c smonoid.h uilist.h
sm_isgd.o : sm_isgd.c smonoid.h uilist.h
sm_isk.o : sm_isk.c smonoid.h uilist.h
sm_isltriv.o : sm_isltriv.c smonoid.h uilist.h
sm_ispt.o : sm_ispt.c smonoid.h
sm_isrtriv.o : sm_isltriv.c smonoid.h uilist.h
sm_issemilat.o : sm_issemilat.c smonoid.h
sm_issf.o : sm_issf.c smonoid.h uilist.h
sm_istd.o : sm_istd.c smonoid.h uilist.h
sm_istf.o : sm_istf.c smonoid.h uilist.h
sm_istgd.o : sm_istgd.c smonoid.h uilist.h
sm_istk.o : sm_istk.c smonoid.h uilist.h
sm_istriv.o : sm_istriv.c smonoid.h uilist.h
sm_lrel.o : sm_rrel.c smonoid.h finsa.h bmatrix.h uilist.h
sm_rrel.o : sm_rrel.c smonoid.h finsa.h bmatrix.h uilist.h

ui_copy.o : ui_copy.c uilist.h
ui_eq.o : ui_eq.c uilist.h
ui_free.o : ui_free.c uilist.h
ui_has_intersect.o : ui_has_intersect.c uilist.h
ui_intersect.o : ui_intersect.c uilist.h
ui_merge.o : ui_merge.c uilist.h
