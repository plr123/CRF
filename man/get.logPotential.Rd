% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/misc.R
\name{get.logPotential}
\alias{get.logPotential}
\title{Calculate the log-potential of CRF}
\usage{
get.logPotential(crf, configuration)
}
\arguments{
\item{crf}{The CRF}

\item{configuration}{The vector of states of nodes}
}
\value{
The function will return the log-potential of CRF with given configuration
}
\description{
Calculate the logarithmic potential of a CRF with given configuration
}
\details{
The function will calculate the logarithmic potential of a CRF with given configuration,
i.e., the assigned states of nodes in the CRF.
}
\seealso{
\code{\link{get.potential}}
}

