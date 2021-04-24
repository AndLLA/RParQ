
library(Rcpp)
library(RcppArmadillo)
library(RcppParallel)
library(tictoc)
library(rbenchmark)

sessionInfo()

numRows = 10000
numCols = 250*10
buf = matrix(data = runif(numRows*numCols)*123, nrow = numRows, ncol = numCols)

listQ = c(0.05, 0.10, 0.15, 0.2, 0.25, 0.5, 0.75, 0.80, 0.85, 0.90, 0.95)
quantile(x = buf[, 1], listQ)

tic()
bufQ = apply(X=buf, FUN = function(x) quantile(x, listQ), MARGIN=2)
toc()
benchmark(apply(X=buf, FUN = function(x) quantile(x, listQ), MARGIN=2))

tic()
bufQPar = parallelQuantileByCol(x = buf, listQ = listQ)
toc()
benchmark(parallelQuantileByCol(x = buf, listQ = listQ))

bufQDelta = abs(bufQ-bufQPar)
max(bufQDelta)
min(bufQDelta)
sum(bufQDelta)
