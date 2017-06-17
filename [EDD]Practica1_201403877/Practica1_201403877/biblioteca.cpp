
#include <biblioteca.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;

void split_string_archivo(string cadena, ListaArtistas* lista){

    char sep = ';';
    string datosCancion;
    for(int p=0, q=0;p!=cadena.npos;p=q){
        datosCancion = cadena.substr(p+(p!=0),(q=cadena.find(sep,p+1))-p-(p!=0));
        //cout<<datosCancion<<endl;
        ingresar_a_biblioteca(datosCancion, lista);
    }
}

void ingresar_a_biblioteca(string cadena, ListaArtistas* lista){
    if(cadena!=" " && cadena!=""){
        char sep = '_';
        string nombreArtista;
        string nombreAlbum;
        string nombreCancion;
        string path;
        float valoracion;

        for(int p=0, q=0,n=0;p!=cadena.npos;p=q,n++){
            if(n==0)
                nombreArtista = cadena.substr(p+(p!=0),(q=cadena.find(sep,p+1))-p-(p!=0));

            else if(n==1)
                nombreAlbum = cadena.substr(p+(p!=0),(q=cadena.find(sep,p+1))-p-(p!=0));
            else if(n==2)
                nombreCancion = cadena.substr(p+(p!=0),(q=cadena.find(sep,p+1))-p-(p!=0));
            else if(n==3)
                path = cadena.substr(p+(p!=0),(q=cadena.find(sep,p+1))-p-(p!=0));
            else if(n==4)
            {
                string aux = (cadena.substr(p+(p!=0),(q=cadena.find(sep,p+1))-p-(p!=0)));
                valoracion = strtod(aux.c_str(),0);
            }

        }
        //cout<<nombreArtista<<"--"<<nombreAlbum<<"--"<<nombreCancion<<"--"<<path<<"--"<<valoracion<<endl;

        Nodo* tmpArtista;
        Nodo* busquedaArtista;
        Nodo1* tmpAlbum;
        Nodo1* busquedaAlbum;
        Nodo2* tmpCancion;
        Nodo2* busquedaCancion;

        /*<------------- artista ----------------->*/
        tmpArtista = lista->primeroDcircular;
        busquedaArtista = lista->buscar(tmpArtista,nombreArtista);
        if(busquedaArtista==0){
            tmpArtista = lista->primeroDcircular;
            lista->agregar(tmpArtista,nombreArtista);
        }
        tmpArtista = lista->primeroDcircular;
        busquedaArtista = lista->buscar(tmpArtista,nombreArtista);

        /*<-------------- album -------------------->*/
        tmpAlbum = busquedaArtista->albums->primeroDoble;
        busquedaAlbum = busquedaArtista->albums->buscarAlbum(tmpAlbum,nombreAlbum);
        if(busquedaAlbum==0){
            tmpAlbum = busquedaArtista->albums->primeroDoble;
            busquedaAlbum = busquedaArtista->albums->agregarAlbum(tmpAlbum,nombreAlbum);
        }
        tmpAlbum = busquedaArtista->albums->primeroDoble;
        busquedaAlbum = busquedaArtista->albums->buscarAlbum(tmpAlbum,nombreAlbum);

        /*<--------------- cancion ----------------->*/
        tmpCancion = busquedaAlbum->canciones->primeroSimple;
        busquedaCancion = busquedaAlbum->canciones->buscarCancion(tmpCancion,nombreCancion);
        if(busquedaCancion==0){
            tmpCancion = busquedaAlbum->canciones->primeroSimple;
            busquedaCancion = busquedaAlbum->canciones->agregarCancion(tmpCancion,nombreCancion,path,valoracion);
        }
        tmpCancion = busquedaAlbum->canciones->primeroSimple;
        busquedaCancion = busquedaAlbum->canciones->buscarCancion(tmpCancion,nombreCancion);

        /*<--------------- valoracion de artista y album ------------------>*/
        busquedaAlbum->valoracion = busquedaAlbum->canciones->calcularValoracion(busquedaAlbum->canciones->primeroSimple);
        busquedaArtista->valoracion = busquedaArtista->albums->calcularValoracion(busquedaArtista->albums->primeroDoble);
    }
}

Nodo* ListaArtistas::buscar(Nodo* actual, string nombre){
    if(actual!=0){
        int comparacion = stricmp(actual->nombre.c_str(),nombre.c_str());
        if(comparacion==0){
            return actual;
        }else{
            if(actual->siguiente!=primeroDcircular){
                Nodo* tmp = actual->siguiente;
                while(tmp!=primeroDcircular){
                    int comparacion = stricmp(tmp->nombre.c_str(),nombre.c_str());
                    if(comparacion==0){
                        return tmp;
                    }else if(comparacion<0){
                        tmp = tmp->siguiente;
                    }else{
                        return 0;
                    }
                }
            }
        }
    }
    return 0;
}

