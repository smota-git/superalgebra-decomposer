# Superalgebra Decomposer

## Overview

**Superalgebra Decomposer** is a C++ program for symbolic normalization of polynomial expressions in generators of the Lie algebra **sl(3)**.

The program rewrites polynomial expressions into a fixed canonical ordering of generators by repeatedly applying predefined rewriting rules derived from commutation relations.

The canonical ordering used by the current implementation is:

```text
E31, E21, E32, H12, H23, E23, E12, E13
```

The additional symbol `H13` may also appear in input expressions. It is not treated as an independent basis generator and is automatically expanded as:

```text
H13 = H12 + H23
```

Although the current version is configured for `sl(3)`, the normalization algorithm itself is not tied to this particular algebra. The algebra-specific part of the program is concentrated mainly in:

```text
convert()
unconvert()
polynomial::transform()
```

The functions `convert()` and `unconvert()` define the internal ordering of generators. The method `polynomial::transform()` defines the rewriting rules for unordered neighbouring generators. By modifying these parts, the program can be adapted to a different ordering of the same generators or, in principle, to another algebra with a compatible rewriting structure.

## Repository structure

```text
README.md
LICENSE
superalgebra-decomposer.sln

superalgebra-decomposer/
    superalgebra-decomposer.vcxproj
    superalgebra-decomposer.vcxproj.filters
    superalgebra-decomposer.cpp
    superalgebra-decomposer-rec.cpp
    monomial.h
    polynomial.h
    input.txt

examples/
    input.txt
    manual-input-example.png

mathematica/
    commutation-sl3.nb
    commutation_sl3_readable.pdf
```

## Features

- symbolic normalization of polynomial expressions
- canonical ordering of `sl(3)` generators
- support for powers of generators
- automatic expansion of `H13`
- iterative implementation
- recursive implementation for comparison
- interactive keyboard input
- input from text file
- text log output
- optional LaTeX output
- optional PDF generation using `pdflatex`
- step-by-step trace of the normalization process

## Mathematical background

The implemented relations are based on the standard matrix representation of the Lie algebra `sl(3)`.

The generators `Eij` correspond to elementary matrices. The generators `H12`, `H23` and `H13` belong to the Cartan subalgebra, with:

```text
H13 = H12 + H23
```

The Wolfram Mathematica notebook included in this repository constructs commutators from the matrix representation. The C++ program then uses the corresponding rewriting rules to move neighbouring generators into the selected canonical order.

## Input format

The program can read expressions either interactively or from `input.txt`.

For file input, each line represents one monomial:

```text
coefficient generator exponent generator exponent ...
```

Example:

```text
1 E12 2 H13 2
3 H23 3 E31 2 E23 1
-2 E32 1 E31 2
```

This represents:

```text
E12^2 * H13^2
+ 3 * H23^3 * E31^2 * E23
- 2 * E32 * E31^2
```

The file `examples/input.txt` contains an example input. A copy of `input.txt` may also be placed directly in the Visual Studio project folder, because the current code opens:

```cpp
ifstream file("input.txt");
```

## Output

The iterative version can generate:

- console output
- text log
- LaTeX source
- PDF document containing the step-by-step normalization process

PDF generation requires a working LaTeX installation with the `breqn` package.

## Building in Visual Studio

The repository contains a Visual Studio solution:

```text
superalgebra-decomposer.sln
```

Open this file in Visual Studio and build the project in the usual way.

The Visual Studio project is configured for the iterative implementation by default.

## Iterative and recursive implementations

The repository contains two implementations of the normalization algorithm.

### Iterative version

```text
superalgebra-decomposer/superalgebra-decomposer.cpp
```

This is the main version. It performs one rewriting step at a time and can generate a detailed step-by-step trace in text and LaTeX/PDF form.

### Recursive version

```text
superalgebra-decomposer/superalgebra-decomposer-rec.cpp
```

The recursive version is included mainly for comparison and verification of the iterative algorithm.

To build the recursive version in Visual Studio:

1. Remove `superalgebra-decomposer.cpp` from the project.
2. Add `superalgebra-decomposer-rec.cpp` to the project.
3. In `polynomial.h`, switch the marked code sections from the iterative implementation to the recursive one by commenting/uncommenting the lines marked:

```cpp
// iterative version
// recursive version
```

In the iterative mode, `decompose()` returns a `polynomial`.

In the recursive mode, `decompose()` has return type `void` and modifies the current object directly.

Alternatively, the two `.cpp` files may be temporarily renamed, but changing the included source file in the Visual Studio project is cleaner.

## Mathematica notebook

The file:

```text
mathematica/commutation-sl3.nb
```

contains a Wolfram Mathematica notebook used to generate commutation relations from the matrix representation of `sl(3)`.

Since GitHub does not render Mathematica notebooks directly, a readable PDF transcription is included:

```text
mathematica/commutation_sl3_readable.pdf
```

The PDF also contains a complete list of rewriting relations used by the C++ implementation.

## Current limitations

The current implementation uses fixed-size arrays. The limits are defined in `monomial.h`:

```cpp
const int monom_length = 10;
const int poly_length_init = 10;
const int p_len_reserved = 40;
```

These values can be changed manually if larger expressions are needed.

A future version may replace fixed-size arrays by dynamic containers such as `std::vector`.

## Development environment

The project was developed and tested using:

- Microsoft Visual Studio
- C++17
- MiKTeX / pdfLaTeX
- Wolfram Mathematica

## License

This project is released under the MIT License.
