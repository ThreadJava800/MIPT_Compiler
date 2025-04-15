# Compiler

It is a simple compiler of abstract language, which supports:
1. Arithmetics
2. Logic functions (and, or, not)
3. Terminal output
4. IF-THEN-ELSE

## Build
Implementation uses [flex](https://github.com/westes/flex) and [GNU Bison](https://www.gnu.org/software/bison/) as well as [boost library](https://www.boost.org/). so in order to build the project, you must have them installed.

```bash
mkdir build
cd build
cmake ..
cd ..
make
```

## Run
Example:
```bash
./compiler --input ../test.txt --graph-dump graph.png --interpret
```

More information can be found by running this command:
```bash
./compiler --help
```

To generate llvm IR run:
```bash
./compiler --input ../example/test.txt --output o.ll
```

To create executable from generated llvm IR:
```bash
clang++ o.ll
```
