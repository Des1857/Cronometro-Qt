#pragma once

#include "Cronometro.h"

#include <QWidget>

class QLabel;
class QPushButton;
class QTimer;
class QTableWidget;
class QToolButton;
class QMenu;
class QAction;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void alternarCronometro();
    void iniciar();
    void pausar();
    void reiniciar();
    void actualizarTiempo();
    void agregarVuelta();
    void cambiarTamanoDigitos();
    void cambiarTema();
    void mostrarAcercaDe();

private:
    QLabel *tiempo;

    QPushButton *botonPrincipal;
    QPushButton *botonReiniciar;
    QPushButton *botonVuelta;
    QToolButton *botonConfiguracion;

    QMenu *menuConfiguracion;
    QAction *accionTamanoDigitos;
    QAction *accionCambiarTema;
    QAction *accionAcercaDe;

    QTimer *timer;
    Cronometro cronometro;

    QTableWidget *tablaVueltas;

    int numeroVuelta = 1;
    qint64 ultimaVuelta = 0;

    bool modoOscuro = false;

    QString formatearTiempo(qint64 ms);

    void cargarConfiguracion();
    void guardarConfiguracion();
};