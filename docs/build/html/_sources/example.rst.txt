
********
Examples
********


PyUnfold is a Python package for incorporating imperfections
of the measurement process into a data analysis pipeline.


------------------
Two peaks spectra
------------------


-------------------
Steeply falling spectra
-------------------


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

.. [1] M. Kuusela, “Uncertainty quantification in unfolding elementary particle spectra at the Large Hadron Collider”, PhD thesis, EPFL (2016)
