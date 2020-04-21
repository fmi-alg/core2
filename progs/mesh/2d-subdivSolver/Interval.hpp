//
//  Interval.hpp
//  Homotopy Continuation
//
//  Created by burr2 on 8/5/18.
//  Copyright © 2018 burr2. All rights reserved.
//

#include <iostream>
#include <mpfr.h>
#include "NumberUtils.hpp"

#ifndef Interval_hpp
#define Interval_hpp

namespace homotopy
{
    class Interval
    {
    public:
        ///////////////////////
        // Constructors
        ///////////////////////
        
        // Default constructor.  Sets everything to 0.
        inline Interval();
        
        // Copy constructor.  Sets upper and lower equal to values in input.
        inline Interval(const Interval &);
        
        // Parameter constructors.  Sets upper and lower values to input value.
        // Should be slightly faster than pass by reference for constants.
        inline Interval(int);
        inline Interval(unsigned int);
        inline Interval(float);
        inline Interval(double);
        
        // Sets upper and lower values to input value.
        // Assumes that the type can be cast to doubles.
        template <typename Type>
        inline Interval(const Type &);
        
        // Sets upper and lower values to input values.
        // Should be slightly faster than pass by reference for constants.
        inline Interval(int, int);
        inline Interval(unsigned int, unsigned int);
        inline Interval(float, float);
        inline Interval(double, double);
        
        // Sets upper and lower values to input values.
        // If the first parameter is greater than the second, sets everything to 0.
        // Assumes that the two types can be cast to doubles.
        template <typename Type1, typename Type2>
        inline Interval(const Type1 &, const Type2 &);
        
        ///////////////////////
        // Operators
        ///////////////////////
 
        // Assignment operator.  Sets upper and lower equal to values in input.
        inline Interval & operator=(const Interval &);
        
        // Assignment operator.  Uses correct rounding.
        // Should be slightly faster than pass by reference for constants.
        inline Interval & operator=(int);
        inline Interval & operator=(unsigned int);
        inline Interval & operator=(float);
        inline Interval & operator=(double);
        
        // Assignment operator.  Sets upper and lower values to input value.
        // Assumes that the type can be cast to doubles.
        template <typename Type>
        inline Interval & operator=(const Type &);
        
        // In-place addition operator.  Uses correct rounding.
        inline Interval & operator+=(const Interval &);
        
        // In-place addition operator.  Uses correct rounding.
        // Should be slightly faster than pass by reference for constants.
        inline Interval & operator +=(int);
        inline Interval & operator +=(unsigned int);
        inline Interval & operator +=(float);
        inline Interval & operator +=(double);
        
        // In-place addition operator.  Uses correct rounding.
        // Assumes the input can be cast to a double.
        template <typename Type>
        inline Interval & operator+=(const Type &);
        
        // In-place subtraction operator.  Uses correct rounding.
        inline Interval & operator-=(const Interval &);
        
        // In-place subtraction operator.  Uses correct rounding.
        // Should be slightly faster than pass by reference for constants.
        inline Interval & operator -=(int);
        inline Interval & operator -=(unsigned int);
        inline Interval & operator -=(float);
        inline Interval & operator -=(double);
        
        // In-place subtraction operator.  Uses correct rounding.
        // Assumes the input can be cast to a double.
        template <typename Type>
        inline Interval & operator-=(const Type &);
        
        // In-place multiplication operator.  Uses correct rounding.
        inline Interval & operator*=(const Interval &);
        
        // In-place multiplication operator.  Uses correct rounding.
        // Should be slightly faster than pass by reference for constants.
        inline Interval & operator *=(int);
        inline Interval & operator *=(unsigned int);
        inline Interval & operator *=(float);
        inline Interval & operator *=(double);
        
        // In-place multiplication operator.  Uses correct rounding.
        // Assumes the input can be cast to a double.
        template <typename Type>
        inline Interval & operator*=(const Type &);
        
        // In-place division operator.  Uses correct rounding.
        // Assumes the right interval does not contain 0 (does not check)
        inline Interval & operator/=(const Interval &);
        
        // In-place division operator.  Uses correct rounding.
        // Should be slightly faster than pass by reference for constants.
        inline Interval & operator /=(int);
        inline Interval & operator /=(unsigned int);
        inline Interval & operator /=(float);
        inline Interval & operator /=(double);
        
        // In-place division operator.  Uses correct rounding.
        // Assumes the input can be cast to a double.
        // Assume the input is not zero
        template <typename Type>
        inline Interval & operator/=(const Type &);
        
        ///////////////////////
        // Interval data
        ///////////////////////
        
