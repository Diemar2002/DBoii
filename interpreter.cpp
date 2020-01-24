#include <iostream>
#include <fstream>
#include <stack>
#include <string>

#define RIBBONSIZE 32000

char instructions[] = {'>', '<', '^', 'v'};

//Variables globales
std::stack<int> stack;
int progCounter = 0;
int ribbon[RIBBONSIZE];
int ribbonPtr = 0;

// Funciones
int interpret(char ord);

int main(int argc, char* argv[]) {
	if (argc == 1) return 1;
	bool debug = false; // Maneja el modo debug
	int delay = 0; // Tiempo entre instrucciones dentro del modo debug
	if (argc == 3) { // Activa el modo debug si al programa se le pasa un número después del nombre del archivo.
		debug = true;
		delay = atoi(argv[2]);
	} 

	std::fstream src; // Código fuente sin tratar
	std::string code;

	src.open(argv[1]);
		{
		char buff;
		while (src.get(buff)) { // Lee el archivo entero
			for (char o : instructions) { // Comprueba si es alguna de las instrucciones
				if (o == buff) {
					code += buff; // Si la encuentra añade la instrucción al buffer
					break;
				}
			}
		}
		std::cout << code << '\n';
		}
	src.close(); // Se termina de purgar el código
	// Ejecución del código
	int progSize = code.length();

	for (int i = 0; i < progSize; i++)
		if (interpret(code[i]) != 0) {
			return 1;
			std::cout << "Ha sudecido un problema durante la ejecución del programa\n";
		}

	return 0;
}

inline int interpret(const char ord) {
	switch (ord)
	{
	case '>': // Mueve el puntero de la cinta hacia la derecha
		if ((ribbonPtr + 1) >= RIBBONSIZE) return 1;
		ribbonPtr ++;
		break;
	
	case '<': // Mueve el puntero de la cinta hacia la izquierda
		if ((ribbonPtr - 1) < 0) return 1;
		ribbonPtr --;
		break;
	
	case '^': // Añade el número que esté seleccionado en la cinta al stack;
		stack.push(ribbon[ribbonPtr]);
		break;
	
	case 'v': // Elimina un número del stack y lo pone en la celda seleccionada
		ribbon[ribbonPtr] = stack.top();
		stack.pop();
		break;

	default:
		return 1;
		break;
	}

	// Debug
	std::cout << ribbonPtr << ' ' << '\n';

	return 0;
}

#define RIBBONDISPLAY 10
#define STACKDISPKAY 10

inline void render() {
	std::cout << "\033[2J;\033[1;1H"; // Limpia la pantalla y establece el cursor en la posición inicial
	for (int i = 0; i < STACKDISPKAY; i++) {
		if (stack.size())
	}
}
