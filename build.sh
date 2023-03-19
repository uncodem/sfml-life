mkdir -p build/
gcc -c icon.c
g++ *.cpp -c -Wall -Wextra -std=c++11 -pedantic
g++ *.o -o build/sflife -O3 -lsfml-window -lsfml-graphics -lsfml-system