Nodo* ListaArtistas::agregar(Nodo* actual, string nombre){

    if(actual!=0){
        int comparacion = stricmp(actual->nombre.c_str(),nombre.c_str());
        if(comparacion<0){
            if(actual->siguiente!=0 && actual->siguiente!=primeroDcircular){
               return agregar(actual->siguiente,nombre);  // recursividad
            }else{
                Nodo* nuevo = new Nodo(nombre);
                actual->siguiente = nuevo;
                nuevo->anterior = actual;
                primeroDcircular->anterior = nuevo;
                nuevo->siguiente = primeroDcircular;
                cout<<nuevo->nombre<<endl;
                return nuevo;
            }
        }else if(comparacion>0){
            Nodo* nuevo = new Nodo(nombre);
            if(actual==primeroDcircular){
                nuevo->siguiente = primeroDcircular;
                primeroDcircular->anterior->siguiente = nuevo;
                nuevo->anterior = primeroDcircular->anterior;
                primeroDcircular->anterior = nuevo;
                primeroDcircular = nuevo;
            }else{
                actual->anterior->siguiente = nuevo;
                nuevo->anterior = actual->anterior;
                actual->anterior = nuevo;
                nuevo->siguiente = actual;
            }
            cout<<nuevo->nombre<<endl;
            return nuevo;
        }
    }else{
        primeroDcircular = new Nodo(nombre);
        primeroDcircular->anterior = primeroDcircular;
        primeroDcircular->siguiente = primeroDcircular;
        return primeroDcircular;
    }
}

Nodo1* ListaAlbum::buscarAlbum(Nodo1* actual, string nombre){
    if (actual!=0){
        int comparacion = stricmp(actual->nombre.c_str(),nombre.c_str());
        if(comparacion==0){
            return actual;
        }else{
            Nodo1* tmp = actual;
            while(tmp!=0){
                int comparacion = stricmp(tmp->nombre.c_str(),nombre.c_str());
                if(comparacion==0){
                    return tmp;
                }else if(comparacion<0){
                    tmp = tmp->siguiente;
                }else{
                    return 0;
                }
            }
        }
    }
    return 0;
}

Nodo1* ListaAlbum::agregarAlbum(Nodo1* actual, string nombre){
    if(actual!=0){
        int comparacion = stricmp(actual->nombre.c_str(),nombre.c_str());
        if(comparacion<0){
            if(actual->siguiente!=0){
                agregarAlbum(actual->siguiente,nombre); // ver si funciona con return
            }else{
                Nodo1* nuevo= new Nodo1(nombre);
                actual->siguiente = nuevo;
                nuevo->anterior = actual;
                return nuevo;
            }
        }else if(comparacion>0){
            Nodo1* nuevo = new Nodo1(nombre);
            if(actual==primeroDoble){
                nuevo->siguiente = primeroDoble;
                primeroDoble->anterior = nuevo;
                primeroDoble = nuevo;
            }else{
                actual->anterior->siguiente = nuevo;
                nuevo->anterior = actual->anterior;
                actual->anterior = nuevo;
                nuevo->siguiente = actual;
            }
            return nuevo;
        }
    }else{
        primeroDoble = new Nodo1(nombre);
        return primeroDoble;
    }
}

Nodo2* ListaCancion::buscarCancion(Nodo2 *actual, string nombre){
    if(actual!=0){
        int comparacion = stricmp(actual->nombre.c_str(),nombre.c_str());
        if(comparacion==0)
            return actual;
        else{
            Nodo2* tmp = actual;
            while(tmp!=0){
                int comparacion = stricmp(tmp->nombre.c_str(),nombre.c_str());
                if(comparacion==0){
                    return tmp;
                }else if(comparacion<0){
                    tmp = tmp->siguiente;
                }else{
                    return 0;
                }
            }
        }
    }
    return 0;
}

