#  Task 2 - Conjugate Gradients in Eigen

![](plot.png)

Eigen's conjugate gradient function has been used to solve Ax  = b with the matrix BCSTKK11 from the matrix market. the CG solver has been used with out a preconditioner explicitly defined (which defaults to `DiagonalPreconditioner`) as well as with `IncompleteCholesky` preconditioner. The quotient of the norm of the residual after each iteration and the norm of the initial residual has been saved to a csv file. The `plot.py` script is used to generate a plot, and the results can be seen above. It can be clearly seen that the `IncompleteCholesky` preconditioner leads to much faster convergence that the `DiagonalPreconditioner`, which also exhibits oscillatory behavior. 