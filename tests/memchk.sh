make clean && cmake ../Projet/ && make
valgrind --leak-check=full --show-reachable=yes --trace-children=yes ./test_calculate