Nodo2* ListaCancion::agregarCancion(Nodo2 *actual, string nombre, string direccion, float valoracion){
    if(actual!=0){
        int comparacion = stricmp(actual->nombre.c_str(),nombre.c_str());
        if(comparacion<0){
            if(actual->siguiente!=0)
                return agregarCancion(actual->siguiente,nombre,direccion,valoracion);
            else{
                Nodo2* nuevo = new Nodo2(nombre,valoracion,direccion);
                actual->siguiente = nuevo;
                return nuevo;
            }
        }else if(comparacion>0){
            Nodo2* nuevo = new Nodo2(nombre,valoracion,direccion);
            if(actual==primeroSimple){
                nuevo->siguiente=primeroSimple;
                primeroSimple = nuevo;
            }else{
                nuevo->siguiente = actual->siguiente;
                actual->siguiente = nuevo;
            }
            return nuevo;
        }
    }else{
        primeroSimple = new Nodo2(nombre,valoracion,direccion);
        return primeroSimple;
    }
}

void ListaArtistas::graficar(Nodo* primero){
    ofstream graph;
    graph.open("biblioteca.txt",ios::out);
    Nodo* tmp = primero;
    graph<<"Digraph g{\n";
    graph.close();

    int contador = 1;

    while(tmp->siguiente!=primero){
        cout<<tmp->valoracion<<endl;

        graph.open("biblioteca.txt",ios::out | ios::app);
        graph<<"\""<<tmp->nombre<<"\" -> \""<<tmp->siguiente->nombre<<"\"[color=red];\n";
        graph<<"\""<<tmp->siguiente->nombre<<"\" -> \""<<tmp->nombre<<"\";\n";
        graph.close();

        tmp->albums->graficar(tmp->albums->primeroDoble, contador);

        graph.open("biblioteca.txt",ios::out | ios::app);
        graph<<"\""<<tmp->nombre<<"\" -> \""<<tmp->albums->primeroDoble->nombre<<"\";";
        graph<<"\""<<tmp->nombre<<"\" [shape=box];\n";
        graph.close();

        tmp->albums->graficarAux(tmp->albums->primeroDoble, contador);
        contador++;
        tmp = tmp->siguiente;
    }

    graph.open("biblioteca.txt",ios::out | ios::app);
    graph<<"\""<<primero->nombre<<"\" -> \""<<primero->anterior->nombre<<"\"[color=red];\n";
    graph<<"\""<<primero->anterior->nombre<<"\" -> \""<<primero->nombre<<"\";\n";
    tmp->albums->graficar(primero->anterior->albums->primeroDoble,contador);
    graph<<"\""<<primero->anterior->nombre<<"\" -> \""<<primero->anterior->albums->primeroDoble->nombre<<"\";";
    graph<<"\""<<primero->anterior->nombre<<"\" [shape=box];\n";
    graph.close();

    cout<<primero->anterior->valoracion<<endl;

    graph.open("biblioteca.txt",ios::out | ios::app);
    graph<<"}";
    graph.close();

    //Graphviz archivo
    system("dot -Tpng biblioteca.txt -o biblioteca.png");
    //system("nohup display biblioteca.png &" );
}

void ListaAlbum::graficar(Nodo1* actual, int nombre){
    Nodo1* tmp = actual;
    int contador = nombre+200;
    ofstream graph;
    while(tmp!=0){
        tmp->canciones->graficar(tmp->canciones->primeroSimple,contador);
        graph.open("biblioteca.txt",ios::out | ios::app);
        graph<<"\""<<tmp->nombre<<"\" -> \""<<tmp->canciones->primeroSimple->nombre<<"\";\n";
        graph.close();
        contador++;
        tmp = tmp->siguiente;
    }

}

void ListaAlbum::graficarAux(Nodo1* primero, int nombre){
    Nodo1* tmp = primero;
    int nombre_ = nombre+2000;
    ofstream graph;
    graph.open("biblioteca.txt",ios::out | ios::app);
    graph<<"subgraph "<<nombre_<<" {\n";
    graph<<"node [style=filled,color=blue];\n";
    graph<<"color=red;\n";
    if(primero->siguiente!=0){
        while(tmp->siguiente!=0){
            graph<<"\""<<tmp->nombre<<"\" -> \""<<tmp->siguiente->nombre<<"\";\n";
            graph<<"\""<<tmp->siguiente->nombre<<"\" -> \""<<tmp->nombre<<"\";\n";
            tmp = tmp->siguiente;
        }
    }else{
        graph<<"\""<<tmp->nombre<<"\";\n";
    }
    graph<<"}\n";
    graph.close();
}

void ListaCancion::graficar(Nodo2 *primero,int nombre){
    /*
     * Escritura al final, sin reemplazar
    ofstream graph;
    graph.open("biblioteca.txt",ios::out | ios::app);
    graph<<"aldo"<<endl;
    graph.close();*/
    int nombre_ = nombre+20;
    Nodo2* tmp = primero;
    ofstream graph;
    graph.open("biblioteca.txt",ios::out | ios::app);
    graph<<"subgraph "<<nombre<<" {\n";
    graph<<"node [style=filled];\n";
    graph<<"color=blue;\n";
    if(primero->siguiente!=0){
        while(tmp->siguiente!=0){
            graph<<"\""<<tmp->nombre<<"\" -> \""<<tmp->siguiente->nombre<<"\";\n";
            tmp = tmp->siguiente;
        }
    }else{
        graph<<"\""<<tmp->nombre<<"\";\n";
    }
    graph<<"}\n";
    graph.close();
}

