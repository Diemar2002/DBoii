#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

#define RIBBONSIZE 32000
#define STACKSIZE 100
#define LOOPSTACK 100

static const char instructions[] = {'>', '<', '^', 'v', '+', '-', '.', 'I', 'D', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
'*', '/', '[', ']', '?', '(', ')', '|', '%', '?', ',', '"', '#', '\n', '=', 'G', 'L', '!'};

// Funciones
int interpret(char ord);
void render();
int getNum();
void gotoNextBlock(const int mode);
int charlen(const char arr[]);

//Variables globales
static int progCounter = 0;
static int ribbon[RIBBONSIZE];
static int ribbonPtr = 0;
static std::string code;
static int progSize;
static bool doDelay = true;

// Flags
static bool f_comparing = false;
static bool f_operating = false;

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
            if (ptr != 0) ptr --;
			return buff;
		}
		return 0;
	}

	T peek() {
		if (__size > 0)
			return arr[ptr];
		return 0;
	}

	T& operator[](const int dir) {
		return arr[dir];
	}

	int size() {
		return __size;
	}
};

static Stack<int> stck(STACKSIZE);
static Stack<int> loopstack(LOOPSTACK);
static Stack<int> comp(LOOPSTACK);
static Stack<int> compMode(LOOPSTACK);


