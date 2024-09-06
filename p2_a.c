/*

We first store the output of strace inside a lof file "strace_out.txt", then apply grep to get the desired output

"Alternatively, We can use pipe like this: strace -e trace='write' ./syst_call 2 | grep 'write'"

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

*/