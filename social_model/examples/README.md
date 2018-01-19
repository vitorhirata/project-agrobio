# Biological Model Examples
Here it's presented some tests of the model use.

## Simple Examples
##### Test with the standard model. The source file is presented in 'standard1.csv'.
```C
### PARAMETERS VALUE ###
### LATTICESIZE = 49, NSPECIE = 100, NRESOURCE = 3, DEATHPROB = 0.1, MUTATIONPROB = 0.001,
### NRESOURCEDIST = 49, NPROPRIETY = 49, MAXTIME = 10000, TIMEINTERVAL = 10, NRUN = 10
```
![Model1](standard1.svg)

##### Another test with standard model, now with no mutation and increased MAXTIME. The source file is presented in 'standard2.csv'.
```C
### PARAMETERS VALUE ###
### LATTICESIZE = 49, NSPECIE = 100, NRESOURCE = 3, DEATHPROB = 0.1, MUTATIONPROB = 0,
### NRESOURCEDIST = 49, NPROPRIETY = 49, MAXTIME = 100000, TIMEINTERVAL = 100, NRUN = 10 ###
```
![Model2](standard2.svg)

## Parameter Variation
##### LATTICESIZE variation (NRESOURCEDIST and NPROPRIETY set equal to actual LATTICESIZE).
```C
### PARAMETERS VALUE ###
### NSPECIE = 100, NRESOURCE = 3, DEATHPROB = 0.1, MUTATIONPROB = 0.001, MAXTIME = 10000,
### TIMEINTERVAL = 10, NRUN = 10
```
![LATTICESIZE Variation multiple resource](varParam_L.svg)

##### DEATHPROB variation.
```C
### PARAMETERS VALUE ###
### LATTICESIZE = 49, NSPECIE = 100, NRESOURCE = 3, MUTATIONPROB = 0.001, NRESOURCEDIST = 49,
### NPROPRIETY = 49, MAXTIME = 10000, TIMEINTERVAL = 10, NRUN = 10
```
![DEATHPROB Variation multiple resource](varParam_u.svg)

##### MUTATIONPROB variation.
```C
### PARAMETERS VALUE ###
### LATTICESIZE = 49, NSPECIE = 100, NRESOURCE = 3, DEATHPROB = 0.1, MUTATIONPROB = 0.005,
### NRESOURCEDIST = 49, NPROPRIETY = 49, MAXTIME = 10000, TIMEINTERVAL = 10, NRUN = 10
```
![MUTATIONPROB Variation multiple resource](varParam_m.svg)

##### NRESOURCEDIST variation.
```C
### PARAMETERS VALUE ###
### LATTICESIZE = 49, NSPECIE = 100, NRESOURCE = 3, DEATHPROB = 0.1, MUTATIONPROB = 0.001,
### NPROPRIETY = 49, MAXTIME = 10000, TIMEINTERVAL = 10, NRUN = 10
```
![NRESOURCEDIST Variation multiple resource](varParam_r.svg)

##### NSPECIE variation.
```C
### PARAMETERS VALUE ###
### LATTICESIZE = 49, NRESOURCE = 3, DEATHPROB = 0.1, MUTATIONPROB = 0.001,
### NRESOURCEDIST = 49, NPROPRIETY = 49, MAXTIME = 10000, TIMEINTERVAL = 10, NRUN = 10
```
![NSPECIE Variation multiple resource](varParam_n.svg)

##### standard Plot
```C
### PARAMETERS VALUE ###
### LATTICESIZE = 49, NSPECIE = 100, NRESOURCE = 3, DEATHPROB = 0.1, MUTATIONPROB = 0.001,
### NRESOURCEDIST = 49, NPROPRIETY = 49, MAXTIME = 10000, TIMEINTERVAL = 100
```
Click <a href="https://github.com/vitorhirata/project-agrobio/raw/master/social_model/examples/Runplot.avi" download> here </a> to download a video of an execution of the program.
