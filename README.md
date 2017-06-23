# Undersmoothed uncertainty quantification for unfolding    

### Authors: Junhyung (Lyle) Kim and Mikael Kuusela, University of Chicago

This page provides an extension for the existing software [TUnfold V17.6](https://www.desy.de/~sschmitt/tunfold.html) written by Stefan Schmitt. The existing software provides several algorithms to unfold measured data from particle detectors to obtain the true physical spectra, but often suffers significant undercoverage with regards to uncertainty quantification. This extension provides another unfolding methodology, `UndersmoothTau`, which is a fully data-driven way of unfolding while meeting the desired coverage at the cost of slight increase in the interval lengths. `UndersmoothTau` is an implementation of undersmoothing debias technique proposed by M. Kuusela (2016). Please refer to the references section for more detail on the statistical methodology.

## Main functions

| Function | Input | Output | Description |
| --- | --- | --- | --- |
| UndersmoothTau | Initial tau, epsilon, max iteration | Undersmoothed Tau | Undersmooths initial tau gradually until the minimum computed coverage meets the desired coverage, which is the nominal coverage subtracted by epsilon. |
| ComputeCoverage | Vector of estimation of the true distribution, tau | Vector of computed coverage | Given a vector of estimation of the true distribution and a regularization strength parameter tau, computes the estimated coverage. |
| ComputeCoverageOracle | Vector of the true distribution, tau | Vector of computed coverage | Same as `ComputeCoverage` but input is the true spectrum. Used for comparing interval lengths. |

## Simulation

This simulation compares the coverage performances of the existing unfolding algorithm `ScanLcurve` provided by `TUnfold` and the algorithm `UndersmoothTau` provided by this extension. Top plots compare binwise coverages of each method, which are obtained by repeating unfolding 1,000 times with independent realizations of data with each algorithm. Bottom plots show one realization of unfolding of each method.

![Simulation](UndersmoothTauSimulation.png)

## Installation
The easiest way to make use of undersmoothed unfolding as of now is to replace the `TUnfold` sourcefile and the headerfile that come with `ROOT` package with the ones in this extension page, and then recompile `ROOT`. For the latest `ROOT V6.10` release, the `TUnfold` sourcefile is located at `root/hist/unfold/src` and the headerfile is located at `root/hist/unfold/inc`.

## Known issues
* Current version is intended to be used with `kEConstraintNone` for the area constraint option when constructing a `TUnfold` object.

## References
[M. Kuusela. Uncertainty quantification in unfolding elementary particle spectra at the Large Hadron Collider. PhD thesis, École Polytechnique Fédérale de Lausanne (EPFL), 2016.](https://infoscience.epfl.ch/record/220015)

[S. Schmitt. TUnfold, an algorithm for correcting migration effects in high energy physics. Journal of Instrumentation, 7:T10003, 2012.](http://iopscience.iop.org/1748-0221/7/10/T10003/)
