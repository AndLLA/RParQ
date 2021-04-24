// [[Rcpp::depends(RcppParallel)]]
// [[Rcpp::depends(RcppArmadillo)]]
#include <RcppArmadillo.h>
#include <RcppParallel.h>
using namespace RcppParallel;
using namespace Rcpp;

struct ParQuantile : public Worker
{
  // source matrix
  const RMatrix<double> input;
  
  // destination matrix
  RMatrix<double> output;
  
  // list of quantiles to calculate
  const NumericVector listQ;
  arma::vec alistQ;
  
  // initialize with source and destination
  ParQuantile(const NumericMatrix& _input, NumericMatrix& _output, const NumericVector& _listQ)
    : input(_input), output(_output), listQ(_listQ) {
    alistQ = as<arma::vec>(_listQ);
  }
  
  void operator()(std::size_t begin, std::size_t end) {
    for (std::size_t i=begin; i<= end; i++) {
      RMatrix<double>::Column inData = input.column(i);
      arma::vec buf(inData.begin(), inData.length());
      
      arma::vec bufOut = arma::quantile(buf, alistQ);

      RMatrix<double>::Column outData = output.column(i);
      for (int i=0; i<outData.length(); i++)
        outData[i] = bufOut[i];
    }// for
  }
  
  
};


// [[Rcpp::export]]
NumericMatrix parallelQuantileByCol(NumericMatrix x, NumericVector listQ) {
  
  // allocate the output matrix
  NumericMatrix output(listQ.length(), x.ncol());
  
  // ParQuantile functor (pass input and output matrixes)
  ParQuantile parQnt(x, output, listQ);
  
  // call parallelFor to do the work
  parallelFor(0, x.ncol()-1, parQnt);
  
  // return the output matrix
  return output;
}

