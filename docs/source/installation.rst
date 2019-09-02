
************
Installation
************

TODO: finish modifying makefile first

----
Make
----
To install, simply set up the ``ROOT`` environment (tested with ``ROOT`` V6.18/00),
`git clone` this repository, and run `make` as below:

.. code-block:: bash

    git clone https://github.com/lylejkim/UndersmoothedUnfolding.git
    make

This installs extended ``TUnfold`` with ``UndersmoothedUnfolding``.

.. _label-name:
---------------------
Checking installation
---------------------
MK: Give here instructions for reproducing the figure from the paper

Running `make` should generate two executables, `UndersmoothDemo1` and `UndersmoothDemo2`.
`UndersmoothDemo1` is test case with two Gaussian peaks, and should generate
:ref:`this plot<demo-two-peaks>`.


`UndersmoothDemo2` is the test case with steeply falling spectra, and should generate
:ref:`this plot<demo-steeply-falling>`.



------------
Dependencies
------------
``UndersmoothedUnfolding`` requires the following dependencies:

- `ROOT <https://root.cern.ch/>`_ = 6.10
