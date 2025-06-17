# MTEL

Virtual machine for its own language - MTEL(**M**a**T**hematical **E**xtensible **L**anguage)

# Getting Started
## Request
- cmake

## Install

```bash
git clone https://github.com/RedTimeAlex/MTEL.git
cd MTEL 
cmake -B build
cmake --build build
```

# Using
At the moment MTEL is not work properly. However, you can play with 2 whole commands of the language:
- add
- printInt
## Example
File [example.mtel](mtel-example/example.mtel):
```
1243 892 add 570 add 720 540 add add printInt
```
Let's break down the code. First, the numbers 1243 and 892 are pushed onto the stack. Then the add command adds the last two numbers on the stack and pushes the number 2135 onto it, then the number 570 is pushed onto the stack, and so on. "printInt" only prints the last number from the stack.
Read more on [wiki](https://en.wikipedia.org/wiki/Reverse_Polish_notation)
