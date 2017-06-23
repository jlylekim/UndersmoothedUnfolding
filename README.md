# Undersmoothed uncertainty quantification for unfolding    

### Authors: Junhyung (Lyle) Kim and Mikael Kuusela, University of Chicago

This repository provides an extension for the unfolding software [TUnfold V17.6](https://www.desy.de/~sschmitt/tunfold.html) written by Stefan Schmitt. TUnfold implements Tikhonov regularization for unfolding smeared data from particle detectors. However, as demonstrated by Kuusela (2016), the resulting confidence intervals may seriously underestimate the uncertainty in the unfolded space, unless care is taken in the choice of the regularization strength. This extension provides a new function for choosing the regularization strength, `UndersmoothTau`, which aims at choosing the regularization strength so that the confidence intervals have nearly nominal coverage. `UndersmoothTau` is an implementation of the data-driven undersmoothing technique introduced in Kuusela (2016). Please refer to the references section for more details on the statistical methodology.

## Main functions

| Function | Input | Output | Description |
| --- | --- | --- | --- |
| UndersmoothTau | Initial tau, epsilon, max iteration | Undersmoothed Tau | Undersmooths initial tau gradually until the minimum computed coverage meets the desired coverage, which is the nominal coverage subtracted by epsilon. |
| ComputeCoverage | Vector of estimation of the true distribution, tau | Vector of computed coverage | Given a vector of estimation of the true distribution and a regularization strength parameter tau, computes the estimated coverage. |
| ComputeCoverageOracle | Vector of the true distribution, tau | Vector of computed coverage | Same as `ComputeCoverage` but input is the true spectrum. Used for comparing interval lengths. |

## Demonstration

This simulation compares the coverage performances of the existing unfolding algorithm `ScanLcurve` provided by `TUnfold` and the algorithm `UndersmoothTau` provided by this extension. Top plots compare binwise coverages of each method, which are obtained by repeating unfolding 1,000 times with independent realizations of data with each algorithm. Bottom plots show one realization of unfolding of each method.

![Demonstration](UndersmoothTauSimulation.png)

## Installation
As of now, the easiest way to use this code is to replace the `TUnfold` sourcefile and headerfile that come with ROOT with the ones in this repository and then recompile ROOT. For the latest [ROOT V6.10](https://root.cern.ch/content/release-61000) release, the `TUnfold` sourcefile is located at `root/hist/unfold/src` and the headerfile is located at `root/hist/unfold/inc`.

## Known issues
* The current version is intended to be used with `kEConstraintNone` for the area constraint option when constructing the `TUnfold` object.

## References
[M. Kuusela. Uncertainty quantification in unfolding elementary particle spectra at the Large Hadron Collider. PhD thesis, École Polytechnique Fédérale de Lausanne (EPFL), 2016.](https://infoscience.epfl.ch/record/220015)

[S. Schmitt. TUnfold, an algorithm for correcting migration effects in high energy physics. Journal of Instrumentation, 7:T10003, 2012.](http://iopscience.iop.org/1748-0221/7/10/T10003/)
