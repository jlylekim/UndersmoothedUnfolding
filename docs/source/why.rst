
****************************
Why UndersmoothedUnfolding?
****************************


---------------------------------------------------------
 Optimal uncertainty quantification VS. point estimation
---------------------------------------------------------

UndersmoothedUnfolding an extension for the unfolding software TUnfold V17.6 [2]_ [3]_ written by Stefan Schmitt.
TUnfold implements Tikhonov regularization for unfolding smeared data from particle detectors.
However, as demonstrated by Kuusela (2016) [1]_, the resulting confidence intervals may seriously underestimate
the uncertainty in the unfolded space, unless care is taken in the choice of the regularization strength tau.

.. image:: plots/coverage_peak_bin_curvature.pdf
    :width: 100%


----------------------------------
Coverage for Gaussian observations
----------------------------------
TODO: probably do not need this section

.. image:: plots/empirical_coverage_curvature_lambdaMC.pdf
    :width: 48%

.. image:: plots/computed_coverage_curvature_lambdaMC.pdf
    :width: 48%


.. [1] M. Kuusela, “Uncertainty quantification in unfolding elementary particle spectra at the Large Hadron Collider”, PhD thesis, EPFL (2016)
.. [2] S. Schmitt, “TUnfold, an algorithm for correcting migration effects in high energy physics”, Journal of Instrumentation **7** (2012).
.. [3] R. Brun and F. Rademakers, “ROOT - An Object Oriented Data Analysis Framework”, Nucl. Inst. & Meth. in Phys. Res. A **389** (1997).



------------------
Two peaks spectra
------------------
.. image:: plots/binwise_coverage_Lcurve_curvature_lambdaMC.pdf
    :width: 45%
.. image:: plots/binwise_coverage_US_curvature_lambdaMC.pdf
    :width: 45%
.. image:: plots/boxplot_length_comparison_curvature_lambdaMC.pdf
    :width: 45%


The simulation above compares the performance of the unfolded confidence
intervals when the regularization strength is chosen using `ScanLcurve`
provided by `TUnfold` and the algorithm `UndersmoothTau` provided by this
software. The tolerance epsilon was set to 0.01, so the intervals from
`UndersmoothTau` should have 67% coverage. The top plots compare the
binwise coverage of the methods. The coverage is estimated by repeating
the unfolding 1,000 times with independent realizations of data.
The bottom plots show one realization of the unfolded confidence intervals
for each method. The confidence intervals provided by `ScanLcurve` are too
short and suffer from drastic undercoverage, while the intervals provided
by `UndersmoothTau` have nearly nominal coverage without being excessively long.





------------------------
Steeply falling spectra
------------------------
