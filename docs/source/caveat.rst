
************
Known issues
************

* Only supports the ``TUnfold`` base class (not the derived classes ``TUnfoldBinning``, ``TUnfoldBinningXML``, ``TUnfoldDensity`` and ``TUnfoldSys``).
* Underflow and overflow bins are currently not supported.
* The current version is intended to be used with ``kEConstraintNone`` for the area constraint option when constructing the ``TUnfold`` object.
