
*******************
Function references
*******************

----------------
ComputeCoverage
----------------


**TVectorD** ``TUnfoldV17::ComputeCoverage`` (**TMatrixD** * beta, **Double_t** tau)

* Input:
    * ``TMatrixD*`` beta
    * ``Double_t`` tau

* Output:
    * Computes coverage probability for each bin and outputs in ``TVectorD`` type.

UndersmoothedUnfolding debiases the unfolded point estimator by gradually reducing
the regularization strength until the target coverage is met within the tolerance epsilon.
The undersmoothed estimator has nearly nominal coverage



---------------
UndersmoothTau
---------------

`Double_t` ``TUnfoldV17::UndersmoothTau`` (`Double_t` tau, `Double_t` epsilon, `Int_t` max_iter)


`TVectorD` ``TUnfoldV17::ComputeCoverage`` (`TMatrixD` * **beta**, `Double_t` **tau**)


* Input:
    * `Double_t` tau
    * `Double_t` epsilon
    * `Int_t` max_iter

* Output:
    * Computes coverage probability for each bin and outputs in ``TVectorD`` type.

UndersmoothedUnfolding debiases the unfolded point estimator by gradually reducing
the regularization strength until the target coverage is met within the tolerance epsilon.
The undersmoothed estimator has nearly nominal coverage
