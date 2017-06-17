#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <biblioteca.h>
#include <fstream>
#include <iostream>
#include <QMediaPlayer>
#include <QFileDialog>

using namespace std;
ListaArtistas* nuevo = new ListaArtistas(); // biblioteca
ListaTops* tops;    // reportes

/*<---- reproduccion  ------>*/
DobleCircular* dobleC;
Pila* pila;
Cola* cola;
Nodo2* actual; //actual reproduciendo

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mMediaPlayer = new QMediaPlayer(this);
    connect(mMediaPlayer,&QMediaPlayer::positionChanged,[&](qint64 pos){
        ui->progressBar->setValue(pos);
    });
    connect(mMediaPlayer, &QMediaPlayer::durationChanged, [&](qint64 dur){
        ui->progressBar->setMaximum(dur);
    });
    ui->verticalSlider->setValue(75);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()    //cargar archivo y crear biblioteca
{    
    QString nombreArchivo = QFileDialog::getOpenFileName(
                this,
                tr("Abrir Archivo"),
                "C://",
                "All files (*.*);;Text File(*.txt)"
                );

    fstream ficheroEntrada;
    string temporal;
    string frase = "";
    ficheroEntrada.open(nombreArchivo.toStdString().c_str(),ios::in);
    if(ficheroEntrada.is_open()){
        while(! ficheroEntrada.eof()){
            getline(ficheroEntrada,temporal);
            frase = frase + temporal;
        }
        ficheroEntrada.close();

    }    
    split_string_archivo(frase, nuevo);

}

void MainWindow::on_pushButton_2_clicked() //graficar biblioteca
{
    if(nuevo->primeroDcircular==0){
        this->ui->label_5->setText("No hay datos en la biblioteca");
    }else{
        nuevo->graficar(nuevo->primeroDcircular);
        system("explorer.exe biblioteca.png");
    }

}

void MainWindow::on_pushButton_5_clicked() //grafica top artistas
{
    tops = 0;
    if(nuevo->primeroDcircular!=0){
        tops = new ListaTops();
        Nodo* tmp = nuevo->primeroDcircular;
        while(tmp->siguiente!=nuevo->primeroDcircular){
            tops->agregar(tmp->nombre,tmp->valoracion);
            tmp = tmp->siguiente;
        }
        tops->agregar(nuevo->primeroDcircular->anterior->nombre,nuevo->primeroDcircular->anterior->valoracion);
        tops->graficar("artista");
        tops = 0;
        delete tops;
        system("explorer.exe top5.png");
    }

}

void MainWindow::on_pushButton_4_clicked() //grafica top albums
{
    tops = 0;
    if(nuevo->primeroDcircular!=0){
        tops = new ListaTops();
        Nodo* tmp = nuevo->primeroDcircular;
        Nodo1* tmp_tmp;
        while(tmp->siguiente!=nuevo->primeroDcircular){
            tmp_tmp = tmp->albums->primeroDoble;
            while(tmp_tmp!=0){
                tops->agregar(tmp_tmp->nombre,tmp_tmp->valoracion);
                //cout<<tmp_tmp->nombre<<"--"<<tmp_tmp->valoracion<<endl;
                tmp_tmp = tmp_tmp->siguiente;
            }
            tmp = tmp->siguiente;
        }
        tmp = nuevo->primeroDcircular;
        tmp_tmp = tmp->anterior->albums->primeroDoble;
        while(tmp_tmp!=0){
            tops->agregar(tmp_tmp->nombre,tmp_tmp->valoracion);
            //cout<<tmp_tmp->nombre<<"--"<<tmp_tmp->valoracion<<endl;
            tmp_tmp = tmp_tmp->siguiente;
        }
        tops->graficar("albums");
        tops = 0;
        delete tops;
        system("explorer.exe top5.png");
    }
}