        // Returns the width of an interval
        inline friend double width(const Interval &);
        inline friend bool width(double &,const Interval &);
        
        // Returns the upper endpoint of an interval
        inline friend double upper(const Interval &);
        inline friend bool upper(double &, const Interval &);
        
        // Returns the lower endpoint of an interval
        inline friend double lower(const Interval &);
        inline friend bool lower(double &, const Interval &);
        
        // Returns the midpoint of an interval
        inline friend double median(const Interval &);
        inline friend bool median(double &, const Interval &);
        
        // Returns largest absolute value of element of interval
        inline friend double abs(const Interval &);
        inline friend bool abs(double &, const Interval &);
        
        // Get the square of all the elements of an interval 
        // The first function save the resulting interval to itself,
        // the second function save the resulting interval to its 1st argument
        inline friend Interval square_norm(const Interval &);
        inline friend bool square_norm(Interval &, const Interval &);

        ///////////////////////
        // Output operator
        ///////////////////////
        
        // Display function for an interval.  Uses width to format output correctly.
        // Changes rounding modes, so it shouldn't be used with time-based testing.
        inline friend std::ostream& operator<<(std::ostream &, const Interval &);
        
        ///////////////////////
        // Precision operators
        ///////////////////////
        
        // Returns the current precision.
        inline friend mpfr_prec_t get_precision(const Interval &);
        
        // Sets the precision of the data (without destroying data).
        // Does not set precision below default 64 .
        // Also checks that the precision is within allowable amounts.
        inline friend bool set_precision(Interval &, mpfr_prec_t);
        
        // Increases the precision to the input precision.
        // If there is only one input, the precision is doubled
        inline friend bool increase_precision(Interval &);
        inline friend bool increase_precision(Interval &, mpfr_prec_t);
        
        // Decreases the precision to the input precision.
        // If there is only one input, the precision is halved
        inline friend bool decrease_precision(Interval &);
        inline friend bool decrease_precision(Interval &, mpfr_prec_t);

        ///////////////////////
        // Interval operations
        ///////////////////////
        
        // Returns or sets exponential of an interval.
        // The power must be an unsigned integer.
        inline friend Interval exp(Interval,unsigned int);
        inline friend bool exp(Interval &,const Interval&, unsigned int);
        
        // Returns or sets square root of an interval.  Uses correct rounding.
        // Assumes the interval is nonnegative (does not check).
        inline friend Interval sqrt(Interval);
        inline friend bool sqrt(Interval &,const Interval &);
        
        ///////////////////////
        // Friend Operators
        ///////////////////////
        
        // Division operator.  Uses correct rounding.
        // Should be slightly faster than pass by reference for constants.
        inline friend Interval operator/(int, Interval);
        inline friend Interval operator/(unsigned int, Interval);
        inline friend Interval operator/(float, Interval);
        inline friend Interval operator/(double, Interval);
        
        // Assumes the right interval does not contain 0 (does not check)
        template <typename Type>
        inline friend Interval operator/(const Type &,Interval);
        
        ///////////////////////
        // Friend Comparison Operators
        ///////////////////////
        
        // Equality operator.  True if and only if both upper and lower values are equal.
        inline friend bool operator==(const Interval&, const Interval&);

        // Equality operator.
        // Should be slightly faster than pass by reference for constants.
        // Can only return true when interval is a point.
        inline friend bool operator==(const Interval &, int);
        inline friend bool operator==(const Interval &, unsigned int);
        inline friend bool operator==(const Interval &, float);
        inline friend bool operator==(const Interval &, double);
        
        // Less than operator.  True if the left interval is completely less than the right interval.
        inline friend bool operator<(const Interval &, const Interval &);
        
        // Less than operator.  True if the interval is completely less than the point.
        // Should be slightly faster than pass by reference for constants.
        inline friend bool operator<(const Interval &, int);
        inline friend bool operator<(int, const Interval &);
        inline friend bool operator<(const Interval &, unsigned int);
        inline friend bool operator<(unsigned int, const Interval &);
        inline friend bool operator<(const Interval &, float);
        inline friend bool operator<(float, const Interval &);
        inline friend bool operator<(const Interval &, double);
        inline friend bool operator<(double, const Interval &);
        
        // Less than or equal operator.  True if the left interval is completely less than or equal the right interval.
        inline friend bool operator<=(const Interval &, const Interval &);
        
        // Less than or equal operator.  True if the interval is completely less than the point.
        // Should be slightly faster than pass by reference for constants.
        inline friend bool operator<=(const Interval &, int);
        inline friend bool operator<=(int, const Interval &);
        inline friend bool operator<=(const Interval &, unsigned int);
        inline friend bool operator<=(unsigned int, const Interval &);
        inline friend bool operator<=(const Interval &, float);
        inline friend bool operator<=(float, const Interval &);
        inline friend bool operator<=(const Interval &, double);
        inline friend bool operator<=(double, const Interval &);
        
