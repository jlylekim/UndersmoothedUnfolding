
************
Installation
************

----
Make
----
To install, simply set up the `ROOT <https://root.cern.ch/>`_ environment
(tested with `ROOT <https://root.cern.ch/>`_ >= V6.12/04),
clone `this repository <https://github.com/jlylekim/UndersmoothedUnfolding>`_,
and run ``make``:

.. code-block:: bash

    git clone https://github.com/lylejkim/UndersmoothedUnfolding.git
    make


---------------------
Checking installation
---------------------

Upon successful installation, two executables should be in the repository, ``UndersmoothDemo1`` and ``UndersmoothDemo2``,
which can be executed as below:

.. code-block:: bash

    ./UndersmoothDemo1
    ./UndersmoothDemo2

The former is the test case with two Gaussian peaks, and should generate
:ref:`this plot<demo-two-peaks>`. The latter is the test case with steeply falling spectra,
and should generate :ref:`this plot<demo-steeply-falling>`.


------------
Dependencies
------------
- `ROOT <https://root.cern.ch/>`_ >= V6.12/04 (tested up to the most recent release V6.18/00)
