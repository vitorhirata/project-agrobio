# Agricultural diversity modeling

## About
This project contain the code of an agent-based model used to analyse agricultural diversity in traditional agroecosystems. This model was developed during my master's graduation in Complex Systems Modeling at Universidade de São Paulo (USP).

The master's dissertation (in portuguese) is available in this [link](https://www.teses.usp.br/teses/disponiveis/100/100132/tde-13122019-195850/pt-br.php).

## Utilization
To utilize the project start running the script initialize.sh, that will set up the project and dowload the required library in src/lib. In addition to that it is also required a C++ compiler, julia and the julia packages Gadfly and DataFrames, which are used to plot the results.

To compile the code go to /src and run `g++ main.cpp -std=c++11 -I lib/bmp -o program.out`.
To execute the code run ./program.out and the desired mode: 's' for standard, 'p' for plot, 'v' for parameter variation, and 'f' to get the fixed points under parameter variation. For 'v' and 'f' mode it is necessary to type the parameter that will vary, for example `./program.out v a 0 0.5 1` will execute the program with α assuming the values of 0, 0.5 and 1.

The result will be saved in src/test, in a csv format. The julia file src/plot.jl contain the function plotAll that plot all the csv files in the src/test folder.
