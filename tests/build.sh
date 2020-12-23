LEX_H=${PROJET_DIR}/lex/*.h
LEX_C=${PROJET_DIR}/lex/*.c

UTILS_H=${PROJET_DIR}/utils/*.h
UTILS_C=${PROJET_DIR}/utils/*.c

COMMON_H=${PROJET_DIR}/common/*.h
COMMON_C=${PROJET_DIR}/common/*.c

RUNTIME_H=${PROJET_DIR}/runtime/*.h
RUNTIME_C=${PROJET_DIR}/runtime/*.c

GCC_MACRO=

mkdir build
read -r -p "[ Info ] Enable Debug Mode? [Y/n] " input

case $input in
[yY][eE][sS] | [yY])
  echo "[ Info ] Debug=Yes"
  GCC_FLAGS=-DDEBUG
  ;;

[nN][oO] | [nN])
  echo "[ Info ] Debug=No"
  ;;

*)
  echo "[ Error ] Invalid input..."
  exit 1
  ;;
esac

echo "[ Info ] Build projet1"
gcc ${PROJET_DIR}/demo/projet1-analex.c ${LEX_H} ${LEX_C} ${UTILS_H} ${UTILS_C} ${COMMON_H} ${COMMON_C} -o build/projet1 -Wall ${GCC_FLAGS}

echo "[ Info ] Build projet2"
gcc ${PROJET_DIR}/demo/projet2-calculate.c ${UTILS_H} ${UTILS_C} ${COMMON_H} ${COMMON_C} -o build/projet2 -Wall ${GCC_FLAGS}

echo "[ Info ] Build projet3"
gcc ${PROJET_DIR}/demo/projet3-symtable.c ${UTILS_H} ${UTILS_C} ${COMMON_H} ${COMMON_C} ${RUNTIME_H} ${RUNTIME_C} -o build/projet3 -Wall ${GCC_FLAGS}

echo "[ Info ] Build projet4"
gcc ${PROJET_DIR}/demo/projet4-lac.c ${LEX_H} ${LEX_C} ${UTILS_H} ${UTILS_C} ${COMMON_H} ${COMMON_C} ${RUNTIME_H} ${RUNTIME_C} -o build/projet4 -Wall ${GCC_FLAGS}

echo "[ Info ] Build color"
gcc ${PROJET_DIR}/tools/color.c ${LEX_H} ${LEX_C} ${UTILS_H} ${UTILS_C} ${COMMON_H} ${COMMON_C} -o build/color -Wall ${GCC_FLAGS}

echo "[ Info ] Build lac"
gcc ${PROJET_DIR}/lac.c ${LEX_H} ${LEX_C} ${UTILS_H} ${UTILS_C} ${COMMON_H} ${COMMON_C} ${RUNTIME_H} ${RUNTIME_C} -o build/lac -Wall ${GCC_FLAGS}

echo "[ Info ] Writing test code to projet4.lac"
echo -e ": incr 1 + ;\n: 2+. incr incr . ;\n7 8 + 2+." >build/projet4.lac

echo "[ Info ] Writing test code to factorielle.lac"
echo -e "\ Fichier \" factorielle.lac\"\n\" uu\"\n( Ce fichier est un \"exemple\" étudié pour tester\nl'analyseur lexical écrit en phase 1 du projet)\n\ BlahBlahBlah\n: 0= 0 = ;\n: 1- 1 - ;\n: cr 1 + ;\n: fact ( n -- n!)\n    dup 0= \n    if\n        drop 1\n    else \n        dup 1- recurse *\n    then ;\n\n: go ( n -- )\n    \" Factorielle\" count type\n    dup .\n    \" vaut : \n\" count type\n    fact . cr ;\n\n6 go\n" >build/factorielle.lac

echo "[ Info ] Build finished"
