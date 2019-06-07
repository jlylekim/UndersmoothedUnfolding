
*******************
Function references
*******************

**TVectorD** ``TUnfoldV17::ComputeCoverage`` (**TMatrixD** * beta, **Double_t** tau)

* Input:
    * ``TMatrixD*`` beta
    * ``Double_t`` tau

* Output:
    * Computes coverage probability for each bin and outputs in ``TVectorD`` type.

UndersmoothedUnfolding debiases the unfolded point estimator by gradually reducing
the regularization strength until the target coverage is met within the tolerance epsilon.
The undersmoothed estimator has nearly nominal coverage

**************************
UndersmoothedUnfolding API
**************************

TODO:
* autodoc only works with Python
* If I want (automated) documentation, need to use Breathe (Breathe provides a bridge between the Sphinx and Doxygen documentation systems.)
* To do so, need to learn Doxygen as well
* Might be better to manually write API as we have 2 functions
* Discuss with Mikael
* Actually maybe I can use Jupyter notebook

MK: Let's just write these manually.

----------------
ComputeCoverage
----------------
>>>>>>> 504f81f9b5d92b2019abf9dc4f9f5bb8011fe2f9

---------------
UndersmoothTau
---------------

`Double_t` ``TUnfoldV17::UndersmoothTau`` (`Double_t` tau, `Double_t` epsilon, `Int_t` max_iter)

* Input:
    * `Double_t` tau
    * `Double_t` epsilon
    * `Int_t` max_iter

* Output:
    * Computes coverage probability for each bin and outputs in ``TVectorD`` type.

UndersmoothedUnfolding debiases the unfolded point estimator by gradually reducing
the regularization strength until the target coverage is met within the tolerance epsilon.
The undersmoothed estimator has nearly nominal coverage
