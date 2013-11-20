#!/bin/sh

./cos_loader \
"c0.o, ;*fprr.o, ;mm.o, ;print.o, ;schedconf.o, ;st.o, ;bc.o, ;cg.o,a1;boot.o,a4;\
!mpool.o,a3;!hls.o,a7;!hlc.o, ;!l.o,a1;!te.o,a3;!sm.o,a3;!e.o,a3;!stat.o,a25;!va.o,a2:\
\
c0.o-fprr.o;\
fprr.o-print.o|mm.o|st.o|schedconf.o|[parent_]bc.o;\
l.o-fprr.o|mm.o|print.o;\
sm.o-print.o|fprr.o|mm.o|boot.o|va.o|l.o|mpool.o;\
mpool.o-print.o|fprr.o|mm.o|boot.o|va.o|l.o;\
va.o-fprr.o|print.o|mm.o|l.o|boot.o;\
te.o-sm.o|print.o|fprr.o|mm.o|va.o;\
mm.o-print.o;\
e.o-sm.o|fprr.o|print.o|mm.o|l.o|st.o|va.o;\
stat.o-sm.o|te.o|fprr.o|l.o|print.o|e.o;\
st.o-print.o;\
schedconf.o-print.o;\
bc.o-print.o;\
cg.o-fprr.o;\
hlc.o-sm.o|print.o|te.o|fprr.o;\
hls.o-sm.o|print.o|hlc.o|te.o|fprr.o;\
boot.o-print.o|fprr.o|mm.o|cg.o\
" ./gen_client_stub
