#include "MainWindow.h"

#include <QStyle>
#include <QAction>
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFont>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QSettings>
#include <QSlider>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTimer>
#include <QToolButton>
#include <QVBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow()
{
    resize(400, 300);
    setWindowTitle("Cronómetro");

    tiempo = new QLabel("00:00.000");

    QFont fuente("Consolas", 32);
    fuente.setBold(true);

    tiempo->setFont(fuente);
    tiempo->setAlignment(Qt::AlignCenter);

    botonPrincipal = new QPushButton("Iniciar");
    botonReiniciar = new QPushButton("Reiniciar");
    botonVuelta = new QPushButton("Vuelta");

    botonVuelta->setEnabled(false);
    botonReiniciar->setEnabled(false);

    botonConfiguracion = new QToolButton;
    botonConfiguracion->setText("⚙");
    botonConfiguracion->setToolTip("Configuración");

    menuConfiguracion = new QMenu(this);

    accionTamanoDigitos =
        menuConfiguracion->addAction("Tamaño de los dígitos...");

    accionCambiarTema =
        menuConfiguracion->addAction("Cambiar tema");

    menuConfiguracion->addSeparator();

    accionAcercaDe =
        menuConfiguracion->addAction("Acerca de");

    botonConfiguracion->setMenu(menuConfiguracion);
    botonConfiguracion->setPopupMode(QToolButton::InstantPopup);

    tablaVueltas = new QTableWidget;

    tablaVueltas->setColumnCount(3);
    tablaVueltas->setHorizontalHeaderLabels({"#", "Total", "Vuelta"});

    tablaVueltas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tablaVueltas->verticalHeader()->setVisible(false);
    tablaVueltas->verticalHeader()->setDefaultSectionSize(28);

    tablaVueltas->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablaVueltas->setSelectionMode(QAbstractItemView::NoSelection);
    tablaVueltas->setFocusPolicy(Qt::NoFocus);

    QFont fuenteTabla("Consolas", 11);
    tablaVueltas->setFont(fuenteTabla);

    tablaVueltas->setAlternatingRowColors(true);
    tablaVueltas->setShowGrid(false);

    timer = new QTimer(this);

    auto *layout = new QVBoxLayout;
    auto *botones = new QHBoxLayout;
    auto *barraSuperior = new QHBoxLayout;

    barraSuperior->addStretch();
    barraSuperior->addWidget(botonConfiguracion);

    layout->setSpacing(15);
    layout->setContentsMargins(15, 15, 15, 15);

    botones->addWidget(botonPrincipal);
    botones->addWidget(botonReiniciar);
    botones->addWidget(botonVuelta);

    layout->addLayout(barraSuperior);
    layout->addLayout(botones);
    layout->addWidget(tiempo);
    layout->addWidget(tablaVueltas);

    setLayout(layout);

    connect(botonPrincipal, &QPushButton::clicked,
            this, &MainWindow::alternarCronometro);

    connect(botonReiniciar, &QPushButton::clicked,
            this, &MainWindow::reiniciar);

    connect(botonVuelta, &QPushButton::clicked,
            this, &MainWindow::agregarVuelta);

    connect(timer, &QTimer::timeout,
            this, &MainWindow::actualizarTiempo);

    connect(accionTamanoDigitos, &QAction::triggered,
            this, &MainWindow::cambiarTamanoDigitos);

    connect(accionCambiarTema, &QAction::triggered,
            this, &MainWindow::cambiarTema);

    connect(accionAcercaDe, &QAction::triggered,
            this, &MainWindow::mostrarAcercaDe);

    cargarConfiguracion();
}

void MainWindow::alternarCronometro()
{
    if (!cronometro.estaCorriendo())
    {
        iniciar();
    }
    else
    {
        pausar();
    }
}

void MainWindow::iniciar()
{
    cronometro.iniciar();

    timer->start(10);
    botonPrincipal->setText("Pausar");

    botonVuelta->setEnabled(true);
    botonReiniciar->setEnabled(true);
}

void MainWindow::pausar()
{
    cronometro.pausar();

    timer->stop();
    botonPrincipal->setText("Iniciar");
}

void MainWindow::reiniciar()
{
    cronometro.reiniciar();

    timer->stop();

    botonPrincipal->setText("Iniciar");
    tiempo->setText("00:00.000");

    botonVuelta->setEnabled(false);
    botonReiniciar->setEnabled(false);

    numeroVuelta = 1;
    tablaVueltas->setRowCount(0);
    ultimaVuelta = 0;
    ;
}

void MainWindow::actualizarTiempo()
{
    qint64 tiempoActual = cronometro.tiempoActual();

    tiempo->setText(formatearTiempo(tiempoActual));
}

