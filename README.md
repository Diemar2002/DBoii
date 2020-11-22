# DBoii
My first esotheric programming lenguaje. It's based on Brainfuck but with greater functionality. It has two main parts. The first is the memory ribbon in which signed integer numbers can be stored. The second part is the stack. This stack can be accessed from anywhere in the program and it can also store signed integer numbers.
## Getting started
The main file is ```interpreter.cpp```. To compile it execute ```g++ interpreter.cpp -o interpreter``` on the terminal. With the compiled to execute a ```.db``` file the fillowing structure has to be used.

```./interpreter file.db 50```
- ```./interpreter``` interpreter.
- ```file.db``` file to execute.
- ```50``` delay between instructions. It also activates the debug option, where memory information can be seen.

## Manual
### Stack and ribbon pointer controllers.

- ```<``` Moves the ribbon pointer to the left.
- ```>``` Moves the ribbon pointer to the right.
- ```#``` This instruction takes one argument and moves the ribbon pointer to the argument value.

```>>><<``` Moves the ribbon pointer three positions to the right and two positions to the left.

```#31```   Moves the ribbon pointer to the cell number 31 (the first cell is 1, not 0)

- ```^``` Pushes the currently selected ribbon number to the stack. This operations doesn't modify the cell.
- ```v``` Pops the first element from the stack to the currently selected cell on the ribbon.

###  Direct modification of the values of the cells.

- ```42``` Puts the number into the current cell. It can be any number even negatives.
- ```I``` Increments the current cell value by 1.
- ```D``` Decrements the current cell value by 1.

```512II``` Puts 512 on the current cell and increments the value by 1 two times. The final value is 514.

### IO.

- ```,``` Asks the user for input. The input can be integers or text. When it's a number the program directly puts the inserted value into the current cell. If it's text then the ASCII values are put into adjacent cells.
- ```.``` Outputs the current cell value into the terminal.
- ```"Sample text"``` Outputs the text between quotes into the terminal. It automatically puts a \n character.

### Math

- ```+``` Adds the two first numbers on the stack and puts the result on the stack.
- ```-``` Subtracts the two first numbers on the stack and puts the result on the stack.
- ```*``` Multiplies the two first numbers on the stack and puts the result on the stack.
- ```/``` Divides the two first numbers on the stack and puts the result on the stack.
- ```%``` Applies the modulo operation the two first numbers on the stack and puts the result on the stack.

### Flow modifiers

- ```(...)``` If statement. To add an else the statement this syntax has to be used ```("if")|("else")
- ```[...]``` Loop statement. 

To use conditionals the following structures have to be used.
-```(...)``` Executes the inside code if the value on the current cell is not zero.
-```?(...)``` Basic conditionals. The default value is =.
  - ```=?(...)``` Checks if the value on the current cell is equal to the first value on the stack.
  - ```G?(...)``` Checks if the value on the stack is greater than the value on the current cell.
  - ```L?(...)``` Checks if the first value on the stack is lower than the value on the current cell.

These rules can also be applied to the loop statement.

### Miscellaneous

- ```!``` Reverses the stack.
- ```\n``` The carriage return character can be used as a separation between instructions. This can avoid unpredictable behaviour.
- ```abcdefghijklmnopqrstuvwxyz``` All the keys that aren't listed here can be used as comments. Thus they'll be ignored by the interpreter.

```
47^32^-
34
```
The output value on the cell will be 34. And the only value on the stack will be 15. This is the solution to this problem.
```
47^32^-34
```
The output on the cell will be -34 and the values on the stack will be 47 and 32. As seen here the ```\n``` character can be used as a code separator.

### Code examples
```"Hello world!"``` Prints the typical hello world on the console.
```1>1..>10^0?[<^^<^>+v<v>.>I]``` Prints indifenitely the fibbonacci's series.
```
"Type the number"
,[^>10^%<^>^/<v#4I#1]
"The separate numbers are: "
#4[D<v.>]
```
Prints the separated characters of an input. Ex: 1514 -> 1 5 1 4.
This github has all the test files used in the examples.

