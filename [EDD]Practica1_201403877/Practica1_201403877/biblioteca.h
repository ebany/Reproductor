#include <string.h>
#include <string>
#include <iostream>
#ifndef biblioteca
#define biblioteca
using namespace std;

class Nodo2{
    public:
        string nombre;
        float valoracion;
        string direccion;
        Nodo2* siguiente;

        Nodo2* anterior; // para reproduccion de doble circular;

        Nodo2(string nombre_,float valoracion_,string direccion_):nombre(nombre_),direccion(direccion_),valoracion(valoracion_),siguiente(0),anterior(0){}
        Nodo2(string nombre_,float valoracion_):nombre(nombre_),direccion(""),valoracion(valoracion_),siguiente(0),anterior(0){} // para top
        Nodo2(string nombre_,string direccion_):nombre(nombre_),direccion(direccion_),valoracion(0),siguiente(0),anterior(0){} // para reproduccion
};

class ListaCancion{
    public:
        Nodo2* primeroSimple;

        Nodo2* buscarCancion(Nodo2* actual, string nombre);
        Nodo2* agregarCancion(Nodo2* actual, string nombre, string direccion, float valoracion);
        void graficar(Nodo2* primero, int nombre);
        float calcularValoracion(Nodo2* primero);
        void eliminar(Nodo2* primero,string nombre);   // nombre de cancion que se va a eliminar y el inicio de la lista
};

class Nodo1{
    public:
        string nombre;
        float valoracion;

        Nodo1* siguiente;
        Nodo1* anterior;

        ListaCancion* canciones;

        Nodo1(string nombre_):nombre(nombre_),valoracion(0),siguiente(0),anterior(0),canciones(new ListaCancion()){}
};

class ListaAlbum{
    public:
        Nodo1* primeroDoble;

        Nodo1* buscarAlbum(Nodo1* actual, string nombre);
        Nodo1* agregarAlbum(Nodo1* actual, string nombre);
        void graficar(Nodo1* primero, int nombre);
        void graficarAux(Nodo1* primero, int nombre);
        float calcularValoracion(Nodo1* primero);
        void eliminar(Nodo1* actual); // nodo que se va a eliminar
};

class Nodo{
    public:
        string nombre;
        float valoracion;

        Nodo* siguiente;
        Nodo* anterior;

        ListaAlbum* albums;

        Nodo(string nombre_):nombre(nombre_),valoracion(0),siguiente(0),anterior(0),albums(new ListaAlbum()){}
};

class ListaArtistas{
    public:
        Nodo* primeroDcircular;        

        Nodo* buscar(Nodo* actual,string nombre);
        Nodo* agregar (Nodo* actual, string nombre);
        void graficar(Nodo* primero);
        void eliminar(Nodo* actual);    // nodo que se que va a eliminar
};

void split_string_archivo(string cadena, ListaArtistas* lista);
void ingresar_a_biblioteca(string cadena, ListaArtistas* lista);

class ListaTops{
    public:
        Nodo2* primero;
        void agregar(string nombre, float valor);
        void graficar(string nombre);
};

class DobleCircular{
    public:
        Nodo2* primero;
        void agregar(string nombre,string direccion);
        void graficar();
};

class Pila{
    public:
        Nodo2* primero;
        void agregar(string nombre, string direccion);
        void eliminar();
        void graficar();
};

class Cola{
    public:
        Nodo2* primero;
        void agregar(string nombre, string direccion);
        void eliminar();
        void graficar();
};

#endif
