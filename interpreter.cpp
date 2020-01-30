#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

#define RIBBONSIZE 32000

char instructions[] = {'>', '<', '^', 'v', '+', '-', '.', 'I', 'D'};

// Funciones
int interpret(char ord);
void render();

//Variables globales
int progCounter = 0;
int ribbon[RIBBONSIZE];
int ribbonPtr = 0;

// Stack
template<typename T>
class Stack {
	private:
	T *arr;
	int ptr = 0;
	int maxInstances;
	int __size = 0;
	public:
	Stack(const int maxInstances) {
		arr = new T[maxInstances];
		this->maxInstances = maxInstances;
		__size = 0;
	}
	
	~Stack() {
		delete[] arr;
	}

	Stack& operator=(const Stack& other) {
		if (other == this) return this;
		this->arr = new T[other.maxInstances];
		memcpy(arr, other.arr, other.maxInstances);
		__size = other.__size;
		ptr = other.ptr;
		return this;
	}

	void push(const T item) {
		if (__size < maxInstances) {
            if (__size != 0) ptr ++;
			arr[ptr] = item;
			__size ++;
		}
	}

	T pop() {
		if (__size > 0) {
            int buff = arr[ptr];
			arr[ptr] = 0;
			__size --;
            ptr --;
			return buff;
		}
		return 0;
	}

	T peek() {
		if (__size > 0)
			return arr[ptr];
		return 0;
	}

	T operator[](const int dir) {
		return arr[dir];
	}

	int size() {
		return __size;
	}
};

static Stack<int> stck(100);


int main(int argc, char* argv[]) {
	if (argc == 1) return 1;
	bool debug = false; // Maneja el modo debug
	unsigned int delay = 0; // Tiempo entre instrucciones dentro del modo debug
	if (argc == 3) { // Activa el modo debug si al programa se le pasa un número después del nombre del archivo.
		debug = true;
		delay = atoi(argv[2]);
		std::cout.flush();
		std::cout << "\033[2J\033[1;4f\n\n";
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
		}
	src.close(); // Se termina de purgar el código
	// Ejecución del código
	int progSize = code.length();

	for (progCounter = 0; progCounter < progSize; progCounter++)
		if (interpret(code[progCounter]) != 0) {
			std::cout << "Ha sudecido un problema durante la ejecución del programa\n";
			return 1;
		} else {
			if (debug) {
				render();
				usleep(delay * 1000);
			}
		}

	return 0;
}

// Intérprete de ódenes
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
		stck.push(ribbon[ribbonPtr]);
		break;
	
	case 'v': // Elimina un número del stack y lo pone en la celda seleccionada
		ribbon[ribbonPtr] = stck.pop();
		break;

	case 'I': // Suma uno a la celda seleccionada
		ribbon[ribbonPtr] ++;
		break;
	
	case 'D': // Resta uno a la celda seleccionada
		ribbon[ribbonPtr] --;
		break;

	case '.': // Muestra el valor de la celda en pantalla
		std::cout << ribbon[ribbonPtr] << '\n';
		break;

	case '+': { // Suma los dos primeros valores del stack y los introduce en el stack
		if (stck.size() < 2) break;
		int num1 = stck.pop();
		int num2 = stck.pop();
		stck.push(num1 + num2);
		break;
	}

	default:
		return 1;
		break;
	}

	// Debug

	return 0;
}

#define RIBBONDISPLAY 10
#define STACKDISPKAY 10

inline void render() {
	std::cout.flush();
	std::cout << "\033[s\033[1;1f\033[2K"; // Limpia la pantalla y establece el cursor en la posición inicial
	// Renderizado del stack
	int stackSize = stck.size();
	for (int i = 0; i < STACKDISPKAY; i++) {
		if (i >= stackSize) break;
		std::cout << stck[i] << ' ';
	}

	//Renderizado de la cinta
	std::cout << "\033[E";
	int from = (ribbonPtr < RIBBONDISPLAY/2 ? 0:(ribbonPtr-RIBBONDISPLAY/2));
    for (int i = 0; i<RIBBONDISPLAY; i++) {
        bool isptr = (i+from == ribbonPtr);
        std::cout << (isptr ? "\033[93m":"") << ribbon[i+from] << " " << (isptr ? "\033[0m":"");
    }
	std::cout << "\n\033[u";
	
}
