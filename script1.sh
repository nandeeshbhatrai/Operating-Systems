# part i

echo "Executing syst_call.c"
gcc -O0 -o syst_call syst_call.c
echo
# execution completed

# part ii

echo "Running strace ..."
strace -e trace='write' ./syst_call 2> strace_out.txt
echo
# stored the output of strace in a text file

# part iii
echo -n "Number of write calls = "
grep -c 'write' ./strace_out.txt
echo
echo "These lines call the write() call"
grep 'write' ./strace_out.txt
echo

echo "Alternatively, We can use pipe like this: strace -e trace='write' ./syst_call 2 | grep 'write'"

strace -e trace='write' ./syst_call 2 | grep 'write'
echo

# We get:
# 3 calls for n = 1
# 5 calls for n = 2
# 7 calls for n = 3
# and so on...
#
# This is because, there is a print outside loop and 2 print statements inside loop
# running for n times
# hence, we have number of write() calls = 1 + 2*n

# Also, we get 0 number of write() calls with func_call_only, because there are no print function called inside it.
