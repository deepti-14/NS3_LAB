set terminal pdf
set output "lab5.pdf"
set title "Congestion control and retransmission"
set xlabel "time"
set ylabel "packets"
plot "seventh.cwnd" using 1:2 with lines title "old congestion window" lw 2, "seventh.cwnd" using 1:3 with lines title "new congestion window" lw 2