#include "Cronometro.h"

void Cronometro::iniciar()
{
    if (corriendo)
        return;

    corriendo = true;
    reloj.start();
}

void Cronometro::pausar()
{
    if (!corriendo)
        return;

    tiempoAcumulado += reloj.elapsed();
    corriendo = false;
}

void Cronometro::reiniciar()
{
    tiempoAcumulado = 0;
    corriendo = false;
}

qint64 Cronometro::tiempoActual() const
{
    if (corriendo)
        return tiempoAcumulado + reloj.elapsed();

    return tiempoAcumulado;
}

bool Cronometro::estaCorriendo() const
{
    return corriendo;
}