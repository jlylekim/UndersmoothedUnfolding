
****************************
Why UndersmoothedUnfolding?
****************************

---------------------------------------------------------
Uncertainty quantification vs. point estimation
---------------------------------------------------------

``UndersmoothedUnfolding`` an extension for the unfolding
software `TUnfoldV17.8 <http://www.desy.de/~sschmitt/tunfold.html>`_ [2]_ written by Stefan Schmitt.
`TUnfold <http://www.desy.de/~sschmitt/tunfold.html>`_ implements Tikhonov regularization for unfolding smeared data
from particle detectors. However, as demonstrated by Kuusela (2016) [1]_,
the resulting confidence intervals may seriously underestimate the uncertainty
in the unfolded space, unless care is taken in the choice of the regularization
strength :math:`\tau`.


.. figure:: plots/coverage_peak_bin_curvature.png
    :width: 80%
    :align: center

    Coverage is empirically obtained with
    10,000 independent realizations of data.

The plot above illustrates the empirical coverage at the peak bin
(with highest :math:`y`-value) for various values of regularization strength.
Coverage starts at the nominal level (68 %) when there is no regularization.
However, as regularization strength increases, coverage gradually
decreases eventually to zero.

The first vertical line indicates the average
regularization strengths chosen by ``UndersmoothedUnfolding::UndersmoothTau``,
and the corresponding coverage is near nominal level (68 %).
On the other hand, the second vertical line, indicating average
regularization strengths chosen by ``TUnfold::ScanLcurve``, and it
shows serious undercoverage at around 20%.


.. _demo-two-peaks:

-------------------------------------------
Coverage demonstration I: two peaks spectra
-------------------------------------------
The first test case we consider has two Gaussian peaks on top of
a uniform background, resembling situations where invariant mass peaks
are observed on top of some background events.

.. figure:: plots/undersmooth_demo_twopeaks.png
    :width: 100%
    :align: center

    The coverage is estimated by repeating
    the unfolding 1,000 times with independent realizations of data.
    The average confidence interval lengths for each method is written below
    on the top plots.

The top two plots compare the performance of the unfolded confidence
intervals when :math:`\tau` is chosen using
``TUnfold::ScanLcurve`` and the algorithm ``UndersmoothedUnfolding::UndersmoothTau``
provided by this software. The tolerance :math:`\epsilon` was set to 0.01, so the intervals from
``UndersmoothTau`` should have 67% coverage.

The bottom plots show one realization of the unfolded confidence intervals
for each method. As shown before, the confidence intervals provided by ``TUnfold::ScanLcurve`` are too
short and suffer from drastic undercoverage, while the intervals provided
by ``UndersmoothedUnfolding::UndersmoothTau`` have nearly nominal coverage without being excessively long.


.. _demo-steeply-falling:

--------------------------------------------------
Coverage demonstration II: steeply falling spectra
--------------------------------------------------
The second test case we consider has a steeply falling particle spectrum,
and we use transverse momentum spectrum [3]_  as an example. Observing a jet,
a collimated stream of energetic particles, in an LHC particle detector
implies the creation of a quark or a gluon in proton-proton collision, and
is an important test of the Standard Model of particle physics.


.. figure:: plots/undersmooth_demo_incjets.png
    :width: 100%
    :align: center

    The coverage is estimated by repeating
    the unfolding 1,000 times with independent realizations of data.
    The average confidence interval lengths for each method is written below
    on the top plots.

Again, we see that the confidence intervals provided by ``TUnfold::ScanLcurve``
suffer drastic undercoverage, while the intervals provided
by ``UndersmoothedUnfolding::UndersmoothTau`` have nearly nominal coverage.


.. [1] M. Kuusela, “Uncertainty quantification in unfolding elementary particle spectra at the Large Hadron Collider”, PhD thesis, EPFL (2016)
.. [2] S. Schmitt, “TUnfold, an algorithm for correcting migration effects in high energy physics”, Journal of Instrumentation **7** (2012)
.. [3] CMS Collaboration, "Measurement of the Inclusive Jet Cross Section in :math:`pp` Collisions at :math:`\sqrt{s} = 7 TeV`", Physical Review D **107** (2011)
