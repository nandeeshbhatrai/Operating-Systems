# Name: Nandeesh Bhatrai
# Roll: 2203121

gcc swap.c -o swap
objdump -d swap > objdump.log

echo -n "add   " > freq_count
grep -wc "add" objdump.log >> freq_count

echo -n "mov   " >> freq_count
grep -wc "mov" objdump.log >> freq_count

echo -n "push   " >> freq_count
grep -wc "push" objdump.log >> freq_count

echo -n "pop   " >> freq_count
grep -wc "pop" objdump.log >> freq_count

echo -n "pushq   " >> freq_count
grep -wc "pushq" objdump.log >> freq_count

echo "Here is the expected output:"
cat freq_count
