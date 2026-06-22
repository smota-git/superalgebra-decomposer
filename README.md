# Superalgebra Decomposer

## Overview

**Superalgebra Decomposer** is a C++ program for symbolic normalization of polynomial expressions in generators of the Lie algebra **sl(3)**.

The program transforms arbitrary polynomial expressions into a canonical ordering of generators by repeatedly applying predefined commutation relations.

The canonical ordering is

```
E31, E21, E32, H12, H23, E23, E12, E13
```

An additional symbol

```
H13
```

may also appear in the input. It is automatically expanded using

```
H13 = H12 + H23
```

and therefore is **not** treated as an independent basis generator.

---

# Repository structure

```
README.md

superalgebra-decomposer.sln

superalgebra-decomposer/
    superalgebra-decomposer.cpp
    superalgebra-decomposer-rec.cpp
    monomial.h
    polynomial.h

examples/
    input.txt
    manual-input-example.png

mathematica/
    commutation-sl3.nb
    commutation_sl3_readable.pdf
```

---

# Features

* symbolic polynomial normalization
* iterative implementation
* recursive implementation
* interactive keyboard input
* input from text file
* optional text log
* optional LaTeX output
* automatic PDF generation using **pdflatex**
* generation of complete step-by-step transformations

---

# Input format

Each line of the input file represents one monomial.

```
coefficient generator exponent generator exponent ...
```

Example

```
E12 2 H13 2
3 H23 3 E31 2 E23 1
-2 E32 1 E31 2

```

corresponds to

```
E12²·H13²
+3·H23³·E31²·E23
-2·E32·E31²
```

The program can also read the same expressions interactively from the keyboard.

---

# Output

The program can generate

* console output
* text log
* LaTeX source
* PDF document containing the complete normalization process

PDF generation requires a working LaTeX installation with the **breqn** package.

---

# Iterative and recursive implementations

The repository contains two implementations of the normalization algorithm.

## Iterative version

```
superalgebra-decomposer/superalgebra-decomposer.cpp
```

This is the main implementation.

It performs the normalization iteratively and supports detailed logging as well as LaTeX/PDF output.

## Recursive version

```
superalgebra-decomposer/superalgebra-decomposer-rec.cpp
```

The recursive implementation is included mainly for comparison and verification of the iterative algorithm.

To build the recursive version:

1. Replace

```
superalgebra-decomposer.cpp
```

by

```
superalgebra-decomposer-rec.cpp
```

in the Visual Studio project (or temporarily rename the files).

2. In

```
polynomial.h
```

switch the marked code sections from the iterative implementation to the recursive one by commenting/uncommenting the lines marked

```cpp
// iterative version
// recursive version
```

The required changes are clearly marked in the source code.

---

# Mathematica notebook

The file

```
commutation-sl3.nb
```

contains a Wolfram Mathematica notebook used to generate the commutation relations automatically from the matrix representation of **sl(3)**.

Since GitHub cannot display Mathematica notebooks directly, a readable transcription is also included:

```
commutation_sl3_readable.pdf
```

---

# Current limitations

The current implementation uses fixed-size arrays.

The limits are defined in

```
monomial.h
```

and can be modified if necessary.

A future version may replace these arrays by dynamic containers (`std::vector`).

---

# Development

The project was developed and tested using

* Microsoft Visual Studio
* C++17
* MiKTeX / pdfLaTeX
* Wolfram Mathematica

---

# License

This project is released under the MIT License.
