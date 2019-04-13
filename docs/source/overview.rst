********
Overview
********

``UndersmoothedUnfolding`` is a software for unfolding in high energy physics with focus on
optimal *uncertainty quantification*. Most unfolding softwares focus on optimal
point estimation, which can suffer significant undercoverage and
thus might not be suitable statistical inference and uncertainty quantification.

On the contrary, this software provides a function for choosing the
regularization strength so that the estimated confidence intervals have
nearly nominal coverage.

``UndersmoothedUnfolding`` is a ``ROOT`` [3]_. implementation of the data-driven
undersmoothing technique introduced in Kuusela (2016) [1]_.
It is  and is extended from the de facto unfolding library, ``TUnfold`` [2]_,
which comes with ``ROOT`` [3]_.



OLD version for back up:
UndersmoothedUnfolding an extension for the unfolding software TUnfold V17.6 [2]_ [3]_ written by Stefan Schmitt.
TUnfold implements Tikhonov regularization for unfolding smeared data from particle detectors.
However, as demonstrated by Kuusela (2016) [1]_, the resulting confidence intervals may seriously underestimate
the uncertainty in the unfolded space, unless care is taken in the choice of the regularization strength tau.


------------------
What is unfolding?
------------------

Any measurement made at the Large Hadron Collider (LHC) at CERN
is smeared by the finite resolution of the particle detectors. The goal in
unfolding is to use these smeared measurements to make non-parametric
inferences about the underlying particle spectrum.







.. [1] M. Kuusela, “Uncertainty quantification in unfolding elementary particle spectra at the Large Hadron Collider”, PhD thesis, EPFL (2016).
.. [2] S. Schmitt, “TUnfold, an algorithm for correcting migration effects in high energy physics”, Journal of Instrumentation **7** (2012).
.. [3] R. Brun and F. Rademakers, “ROOT - An Object Oriented Data Analysis Framework”, Nucl. Inst. & Meth. in Phys. Res. A **389** (1997).