void MainWindow::on_pushButton_3_clicked() //grafica top canciones
{
    tops = 0;
    if(nuevo->primeroDcircular!=0){
        tops = new ListaTops();
        Nodo* tmp = nuevo->primeroDcircular;
        Nodo1* tmp_tmp;
        Nodo2* tmp_tmp_tmp;
        while(tmp->siguiente!=nuevo->primeroDcircular){
            tmp_tmp = tmp->albums->primeroDoble;
            while(tmp_tmp!=0){
                tmp_tmp_tmp = tmp_tmp->canciones->primeroSimple;
                while(tmp_tmp_tmp!=0){
                    string dato1 = tmp_tmp_tmp->nombre;
                    float dato2 = tmp_tmp_tmp->valoracion;
                    //cout<<dato1<<"--"<<dato2<<endl;
                    tops->agregar(dato1,dato2);
                    tmp_tmp_tmp = tmp_tmp_tmp->siguiente;
                }
                tmp_tmp = tmp_tmp->siguiente;
            }
            tmp = tmp->siguiente;
        }
        tmp = nuevo->primeroDcircular;
        tmp_tmp = tmp->anterior->albums->primeroDoble;
        while(tmp_tmp!=0){
            //tops->agregar(tmp_tmp->nombre,tmp_tmp->valoracion);
            tmp_tmp_tmp = tmp_tmp->canciones->primeroSimple;
            while(tmp_tmp_tmp!=0){
                string dato1 = tmp_tmp_tmp->nombre;
                float dato2 = tmp_tmp_tmp->valoracion;
                //cout<<dato1<<"--"<<dato2<<endl;
                tops->agregar(dato1,dato2);
                tmp_tmp_tmp = tmp_tmp_tmp->siguiente;
            }
            tmp_tmp = tmp_tmp->siguiente;

        }
        tops->graficar("canciones");
        tops = 0;
        delete tops;
        system("explorer.exe top5.png");
    }
}

void MainWindow::on_pushButton_6_clicked()  //establecer la estructura para reproduccion
{
    dobleC = 0;
    pila = 0;
    cola = 0;
    actual = 0;
    if(this->ui->radioButton->isChecked()){
        this->ui->label_2->setText("Tipo de reproduccion: Doble Circular");
        dobleC = new DobleCircular();
    }else if(this->ui->radioButton_2->isChecked()){
        this->ui->label_2->setText("Tipo de reproduccion: Pila");
        pila = new Pila();
    }else if(this->ui->radioButton_3->isChecked()){
        this->ui->label_2->setText("Tipo de reproduccion: Cola");
        cola = new Cola();
    }
}

