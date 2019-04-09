
**********
How to use
**********


--------------
Example usage
--------------
``UndersmoothTau`` is implemented so that it can be used with any initial estimate of tau.
Below is an example usage of ``UndersmoothTau`` with the ``ScanLcurve`` method provided in ``TUnfold``.

.. code-block:: c++

    TUnfold unfold = new TUnfold();          // construct a TUnfold object
    unfold.ScanLcurve();                     // unfold using ScanLcurve method
    TauFromLcurve = unfold.GetTau();         // retrieve tau chosen by ScanLcurve

    // starting from tau chosen by ScanLcurve, undersmooth tau until the minimum estimated coverage
    // meets the target coverage, which is the nominal 68% minus the tolerance epsilon (0.01 in this example).
    TauFromUndersmoothing = unfold.UndersmoothTau(TauFromLcurve, 0.01, 1000);
    unfold.DoUnfold(TauFromUndersmoothing);   // unfold again with undersmoothed tau




------------
Known issues
------------
