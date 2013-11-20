#!/bin/sh

./cos_loader \
"c0.o, ;llboot.o, ;*fprr.o, ;mm.o, ;print.o, ;boot.o, ;\
\
!mpool.o,a3;!trans.o,a6;!sm.o,a4;!l.o,a1;!te.o,a3;!e.o,a4;!stat.o,a25;!buf.o,a5;!bufp.o, ;!tp.o,a6;!microfpu.o, ;!va.o,a2:\
\
c0.o-llboot.o;\
fprr.o-print.o|[parent_]mm.o|[faulthndlr_]llboot.o;\
mm.o-[parent_]llboot.o|print.o;\
boot.o-print.o|fprr.o|mm.o|llboot.o;\
l.o-fprr.o|mm.o|print.o;\
te.o-sm.o|print.o|fprr.o|mm.o|va.o;\
e.o-sm.o|fprr.o|print.o|mm.o|l.o|va.o;\
stat.o-sm.o|te.o|fprr.o|l.o|print.o|e.o;\
sm.o-print.o|fprr.o|mm.o|boot.o|va.o|l.o|mpool.o;\
buf.o-boot.o|sm.o|fprr.o|print.o|l.o|mm.o|va.o|mpool.o;\
bufp.o-sm.o|fprr.o|print.o|l.o|mm.o|va.o|mpool.o|buf.o;\
mpool.o-print.o|fprr.o|mm.o|boot.o|va.o|l.o;\
tp.o-sm.o|buf.o|bufp.o|print.o|te.o|fprr.o|mm.o|va.o|mpool.o;\
va.o-fprr.o|print.o|mm.o|l.o|boot.o;\
trans.o-sm.o|fprr.o|l.o|buf.o|bufp.o|mm.o|va.o|e.o|print.o;\
\
microfpu.o-sm.o|fprr.o|print.o|mm.o|va.o|l.o\
" ./gen_client_stub
