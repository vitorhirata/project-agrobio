# Agricultural diversity modeling

## About
This project contains the code of an agent-based model used to analyze varietal agricultural diversity in traditional agro-ecosystems. It is the result of my master's degree in Complex Systems Modeling at Universidade de São Paulo (USP).

This work is published at Plos One:

Hirata Sanches, V., Adams, C. and Ferreira, F.F., 2022. An integrated model to study varietal diversity in traditional agroecosystems. Plos one, 17(1), p.e0263064. DOI: [10.1371/journal.pone.0263064](https://doi.org/10.1371/journal.pone.0263064) 

## Running the model

To run the model, run the executable `src/agrobiodiversity_model.out` and the desired mode:
- 's' for a standard run with default parameters
- 'p' for plotting model's state of one execution of the model
- 'v' for a compared run varying the specified parameter with specified values
- 'f' for generating the model's final states under a variation of the specified parameter variation
- 'd' for generating a phase diagram varying two specified parameters

For example, run `./agrobiodiversity_model.out s` to execute the standard run or `./agrobiodiversity_model.out v V 5 10 25 50 100` to execute the model with the initial number of varieties in the community (V) assuming the values of 5, 10, 25, 50 and 100. The result will be saved in the src/test folder (if the file does not exist you need to create it) in the csv format.

[Julia](https://julialang.org/) and the packages [Gadfly](http://gadflyjl.org/stable/) and [DataFrames](https://dataframes.juliadata.org/stable/) are required for plotting the results. Run the julia console in the src folder, then run `include("plot.jl")` to include the plot file and finally run `plotAll` to plot all the csv files in the src/test folder.

## Compiling the model
This setup was only tested on Linux operating system and requires [GCC](https://gcc.gnu.org/) compiler. To compile the code run the makefile with `make`. The output will be a agrobiodiversity_model.out in the root of the project.

