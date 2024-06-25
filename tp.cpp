#include <iostream>
#include <cstdlib> // rand y srand
#include <ctime> // time para inicializar srand
#include <unistd.h> // read para lecturas individuales
#include <termios.h> // Configurar terminal para capturar entradas de teclado

using namespace std;

const int FILAS = 30;
const int COLUMNAS = 30;

char laberinto[FILAS][COLUMNAS];
int jugadorX, jugadorY;
int salidaX, salidaY;
struct termios terminosoriginal;

void setup_terminal() {
    struct termios nuevotermios;
    tcgetattr(STDIN_FILENO, &terminosoriginal);
    nuevotermios = terminosoriginal;
    nuevotermios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &nuevotermios);
}

char getch() {
    char buf = 0;
    if (read(STDIN_FILENO, &buf, 1) < 0) {
        perror("read()");
    }
    return buf;
}

enum Key {
    UP_ARROW = 'A',
    DOWN_ARROW = 'B',
    RIGHT_ARROW = 'C',
    LEFT_ARROW = 'D',
    OTHER_KEY = 0
};

Key keyPress() {
    char c = getch();
    if (c == 27) {
        c = getch();
        if (c == '[') {
            c = getch();
            switch (c) {
                case 'A': return UP_ARROW;
                case 'B': return DOWN_ARROW;
                case 'C': return RIGHT_ARROW;
                case 'D': return LEFT_ARROW;
            }
        }
    }
    return static_cast<Key>(c);
}

void inicializarLaberinto() {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (i == 0 || i == FILAS - 1 || j == 0 || j == COLUMNAS - 1) {
                laberinto[i][j] = '#'; // Paredes exteriores
            } else {
                laberinto[i][j] = ' '; // Espacio vacío
            }
        }
    }

    srand(static_cast<unsigned int>(time(0)));
    for (int i = 2; i < FILAS - 2; i += 2) {
        for (int j = 2; j < COLUMNAS - 2; j += 2) {
            if (rand() % 2 == 0) {
                laberinto[i][j] = '#'; // Pared interior
            }
        }
    }

    jugadorX = 1;
    jugadorY = 1;
    laberinto[jugadorY][jugadorX] = '@';

    salidaX = COLUMNAS - 2;
    salidaY = FILAS - 2;
    laberinto[salidaY][salidaX] = 'X';
}

void dibujarLaberinto() {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            cout << laberinto[i][j];
        }
        cout << endl;
    }
}

void moverJugador(Key movimiento) {
    int nuevaX = jugadorX, nuevaY = jugadorY;

    switch (movimiento) {
        case UP_ARROW:
        case 'w':
            nuevaY--;
            break;
        case DOWN_ARROW:
        case 's':
            nuevaY++;
            break;
        case LEFT_ARROW:
        case 'a':
            nuevaX--;
            break;
        case RIGHT_ARROW:
        case 'd':
            nuevaX++;
            break;
    }

    if (laberinto[nuevaY][nuevaX] != '#') {
        laberinto[jugadorY][jugadorX] = ' ';
        jugadorX = nuevaX;
        jugadorY = nuevaY;
        laberinto[jugadorY][jugadorX] = '@';
    }
}

void juegoLaberinto() {
    setup_terminal();
    inicializarLaberinto();

    while (true) {
        system("clear");
        dibujarLaberinto();
        Key movimiento = keyPress();
        moverJugador(movimiento);

        if (jugadorX == salidaX && jugadorY == salidaY) {
            system("clear");
            cout << "¡Felicidades! ¡Has encontrado la salida!" << endl;
            break;
        }
    }

}

void juegoAdivinanza() {
    srand(static_cast<unsigned int>(time(0)));

    int numeroadivinar, intento, intentosRestantes = 5;
    bool adivinado = false;

    numeroadivinar = rand() % 100 + 1;

    cout << "Bienvenido al Juego de Adivinanza de Números!" << endl;
    cout << "Tienes que adivinar un número entre 1 y 100." << endl;

    while (intentosRestantes > 0 && !adivinado) {
        cout << "Intentos restantes: " << intentosRestantes << endl;
        cout << "Ingresa tu intento: ";
        cin >> intento;

        if (intento == numeroadivinar) {
            adivinado = true;
            cout << "¡Felicidades! ¡Has adivinado el número!" << endl;
        } else if (intento < numeroadivinar) {
            cout << "El número secreto es mayor." << endl;
        } else {
            cout << "El número secreto es menor." << endl;
        }

        intentosRestantes--;
    }

    if (!adivinado) {
        cout << "No pudiste adivinar el número. El número secreto era " << numeroadivinar << "." << endl;
    }
}

int main() {
    int juego;
    cout << "Elegir juego:" << endl;
    cout << "1. Juego de Laberinto" << endl;
    cout << "2. Juego de Adivinanza de Números" << endl;
    cin >> juego;

    if (juego == 1) {
        juegoLaberinto();
    } else if (juego == 2) {
        juegoAdivinanza();
    } else {
        cout << "Opción inválida. Saliendo del programa." << endl;
    }

}
