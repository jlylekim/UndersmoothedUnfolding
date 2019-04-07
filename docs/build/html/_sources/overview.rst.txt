********
Overview
********

UndersmoothedUnfolding an extension for the unfolding software TUnfold V17.6 written by Stefan Schmitt.
TUnfold implements Tikhonov regularization for unfolding smeared data from particle detectors.
However, as demonstrated by Kuusela (2016), the resulting confidence intervals may seriously underestimate
the uncertainty in the unfolded space, unless care is taken in the choice of the regularization strength tau.
This extension provides a new function for choosing the regularization strength, UndersmoothTau,
which aims at choosing tau so that the confidence intervals have nearly nominal coverage.
UndersmoothTau is an implementation of the data-driven undersmoothing technique introduced in Kuusela (2016).
Please refer to the references section for more details on the statistical methodology.


------------------
New functions
------------------


------------------
What is unfolding?
------------------
