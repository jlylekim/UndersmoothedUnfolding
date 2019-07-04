
****************************
Why UndersmoothedUnfolding?
****************************

---------------------------------------------------------
Uncertainty quantification vs. point estimation
---------------------------------------------------------

MK: Can we do Latex for tau here?

``UndersmoothedUnfolding`` an extension for the unfolding
software ``TUnfoldV17.6`` [2]_ written by Stefan Schmitt.
``TUnfold`` implements Tikhonov regularization for unfolding smeared data
from particle detectors. However, as demonstrated by Kuusela (2016) [1]_,
the resulting confidence intervals may seriously underestimate the uncertainty
in the unfolded space, unless care is taken in the choice of the regularization
strength tau.


MK: Explain the plot below

.. figure:: plots/coverage_peak_bin_curvature.png
    :width: 80%
    :align: center

    Coverage is empirically obtained with
    10,000 independent realizations of data.

The plot above illustrates the empirical coverage at the peak bin
(with highest y-value) for various values of regularization strength.
Coverage starts at the nominal level (68 %) when there is no regularization.
However, as regularization strength increases, coverage gradually
decreases eventually to zero.

The first vertical line indicates the average
regularization strengths chosen by ``UndersmoothedUnfolding::UndersmoothTau``,
and the corresponding coverage is near nominal level (68 %).
On the other hand, the second vertical line, indicating average
regularization strengths chosen by ``TUnfold::ScanLcurve``, and it
shows serious undercoverage at around 20%.



------------------
Two peaks spectra
------------------
.. figure:: plots/undersmooth_demo_twopeaks.png
    :width: 100%
    :align: center

    The coverage is estimated by repeating
    the unfolding 1,000 times with independent realizations of data.


The simulation above compares the performance of the unfolded confidence
intervals when the regularization strength is chosen using
``TUnfold::ScanLcurve`` the algorithm ``UndersmoothedUnfolding::UndersmoothTau``
provided by this software. The tolerance epsilon was set to 0.01, so the intervals from
``UndersmoothTau`` should have 67% coverage. The top plots compare the
binwise coverage of the methods.

The bottom plots show one realization of the unfolded confidence intervals
for each method. As shown before, the confidence intervals provided by ``ScanLcurve`` are too
short and suffer from drastic undercoverage, while the intervals provided
by ``UndersmoothTau`` have nearly nominal coverage without being excessively long.
(Note that the average confidence interval lengths for each method is written below
on the top plots.)

------------------------
Steeply falling spectra
------------------------
.. figure:: plots/undersmooth_demo_incjets.png
    :width: 100%
    :align: center

    The coverage is estimated by repeating
    the unfolding 1,000 times with independent realizations of data.



.. [1] M. Kuusela, “Uncertainty quantification in unfolding elementary particle spectra at the Large Hadron Collider”, PhD thesis, EPFL (2016)
.. [2] S. Schmitt, “TUnfold, an algorithm for correcting migration effects in high energy physics”, Journal of Instrumentation **7** (2012).