void MainWindow::on_pushButton_7_clicked() //agregar a estructura de reproduccion
{
    QString dato = this->ui->lineEdit->text();
    string Cancion = dato.toStdString();
    if(Cancion!=" " && Cancion!=""){
        char sep = '_';
        string nombreArtista;
        string nombreAlbum;
        string nombreCancion;

        for(int p=0, q=0,n=0;p!=Cancion.npos;p=q,n++){
            if(n==0)
                nombreArtista = Cancion.substr(p+(p!=0),(q=Cancion.find(sep,p+1))-p-(p!=0));
            else if(n==1)
                nombreAlbum = Cancion.substr(p+(p!=0),(q=Cancion.find(sep,p+1))-p-(p!=0));
            else if(n==2)
                nombreCancion = Cancion.substr(p+(p!=0),(q=Cancion.find(sep,p+1))-p-(p!=0));

        }
        //cout<<nombreArtista<<"--"<<nombreAlbum<<"--"<<nombreCancion<<endl;
        //scene->setBackgroundBrush(m.scaled(901,361,Qt::IgnoreAspectRatio));
        //QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage("reproductor.png"));
        //scene->addItem(item);

        Nodo* tmpArtista;
        Nodo* busquedaArtista;
        Nodo1* tmpAlbum;
        Nodo1* busquedaAlbum;
        Nodo2* tmpCancion;
        Nodo2* busquedaCancion;

        tmpArtista = nuevo->primeroDcircular;
        busquedaArtista = nuevo->buscar(tmpArtista,nombreArtista);
        if(busquedaArtista!=0){
            tmpAlbum = busquedaArtista->albums->primeroDoble;
            busquedaAlbum = busquedaArtista->albums->buscarAlbum(tmpAlbum,nombreAlbum);
            if(busquedaAlbum!=0){
                tmpCancion = busquedaAlbum->canciones->primeroSimple;
                busquedaCancion = busquedaAlbum->canciones->buscarCancion(tmpCancion,nombreCancion);
                if(busquedaCancion!=0){
                    this->ui->label_5->setText("Si existe XD");
                    if(dobleC!=0){
                        dobleC->agregar(busquedaCancion->nombre,busquedaCancion->direccion);
                        dobleC->graficar();
                        /*
                        QGraphicsScene *scene = new QGraphicsScene();
                        QPixmap m("reproductor.png");
                        scene->setBackgroundBrush(m.scaled(901,361,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
                        this->ui->graphicsView->setScene(scene);*/
                        QImage img;
                        img.load ("reproductor.png");
                        ui->lblbiblioteca->setPixmap(QPixmap(QPixmap::fromImage(img)));

                        mMediaPlayer->stop();
                        actual = 0;

                    }else if(pila!=0){
                        pila->agregar(busquedaCancion->nombre,busquedaCancion->direccion);
                        pila->graficar();
                        /*
                        QGraphicsScene *scene = new QGraphicsScene();
                        QPixmap m("reproductor.png");
                        scene->setBackgroundBrush(m.scaled(901,361,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
                        this->ui->graphicsView->setScene(scene);*/
                        QImage img;
                        img.load ("reproductor.png");
                        ui->lblbiblioteca->setPixmap(QPixmap(QPixmap::fromImage(img)));

                        mMediaPlayer->stop();
                        actual = 0;

                    }else if(cola!=0){
                        cola->agregar(busquedaCancion->nombre,busquedaCancion->direccion);
                        cola->graficar();

                        /*QGraphicsScene *scene = new QGraphicsScene();
                        QPixmap m("reproductor.png");
                        scene->setBackgroundBrush(m.scaled(901,361,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
                        this->ui->graphicsView->setScene(scene);/**/
                        QImage img;
                        img.load ("reproductor.png");
                        ui->lblbiblioteca->setPixmap(QPixmap(QPixmap::fromImage(img)));

                        mMediaPlayer->stop();
                        actual = 0;

                    }else{
                        this->ui->label_5->setText("No hay estructura seleccionada");
                    }
                }else{
                    this->ui->label_5->setText("No existe la Cancion");
                }
            }else{
                this->ui->label_5->setText("No existe el Album");
            }
        }else{
            this->ui->label_5->setText("No existe el Artista");
        }
    }

}

void MainWindow::on_pushButton_9_clicked()  // play cancion
{
    if(actual==0){
        if(dobleC!=0 && dobleC->primero!=0){
            actual = dobleC->primero;            
            this->ui->label_3->setText(actual->nombre.c_str());

            mMediaPlayer->setMedia(QUrl::fromLocalFile(actual->direccion.c_str()));
            mMediaPlayer->setVolume(ui->verticalSlider->value());
            mMediaPlayer->play();
            cout<<actual->direccion<<endl;
        }else if(pila!=0 && pila->primero!=0){
            actual = pila->primero;
            this->ui->label_3->setText(actual->nombre.c_str());

            mMediaPlayer->setMedia(QUrl::fromLocalFile(actual->direccion.c_str()));
            mMediaPlayer->setVolume(ui->verticalSlider->value());
            mMediaPlayer->play();

        }else if(cola!=0 && cola->primero!=0){
            actual = cola->primero;
            this->ui->label_3->setText(actual->nombre.c_str());

            mMediaPlayer->setMedia(QUrl::fromLocalFile(actual->direccion.c_str()));
            mMediaPlayer->setVolume(ui->verticalSlider->value());
            mMediaPlayer->play();

        }else{
            this->ui->label_5->setText("No hay estructura seleccionada o la estrucutra esta vacia");
        }
    }else if(actual!=0){
        mMediaPlayer->setMedia(QUrl::fromLocalFile(actual->direccion.c_str()));
        mMediaPlayer->setVolume(ui->verticalSlider->value());
        mMediaPlayer->play();
    }

}

