#ifndef STDAIR_BAS_OPTIMISATIONMETHOD_HPP
#define STDAIR_BAS_OPTIMISATIONMETHOD_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/basic/StructAbstract.hpp>

namespace stdair {

  /** Enumeration of Optimisation methods. */
  struct OptimisationMethod : public StructAbstract {
  public:
    enum EN_OptimisationMethod {
      LEG_BASED_MC = 0,
      LEG_BASED_EMSR_B,
      LAST_VALUE
    };

    /** Get the label as a string (e.g., "Leg based Monte Carlo" or 
     * "Leg based EMSRb"). */
    static const std::string& getLabel (const EN_OptimisationMethod&);

    /** Get the label as a single char (e.g., 'M' or 'E'). */
    static char getMethodLabel (const EN_OptimisationMethod&);

    /** Get the label as a string of a single char (e.g., "M" or "E"). */
    static std::string getMethodLabelAsString (const EN_OptimisationMethod&);

    /** List the labels. */
    static std::string describeLabels();

    /** Get the enumerated value. */
    EN_OptimisationMethod getMethod() const;

    /** Get the enumerated value as a short string (e.g., "M" or "E"). */
    std::string getMethodAsString() const;
    
    /** Give a description of the structure (e.g., "Leg based Monte Carlo" or 
     * "Leg based EMSRb"). */
    const std::string describe() const;

  public:
    /** Comparaison operator. */
    bool operator== (const EN_OptimisationMethod&) const;
    
  public:
    /** Constructor. */
    OptimisationMethod (const EN_OptimisationMethod&);
    /** Constructor. */
    OptimisationMethod (const char iMethod);
    /** Default copy constructor. */
    OptimisationMethod (const OptimisationMethod&);
    /** Default copy assignment operator. */
    OptimisationMethod& operator= (const OptimisationMethod&) = default;

  private:
    /** Default constructor. */
    OptimisationMethod();
  

  private:
    /** String version of the enumeration. */    
    static const std::string _labels[LAST_VALUE];
    /** Method version of the enumeration. */    
    static const char _methodLabels[LAST_VALUE];


  private:
    // //////// Attributes /////////
    /** Optimisation Method. */
    EN_OptimisationMethod _method;
  };

}
#endif // STDAIR_BAS_OPTIMISATIONMETHOD_HPP
