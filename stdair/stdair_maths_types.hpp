#ifndef STDAIR_STDAIR_MATHS_TYPES_HPP
#define STDAIR_STDAIR_MATHS_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>
#include <map>
// Boost Random
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/exponential_distribution.hpp>
#include <boost/random/variate_generator.hpp>

namespace stdair {

  // //////// Type definitions /////////
  /**
   * Define the replication number.
   */
  using ReplicationNumber_T = unsigned int;

  /**
   * Define the seed type of an Exponential function.
   */
  using ExponentialSeed_T = unsigned long int;

  /**
   * Define the seed type of an Uniform function.
   */
  using UniformSeed_T = unsigned long int;

  /**
   * Seed for the random generation, so that it can be reproductible.
   */
  using RandomSeed_T = unsigned long int;
  
  /**
   * Random number generator.
   */
  using BaseGenerator_T = boost::minstd_rand;

  /**
   * Uniform distribution of real numbers (by default, double).
   */
  using UniformDistribution_T = boost::uniform_real<>;

  /**
   * Uniform random generator.
   */
  using UniformGenerator_T =
    boost::variate_generator<BaseGenerator_T&, UniformDistribution_T>;

  /**
   * Normal distribution of real numbers (by default, double).
   */
  using NormalDistribution_T = boost::normal_distribution<>;

  /**
   * Nornal random generator.
   */
  using NormalGenerator_T =
    boost::variate_generator<BaseGenerator_T&, NormalDistribution_T>;

  /** Type definiton for the exponential distribution (characteristics). */
  using ExponentialDistribution_T = boost::exponential_distribution<>;


  /** Type definition for the exponential distribution random generator. */
  using ExponentialGenerator_T =
    boost::variate_generator<BaseGenerator_T&, ExponentialDistribution_T>;

  /**
   * Define a mean value (e.g., 20.2).
   */
  using MeanValue_T = double;

  /**
   * Define a standard deviation value (e.g., 1.5).
   */
  using StdDevValue_T = double;

  /**
   * Define a couple (mean, standart deviation) (e.g., (20.2,1.5)).
   */
  using MeanStdDevPair_T = std::pair<MeanValue_T, StdDevValue_T>;

  /**
   * Define a vector of couple (mean, standart deviation)
   */ 
  using MeanStdDevPairVector_T = std::vector<MeanStdDevPair_T>;

  /**
   * Probability.
   */
  using Probability_T = float;
  
}
#endif // STDAIR_STDAIR_MATHS_TYPES_HPP
