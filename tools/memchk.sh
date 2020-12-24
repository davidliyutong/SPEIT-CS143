make clean && cmake ../Projet/ && make
valgrind --leak-check=full --show-reachable=yes --trace-children=yes ./build/lac ./Projet/playground/memchk.lac
