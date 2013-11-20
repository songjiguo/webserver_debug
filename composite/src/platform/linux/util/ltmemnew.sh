#!/bin/sh

./cos_loader \
"c0.o, ;*fprr.o, ;mm.o, ;boot.o, ;print.o, ;\
\
!l.o,a1;!va.o,a2;!mpool.o,a3;!te.o,a3;!sm.o,a4;!e.o,a4;!buf.o,a5;!tp.o,a6;!stat.o,a25;\
\
(!p0.o=exe_cb_pt.o),a10'p3 e600 s0 d120';\
(!p1.o=exe_cb_pt.o),a11'p4 e400 s0 d120';\
(!p2.o=exe_cb_pt.o),a10'p3 e7500 s0 d120';\
(!p3.o=exe_cb_pt.o),a12'p6 e22800 s0 d120';\
(!p4.o=exe_cb_pt.o),a10'p3 e300 s0 d120';\
(!p5.o=exe_cb_pt.o),a13'p8 e800 s0 d120';\
(!p6.o=exe_cb_pt.o),a14'p9 e6300 s0 d120';\
(!p7.o=exe_cb_pt.o),a15'p10 e5000 s0 d120';\
\
(!sh0.o=exe_cb_sh.o),'s50000 n20000 a0';(!sh1.o=exe_cb_sh.o),'s5000 n20000 r2 a0';(!sh2.o=exe_cb_sh.o),'s50000 n20000 r96 a0';\
(!sh3.o=exe_cb_sh.o),'s5000 n20000 r32 a0';(!sh4.o=exe_cb_sh.o),'s50000 n20000 r125 a0';(!sh5.o=exe_cb_sh.o),'s50000 n20000 r96 a0';\
(!sh6.o=exe_cb_sh.o),'s50000 n20000 r32 a0';(!sh7.o=exe_cb_sh.o),'s50000 n20000 r96 a0';(!sh8.o=exe_cb_sh.o),'s500000 n20000 a0';\
\
(!sh9.o=exe_cb_sh.o),'s50000 n20000 r96 a0';\
(!sh10.o=exe_cb_sh.o),'s50000 n20000 r96 a0';\
(!sh11.o=exe_cb_sh.o),'s50000 n20000 r96 a0';\
(!sh12.o=exe_cb_sh.o),'s50000 n20000 r96 a0';\
\
!exe_cb_sbc.o, :\
\
c0.o-fprr.o;\
fprr.o-print.o|[parent_]mm.o;\
l.o-fprr.o|mm.o|print.o;\
te.o-sm.o|print.o|fprr.o|mm.o|va.o;\
mm.o-print.o;\
e.o-sm.o|fprr.o|print.o|mm.o|l.o|va.o;\
stat.o-sm.o|te.o|fprr.o|l.o|print.o|e.o;\
boot.o-print.o|fprr.o|mm.o;\
sm.o-print.o|fprr.o|mm.o|boot.o|va.o|l.o|mpool.o;\
buf.o-boot.o|sm.o|fprr.o|print.o|l.o|mm.o|va.o|mpool.o;\
mpool.o-print.o|fprr.o|mm.o|boot.o|va.o|l.o;\
tp.o-sm.o|buf.o|print.o|te.o|fprr.o|mm.o|va.o|mpool.o;\
va.o-fprr.o|print.o|mm.o|l.o|boot.o;\
\
p0.o-te.o|fprr.o|print.o|sh9.o|sm.o|buf.o|va.o|mm.o;\
p1.o-te.o|fprr.o|print.o|sh9.o|sm.o|buf.o|va.o|mm.o;\
p2.o-te.o|fprr.o|print.o|sh9.o|sm.o|buf.o|va.o|mm.o;\
p3.o-te.o|fprr.o|print.o|sh9.o|sm.o|buf.o|va.o|mm.o;\
p4.o-te.o|fprr.o|print.o|sh9.o|sm.o|buf.o|va.o|mm.o;\
p5.o-te.o|fprr.o|print.o|sh9.o|sm.o|buf.o|va.o|mm.o;\
p6.o-te.o|fprr.o|print.o|sh9.o|sm.o|buf.o|va.o|mm.o;\
p7.o-te.o|fprr.o|print.o|sh9.o|sm.o|buf.o|va.o|mm.o;\
\
sh9.o-fprr.o||print.o|[calll_]sh10.o|[callr_]sh11.o|sm.o|buf.o|va.o|mm.o|l.o|te.o;\
sh10.o-fprr.o||print.o|[calll_]exe_cb_sbc.o|[callr_]sh12.o|sm.o|buf.o|va.o|mm.o|l.o|te.o;\
sh11.o-fprr.o||print.o|[calll_]sh12.o|[callr_]sh0.o|sm.o|buf.o|va.o|mm.o|l.o|te.o;\
sh12.o-fprr.o||print.o|[calll_]exe_cb_sbc.o|[callr_]sh1.o|sm.o|buf.o|va.o|mm.o|l.o|te.o;\
\
sh0.o-fprr.o||print.o|[calll_]sh1.o|[callr_]sh2.o|sm.o|buf.o|va.o|mm.o|l.o|te.o;\
sh1.o-fprr.o||print.o|[calll_]sh3.o|[callr_]sh4.o|sm.o|buf.o|va.o|mm.o|l.o|te.o;\
sh2.o-fprr.o||print.o|[calll_]sh4.o|[callr_]sh5.o|sm.o|buf.o|va.o|mm.o|l.o|te.o;\
sh3.o-fprr.o||print.o|[calll_]exe_cb_sbc.o|[callr_]sh6.o|sm.o|buf.o|va.o|mm.o|l.o|te.o;\
sh4.o-fprr.o||print.o|[calll_]sh6.o|[callr_]sh7.o|sm.o|buf.o|va.o|mm.o|l.o|te.o;\
sh5.o-fprr.o||print.o|[calll_]sh7.o|[callr_]exe_cb_sbc.o|sm.o|buf.o|va.o|mm.o|l.o|te.o;\
sh6.o-fprr.o||print.o|[calll_]exe_cb_sbc.o|[callr_]sh8.o|sm.o|buf.o|va.o|mm.o|l.o|te.o;\
sh7.o-fprr.o||print.o|[calll_]sh8.o|[callr_]exe_cb_sbc.o|sm.o|buf.o|va.o|mm.o|l.o|te.o;\
sh8.o-fprr.o||print.o|[calll_]exe_cb_sbc.o|[callr_]exe_cb_sbc.o|sm.o|buf.o|va.o|mm.o|l.o|te.o;\
\
exe_cb_sbc.o-sm.o|buf.o|va.o|mm.o|print.o\
" ./gen_client_stub

#mpd.o-sm.o|cg.o|fprr.o|print.o|te.o|mm.o|va.o;\
#!mpd.o,a5;
#[print_]trans.o
