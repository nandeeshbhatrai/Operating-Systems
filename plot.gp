LOG_FILE="$1"
if [ ! -f "$LOG_FILE" ]; then
    echo "Log file $LOG_FILE not found!"
    exit 1
fi

# Find unique commands in the log file
commands=$(grep -oP '(?<=Command: ).*?(?=,)' "$LOG_FILE" | uniq)

> data.txt # create text file for data to be plotted

for cmd in $commands; do
    # Count the particular command using grep -c
    count=$(grep -c "Command: $cmd," "$LOG_FILE") 
    echo "$cmd $count" >> data.txt
done

GNUPLOT_SCRIPT="plot_commands.gp"
cat <<EOF > $GNUPLOT_SCRIPT
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'command_freq.png'
set title 'Command Frequency'
set xlabel 'Commands'
set ylabel 'Frequency'
set grid
set yrange [0:*]
set style data histograms
set style fill solid 1.0 border -1
set boxwidth 0.9
plot 'data.txt' using 2:xtic(1) title 'Frequency' lt rgb "orange"
EOF

gnuplot $GNUPLOT_SCRIPT

echo "Command frequency plot generated as 'command_freq.png'."