void MainWindow::on_pushButton_8_clicked()  //reproducir anterior cancion
{
    if(actual!=0){
        if(dobleC!=0){
            actual = actual->anterior;
            this->ui->label_3->setText(actual->nombre.c_str());

            mMediaPlayer->stop();
            mMediaPlayer->setMedia(QUrl::fromLocalFile(actual->direccion.c_str()));
            mMediaPlayer->setVolume(ui->verticalSlider->value());
            mMediaPlayer->play();

        }else{
            this->ui->label_5->setText("No habilitado");
        }
    }
}

void MainWindow::on_pushButton_11_clicked() //reproducir siguiente cancion
{
    if(actual!=0){
        if(dobleC!=0){
            actual = actual->siguiente;
            this->ui->label_3->setText(actual->nombre.c_str());

            mMediaPlayer->stop();
            mMediaPlayer->setMedia(QUrl::fromLocalFile(actual->direccion.c_str()));
            mMediaPlayer->setVolume(ui->verticalSlider->value());
            mMediaPlayer->play();

        }else if(pila!=0){
            pila->eliminar();
            if(pila->primero!=0){
                actual = pila->primero;
                this->ui->label_3->setText(actual->nombre.c_str());
                pila->graficar();
                /*
                QGraphicsScene *scene = new QGraphicsScene();
                QPixmap m("reproductor.png");
                scene->setBackgroundBrush(m.scaled(901,361,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
                this->ui->graphicsView->setScene(scene);*/

                QImage img;
                img.load ("reproductor.png");
                ui->lblbiblioteca->setPixmap(QPixmap(QPixmap::fromImage(img)));

                mMediaPlayer->stop();
                mMediaPlayer->setMedia(QUrl::fromLocalFile(actual->direccion.c_str()));
                mMediaPlayer->setVolume(ui->verticalSlider->value());
                mMediaPlayer->play();

            }else{
                pila->graficar();
                /*
                QGraphicsScene *scene = new QGraphicsScene();
                QPixmap m("reproductor.png");
                scene->setBackgroundBrush(m.scaled(901,361,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
                this->ui->graphicsView->setScene(scene);*/

                QImage img;
                img.load ("reproductor.png");
                ui->lblbiblioteca->setPixmap(QPixmap(QPixmap::fromImage(img)));

                actual = 0;
                this->ui->label_3->setText("No hay canciones");
                mMediaPlayer->stop();
            }

        }else if(cola!=0){
            cola->eliminar();
            if(cola->primero!=0){
                actual = cola->primero;
                this->ui->label_3->setText(actual->nombre.c_str());
                cola->graficar();
                /*
                QGraphicsScene *scene = new QGraphicsScene();
                QPixmap m("reproductor.png");
                scene->setBackgroundBrush(m.scaled(901,361,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
                this->ui->graphicsView->setScene(scene);*/

                QImage img;
                img.load ("reproductor.png");
                ui->lblbiblioteca->setPixmap(QPixmap(QPixmap::fromImage(img)));

                mMediaPlayer->stop();
                mMediaPlayer->setMedia(QUrl::fromLocalFile(actual->direccion.c_str()));
                mMediaPlayer->setVolume(ui->verticalSlider->value());
                mMediaPlayer->play();

            }else{
                cola->graficar();

                /*
                QGraphicsScene *scene = new QGraphicsScene();
                QPixmap m("reproductor.png");
                scene->setBackgroundBrush(m.scaled(901,361,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
                this->ui->graphicsView->setScene(scene);*/

                QImage img;
                img.load ("reproductor.png");
                ui->lblbiblioteca->setPixmap(QPixmap(QPixmap::fromImage(img)));

                actual = 0;
                this->ui->label_3->setText("No hay canciones");
                mMediaPlayer->stop();
            }
        }else{
            this->ui->label_5->setText("No hay estructura seleccionada");
        }
    }
}

