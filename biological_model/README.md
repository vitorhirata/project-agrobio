# Biological Model

This model takes into account a biological model for the study of biodiversity, based on: de Souza Júnior, Misael B., Fernando F. Ferreira, and Viviane M. de Oliveira. *Effects of the spatial heterogeneity on the diversity of ecosystems with resource competition.* Physica A: Statistical Mechanics and its Applications 393 (2014): 312-319. doi:[10.1016/j.physa.2013.08.045](https://doi.org/10.1016/j.physa.2013.08.045).

## Requirements
This program uses, besides some standard, two libraries: [boost](http://www.boost.org/) and [EasyBMP](https://github.com/aburgh/EasyBMP). In the link it is possible to download them. Both have been put in /usr/local/.

## Running
To compile the code, using g++, type:
``` bash
g++ -o model -I /usr/local/boost_1_65_1/ -I /usr/local/bpm  model-bio.cpp -std=c++11
```

To run the standard mode type:
``` bash
./model s
```

To run the plot mode:
``` bash
./model p
```

To run the model variating some parameter (L, u, m, n, r) run `./model v $parameter $list_of_values`, where $list_of_values is the values assumed for the parameter, for example:
``` bash
./model v L 10 30 50 70 100
```

To run the plotting in julia type `julia plot.jl $name_of_file.csv $mode`, where mode if multiple when there are some parameter variation or simple if there aren't. For example:
``` bash
julia plot.jl varParam_L.csv multiple
```


## Examples
There is a file with tests of the model simulation, with the data generated and images of that. In the data file it is also presented the parameters used and additional information.

## TODO
- Implement tests.
- Improve comments.
