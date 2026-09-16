#pragma once

#include <QElapsedTimer>

class Cronometro
{
public:
    void iniciar();
    void pausar();
    void reiniciar();

    qint64 tiempoActual() const;
    bool estaCorriendo() const;

private:
    QElapsedTimer reloj;
    qint64 tiempoAcumulado = 0;
    bool corriendo = false;
};