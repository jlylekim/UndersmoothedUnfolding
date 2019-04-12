********
Overview
********

UndersmoothedUnfolding an extension for the unfolding software TUnfold V17.6 [2]_ [3]_ written by Stefan Schmitt.
TUnfold implements Tikhonov regularization for unfolding smeared data from particle detectors.
However, as demonstrated by Kuusela (2016) [1]_, the resulting confidence intervals may seriously underestimate
the uncertainty in the unfolded space, unless care is taken in the choice of the regularization strength tau.
This extension provides a new function for choosing the regularization strength, UndersmoothTau,
which aims at choosing tau so that the confidence intervals have nearly nominal coverage.
UndersmoothTau is an implementation of the data-driven undersmoothing technique introduced in Kuusela (2016) [1]_.
Please refer to the references section for more details on the statistical methodology.


------------------
What is unfolding?
------------------


------------------
New functions
------------------


.. [1] M. Kuusela, “Uncertainty quantification in unfolding elementary particle spectra at the Large Hadron Collider”, PhD thesis, EPFL (2016).
.. [2] S. Schmitt, “TUnfold, an algorithm for correcting migration effects in high energy physics”, Journal of Instrumentation **7** (2012).
.. [3] R. Brun and F. Rademakers, “ROOT - An Object Oriented Data Analysis Framework”, Nucl. Inst. & Meth. in Phys. Res. A **389** (1997).
