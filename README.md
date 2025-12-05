## PHYLOG Compiler  
A mini-compiler for the educational PHYLOG programming language, built using FLEX for lexical analysis and BISON for syntax and semantic analysis.

## Overview  
This project implements a compiler for PHYLOG, a teaching language defined by the professor in the compilation module.  
The compiler covers the main phases of compilation:

- **Lexical analysis** (FLEX)  
- **Syntax analysis** (BISON, LALR grammar)  
- **Semantic analysis** (type checking, symbol table handling)  
- **Symbol table implementation** with hashing  
- **Intermediate code generation** using quadruples  
- **Basic optimization routines**  
- **Error reporting** with line and column numbers  

The repository also includes test files and documentation describing the PHYLOG language specification.

## Requirements  
The compiler was built and tested with the following versions:  
- Bison 2.4.1  
- Flex 2.5.4  
- GCC 3.4.5   
Ensure FLEX, BISON, and GCC are installed and available in your system PATH.  

## Files

### Main source files:  
- lexical.l — Lexical analyzer specification  
- syntaxique.y — Bison grammar for syntax and semantic rules  
- ts.c, ts.h — Symbol table implementation  
- semantique.c, semantique.h — Semantic analysis  
- quad.h — Quadruple structure for intermediate code  
- optimisation.h — Optimization routines  
- GenerationCodeObjet.h — Object code generation  
### Test files:  
- test.txt  
- test1.txt
- test-opti.txt  
### Documentation:  
Projet Compil 2024_2025. docx.pdf (PHYLOG language description)

## How to Compile  
Inside the project directory, run the following commands in order:  
- Generate the lexical analyzer:  
flex lexical.l  
- Generate the syntax and semantic analyzer:  
bison -d syntaxique.y  
- Compile all components with GCC:  
gcc lex.yy.c syntaxique.tab.c ts.c semantique.c -o pgm.exe -lfl -ly  
This will produce an executable file named pgm.exe.  
## How to Run  
Provide a PHYLOG source file as input:  
pgm.exe test-opti.txt  

You may replace test-opti.txt with any of the provided test files or your own program written in the PHYLOG language.

## Testing

The repository includes several test files demonstrating:  
- Variable declarations  
- Constants  
- Arithmetic and logical expressions  
- Conditions (IF / ELSE)  
- Loops (FOR)  
- Input and output instructions  
- Error handling and syntax cases  
- Optimization examples  

These files can be used to validate that each phase of the compiler works correctly.

## Documentation  
A detailed PDF explaining the complete PHYLOG language specification is included in the repository:  
- Grammar rules  
- Types and identifiers  
- Operators  
- Control structures  
- I/O instructions  
- Examples  

This document serves as the reference for all language features supported by the compiler.

##Notes

PHYLOG is an educational language designed specifically for this university project.
The goal of the project is to practice the construction of a compiler using FLEX, BISON, and standard compilation techniques.
