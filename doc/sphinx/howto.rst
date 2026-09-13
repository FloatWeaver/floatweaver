How-to guides
=============

Each guide below is pulled directly from a compiled, executed Catch2 test
(tagged ``[doc]`` in ``doc/snippets/``), so it can't drift out of sync with
the API it demonstrates -- if a guide stops matching reality, the docs build
fails.

Add two float8 vectors
-----------------------

``weaver::float8`` behaves like a scalar ``float`` for the usual arithmetic
operators, but computes on 8 lanes at once:

.. literalinclude:: ../snippets/float8_snippets.cpp
   :language: cpp
   :start-after: //! [Add float8]
   :end-before: //! [Add float8]
