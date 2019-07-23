
*******************
Function references
*******************


---------------
UndersmoothTau
---------------

* `Double_t` ``UndersmoothedUnfolding::UndersmoothTau`` (`Double_t` **tau**, `Double_t` **epsilon**, `Int_t` **max_iter**)

  * Input:
      * `Double_t` **tau**: pilot estimate of regularization strength :math:`\tau^{initial}`
      * `Double_t` **epsilon**: tolerance for target coverage
      * `Int_t` **max_iter**: maximum number of undersmoothing iterations

  * Output:
      * Undersmoothed regularization strength :math:`\tau^{undersmoothed}`



----------------
ComputeCoverage
----------------

* `TVectorD` ``UndersmoothedUnfolding::ComputeCoverage`` (`TMatrixD` * **beta**, `Double_t` **tau**)

  * Input:
      * `TMatrixD` * **beta**: estimate of the true histogram in `TMatrixD` type
      * `Double_t` **tau**: regularization strength :math:`\tau`

  * Output:
      * Computes coverage probability for each bin and outputs in `TVectorD` type


* `TVectorD` ``UndersmoothedUnfolding::ComputeCoverage`` (`TH1` * **beta**, `Double_t` **tau**)

  * Input:
      * `TH1` * **beta**: estimate of the true histogram in `TH1` type
      * `Double_t` **tau**: regularization strength :math:`\tau`

  * Output:
      * Computes coverage probability for each bin and outputs in `TVectorD` type