float ListaCancion::calcularValoracion(Nodo2* primero){
    float suma = 0;
    int contador = 0;
    Nodo2* tmp = primero;
    if(primero!=0){
        while(tmp!=0){
            suma = suma + tmp->valoracion;
            contador++;
            tmp = tmp->siguiente;
        }
        return suma/contador;
    }
    return 0;
}

float ListaAlbum::calcularValoracion(Nodo1* primero){
    float suma = 0;
    int contador = 0;
    Nodo1* tmp = primero;
    if(primero!=0){
        while(tmp!=0){
            suma = suma + tmp->valoracion;
            contador++;
            tmp = tmp->siguiente;
        }
        return suma/contador;
    }
    return 0;
}

void ListaTops::agregar(string nombre, float valor){
    if(primero!=0){
        Nodo2* tmp = primero;
        if(tmp->valoracion<valor){
            Nodo2* nuevo = new Nodo2(nombre,valor);
            nuevo->siguiente = tmp;
            primero = nuevo;
            return;
        }else{
            Nodo2* anterior = 0;
            while(tmp!=0){
                if(tmp->valoracion>valor){
                    anterior = tmp;
                    tmp=tmp->siguiente;
                }else if(tmp->valoracion==valor){
                    Nodo2* nuevo = new Nodo2(nombre,valor);
                    if(anterior!=0){
                        anterior->siguiente = nuevo;
                    }else{
                        nuevo->siguiente = tmp;
                        primero = nuevo;
                    }
                    return;
                }else{
                    Nodo2* nuevo = new Nodo2(nombre,valor);
                    anterior->siguiente = nuevo;
                    nuevo->siguiente = tmp;
                    return;
                }
            }
            Nodo2* nuevo = new Nodo2(nombre,valor);
            if(anterior!=0){
                anterior->siguiente = nuevo;
                return;
            }else{
                nuevo->siguiente = tmp;
                primero = nuevo;
                return;
            }
        }
    }else{
        Nodo2* nuevo = new Nodo2(nombre,valor);
        primero = nuevo;
    }
}

void ListaTops::graficar(string nombre){
    ofstream graph;
    graph.open("top5.txt",ios::out);
    graph<<"Digraph g{\n";
    Nodo2* tmp = primero;
    for(int a=0;a<4;a++){
        if(tmp->siguiente!=0){
            string dato1 = tmp->nombre;
            string dato2 = tmp->siguiente->nombre;
            graph<<"\""<<dato1<<" valor: "<<tmp->valoracion<<"\" -> \""<<dato2<<" valor: "<<tmp->siguiente->valoracion<<"\";\n";
            tmp = tmp->siguiente;
        }
    }
    graph<<"\""<<tmp->nombre<<" valor: "<<tmp->valoracion<<"\";\n";
    graph<<nombre<<";\n}";
    graph.close();
    system("dot -Tpng top5.txt -o top5.png");
}

void DobleCircular::agregar(string nombre, string direccion){
    if(primero!=0){
        Nodo2* nuevo = new Nodo2(nombre,direccion);
        Nodo2* tmp = primero->anterior;
        nuevo->siguiente = primero;
        primero->anterior->siguiente = nuevo;
        nuevo->anterior = tmp;
        primero->anterior = nuevo;
    }else{
        Nodo2* nuevo = new Nodo2(nombre,direccion);
        nuevo->siguiente = nuevo;
        nuevo->anterior = nuevo;
        primero = nuevo;
    }
}

void DobleCircular::graficar(){
    if(primero!=0){
        fstream graph;
        graph.open("reproductor.txt",ios::out);
        graph<<"Digraph g{\n";
        Nodo2* tmp = primero->siguiente;
        while(tmp!=primero){
            graph<<"\""<<tmp->nombre<<"\" -> \""<<tmp->siguiente->nombre<<"\";\n";
            graph<<"\""<<tmp->siguiente->nombre<<"\" -> \""<<tmp->nombre<<"\";\n";
            tmp = tmp->siguiente;
        }
        graph<<"\""<<primero->nombre<<"\" [shape=box];";
        graph<<"\""<<primero->nombre<<"\" -> \""<<primero->siguiente->nombre<<"\";\n";
        graph<<"\""<<primero->siguiente->nombre<<"\" -> \""<<primero->siguiente->anterior->nombre<<"\";\n}";
        graph.close();
        system("dot -Tpng reproductor.txt -o reproductor.png");
    }else{
        fstream graph;
        graph.open("reproductor.txt",ios::out);
        graph<<"Digraph g{\n";
        graph<<"}";
        graph.close();
    }
}