        ///////////////////////
        // Friend Inclusion Operators
        ///////////////////////
        
        // Returns true if the first value is contained in the interval
        inline friend bool contains(int, const Interval &);
        inline friend bool contains(unsigned int, const Interval &);
        inline friend bool contains(float, const Interval &);
        inline friend bool contains(double, const Interval &);
        
        // Templated version
        template <typename Type>
        inline friend bool contains(const Type &, const Interval &);
        
        // Returns true if 0 is in the interval
        inline friend bool containsZero(const Interval &);
        
    private:
        ///////////////////////
        // Data
        ///////////////////////
        
        // Lower and upper endpoint of interval
        // The lower endpoint is stored as the negative of the lower endpoint to avoid changing rounding modes.
        double lower, upper;
    };
    
    ///////////////////////
    // Friend Interval data
    ///////////////////////
    
    // Sets the width of an interval
    inline bool width(double &,const Interval &);
    
    // Sets the upper endpoint of an interval
    inline bool upper(double &,const Interval &);
    
    // Sets the lower endpoint of an interval
    inline bool lower(double &,const Interval &);
    
    // Sets the midpoint of an interval
    inline bool median(double &,const Interval &);
    
    // Sets largest absolute value of element of interval
    inline bool abs(double &,const Interval &);
    
    ///////////////////////
    // Operators
    ///////////////////////
    
    // Addition operator.  Uses correct rounding.
    inline Interval operator+(Interval,const Interval &);
    
    // Addition operator.  Uses correct rounding.
    // Should be slightly faster than pass by reference for constants.
    inline Interval operator+(Interval, int);
    inline Interval operator+(int, Interval);
    inline Interval operator+(Interval, unsigned int);
    inline Interval operator+(unsigned int, Interval);
    inline Interval operator+(Interval, float);
    inline Interval operator+(float, Interval);
    inline Interval operator+(Interval, double);
    inline Interval operator+(double, Interval);
    
    // Addition operator.  Uses correct rounding.
    // Assumes the input can be cast to a double.
    template <typename Type>
    inline Interval operator+(Interval,const Type &);
    
    template <typename Type>
    inline Interval operator+(const Type &,Interval);

    // Subtraction operator.  Uses correct rounding.
    inline Interval operator-(Interval,const Interval &);
    
    // Subtraction operator.  Uses correct rounding.
    // Should be slightly faster than pass by reference for constants.
    inline Interval operator-(Interval, int);
    inline Interval operator-(int, Interval);
    inline Interval operator-(Interval, unsigned int);
    inline Interval operator-(unsigned int, Interval);
    inline Interval operator-(Interval, float);
    inline Interval operator-(float, Interval);
    inline Interval operator-(Interval, double);
    inline Interval operator-(double, Interval);
    
    // Subtraction operator.  Uses correct rounding.
    // Assumes the input can be cast to a double.
    template <typename Type>
    inline Interval operator-(Interval,const Type &);
    
    template <typename Type>
    inline Interval operator-(const Type &,Interval);
    
    // Multiplication operator.  Uses correct rounding.
    inline Interval operator*(Interval,const Interval &);
    
    // Multiplication operator.  Uses correct rounding.
    // Should be slightly faster than pass by reference for constants.
    inline Interval operator*(Interval,int);
    inline Interval operator*(int, Interval);
    inline Interval operator*(Interval, unsigned int);
    inline Interval operator*(unsigned int, Interval);
    inline Interval operator*(Interval, float);
    inline Interval operator*(float, Interval);
    inline Interval operator*(Interval, double);
    inline Interval operator*(double, Interval);
    
    // Multiplication operator.  Uses correct rounding.
    // Assumes the input can be cast to a double.
    template <typename Type>
    inline Interval operator*(Interval,const Type &);
    
    template <typename Type>
    inline Interval operator*(const Type &,Interval);
    
    // Division operator.  Uses correct rounding.
    // Assumes the right interval does not contain 0 (does not check)
    inline Interval operator/(Interval,const Interval &);
    
    // Division operator.  Uses correct rounding.
    // Should be slightly faster than pass by reference for constants.
    inline Interval operator/(Interval, int);
    inline Interval operator/(Interval, unsigned int);
    inline Interval operator/(Interval, float);
    inline Interval operator/(Interval, double);
    
    // Division operator.  Uses correct rounding.
    // Assumes the input can be cast to a double.
    // Assume the input is not zero
    template <typename Type>
    inline Interval operator/(Interval,const Type &);
    
