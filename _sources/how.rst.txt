
**********
How to use
**********

--------------
New functions
--------------
``UndersmoothTau`` is the core functionality of ``UndersmoothedUnfolding``, and
is implemented so that it can be used with any initial pilot estimate
of :math:`\tau`, from such as cross-validation, L-curve, etc.
For general usage, users typically just need to use ``UndersmoothTau`` function.

Given an initial estimate of :math:`\tau`,
``UndersmoothTau`` gradually reduces the amount of regularization,
until the target coverage is met within the tolerance :math:`\epsilon`.

It depends on
another core functionality, ``ComputeCoverage``. Under some common assumptions,
the coverage probability of the (Gaussian) confidence intervals can be
written down in closed form, thus providing ``UndersmoothTau`` a principled
way how much to undersmooth.

For more mathematical detail, please refer to Kuusela (2016) [1]_.


--------------
Example usage
--------------
Below is an example usage of ``UndersmoothTau`` with the ``ScanLcurve`` method provided in ``TUnfold``.

.. code-block:: c++

    TUnfold unfold = new TUnfold();          // construct a TUnfold object
    unfold.ScanLcurve();                     // unfold using ScanLcurve method
    TauFromLcurve = unfold.GetTau();         // retrieve tau chosen by ScanLcurve

    // starting from tau chosen by ScanLcurve, undersmooth tau until the minimum estimated coverage
    // meets the target coverage, which is the nominal 68% minus the tolerance epsilon (0.01 in this example).
    TauFromUndersmoothing = unfold.UndersmoothTau(TauFromLcurve, 0.01, 1000);
    unfold.DoUnfold(TauFromUndersmoothing);   // unfold again with undersmoothed tau



---------------------
Expected input/output
---------------------
TODO:
  * input: throw a lot of error/warning messages



.. [1] M. Kuusela, “Uncertainty quantification in unfolding elementary particle spectra at the Large Hadron Collider”, PhD thesis, EPFL (2016)