void MainWindow::on_pushButton_12_clicked() // eliminar de biblioteca
{
    QString dato = this->ui->lineEdit->text();
    string Cancion = dato.toStdString();
    if(Cancion!=" " && Cancion!=""){
        char sep = '_';
        string nombreArtista;
        string nombreAlbum;
        string nombreCancion;

        for(int p=0, q=0,n=0;p!=Cancion.npos;p=q,n++){
            if(n==0)
                nombreArtista = Cancion.substr(p+(p!=0),(q=Cancion.find(sep,p+1))-p-(p!=0));
            else if(n==1)
                nombreAlbum = Cancion.substr(p+(p!=0),(q=Cancion.find(sep,p+1))-p-(p!=0));
            else if(n==2)
                nombreCancion = Cancion.substr(p+(p!=0),(q=Cancion.find(sep,p+1))-p-(p!=0));
        }
        Nodo* tmpArtista;
        Nodo* busquedaArtista;
        Nodo1* tmpAlbum;
        Nodo1* busquedaAlbum;
        Nodo2* tmpCancion;
        Nodo2* busquedaCancion;

        /*<------------- artista ----------------->*/
        tmpArtista = nuevo->primeroDcircular;
        busquedaArtista = nuevo->buscar(tmpArtista,nombreArtista);
        if(busquedaArtista!=0){
            tmpAlbum = busquedaArtista->albums->primeroDoble;
            busquedaAlbum = busquedaArtista->albums->buscarAlbum(tmpAlbum,nombreAlbum);
            if(busquedaAlbum!=0){
                tmpCancion = busquedaAlbum->canciones->primeroSimple;
                busquedaCancion = busquedaAlbum->canciones->buscarCancion(tmpCancion,nombreCancion);
                if(busquedaCancion!=0){
                    //eliminacion de cancion
                    if(busquedaCancion==busquedaAlbum->canciones->primeroSimple && busquedaCancion->siguiente==0){
                        //eliminar album y verificar
                        if(busquedaAlbum==busquedaArtista->albums->primeroDoble && busquedaAlbum->siguiente==0){
                            //eliminar artista <------ elimina artista, album y cancion
                            if(busquedaArtista==nuevo->primeroDcircular){
                                if(busquedaArtista->siguiente==nuevo->primeroDcircular){
                                    nuevo->primeroDcircular = 0;
                                }else{
                                    Nodo* tmp = busquedaArtista->siguiente;
                                    nuevo->eliminar(busquedaArtista);
                                    nuevo->primeroDcircular = tmp;
                                }
                            }else{
                                nuevo->eliminar(busquedaArtista);
                            }
                            this->ui->label_5->setText("Se elimino Artista");
                            return;
                        }else{
                            //llamar a metodo de eliminacion de album
                            if(busquedaAlbum==busquedaArtista->albums->primeroDoble){
                                Nodo1* tmp = busquedaAlbum;
                                busquedaArtista->albums->primeroDoble = tmp->siguiente;
                                tmp = 0;
                                delete tmp;
                            }else{
                                busquedaArtista->albums->eliminar(busquedaAlbum);
                            }
                            this->ui->label_5->setText("Se elimino album");
                            //busquedaAlbum->valoracion = busquedaAlbum->canciones->calcularValoracion(busquedaAlbum->canciones->primeroSimple);
                            busquedaArtista->valoracion = busquedaArtista->albums->calcularValoracion(busquedaArtista->albums->primeroDoble);
                            return;
                        }
                    }else{
                        //llamar metodo de eliminacion cancion
                        if(busquedaAlbum->canciones->primeroSimple==busquedaCancion){
                            Nodo2* tmp = busquedaCancion;
                            busquedaAlbum->canciones->primeroSimple = tmp->siguiente;
                            tmp=0;
                            delete tmp;
                        }else{
                            busquedaAlbum->canciones->eliminar(busquedaAlbum->canciones->primeroSimple,nombreCancion);
                        }
                        this->ui->label_5->setText("Se elimino Cancion");
                        busquedaAlbum->valoracion = busquedaAlbum->canciones->calcularValoracion(busquedaAlbum->canciones->primeroSimple);
                        busquedaArtista->valoracion = busquedaArtista->albums->calcularValoracion(busquedaArtista->albums->primeroDoble);
                        return;
                    }
                }else{
                    this->ui->label_5->setText("No existe la cancion");
                }
            }else{
                this->ui->label_5->setText("No existe el album");
            }
        }else{
            this->ui->label_5->setText("No existe el artista");
        }
    }
}

void MainWindow::on_pushButton_10_clicked() // detener cancion
{
    mMediaPlayer->stop();
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    // barra de progreso
    mMediaPlayer->setVolume(value);
}
