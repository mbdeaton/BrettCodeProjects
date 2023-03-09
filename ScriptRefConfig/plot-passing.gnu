reset

load 'linestyle_defs.gnu'

col = 5
col_label = "total passing"

set xdata time
set timefmt "%Y-%m-%d"
set yrange [0:*]
set xlabel "Date"
set ylabel col_label
p \
'emails-p_ccs.dat' u 1:col w l ls 1 t 'P CCS',\
'emails-p_fts.dat' u 1:col w l ls 2 t 'P FTS',\
'emails-l_ccs.dat' u 1:col w l ls 3 t 'L CCS',\
'emails-l_fts.dat' u 1:col w l ls 4 t 'L FTS',\
'emails-4xx.dat' u 1:col w l ls 5 t '4xx',\
'emails-ind.dat' u 1:col w l ls 6 t 'ind',\
'emails-met.dat' u 1:col w l ls 7 t 'met'
