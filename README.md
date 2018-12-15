# Agrobiodiversity Simulation

## About
This is a project I am doing at my Master in Complex System at Universidade de São Paulo (USP), regarding emergence and maintenance of agrobiodiversity in traditional communities. This project will analyse three different dynamics, to study their importance for agrobiodiversity: the social dynamics (ex. seed exchange networks), biologic dynamics (ex. productivity, adaptivity) and the individual criteria for seed selection (ex. cultural importance, size of useful parts). For more information consult this [Master's Project](Masters_Project.pdf) (in portuguese). To see preliminar results see [results](results/).

## Utilization
To utilize the project start running the script initialize.sh, that will set up the project and dowload the required library in src/lib.
To compile the code go to /src and run "g++ main.cpp -std=c++11 -I lib/bmp -o program.out".
To execute the code run ./program.out and the desired mode: 's' for standard, 'p' for plot, 'v' for parameter variation, and 'f' to get the fixed points under parameter variation.
For 'v' and 'f' mode it is necessary to type the parameter that will vary: 'a', 'i', 'o', 'n', 'r', 'v', 'L'.
The result will be saved in src/test, in a csv format. To plot this results run "julia plot.jl name-of-file".