void Pila::agregar(string nombre, string direccion){
    if(primero!=0){
        Nodo2* nuevo = new Nodo2(nombre,direccion);
        nuevo->siguiente = primero;
        primero = nuevo;
    }else{
        Nodo2* nuevo = new Nodo2(nombre,direccion);
        primero = nuevo;
    }
}

void Pila::eliminar(){
    if(primero!=0){
        Nodo2* tmp = primero;
        primero = tmp->siguiente;
        tmp = 0;
        delete tmp;
    }
}

void Pila::graficar(){
    if(primero!=0){
        fstream graph;
        graph.open("reproductor.txt",ios::out);
        graph<<"Digraph g{\n";
        Nodo2* tmp = primero;
        while(tmp->siguiente!=0){
            graph<<"\""<<tmp->nombre<<"\" -> \""<<tmp->siguiente->nombre<<"\";\n";
            tmp = tmp->siguiente;
        }
        graph<<"\""<<primero->nombre<<"\" [shape=doubleoctagon];";
        //graph<<"\""<<tmp->nombre<<"\" -> \""<<tmp->siguiente->nombre<<"\";\n";
        graph<<"}";
        graph.close();
        system("dot -Tpng reproductor.txt -o reproductor.png");
    }else{
        fstream graph;
        graph.open("reproductor.txt",ios::out);
        graph<<"Digraph g{\n";
        graph<<"}";
        graph.close();
        system("dot -Tpng reproductor.txt -o reproductor.png");
    }
}

void Cola::agregar(string nombre, string direccion){
    if(primero!=0){
        Nodo2* nuevo = new Nodo2(nombre,direccion);
        Nodo2* tmp = primero;
        Nodo2* anterior = 0;
        while(tmp!=0){
            anterior = tmp;
            tmp = tmp->siguiente;
        }
        anterior->siguiente = nuevo;
    }else{
        Nodo2* nuevo = new Nodo2(nombre,direccion);
        primero = nuevo;
    }
}

void Cola::eliminar(){
    if(primero!=0){
        Nodo2* tmp = primero;
        primero = tmp->siguiente;
        tmp = 0;
        delete tmp;
    }
}

void Cola::graficar(){
    if(primero!=0){
        fstream graph;
        graph.open("reproductor.txt",ios::out);
        graph<<"Digraph g{\n";
        Nodo2* tmp = primero;
        while(tmp->siguiente!=0){
            graph<<"\""<<tmp->nombre<<"\" -> \""<<tmp->siguiente->nombre<<"\";\n";
            tmp = tmp->siguiente;
        }
        graph<<"\""<<primero->nombre<<"\" [shape=hexagon];";
        //graph<<"\""<<tmp->nombre<<"\" -> \""<<tmp->siguiente->nombre<<"\";\n";
        graph<<"}";
        graph.close();
        system("dot -Tpng reproductor.txt -o reproductor.png");
    }else{
        fstream graph;
        graph.open("reproductor.txt",ios::out);
        graph<<"Digraph g{\n";
        graph<<"}";
        graph.close();
        system("dot -Tpng reproductor.txt -o reproductor.png");
    }
}

void ListaCancion::eliminar(Nodo2* primero, string nombre){
    if(primero!=0){
        Nodo2* tmp = primero;
        Nodo2* anterior = 0;
        while (tmp->nombre!=nombre) {
            anterior = tmp;
            tmp = tmp->siguiente;
        }
        anterior->siguiente = tmp->siguiente;
        tmp = 0;
        delete tmp;
    }
}

void ListaAlbum::eliminar(Nodo1* actual){
    if(actual->siguiente==0){
        actual->anterior->siguiente = 0;
        actual = 0;
        delete actual;
    }else{
        Nodo1* tmp = actual;
        tmp->anterior->siguiente = tmp->siguiente;
        tmp->siguiente->anterior = tmp->anterior;
        tmp=0;
        delete tmp;
    }
}

void ListaArtistas::eliminar(Nodo *actual){
    if(actual!=0){
        actual->siguiente->anterior = actual->anterior;
        actual->anterior->siguiente = actual->siguiente;
        actual = 0;
        delete actual;
    }
}
