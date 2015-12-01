gcc -pedantic -W -Wall -o digenv digenv.c

./digenv.c

-W -Wall turns on many, many useful warnings. These warning messages will often be the first hint that your program has a serious error in it which needs immediate attention. Without the flags, your compiler would just give you your program without letting you know about these potential problems.

-ansi -pedantic makes gcc conform strictly to the C standard; again, extra strictness means less chance of errors going past the compiler undetected.