    ///////////////////////
    // Comparison Operators
    ///////////////////////
    
    // Equality operator.
    // Should be slightly faster than pass by reference for constants.
    // Can only return true when interval is a point.
    inline bool operator==(int, const Interval &);
    inline bool operator==(unsigned int, const Interval &);
    inline bool operator==(float, const Interval &);
    inline bool operator==(double, const Interval &);
    
    // Equality operator.  Assumes that the input can be cast to a double.
    // Can only return true when interval is a point.
    template <typename Type>
    inline bool operator==(const Interval &, const Type &);
    
    template <typename Type>
    inline bool operator==(const Type &,const Interval &);
    
    // Inequality operator.  False if and only if both upper and lower values are equal.
    inline bool operator!=(const Interval &, const Interval &);
    
    // Inquality operator.
    // Should be slightly faster than pass by reference for constants.
    // Always returns true when interval has width.
    inline bool operator!=(const Interval &, int);
    inline bool operator!=(int, const Interval &);
    inline bool operator!=(const Interval &, unsigned int);
    inline bool operator!=(unsigned int, const Interval &);
    inline bool operator!=(const Interval &, float);
    inline bool operator!=(float, const Interval &);
    inline bool operator!=(const Interval &, double);
    inline bool operator!=(double, const Interval &);
    
    // Inequality operator.  Assumes that the input can be cast to a double.
    // Always returns true when interval has width.
    template <typename Type>
    inline bool operator!=(const Interval &,const Type &);
    
    template <typename Type>
    inline bool operator!=(const Type &,const Interval &);
    
    // Less than operator.  True if the interval is completely less than the point.
    // Assumes that the input can be cast to a double.
    template <typename Type>
    inline bool operator<(const Interval &,const Type &);
    
    // Less than operator.  True if the interval is completely greater than the point.
    // Assumes that the input can be cast to a double.
    template <typename Type>
    inline bool operator<(const Type &,const Interval &);
    
    // Less than or equal operator.  True if the interval is completely less than or equal the point.
    // Assumes that the input can be cast to a double.
    template <typename Type>
    inline bool operator<=(const Interval &,const Type &);
    
    // Less than or equal operator.  True if the interval is completely greater than or equal the point.
    // Assumes that the input can be cast to a double.
    template <typename Type>
    inline bool operator<=(const Type &,const Interval &);
    
    // Greater than operator.  True if the left interval is completely greater than the right interval.
    inline bool operator>(const Interval &, const Interval &);
    
    // Greater than operator.  True if the interval is completely greater than the point.
    // Assumes that the input can be cast to a double.
    template <typename Type>
    inline bool operator>(const Interval &,const Type &);
    
    // Greater than operator.  True if the interval is completely greater than the point.
    // Should be slightly faster than pass by reference for constants.
    inline bool operator>(const Interval &, int);
    inline bool operator>(int, const Interval &);
    inline bool operator>(const Interval &, unsigned int);
    inline bool operator>(unsigned int, const Interval &);
    inline bool operator>(const Interval &, float);
    inline bool operator>(float, const Interval &);
    inline bool operator>(const Interval &, double);
    inline bool operator>(double, const Interval &);
    
    // Greater than operator.  True if the interval is completely less than the point.
    // Assumes that the input can be cast to a double.
    template <typename Type>
    inline bool operator>(const Type &,const Interval &);
    
    // Greater than or equal operator.  True if the left interval is completely greater than or equal the right interval.
    inline bool operator>=(const Interval &, const Interval &);
    
    // Greater than or equal operator.  True if the interval is completely greater than or equal the point.
    // Should be slightly faster than pass by reference for constants.
    inline bool operator>=(const Interval &, int);
    inline bool operator>=(int, const Interval &);
    inline bool operator>=(const Interval &, unsigned int);
    inline bool operator>=(unsigned int, const Interval &);
    inline bool operator>=(const Interval &, float);
    inline bool operator>=(float, const Interval &);
    inline bool operator>=(const Interval &, double);
    inline bool operator>=(double, const Interval &);
    
    // Greater than or equal operator.  True if the interval is completely greater than or equal the point.
    // Assumes that the input can be cast to a double.
    template <typename Type>
    inline bool operator>=(const Interval &,const Type &);
    
    // Greater than or equal operator.  True if the interval is completely less than or equal the point.
    // Assumes that the input can be cast to a double.
    template <typename Type>
    inline bool operator>=(const Type &,const Interval &);

}

// The following is included because the functions are encouraged to be inline or templated.
#include "../Implementations/Interval.tcc"

#endif /* Interval_hpp */
