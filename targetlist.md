- ~~function `Calculate` should be defined~~
- ~~The `if` `else` `0=`, in short conditional branches.~~
- ~~how to type a string ?~~
- ~~come up with a better solution of runtime error(clever throw)?~~
- ~~expand the symtable / vmtable~~
- ~~compilation of declaration~~
- ~~output of debug info~~
- ~~UTF-8 support?~~ Yes
- ~~interaction mode~~
- ~~recursive and loop?~~

- ~~(xxx)<number> allowed ? <number>(xxx)?~~
- ~~calculate.c, c'est vraiment automate à pile~~
- ~~syntax of lac~~


## Error throw: 👌
- Check the program for return value
- exit()

## Debug 👌
- analex
- tables
- stacks
- execution

## Use variables - 'variable' 👌
keyword `variable`(function de base)

donner value: `123 x !`

read variable: `x @`

## Use recurse 👌
recurse is a basic funciton 

put the cfa of current function to VM.

the length of `StkReturn` is no longer 1, could have return stack overflow

## Use loop 👌
- `while` (start) `loop` (end) `break`

think about it 

## Use vec
keyword ` {N} vec` (vector)

## " Bonjour" " Alain !" catenate


## Restrictions:

~~\ 注释必须强制换一行~~
~~程序应该以换行符结尾，最好以换行符开头~~

- ~~添加向量Vec类，考虑一下字符串可能可以用更优雅的表达~~
- ~~函数内的变量~~
- ~~单行解释的时候也要考虑字符串开头的问题~~
- 增加include功能
- ~~catenate 研究如何实现~~
- ~~完成factorielle.lac的实现~~

- ~~调整debug模式下输出信息的次序~~

- 将factorielle的执行体现在rapport里
- 重新介绍strtable->vmtable
- 将catenate的执行体现在rapport里