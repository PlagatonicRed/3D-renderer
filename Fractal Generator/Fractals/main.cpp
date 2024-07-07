#include "screen.h"
#include <iostream>
#include <complex>
#include <omp.h>

int fractalDeterminance = 1000;

int computeIterationsMandelbrot(std::complex<double> c, int max_iterations)
{
    std::complex<double> z = 0;
    int iteration = 0;
    while (std::abs(z) < fractalDeterminance && iteration < max_iterations)
    {
        z = z * z + c;
        iteration++;
    }
    return iteration;
}

int computeIterationsJulia(std::complex<double> z0, std::complex<double> constant, int max_iterations)
{
    std::complex<double> z = z0;
    int iteration = 0;
    while (std::abs(z) < fractalDeterminance && iteration < max_iterations)
    {
        z = z * z + constant;
        iteration++;
    }
    return iteration;
}

void CreateFractal(Screen &screen, bool isMandelbrot, std::complex<double> juliaConstant)
{
    int width = screen.WIDTH;
    int height = screen.HEIGHT;

#pragma omp parallel for
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Map pixel coordinates to the complex plane
            double real = (x - width / 2.0) * 4.0 / width;
            double imag = (y - height / 2.0) * 4.0 / height;

            std::complex<double> z(real, imag);

            int iterations;
            if (isMandelbrot)
            {
                iterations = computeIterationsMandelbrot(z, 50);
            }
            else
            {
                iterations = computeIterationsJulia(z, juliaConstant, 50);
            }

            if (iterations == 0)
            {
                SDL_SetRenderDrawColor(screen.renderer, 0, 0, 0, 255);
                SDL_RenderDrawPoint(screen.renderer, x, y);
            }
            else
            {
                // Adjust color based on the number of iterations
                SDL_SetRenderDrawColor(screen.renderer, 2 * (int)iterations % 255, 3 * (int)iterations % 255, 4 * (int)iterations % 255, 255);
                SDL_RenderDrawPoint(screen.renderer, x, y);
            }
        }
    }
}

int main(int argc, char **argv)
{
    double num1, num2;
    bool isMandelbrot;
    std::complex<double> juliaConstant;

    std::cin >> isMandelbrot >> num1 >> num2;;
    juliaConstant = std::complex<double>(num1, num2);

    Screen screen(2000, 2000, 1);
    CreateFractal(screen, isMandelbrot, juliaConstant);
    screen.show();

    while (true)
    {
        screen.Input();
        if (!screen.fractalGenerated && screen.generateFractal)
        {
            std::cin >> isMandelbrot;
            if (!isMandelbrot)
            {
                std::cin >> num1 >> num2;
                juliaConstant = std::complex<double>(num1, num2);
            }
            CreateFractal(screen, isMandelbrot, juliaConstant);
            screen.fractalGenerated = true;
            screen.generateFractal = false;
        }
        else if(screen.iterateFractals)
        {
            screen.clear();
            juliaConstant += std::complex<double>(0.001,0.001);
            CreateFractal(screen, isMandelbrot, juliaConstant);
        }
        screen.show();
        SDL_Delay(100);
    }

    return 0;
}
