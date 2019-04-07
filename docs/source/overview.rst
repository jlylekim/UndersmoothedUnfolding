.. _overview:

:github_url: https://github.com/jrbourbeau/pyunfold

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

In an ideal world, we would have access to the perfect detector:
an apparatus that makes no error in measuring a desired quantity.
However, in real life, detectors have:

- Finite resolutions
- Characteristic biases that cannot be eliminated
- Less than 100% detection efficiencies
- Statistical uncertainties

By building a matrix that encodes a detector's smearing of the desired true quantity
into the measured observable(s), a deconvolution can be performed that provides
an estimate of the true variable. This deconvolution process is known as unfolding.


-------------------
Iterative unfolding
-------------------

The unfolding method implemented in PyUnfold accomplishes this deconvolution
by harnessing the power of Bayes' Theorem in an iterative procedure, providing results
based on physical expectations of the desired quantity [1]_.
Furthermore, tedious book-keeping for both statistical and systematic errors
produces precise final uncertainty estimates.


-----------
Terminology
-----------

In the measurement process, a true distribution of **causes** is detected by some
apparatus having a characteristic **response function**, or matrix, which produces a
*measurable* distribution of **effects**.

The act of unfolding approximates the inverse of the response function by convolving it with
a **prior** distribution which encodes a guess at the form of the true cause distribution.
Including the measured effects distribution, we obtain an improved estimate of the cause
distribution. This updated, better guess can be used as a prior for another unfolding,
making this an iterative procedure.

These unfolding iterations continue until some stopping condition (a test statistic
comparing the unfolded cause distribution between subsequent iterations) satisfies some
pre-defined threshold, typically taking just a few iterations.

To use PyUnfold, one needs only to provide the

- Measured effects distribution
- Response matrix
- Prior distribution

For further mathematical details on the unfolding procedure, see the :doc:`mathematics` section.

.. [1] G. D'Agostini, “A Multidimensional unfolding method based on Bayes' theorem”, Nucl. Instrum. Meth. A **362** (1995) 487.
