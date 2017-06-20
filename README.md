# Undersmoothed uncertainty quantification for unfolding    

Extension for the existing software [TUnfold](https://www.desy.de/~sschmitt/tunfold.html) written by Stefan Schmitt. The existing software provides several algorithms to unfold measured data from particle detectors to obtain the true physical spectra, but often suffers significant undercoverage with regards to uncertainty quantification. This extension provides another unfolding methodology, `DebiasTau`, which is a fully data-driven way of unfolding while meeting the desired coverage at the cost of slight increase in the interval lengths.

## Main functions

| Function | Input | Output | Description |
| --- | --- | --- | --- |
| DebiasTau() | Initial Tau, Epsilon(tolerance), Max iteration | Debiased Tau | Debiases initial tau gradually until the minimum computed coverage meets the desired coverage, which is the nominal coverage subtracted by epsilon. |

## Demo

## Installation
The easiest way to make use of undersmoothed unfolding as of now is to replace the `TUnfold` sourcefile that comes with `ROOT` package. For the latest `ROOT` release, the `TUnfold` sourcefile is located at `root/hist/unfold/src` and the headerfile is located at `root/hist/unfold/inc`.


### Copyright (c) 2016-2017 Junhyung Kim and Mikael Kuusela
