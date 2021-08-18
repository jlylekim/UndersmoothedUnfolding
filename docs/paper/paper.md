---
title: 'UndersmoothedUnfolding: Undersmoothed uncertainty quantification for unfolding in ROOT'
tags:
  - unfolding
  - uncertainty quantification
  - deconvolution
  - inverse problem
  - undersmoothing
  - high energy physics
authors:
  - name: Junhyung Lyle Kim
    orcid: 0000-0002-4774-5750
    affiliation: "1"
    email: jlylekim@rice.edu
  - name: Mikael Kuusela
    orcid: 0000-0001-6875-945X
    affiliation: "2"
    email: mkuusela@andrew.cmu.edu
affiliations:
 - name: Rice University
   index: 1
 - name: Carnegie Mellon University
   index: 2
date: 18 August 2021
bibliography: paper.bib
---

# Summary

The high energy physics unfolding problem [@Lyons2011; @Cowan1998; @Blobel2013; @Zech2016; @Kuusela2016] is an ill-posed inverse problem arising in data analysis at the Large Hadron Collider (LHC) at CERN. Due to the limited resolution of particle detectors, any measurement made at the LHC is smeared (noisy), and the goal in unfolding is to make inferences about the true particle spectrum given the smeared observations. Mathematically, the problem is to infer the intensity function of an indirectly observed Poisson point process.

Unfolding is a common task in experimental high-energy physics, and rigorous uncertainty quantification of the unfolded spectrum is of central importance to particle physicists. The standard approach is to form a regularized point estimator, and then to form frequentist confidence intervals using the variability of this estimator. However, these confidence intervals can seriously underestimate the true uncertainty since they ignore the bias that is used to regularize the ill-posed problem [@Kuusela2015; @Kuusela2016; @Kuusela2017]. 

To address this issue, @Kuusela2016 proposed a data-driven technique for adjusting the bias-variance trade-off in order to achieve better uncertainty quantification. The method consists in reducing the regularization strength from a pilot estimate until the frequentist coverage of the unfolded uncertainties is estimated to be at a near-nominal level. `UndersmoothedUnfolding` provides a software implementation of this method for `ROOT` [@root1997], the de facto data analysis framework used in high energy physics. Our implementation is based on extending `TUnfold` [@tunfold2012], an existing unfolding library for ROOT, to include new functionality for unfolding with data-driven undersmoothing. Please refer to the `Simulated example` section below for a demonstration of `UndersmoothedUnfolding`.

# Statement of need

Most existing unfolding software frameworks do not implement data-driven methods for choosing the regularization strength. If data-driven methods are implemented, they are usually based on optimizing the point estimation performance of the unfolded estimator, which leads to too strong regularization from the perspective of uncertainty quantification. For example, 'TUnfold' implements the L-curve method, global correlation coefficient minimization and, more recently, SURE minimization [@Schmitt2019]. All of these techniques aim for optimal point estimation and not optimal uncertainty quantification. The method proposed in @Kuusela2016 and implemented in `UndersmoothedUnfolding` is fundamentally different as it aims to optimize uncertainty quantification instead of point estimation, leading to drastic improvements in the calibration of the uncertainties.

Recently, @Brenner2020 provided an implementation a related method based on optimizing the uncertainties with respect to an ansatz solution. In addition to being based on a different underlying library, the key difference between our implementation here and that of @Brenner2020 is that our method is fully data-driven and does not require specifying an ansatz. As such, the required software implementation in `UndersmoothedUnfolding` is more complex, but it provides a fully automated method for well-calibrated unfolded uncertainty quantification.

# Simulated example

`UndersmoothedUnfolding` is implemented so that it can be used with any pilot estimate of the regularization strength. We demonstrate the performance of the main function `UndersmoothTau` in a case with two Gaussian peaks on top of a uniform background, resembling situations where invariant mass peaks are observed on top of some background events.

![Demonstration](UndersmoothDemo.png)

The simulation above compares the performance of the unfolded confidence intervals when the regularization strength is chosen using `ScanLcurve` provided by `TUnfold` and the algorithm `UndersmoothTau` provided by this software. The tolerance epsilon was set to 0.01, so the intervals from `UndersmoothTau` should have 67% coverage. The top plots compare the binwise coverage of the methods. The coverage is estimated by repeating the unfolding 1,000 times with independent realizations of the data. The bottom plots show one realization of the unfolded confidence intervals for each method. The confidence intervals provided by `ScanLcurve` are too short and suffer from drastic undercoverage, while the intervals provided by `UndersmoothTau` have nearly nominal coverage without being excessively long. We emphasize that `UndersmoothTau` chose the required amount of undersmoothing in a fully automated manner based only on the observed data.

Please refer to the [online documentation](https://jlylekim.github.io/UndersmoothedUnfolding/) for further details, which also
contains a second test case with a steeply falling particle spectrum.

# References