QString MainWindow::formatearTiempo(qint64 ms)
{
    int minutos = ms / 60000;
    int segundos = (ms / 1000) % 60;
    int milisegundos = ms % 1000;

    return QString("%1:%2.%3")
        .arg(minutos, 2, 10, QChar('0'))
        .arg(segundos, 2, 10, QChar('0'))
        .arg(milisegundos, 3, 10, QChar('0'));
}

void MainWindow::agregarVuelta()
{
    qint64 tiempoActual = cronometro.tiempoActual();
    qint64 tiempoVuelta = tiempoActual - ultimaVuelta;

    int fila = tablaVueltas->rowCount();
    tablaVueltas->insertRow(fila);

    QTableWidgetItem *itemNumero =
        new QTableWidgetItem(QString::number(numeroVuelta));
    itemNumero->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *itemTotal =
        new QTableWidgetItem(formatearTiempo(tiempoActual));
    itemTotal->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *itemVuelta =
        new QTableWidgetItem(formatearTiempo(tiempoVuelta));
    itemVuelta->setTextAlignment(Qt::AlignCenter);

    tablaVueltas->setItem(fila, 0, itemNumero);
    tablaVueltas->setItem(fila, 1, itemTotal);
    tablaVueltas->setItem(fila, 2, itemVuelta);

    ultimaVuelta = tiempoActual;
    numeroVuelta++;
}

void MainWindow::cambiarTamanoDigitos()
{

    QDialog dialog(this);
    dialog.setWindowTitle("Tamaño de los dígitos");

    int tamanoOriginal = tiempo->font().pointSize();
    auto *layout = new QVBoxLayout;

    auto *texto = new QLabel("Seleccione el tamaño:");
    layout->addWidget(texto);

    auto *slider = new QSlider(Qt::Horizontal);

    slider->setMinimum(20);
    slider->setMaximum(60);
    slider->setValue(tiempo->font().pointSize());

    layout->addWidget(slider);

    auto *botones = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    layout->addWidget(botones);

    dialog.setLayout(layout);

    connect(slider, &QSlider::valueChanged,
            this, [this](int valor)
            {
        QFont fuente = tiempo->font();
        fuente.setPointSize(valor);
        fuente.setBold(true);

        tiempo->setFont(fuente); });

    connect(botones, &QDialogButtonBox::accepted,
            &dialog, &QDialog::accept);

    connect(botones, &QDialogButtonBox::rejected,
            &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Rejected)
    {
        QFont fuente = tiempo->font();
        fuente.setPointSize(tamanoOriginal);
        fuente.setBold(true);

        tiempo->setFont(fuente);
    }
}

void MainWindow::cambiarTema()
{
    modoOscuro = !modoOscuro;

    if (modoOscuro)
    {
        qApp->setStyleSheet(
            "QWidget {"
            "    background-color: #202020;"
            "    color: white;"
            "}"
            "QPushButton {"
            "    background-color: #333333;"
            "    color: white;"
            "    border: 1px solid #555555;"
            "    padding: 6px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #444444;"
            "}"
            "QTableWidget {"
            "    background-color: #282828;"
            "    color: white;"
            "    gridline-color: #555555;"
            "}"
            "QHeaderView::section {"
            "    background-color: #333333;"
            "    color: white;"
            "}"
            "QMenu {"
            "    background-color: #333333;"
            "    color: white;"
            "}"
            "QMenu::item:selected {"
            "    background-color: #555555;"
            "}");
    }
    else
    {
        qApp->setStyleSheet("");
    }

    guardarConfiguracion();
}

void MainWindow::cargarConfiguracion()
{
    QSettings configuracion("Ezequiel", "Cronometro");

    modoOscuro =
        configuracion.value("modoOscuro", false).toBool();
    if (modoOscuro)
    {
        qApp->setStyleSheet(
            "QWidget {"
            "    background-color: #202020;"
            "    color: white;"
            "}"
            "QPushButton {"
            "    background-color: #333333;"
            "    color: white;"
            "    border: 1px solid #555555;"
            "    padding: 6px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #444444;"
            "}"
            "QTableWidget {"
            "    background-color: #282828;"
            "    color: white;"
            "    gridline-color: #555555;"
            "}"
            "QHeaderView::section {"
            "    background-color: #333333;"
            "    color: white;"
            "}"
            "QMenu {"
            "    background-color: #333333;"
            "    color: white;"
            "}"
            "QMenu::item:selected {"
            "    background-color: #555555;"
            "}");
    }
    else
    {
        qApp->setStyleSheet("");
    }
}

void MainWindow::guardarConfiguracion()
{
    QSettings configuracion("Ezequiel", "Cronometro");

    configuracion.setValue("modoOscuro", modoOscuro);
}

void MainWindow::mostrarAcercaDe()
{
    QMessageBox::about(
        this,
        "Acerca de Cronómetro",
        "<h2>Cronómetro</h2>"
        "<p>Aplicación de cronómetro desarrollada en C++ "
        "utilizando Qt.</p>"
        "<p>Versión 1.0</p>");
}