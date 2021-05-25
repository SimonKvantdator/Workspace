# Git repo for "Two-spinors and Symmetry Operators: An Investigation into the Existence of Symmetry Operators for the Massive Dirac"

This is a git repo for my Master's thesis.
It contains the Mathematica code that was used for producing the results of my thesis.

`example_from_section_4-2-2.nb` and `example_from_section_4-2-3.nb` contain the code from the methods chapter.
`MyInit.nb` contains initialization code that most of my other programs use. It generates a file, `MyInit.m`, that the other programs import.
`TexActModified.nb` contains a slight modification of the TexAct package. It generates a file, `TexActModified.m`, that `MyInit.nb` Imports.
`calculations/dirac_equation_0th_order_symmetry_operator.nb`, `calculations/dirac_equation_1st_order_symmetry_operator.nb`, and `calculations/dirac_equation_2nd_order_symmetry_operator.nb` decompose the condition that $(\phi, \chi) \mapsto (\lambda, \gamma)$ is a symmetry operator into a list of equations involving the symop coefficients.
These equations are saved in `calculations/1st_order_auxiliary_conditions.m` and `calculations/2nd_order_auxiliary_conditions.m`, which are imported by `calculations/simplify_1st_order_auxiliary_conditions.nb` and  `calculations/simplify_2nd_order_auxiliary_conditions.nb` where they are simplified.
`configs.json` contains the path to where `calculations/simplify_1st_order_auxiliary_conditions.nb` and  `calculations/simplify_2nd_order_auxiliary_conditions.nb` write their LaTeXed output.

TODO: link to the thesis when it gets published online.