int main(int argc, char* argv[]) {
	if (argc == 1) return 1;
	bool debug = false; // Maneja el modo debug
	unsigned int delay = 0; // Tiempo entre instrucciones dentro del modo debug
	if (argc == 3) { // Activa el modo debug si al programa se le pasa un número después del nombre del archivo
		debug = true;
		delay = atoi(argv[2]);
		std::cout.flush();
		std::cout << "\033[2J\033[1;4f\n\n";
		render(); // Primer renderizado
	} 

	std::fstream src; // Código fuente sin tratar

	src.open(argv[1]);
		{
		char buff;
		bool print = false;
		while (src.get(buff)) { // Lee el archivo entero
			for (char o : instructions) { // Comprueba si es alguna de las instrucciones
				if (o == buff || print) {
					code += buff; // Si la encuentra añade la instrucción al buffer
					if (o == '\"') print = !print;
					break;
				}
			}
		}
		}
	src.close(); // Se termina de purgar el código
	// Ejecución del código
	progSize = code.length();

	for (progCounter = 0; progCounter < progSize; progCounter++) { // Bucle principal que ejecuta el programa
		doDelay = true;
		f_comparing = false;
		f_operating = false;

		if (interpret(code[progCounter]) != 0) { // Ejecuta la instrucción y espera algún error
			std::cout << "Ha ocurrido un error en la línea: " << progCounter << ' ' << code[progCounter];
			return 1;
		} else {
			if (debug) { // Si el debug está activado, entonces muestra el stack y la cinta
				render();
				if (doDelay) usleep(delay * 1000);
			}
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

	case ',': {// Permite al usuario introducir texto o números
		doDelay = false;

		char buff[100];
		std::cin.get(buff, 100);
		std::cin.ignore();
		
		// Comprobar si la entrada es un número
		bool num = true;
		for (char c : buff) {
			if (c == '\0') break;
			if (c < 48 || c > 57) {
				num = false;
				break;
			}
		}

		if (num) {
			ribbon[ribbonPtr] = std::atoi(buff);
		} else {
			int end = charlen(buff) + ribbonPtr;
			for (int i = ribbonPtr; i < end; i++)
				ribbon[i] = buff[i - ribbonPtr];
		}

		break;
	}

	case '+': { // Operaciones básicas. Pone el resultado dentro del stack
	case '-':
	case '*':
	case '/':
	case '%':
		if (ord == '-') {
			if ((progCounter + 1) >= progSize) goto notNumber;
			else if ((code[progCounter + 1] >= 48) && (code[progCounter + 1] <= 57)) {
				ribbon[ribbonPtr] = getNum();
				break;
			}
		}
		notNumber:
		if (stck.size() < 2) break;

		f_operating = true;
		
		int num1 = stck.pop(); // Primer número
		int num2 = stck.pop(); // Segundo número

		switch (ord) {
			case '+':
				stck.push(num2 + num1);
				break;
			case '-':
				stck.push(num2 - num1);
				break;
			case '*':
				stck.push(num2 * num1);
				break;
			case '/':
				stck.push(num2 / num1);
				break;
			case '%':
				stck.push(num2 % num1); 
				break;
		}

		break;
	}

	{
	case '[': // Inicio del bucle
		f_comparing = true;

		loopstack.push(progCounter-1); // Añade el bucle al bucle principal

		if ((progCounter - 1) >= 0) {
			if (code[progCounter - 1] == '?') {
				if ((compMode.peek() == 1) && !(stck.peek() > ribbon[ribbonPtr]))
					gotoNextBlock(1);
				else if ((compMode.peek() == 2) && !(stck.peek() < ribbon[ribbonPtr]))
					gotoNextBlock(1);
				else if ((compMode.peek() != 0) && (stck.peek() == ribbon[ribbonPtr]))
					gotoNextBlock(1);
			} else if (ribbon[ribbonPtr] == 0)
				gotoNextBlock(1);
		}

		break;
	
	case ']': // Final del bucle
		doDelay = false;
		
		if (loopstack.size() > 0) progCounter = loopstack.pop();
		break;

	case '(': { // Inicio de un bloque de código
		doDelay = false;
		f_comparing = true;

		bool comparation = false;

		if ((progCounter - 1) < 0) { // Comprueba si puede existir alguna instrucción detrás de la actual
			if (ribbon[ribbonPtr] == 0) comparation = true; // Si no existe comprueba que el valor actual de la cinta no sea 0
		} else {
			if (code[progCounter - 1] == '?') { // Si anteriormente hay '?' entonces comprueba si el valor de la celda coincide con el primero del stack principal
				if ((compMode.peek() == 1) && (stck.peek() > ribbon[ribbonPtr]))
					comparation = true;
				else if ((compMode.peek() == 2) && (stck.peek() < ribbon[ribbonPtr]))
					comparation = true;
				else if ((compMode.peek() == 0) && (stck.peek() == ribbon[ribbonPtr]))
					comparation = true;
			} else if (ribbon[ribbonPtr] == 0) // De lo contrario comprueba si el valor de la celda no es 0
				comparation = true;
		}

		comp.push(comparation);

		if (!comparation) {
			compMode.pop();
			gotoNextBlock(0);
		}
		
		break;
	}

	case '|': // Caracter para marcar un else
		doDelay = false;

		if ((progCounter - 1) < 0 || (progCounter + 1) >= progSize) break;
		if (code[progCounter - 1] != ')' || code[progCounter + 1] != '(') break;

		if (comp.size() > 0) {
			bool c = comp.pop();
			progCounter ++;
			if (c) gotoNextBlock(0); // Si no se ejecutó el bucle entonces salta al siguiente bloque
		}
		break;

	case ')': // Cierra la comparación
		doDelay = false;

		if ((progCounter + 1) >= progSize) break;
		if (code[progCounter + 1] != '|')
			if (comp.size() > 0) comp.pop();
		break;

	case '?': { // Inicio de la comparación. Establece el modo en el que se hacen las comparacinoes
		if ((progCounter - 1) < 0) break;
		int mode = 0;
		if (code[progCounter - 1] == 'G') mode = 1;
		else if (code[progCounter - 1] == 'L') mode = 2;
		compMode.push(mode);
		f_comparing = true;
		break;
	}

	}

	case '\"': // Imprime en pantalla
		progCounter ++;
		while (true) {
			char buff = code[progCounter++];
			if (buff == '\"') {
				progCounter --;
				break;
			}
		
			std::cout << buff;
		}
		std::cout << '\n';
		break;

	case '#': { // Mueve la celda a la posición indicada
		if (progCounter + 1 >= progSize) break;
		if ((code[progCounter + 1] < 48) || (code[progCounter] > 57)) break;
		progCounter ++;
		int num = getNum() - 1;
		if (num >= 0 && num < RIBBONSIZE) ribbonPtr = num;
		break;
	}

	case '!': { // Invierte el stack
		int steps = stck.size() / 2;
		for (int i = 0; i < steps; i++) {
			int buff_1 = stck[i];
			int buff_2 = stck[stck.size() - i - 1];
			stck[i] = buff_2;
			stck[stck.size() - i - 1] = buff_1;
		}
		break;
	}

	case '0': // Comprueba todos los números para leerlo entero
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		ribbon[ribbonPtr] = getNum(); // Cuando lee el número completo, entonces
	break;

	default:
		doDelay = false;
		break;
	}

	// Debug

	return 0;
}

#define RIBBONDISPLAY 10
#define STACKDISPLAY 10

inline void render() {
	std::cout.flush();
	std::cout << "\033[s\033[1;1f\033[2K"; // Limpia la pantalla y establece el cursor en la posición inicial
	// Renderizado del stack
	int stackSize = stck.size();
	int from = (stackSize < STACKDISPLAY ? 0:(stackSize-STACKDISPLAY));
	for (int i = 0; i < STACKDISPLAY; i++) {
		if (i >= stackSize) break;
		if (((from + i) == (stackSize - 1)) && f_comparing) std::cout << "\033[94m";
		if (((from + i) == (stackSize - 1)) && f_operating) std::cout << "\033[92m";
		std::cout << stck[from + i] << " \033[0m";
	}

	//Renderizado de la cinta
	std::cout << "\033[E";
	from = (ribbonPtr < RIBBONDISPLAY/2 ? 0:(ribbonPtr-RIBBONDISPLAY/2));
    for (int i = 0; i<RIBBONDISPLAY; i++) {
        bool isptr = (i+from == ribbonPtr);
		if (isptr) {
			std::cout << "\033[93m";
			if (f_comparing) std::cout << "\033[94m";
		}
        std::cout << ribbon[i+from] << " " << (isptr ? "\033[0m":"");
    }
	std::cout << "\n\033[u";
	
}

inline int getNum() {
	int num = 0;
	bool negative = false;
	while (true) {
		num *= 10;
		char buff = code[progCounter++];
		if (buff == '-' && !negative) {
			negative = true;
			buff = code[progCounter++];
		}
		if ((int)buff <= 57 && (int)buff >= 48) {
			num += ((int)buff - 48);
		} else {
			progCounter -= 2;
			num /= 10;
			break;
		}
	}
	
	return (negative ? -num:num);
}

void gotoNextBlock(const int mode) {
	char open = 0, close = 0;
	if (mode == 0) { // Comparación
		open = '(';
		close = ')';
	} else if (mode == 1) { // Bucle
		open = '[';
		close = ']';
	}

	if (open == 0 || close == 0) return;

	int cnt = 0; // Varable que cuenta el número de veces que se han abierto paréntesis para saber el nivel del paréntesis de cierre
			while (true) { // Salta hasta el siguiente ')' que esté en su nivel
			char buff = code[progCounter++];
				if (buff == open)
					cnt ++;
				else if (buff == close) {
					cnt --;
					if (cnt <= 0) {
						progCounter --;
						break;
					}
				}
			}

}

int charlen(const char arr[]) {
	int cnt = 0;
	while (true) {
		if (arr[cnt] == '\0') return cnt;
		cnt ++;
	}
}