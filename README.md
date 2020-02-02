# DBoii
My first esotheric programming lenguaje. It's based on Brainfuck but with greather functionality. It has two main parts. The first is the memory ribbon in which signed integer numbers can be stored. The second part is the stack. This stack can be accessed from anywhere in the program and it can also store signed integer numbers.
## Manual
### These instructions control the stack and ribbon pointer.

- ```<``` Moves the ribbon pointer to the left.
- ```>``` Moves the ribbon pointer to the right.
- ```#``` This instruction takes one argument and moves the ribbon pointer to the argument value.

```>>><<``` Moves the ribbon pointer three positions to the right and two positions to the left.

```#31```   Moves the ribbon pointer to the cell number 31 (the first cell is 1, not 0)

- ```^``` Pushes the currently selected ribbon number to the stack. This operations doesn't modify the cell.
- ```v``` Pops the first element from the stack to the currently selected cell on the ribbon.

###  These instructions modify directly the values on the cells.

- ```42``` Puts the number into the current cell. It can be any number even negatives.
- ```I``` Increments the current cell value by 1.
- ```D``` Decrements the current cell value by 1.

```512II``` Puts 512 on the current cell and increments the value by 1 two times. The final value is 514.

### These instructions manage the IO.

- ```,``` Asks the user for input. The input can be integers or text. When it's a number the program directly puts the inserted value into the current cell. If it's text then the ASCII values are put into adjacent cells.
- ```.``` Outputs the current cell value into the terminal.
- ```"Sample text"``` Outputs the text between quotes into the terminal. It automatically puts a \n character.

### Math

- ```
