
**********
How to use
**********

--------------
New functions
--------------
``UndersmoothTau`` is the core functionality of ``UndersmoothedUnfolding``, and
is implemented so that it can be used with any initial pilot estimate
of :math:`\tau` from, say, cross-validation, L-curve, etc.
For typical usage, users simply need to add a call to ``UndersmoothTau`` to their usual TUnfold workflow.

Given an initial estimate of :math:`\tau`,
``UndersmoothTau`` gradually reduces the amount of regularization
until the 68% target coverage is met within a tolerance :math:`\epsilon`.

``UndersmoothTau`` depends on
another core functionality, ``ComputeCoverage``. Under some common assumptions,
the coverage probability of the (Gaussian) confidence intervals can be
written down in closed form, thus providing ``UndersmoothTau`` a principled
way of choosing the amount of undersmoothing. The expression for the coverage depends on the unknown true spectrum, which ``UndersmoothTau`` substitutes with a nontrivial plug-in estimate when calling ``ComputeCoverage``. For more mathematical and technical detail, please refer to Kuusela (2016) [1]_.

--------------
Example usage
--------------
``UndersmoothTau`` is implemented so that it can be used with any initial estimate of :math:`\tau`.
Below is an example usage of ``UndersmoothTau`` with the ``TUnfold::ScanLcurve`` method.

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
Please refer to :ref:`function references<function-references>` page for details about supported input and expected output.



.. [1] M. Kuusela, “Uncertainty quantification in unfolding elementary particle spectra at the Large Hadron Collider”, PhD thesis, EPFL (2016)
