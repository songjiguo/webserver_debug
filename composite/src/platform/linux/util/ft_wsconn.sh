#!/bin/sh

#;!rotar.o,a7

#httpt.o-sm.o|l.o|print.o|fprr.o|mm.o|buf.o|[server_]rotar.o|te.o|va.o|pfr.o;\
#rotar.o-sm.o|fprr.o|print.o|mm.o|buf.o|l.o|eg.o|va.o|initfs.o|pfr.o;\
# httperf --server=10.0.2.8 --port=200 --uri=/fs/bar --num-conns=7000

./cos_loader \
"c0.o, ;llboot.o, ;*fprr.o, ;mm.o, ;print.o, ;boot.o, ;\
\
!sm.o,a1;!mpool.o, ;!buf.o,a5;!bufp.o, ;!va.o,a2;!mpd.o,a5;!tif.o,a5;!tip.o, ;\
!port.o, ;!l.o,a4;!te.o,a3;!tnet.o, ;!eg.o,a5;\
!stconn.o, '1:1:10:/bind:0:%d/listen:255';\
!pfr.o, ;!httpt.o,a8;!rfs.o, ;!initfs.o,a3;!unique_map.o, ;!popcgi.o, ;!fi.o, :\
\
c0.o-llboot.o;\
fprr.o-print.o|[parent_]mm.o|[faulthndlr_]llboot.o;\
mm.o-[parent_]llboot.o|print.o;\
boot.o-print.o|fprr.o|mm.o|llboot.o;\
\
l.o-fprr.o|mm.o|print.o;\
te.o-sm.o|print.o|fprr.o|mm.o|va.o;\
sm.o-print.o|fprr.o|mm.o|boot.o|va.o|l.o|mpool.o;\
pfr.o-sm.o|fprr.o|mm.o|print.o|va.o|l.o|boot.o;\
buf.o-boot.o|sm.o|fprr.o|print.o|l.o|mm.o|va.o|mpool.o;\
bufp.o-sm.o|fprr.o|print.o|l.o|mm.o|va.o|mpool.o|buf.o;\
mpool.o-print.o|fprr.o|mm.o|boot.o|va.o|l.o;\
va.o-fprr.o|print.o|mm.o|l.o|boot.o;\
\
stconn.o-sm.o|print.o|fprr.o|mm.o|va.o|l.o|httpt.o|te.o|[from_]tnet.o|buf.o|bufp.o|eg.o|pfr.o;\
\
httpt.o-sm.o|l.o|print.o|fprr.o|mm.o|buf.o|bufp.o|[server_]rfs.o|te.o|va.o|pfr.o;\
initfs.o-fprr.o|print.o|mm.o|va.o|l.o|pfr.o|buf.o|bufp.o;\
\
tnet.o-sm.o|fprr.o|mm.o|print.o|l.o|te.o|eg.o|[parent_]tip.o|port.o|va.o|buf.o|bufp.o|pfr.o;\
tip.o-sm.o|[parent_]tif.o|va.o|fprr.o|print.o|te.o|l.o|eg.o|buf.o|bufp.o|mm.o|pfr.o;\
tif.o-sm.o|print.o|fprr.o|mm.o|l.o|va.o|te.o|eg.o|buf.o|bufp.o|pfr.o;\
port.o-sm.o|l.o|print.o|pfr.o;\
\
rfs.o-sm.o|fprr.o|print.o|mm.o|buf.o|bufp.o|l.o|va.o|unique_map.o|eg.o|pfr.o;\
unique_map.o-sm.o|fprr.o|print.o|mm.o|l.o|va.o|buf.o|bufp.o;\
popcgi.o-sm.o|fprr.o|print.o|mm.o|buf.o|bufp.o|va.o|l.o|rfs.o|eg.o|te.o;\
\
fi.o-sm.o|fprr.o|print.o|mm.o|va.o|te.o;\
\
mpd.o-sm.o|boot.o|fprr.o|print.o|te.o|mm.o|va.o|pfr.o;\
eg.o-sm.o|fprr.o|print.o|mm.o|l.o|va.o|pfr.o\
" ./gen_client_stub


