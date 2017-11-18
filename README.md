# project-agrobio

## Synopsis
This is a project I am doing at my Master in Complex System at Universidade de São Paulo (USP), regarding emergence and maintenance of agrobiodiversity. C++ language are being used at this project.

## Parts
This project will have two different parts, one social and one biological (the file *model-bio.cpp*).

## Tests
This images presents tests for the model.
- test1 - Test with k=1000 species, n=3 resources, L=500, t=1E6, nRun=2, m=0.0, u=0.1
- test2 - Test with k=500 species, n=3 resources, L=500, t=1E5, nRun=2, m=0.1, u=0.1
- test3 - Test with k=100 species, n=3 resources, L=50, t=1E6, nRun=10, m=0.0, u=0.1
- test4 - Test with k=100 species, n=3 resources, L=50, t=1E6, nRun=10, m=0.01, u=0.1

## Future Steps
- Make possible to use uniform resource along the patch.
- Add a global class 'ambient'.
- Optimeze 'haveNeighbor' and 'fitness' function.
