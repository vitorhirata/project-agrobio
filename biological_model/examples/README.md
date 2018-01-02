# Biological Model Examples
Here it's presented some tests of the model use

## Simple Examples
* Test similar to the original model, multiple resource. The source file is presented in 'standart1.csv'.
> ### PARAMETERS VALUE ###
> ### LATTICESIZE = 500, NSPECIE = 1000, NRESOURCE = 3, DEATHPROB = 0.1, MUTATIONPROB = 0, NRESOURCEDIST = 250000, MAXTIME = 1000000, TIMEINTERVAL = 10000, NRUN = 50 ###

![Model1](standart1.svg)

* Test similar to the original model, uniform resource. The source file is presented in 'standart2.csv'.
> ### PARAMETERS VALUE ###
> ### LATTICESIZE = 500, NSPECIE = 1000, NRESOURCE = 3, DEATHPROB = 0.1, MUTATIONPROB = 0, NRESOURCEDIST = 1, MAXTIME = 1000000, TIMEINTERVAL = 10000, NRUN = 10 ###

![Model2](standart2.svg)

## Parameter Variation
* LATTICESIZE variation (multiple resource L*L).
> ### PARAMETERS VALUE ###
> ### NSPECIE = 100, NRESOURCE = 3, DEATHPROB = 0.1, MUTATIONPROB = 0, MAXTIME = 100000, TIMEINTERVAL = 1000, NRUN = 10 ###

![LATTICESIZE Variation multiple resource](varParam_L.svg)

* LATTICESIZE variation (fix 10 resource).
> ### PARAMETERS VALUE ###
> ### NSPECIE = 100, NRESOURCE = 3, DEATHPROB = 0.1, MUTATIONPROB = 0, NRESOURCEDIST = 10, MAXTIME = 100000, TIMEINTERVAL = 1000, NRUN = 10 ###

![LATTICESIZE Variation 10 resources](varParam_L_10r.svg)

* DEATHPROB variation (multiple resource L*L).
> ### PARAMETERS VALUE ###
> ### ### LATTICESIZE = 50, NSPECIE = 100, NRESOURCE = 3, MUTATIONPROB = 0, NRESOURCEDIST = 2500, MAXTIME = 100000, TIMEINTERVAL = 1000, NRUN = 10 ###

![DEATHPROB Variation multiple resource](varParam_u.svg)

* MUTATIONPROB variation (multiple resource L*L).
> ### PARAMETERS VALUE ###
> ### LATTICESIZE = 50, NSPECIE = 100, NRESOURCE = 3, DEATHPROB = 0.1, NRESOURCEDIST = 2500, MAXTIME = 100000, TIMEINTERVAL = 1000, NRUN = 10 ###

![MUTATIONPROB Variation multiple resource](varParam_m.svg)

* NRESOURCEDIST variation (multiple resource L*L).
> ### PARAMETERS VALUE ###
> ### LATTICESIZE = 50, NSPECIE = 100, NRESOURCE = 3, DEATHPROB = 0.1, MUTATIONPROB = 0, MAXTIME = 100000, TIMEINTERVAL = 1000, NRUN = 10 ###

![NRESOURCEDIST Variation multiple resource](varParam_r.svg)

* NSPECIE variation (multiple resource L*L).
> ### PARAMETERS VALUE ###
> ### LATTICESIZE = 50, NRESOURCE = 3, DEATHPROB = 0.1, MUTATIONPROB = 0, NRESOURCEDIST = 2500, MAXTIME = 100000, TIMEINTERVAL = 1000, NRUN = 10 ###

![NSPECIE Variation multiple resource](varParam_n.svg)
