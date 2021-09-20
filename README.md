# k-XORSAT

Implementing WalkSat to solve k-XORSAT problem, and also, using MiniZinc to solve this problem.

## Overview

k-XORSAT is a type of CSPs. In k-XORSAT, the constraints oblige the XOR of k Boolean variables, or their negations must equal zero. It is somehow similar to k-SAT, where the OR of k Boolean variables must be True (Abbe and Montanari 2015).

In this project, I have implemented WalkSAT to solve the problem for a set of variables as an inexact approach.

Also, for an exact approach for solving the problem, I have used [MiniZinc](https://www.minizinc.org/).

## Reuirements

To run the exact method, you need to have MiniZinc installed on your system. See the instructions [here](https://www.minizinc.org/).

## Usage

By using a python script you can run the exact method:
```bash
python script.py
```
For the inexact method you need to have C++ on your system and then enter the commands below (in linux):
```bash
g++ walksat.cpp
./a.out
```
