#!/bin/sh

./cos_loader \
"c0.o, ;llboot.o, ;*fprr.o, ;mm.o, ;print.o, ;boot.o, ;\
!pfr.o, ;!mpool.o,a3;!sm.o,a4;!l.o,a1;!te.o,a3;!e.o,a4;!buf.o,a5;!bufp.o, ;!tp.o,a6;!rfs.o, ;!ramfs_rec_test1.o,a10;!ramfs_rec_test2.o, ;!fi.o, ;!va.o,a2;!unique_map.o, :\
\
c0.o-llboot.o;\
fprr.o-print.o|[parent_]mm.o|[faulthndlr_]llboot.o;\
mm.o-print.o|[parent_]llboot.o|[faulthndlr_]llboot.o;\
boot.o-print.o|fprr.o|mm.o|llboot.o;\
\
l.o-fprr.o|mm.o|print.o;\
te.o-sm.o|print.o|fprr.o|mm.o|va.o;\
e.o-sm.o|va.o|fprr.o|print.o|mm.o|l.o;\
sm.o-print.o|va.o|fprr.o|mm.o|boot.o|l.o|mpool.o;\
pfr.o-fprr.o|sm.o|mm.o|l.o|print.o|boot.o;\
buf.o-sm.o|va.o|fprr.o|boot.o|print.o|l.o|mm.o|mpool.o;\
bufp.o-sm.o|fprr.o|print.o|l.o|mm.o|va.o|mpool.o|buf.o;\
mpool.o-print.o|va.o|fprr.o|mm.o|boot.o|l.o;\
tp.o-sm.o|buf.o|print.o|va.o|te.o|fprr.o|mm.o|mpool.o|bufp.o;\
va.o-fprr.o|print.o|mm.o|l.o|boot.o;\
\
fi.o-sm.o|va.o|fprr.o|print.o|mm.o|te.o;\
\
rfs.o-sm.o|va.o|fprr.o|print.o|mm.o|buf.o|bufp.o|l.o|e.o|unique_map.o|pfr.o;\
unique_map.o-sm.o|va.o|fprr.o|print.o|mm.o|l.o|e.o|buf.o|bufp.o;\
ramfs_rec_test2.o-sm.o|va.o|fprr.o|print.o|mm.o|buf.o|bufp.o|l.o|rfs.o|e.o;\
ramfs_rec_test1.o-sm.o|va.o|fprr.o|print.o|mm.o|buf.o|bufp.o|l.o|rfs.o|e.o|ramfs_rec_test2.o|te.o|pfr.o\
" ./gen_client_stub
