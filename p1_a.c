//////////
/* TIME */
//////////


// we can use the bash utility "time" to calculate the time taken
// to run any executable file.
//
// It goes like this:
// >> time <exeName>
// for eg.
//
// >> gcc -o sort sort.c
// >> time ./sort
//
//////OUTPUT//////
//
// Array size = 100000
//
// real	0m4.874s
// user	0m4.867s
// sys	0m0.000s
//
//////////////////

///////////
/* CLOCK */
///////////


// The "clock" function is an external C library function, that can
// be used to get the current time in seconds/mili-seconds/etc.
//
// It can be used to calculate the time taken for execution of any 
// lines of code.
//
// It goes like this:
//
// #include<ctime>
//
// ###your code
//
// start_time = clock()
// ## Code Block
// time_elapsed = clock() - start_time
//
// /////////////////


//////////////////
/* GETTIMEOFDAY */
//////////////////


// "gettimeofday" is also a very useful C function that helps us get
// the current date and time with high precision.
//
// It goes like this:
//
//    struct timeval tv;
//    struct timezone tz;
//    struct tm *today;
//    int zone;
//
//    gettimeofday(&tv,&tz);
//
      /*** get time details ***/
//    today = localtime(&tv.tv_sec);
//   printf("It's %d:%0d:%0d.%d\n",
//            today->tm_hour,
//            today->tm_min,
//            today->tm_sec,
//            tv.tv_usec
//          );
//
//
////OUTPUT////
//It's 15:59:3.676897
//////////////



