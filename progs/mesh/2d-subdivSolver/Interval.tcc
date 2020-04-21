//
//  Interval.tcc
//  Homotopy Continuation
//
//  Created by burr2 on 8/5/18.
//  Copyright © 2018 burr2. All rights reserved.
//

#ifndef Interval_tcc
#define Interval_tcc

#include <iostream>
#include <cfenv>
#include <algorithm>
#include <cmath>
#include "Interval.hpp"

namespace homotopy
{
    ///////////////////////
    // Constructors
    ///////////////////////
    
    // Default constructor.  Sets everything to 0.
    Interval::Interval() : lower(-0.0), upper(0.0)
    {
    }
    
    // Copy constructor.  Sets everything to values of interval.
    Interval::Interval(const Interval &interval) : lower(interval.lower), upper(interval.upper)
    {
    }
    
    // Parameter constructors.  Sets upper and lower values to input value.
    // Should be slightly faster than pass by reference for constants.
    Interval::Interval(int value) : upper(value), lower(-value)
    {
    }
    
    Interval::Interval(unsigned int value) : upper(value), lower(-value)
    {
    }
    
    Interval::Interval(float value) : upper(value), lower(-value)
    {
    }
    
    Interval::Interval(double value) : upper(value), lower(-value)
    {
    }
    
    // Sets upper and lower values to input value.
    // If the first parameter is greater than the second, sets everything to 0.
    // Should be slightly faster than pass by reference for constants.
    Interval::Interval(int low, int up) : upper(up), lower(-low)
    {
        if(upper + lower < 0)
        {
            lower = -0.0;
            upper = 0.0;
        }
    }
    
    Interval::Interval(unsigned int low, unsigned int up) : upper(up), lower(-low)
    {
        if(upper + lower < 0)
        {
            lower = -0.0;
            upper = 0.0;
        }
    }
    
    Interval::Interval(float low, float up) : upper(up), lower(-low)
    {
        if(upper + lower < 0)
        {
            lower = -0.0;
            upper = 0.0;
        }
    }
    
    Interval::Interval(double low, double up) : upper(up), lower(-low)
    {
        if(upper + lower < 0)
        {
            lower = -0.0;
            upper = 0.0;
        }
    }
    
    // Sets upper and lower values to input values.
    // If the first parameter is greater than the second, sets everything to 0.
    // Assumes that the two types can be cast to doubles.
    template <typename Type1, typename Type2>
    Interval::Interval(const Type1 &low, const Type2 &up) : upper((double)up), lower((double)(-low))
    {
        if(upper + lower < 0)
        {
            lower = -0.0;
            upper = 0.0;
        }
    }
    
    ///////////////////////
    // Operators
    ///////////////////////
    
    // Assignment operator.  Sets upper and lower equal to values in input.
    Interval & Interval::operator=(const Interval &interval)
    {
        upper = interval.upper;
        lower = interval.lower;
        return *this;
    }
    
    // Assignment operator.  Uses correct rounding.
    // Should be slightly faster than pass by reference for constants.
    Interval & Interval::operator=(int value)
    {
        upper = value;
        lower = -upper;
        return *this;
    }
    
    Interval & Interval::operator=(unsigned int value)
    {
        upper = value;
        lower = -upper;
        return *this;
    }
    
    Interval & Interval::operator =(float value)
    {
        upper = value;
        lower = -upper;
        return *this;
    }
    
    Interval & Interval::operator =(double value)
    {
        upper = value;
        lower = -upper;
        return *this;
    }
    
    // Assignment operator.  Sets upper and lower equal to input value.
    template <typename Type>
    Interval & Interval::operator=(const Type &value)
    {
        upper = (double)value;
        
        // The lower endpoint is stored as the negative of the lower endpoint to avoid changing rounding modes.
        lower = -upper;
        return *this;
    }
    
    // In-place addition operator.  Uses correct rounding.
    Interval & Interval::operator+=(const Interval &interval)
    {
        upper += interval.upper;
        lower += interval.lower;
        return *this;
    }
    
    // In-place addition operator.  Uses correct rounding.
    // Should be slightly faster than pass by reference for constants.
    Interval & Interval::operator+=(int value)
    {
        upper += value;
        lower -= value;
        return *this;
    }
    
    Interval & Interval::operator+=(unsigned int value)
    {
        upper += value;
        lower -= value;
        return *this;
    }
    
    Interval & Interval::operator +=(float value)
    {
        upper += value;
        lower -= value;
        return *this;
    }
    
    Interval & Interval::operator +=(double value)
    {
        upper += value;
        lower -= value;
        return *this;
    }
    
    // In-place addition operator.  Uses correct rounding.
    // Assumes the input can be cast to a double.
    template <typename Type>
    Interval & Interval::operator+=(const Type &value)
    {
        upper += value;
        lower -= value;
        return *this;
    }
    
    // In-place subtraction operator.  Uses correct rounding.
    Interval & Interval::operator-=(const Interval &interval)
    {
        upper += interval.lower;
        lower += interval.upper;
        return *this;
    }
    
    // In-place subtraction operator.  Uses correct rounding.
    // Should be slightly faster than pass by reference for constants.
    Interval & Interval::operator-=(int value)
    {
        upper -= value;
        lower += value;
        return *this;
    }
    
    Interval & Interval::operator-=(unsigned int value)
    {
        upper -= value;
        lower += value;
        return *this;
    }
    
    Interval & Interval::operator -=(float value)
    {
        upper -= value;
        lower += value;
        return *this;
    }
    
    Interval & Interval::operator -=(double value)
    {
        upper -= value;
        lower += value;
        return *this;
    }
    
    // In-place subtraction operator.  Uses correct rounding.
    // Assumes the input can be cast to a double.
    template <typename Type>
    Interval & Interval::operator-=(const Type &value)
    {
        double input = (double)value;
        upper -= input;
        lower += input;
        return *this;
    }
    
    // In-place multiplication operator.  Uses correct rounding.
    Interval & Interval::operator*=(const Interval &interval)
    {
        // Temporary data type if we need to swap upper and lower's roles.
        double newlower;
        
        // Broken into if statements to reduce multiplication and (hopefully) branching.
        
        // The left interval is entirely nonnegative.
        if(lower<=0)
        {
            // The right interval is entirely nonnegative.
            if(interval.lower<=0)
            {
                lower = (-lower) * interval.lower;
                upper = upper * interval.upper;
            }
            
            // The right interval is entirely nonpositive.
            else if(interval.upper<=0)
            {
                newlower = upper * interval.lower;
                upper = (-lower) * interval.upper;
                lower = newlower;
            }
            
            // The right interval contains 0 in interior.
            else
            {
                lower = upper * interval.lower;
                upper = upper * interval.upper;
            }
        }
                         
        // The left interval is entirely nonpositive.
        else if(upper<=0)
        {
            // The right interval is entirely nonnegative.
            if(interval.lower<=0)
            {
                lower = lower * interval.upper;
                upper = (-upper) * interval.lower;
            }
            
            // The right interval is entirely nonpositive.
            else if(interval.upper<=0)
            {
                newlower = (-upper) * interval.upper;
                upper = lower * interval.lower;
                lower = newlower;
            }
            
            // The right interval contains 0 in interior.
            else
            {
                upper = lower * interval.lower;
                lower = lower * interval.upper;
            }
        }
                         
        // The left interval contains 0 in the interior.
        else
        {
            // The right interval is entirely nonnegative.
            if(interval.lower<=0)
            {
                lower = lower * interval.upper;
                upper = upper * interval.upper;
            }
            
            // The right interval is entirely nonpositive.
            else if(interval.upper<=0)
            {
                newlower = upper * interval.lower;
                upper = lower * interval.lower;
                lower = newlower;
            }
            
            // The right interval contains 0 in interior.
            else
            {
                newlower = std::max(lower * interval.upper, upper * interval.lower);
                upper = std::max(lower * interval.lower, upper * interval.upper);
                lower = newlower;
            }
        }
        return *this;
    }
    
    // In-place multiplication operator.  Uses correct rounding.
    // Should be slightly faster than pass by reference for constants.
    Interval & Interval::operator *=(int value)
    {
        // Broken into if statements to reduce multiplication and (hopefully) branching.
        // If the input is nonnegative, don't reverse the product.
        if(value>=0)
        {
            upper *= value;
            lower *= value;
        }
        
        // If the input is negative, then reverse the product.
        else
        {
            double newlower;
            newlower = (-upper)*value;
            upper = (-lower)*value;
            lower = newlower;
        }
        return *this;
    }
    
    Interval & Interval::operator *=(unsigned int value)
    {
        upper *= value;
        lower *= value;
        return *this;
    }
    
    Interval & Interval::operator *=(float value)
    {
        // Broken into if statements to reduce multiplication and (hopefully) branching.
        // If the input is nonnegative, don't reverse the product.
        if(value>=0)
        {
            upper *= value;
            lower *= value;
        }
        
        // If the input is negative, then reverse the product.
        else
        {
            double newlower;
            newlower = (-upper)*value;
            upper = (-lower)*value;
            lower = newlower;
        }
        return *this;
    }
    
    Interval & Interval::operator *=(double value)
    {
        // Broken into if statements to reduce multiplication and (hopefully) branching.
        // If the input is nonnegative, don't reverse the product.
        if(value>=0)
        {
            upper *= value;
            lower *= value;
        }
        
        // If the input is negative, then reverse the product.
        else
        {
            double newlower;
            newlower = (-upper)*value;
            upper = (-lower)*value;
            lower = newlower;
        }
        return *this;
    }
    
    // In-place multiplication operator.  Uses correct rounding.
    // Assumes the input can be cast to a double.
    template <typename Type>
    Interval & Interval::operator*=(const Type &value)
    {
        // Broken into if statements to reduce multiplication and (hopefully) branching.
        // If the input is nonnegative, don't reverse the product.
        if(value>=0)
        {
            upper *= value;
            lower *= value;
        }
        
        // If the input is negative, then reverse the product.
        else
        {
            double newlower;
            newlower = upper;
            newlower *= -1;
            newlower *= value;
            upper = lower;
            upper *= -1;
            upper *=value;
            lower = newlower;
        }
        return *this;
    }
    
    // In-place division operator.  Uses correct rounding.
    // Assumes the right interval does not contain 0 (does not check)
    Interval & Interval::operator/=(const Interval &interval)
    {
        // Broken into if statements to reduce division and (hopefully) branching.
        // Only two cases since the denominator can't include zero.
        
        // If the denominator is positive.
        if(interval.lower<=0)
        {
            // If the numerator is nonnegative
            if(lower<=0)
            {
                lower = lower/interval.upper;
                upper = upper/(-interval.lower);
            }
            
            // If the numerator is nonpositive
            else if(upper<=0)
            {
                lower = (-lower)/interval.lower;
                upper = upper/interval.upper;
            }
            
            // If the numerator includes zero in the interior.
            else
            {
                lower = (-lower)/interval.lower;
                upper = upper/(-interval.lower);
            }
                
        }
        
        // If the denominator is negative.
        else
        {
            double newlower;
            // If the numerator is nonnegative
            if(lower<=0)
            {
                newlower = (-upper)/interval.upper;
                upper = lower/interval.lower;
                lower = newlower;
            }
            
            // If the numerator is nonpositive
            else if(upper<=0)
            {
                newlower = upper/interval.lower;
                upper = (-lower)/interval.upper;
                lower = newlower;
            }
            
            // If the numerator includes zero in the interior.
            else
            {
                newlower = (-upper)/interval.upper;
                upper = (-lower)/interval.upper;
                lower = newlower;
            }
        }
        return *this;
    }
    
    // In-place division operator.  Uses correct rounding.
    // Should be slightly faster than pass by reference for constants.
    Interval & Interval::operator /=(int value)
    {
        // Broken into if statements to reduce division and (hopefully) branching.
        // If the input is nonnegative, don't reverse the product.
        if(value>=0)
        {
            upper /= value;
            lower /= value;
        }
        
        // If the input is negative, then reverse the product.
        else
        {
            double newlower;
            newlower = (-upper)/value;
            upper = (-lower)/value;
            lower = newlower;
        }
        return *this;
    }
    
    Interval & Interval::operator /=(unsigned int value)
    {
        upper /= value;
        lower /= value;
        return *this;
    }
    
    Interval & Interval::operator /=(float value)
    {
        // Broken into if statements to reduce division and (hopefully) branching.
        // If the input is nonnegative, don't reverse the product.
        if(value>=0)
        {
            upper /= value;
            lower /= value;
        }
        
        // If the input is negative, then reverse the product.
        else
        {
            double newlower;
            newlower = (-upper)/value;
            upper = (-lower)/value;
            lower = newlower;
        }
        return *this;
    }
    
    Interval & Interval::operator /=(double value)
    {
        // Broken into if statements to reduce division and (hopefully) branching.
        // If the input is nonnegative, don't reverse the product.
        if(value>=0)
        {
            upper /= value;
            lower /= value;
        }
        
        // If the input is negative, then reverse the product.
        else
        {
            double newlower;
            newlower = (-upper)/value;
            upper = (-lower)/value;
            lower = newlower;
        }
        return *this;
    }
    
    // In-place division operator.  Uses correct rounding.
    // Assumes the input can be cast to a double.
    // Assume the input is not zero
    template <typename Type>
    Interval & Interval::operator/=(const Type &value)
    {
        double input = (double)value;
        
        // Broken into if statements to reduce division and (hopefully) branching.
        // If the input is nonnegative, don't reverse the product.
        if(input>=0)
        {
            upper /= input;
            lower /= input;
        }
        
        // If the input is negative, then reverse the product.
        else
        {
            double newlower;
            newlower = (-upper)/input;
            upper = (-lower)/input;
            lower = newlower;
        }
        return *this;
    }
    
    ///////////////////////
    // Interval data
    ///////////////////////
    
    // Returns the width of the interval.
    double width(const Interval &interval)
    {
        return interval.upper + interval.lower;
    }
    
    bool width(double &value,const Interval &interval)
    {
        value = width(interval);
        return true;
    }
    
    // Returns or sets the upper endpoint of interval.
    double upper(const Interval &interval)
    {
        return interval.upper;
    }
    
    bool upper(double &value,const Interval &interval)
    {
        value = upper(interval);
        return true;
    }
    
    // Returns or sets the lower endpoint of an interval
    double lower(const Interval &interval)
    {
        return (-interval.lower);
    }
    
    bool lower(double &value,const Interval &interval)
    {
        value = lower(interval);
        return true;
    }
    
    // Redurns or sets the midpoint of an interval
    double median(const Interval &interval)
    {
        return (interval.upper-interval.lower)/2;
    }
    
    bool median(double &value,const Interval &interval)
    {
        value = median(interval);
        return true;
    }
    
    // Returns or sets largest absolute value of element of interval
    double abs(const Interval &interval)
    {
        return std::max(interval.upper,interval.lower);
    }
    
    bool abs(double &value,const Interval &interval)
    {
        value = abs(interval);
        return true;
    }
    
    // Get the square of all the elements of an interval 
    // The first function save the resulting interval to itself,
    // the second function save the resulting interval to its 1st argument
    inline Interval square_norm(const Interval & interval)
    {
        Interval temp = interval;
        temp *= interval;
        if (containsZero(temp))
            temp.lower = 0;
        return temp;
    }
    inline bool square_norm(Interval & value, const Interval & interval)
    {
        value = interval;
        value *= interval;
        if (containsZero(value))
            value.lower = 0;
        return true;
    }

    ///////////////////////
    // Output operator
    ///////////////////////
    
    // Display function for an interval.  Uses width to format output correctly.
    // Changes rounding modes, so it shouldn't be used with time-based testing.
    std::ostream& operator<<(std::ostream &out, const Interval &interval)
    {
        std::fesetround(FE_TONEAREST);
        if(width(interval)==0)
            out << -interval.lower;
        else
            out << "(" << -interval.lower << "," << interval.upper << ")";
        std::fesetround(FE_UPWARD);
        return out;
    }
    
    ///////////////////////
    // Precision operators
    ///////////////////////
    
    // Returns the current precision.
    // Returns 0 since doubles can't change precision
    inline mpfr_prec_t get_precision(const Interval &interval)
    {
        return 0;
    }
    
    // Sets the precision of the data (without destroying data).
    // Returns false since precisions cannot be changed for doubles
    inline bool set_precision(Interval &interval, mpfr_prec_t prec)
    {
        return false;
    }
    
    // Increases the precision to the input precision.
    // If there is only one input, the precision is doubled
    inline bool increase_precision(Interval &interval)
    {
        return false;
    }
    
    inline bool increase_precision(Interval &interval, mpfr_prec_t prec)
    {
        return false;
    }
    
    // Decreases the precision to the input precision.
    // If there is only one input, the precision is halved
    inline bool decrease_precision(Interval &interval)
    {
        return false;
    }
    
    inline bool decrease_precision(Interval &interval, mpfr_prec_t prec)
    {
        return false;
    }
    
    ///////////////////////
    // Interval operations
    ///////////////////////
    
    // Returns or sets exponential of an interval.
    // The power must be an unsigned integer.
    Interval exp(Interval interval,unsigned int power)
    {
        Interval base(1);
        while (power != 0)
        {
            if ((power % 2)==1)
                base *= interval;
            power /= 2;
            interval *= interval;
        }
        return base;
    }
    
    bool exp(Interval &interval,const Interval& value, unsigned int power)
    {
        interval = 1;
        Interval base = value;
        while (power != 0)
        {
            if ((power % 2)==1)
                interval *= base;
            power /= 2;
            base *= base;
        }
        return true;
    }

    // Returns square root of an interval.  Uses correct rounding.
    // Assumes the interval is nonnegative (does not check).
    // I don't know how to remove the rounding from this function.
    Interval sqrt(Interval interval)
    {
        // Using std::sqrt to make sure to leave the namespace and use cmath's sqrt.
        interval.upper = std::sqrt(interval.upper);
        std::fesetround(FE_DOWNWARD);
        // Using std::sqrt to make sure to leave the namespace and use cmath's sqrt.
        interval.lower = -std::sqrt(-interval.lower);

        std::fesetround(FE_UPWARD);
        return interval;
    }
    
    bool sqrt(Interval &interval,const Interval &value)
    {
        // Using std::sqrt to make sure to leave the namespace and use cmath's sqrt.
        interval.upper = std::sqrt(value.upper);
        std::fesetround(FE_DOWNWARD);
        // Using std::sqrt to make sure to leave the namespace and use cmath's sqrt.
        interval.lower = -std::sqrt(-value.lower);
        std::fesetround(FE_UPWARD);
        return true;
    }
    
    ///////////////////////
    // Friend Operators
    ///////////////////////
    
    // Addition operator.  Uses correct rounding.
    Interval operator+(Interval interval1,const Interval &interval2)
    {
        interval1+=interval2;
        return interval1;
    }
    
    // Addition operator.  Uses correct rounding.
    // Should be slightly faster than pass by reference for constants.
    Interval operator+(Interval interval,int value)
    {
        interval += value;
        return interval;
    }
    
    Interval operator+(int value, Interval interval)
    {
        interval += value;
        return interval;
    }
    
    Interval operator+(Interval interval, unsigned int value)
    {
        interval += value;
        return interval;
    }
    
    Interval operator+(unsigned int value, Interval interval)
    {
        interval += value;
        return interval;
    }
    
    Interval operator+(Interval interval, float value)
    {
        interval += value;
        return interval;
    }
    
    Interval operator+(float value, Interval interval)
    {
        interval += value;
        return interval;
    }
    
    Interval operator+(Interval interval, double value)
    {
        interval += value;
        return interval;
    }
    
    Interval operator+(double value, Interval interval)
    {
        interval += value;
        return interval;
    }
    
    // Addition operator.  Uses correct rounding.
    // Assumes the input can be cast to a double.
    template <typename Type>
    Interval operator+(Interval interval,const Type &value)
    {
        interval += value;
        return interval;
    }
    
    template <typename Type>
    Interval operator+(const Type &value,Interval interval)
    {
        interval+=value;
        return interval;
    }
    
    // Subtraction operator.  Uses correct rounding.
    Interval operator-(Interval interval1,const Interval &interval2)
    {
        interval1-=interval2;
        return interval1;
    }
    
    // Addition operator.  Uses correct rounding.
    // Should be slightly faster than pass by reference for constants.
    Interval operator-(Interval interval,int value)
    {
        interval -= value;
        return interval;
    }
    
    Interval operator-(int value, Interval interval)
    {
        interval *= -1;
        interval+=value;
        return interval;
    }
    
    Interval operator-(Interval interval, unsigned int value)
    {
        interval -= value;
        return interval;
    }
    
    Interval operator-(unsigned int value, Interval interval)
    {
        interval *= -1;
        interval+=value;
        return interval;
    }
    
    Interval operator-(Interval interval, float value)
    {
        interval -= value;
        return interval;
    }
    
    Interval operator-(float value, Interval interval)
    {
        interval *= -1;
        interval+=value;
        return interval;
    }
    
    Interval operator-(Interval interval, double value)
    {
        interval -= value;
        return interval;
    }
    
    Interval operator-(double value, Interval interval)
    {
        interval *= -1;
        interval+=value;
        return interval;
    }
    
    // Subtraction operator.  Uses correct rounding.
    // Assumes the input can be cast to a double.
    template <typename Type>
    Interval operator-(Interval interval,const Type &value)
    {
        interval -= value;
        return interval;
    }
    
    template <typename Type>
    Interval operator-(const Type &value,Interval interval)
    {
        interval *= -1;
        interval+=value;
        return interval;
    }
    
    // Multiplication operator.  Uses correct rounding.
    Interval operator*(Interval interval1,const Interval &interval2)
    {
        interval1*=interval2;
        return interval1;
    }
    
    // Multiplication operator.  Uses correct rounding.
    // Should be slightly faster than pass by reference for constants.
    Interval operator*(Interval interval,int value)
    {
        interval *= value;
        return interval;
    }
    
    Interval operator*(int value, Interval interval)
    {
        interval *= value;
        return interval;
    }
    
    Interval operator*(Interval interval, unsigned int value)
    {
        interval *= value;
        return interval;
    }
    
    Interval operator*(unsigned int value, Interval interval)
    {
        interval *= value;
        return interval;
    }
    
    Interval operator*(Interval interval, float value)
    {
        interval *= value;
        return interval;
    }
    
    Interval operator*(float value, Interval interval)
    {
        interval *= value;
        return interval;
    }
    
    Interval operator*(Interval interval, double value)
    {
        interval *= value;
        return interval;
    }
    
    Interval operator*(double value, Interval interval)
    {
        interval *= value;
        return interval;
    }
    
    // Multiplication operator.  Uses correct rounding.
    // Assumes the input can be cast to a double.
    template <typename Type>
    Interval operator*(Interval interval,const Type &value)
    {
        interval *= value;
        return interval;
    }
    
    template <typename Type>
    Interval operator*(const Type &value,Interval interval)
    {
        interval *= value;
        return interval;
    }

    // Division operator.  Uses correct rounding.
    // Assumes the right interval does not contain 0 (does not check)
    Interval operator/(Interval interval1,const Interval &interval2)
    {
        interval1/=interval2;
        return interval1;
    }
    
    // Division operator.  Uses correct rounding.
    // Should be slightly faster than pass by reference for constants.
    Interval operator/(Interval interval, int value)
    {
        interval /= value;
        return interval;
    }
    
    Interval operator/(int value, Interval interval)
    {
        // Broken into if statements to reduce division and (hopefully) branching.
        // If the input is nonnegative, reverse the product.
        if(value>=0)
        {
            double newlower;
            newlower = (-value)/interval.upper;
            interval.upper = value/(-interval.lower);
            interval.lower = newlower;
        }
        else
        {
            interval.lower = value/interval.lower;
            interval.upper = value/interval.upper;
        }
        return interval;
    }
    
    Interval operator/(Interval interval, unsigned int value)
    {
        interval /= value;
        return interval;
    }
    
    Interval operator/(unsigned int value, Interval interval)
    {
        double newlower;
        newlower = (-value)/interval.upper;
        interval.upper = value/(-interval.lower);
        interval.lower = newlower;
        return interval;
    }
    
    Interval operator/(Interval interval, float value)
    {
        interval /= value;
        return interval;
    }
    
    Interval operator/(float value, Interval interval)
    {
        {
            // Broken into if statements to reduce division and (hopefully) branching.
            // If the input is nonnegative, reverse the product.
            if(value>=0)
            {
                double newlower;
                newlower = (-value)/interval.upper;
                interval.upper = value/(-interval.lower);
                interval.lower = newlower;
            }
            else
            {
                interval.lower = value/interval.lower;
                interval.upper = value/interval.upper;
            }
            return interval;
        }
    }
    
    Interval operator/(Interval interval, double value)
    {
        interval /= value;
        return interval;
    }
    
    Interval operator/(double value, Interval interval)
    {
        // Broken into if statements to reduce division and (hopefully) branching.
        // If the input is nonnegative, reverse the product.
        if(value>=0)
        {
            double newlower;
            newlower = (-value)/interval.upper;
            interval.upper = value/(-interval.lower);
            interval.lower = newlower;
        }
        else
        {
            interval.lower = value/interval.lower;
            interval.upper = value/interval.upper;
        }
        return interval;
    }
    
    // Division operator.  Uses correct rounding.
    // Assumes the input can be cast to a double.
    // Assume the input is not zero
    template <typename Type>
    Interval operator/(Interval interval,const Type &value)
    {
        interval /= value;
        return interval;
    }
    
    // Assumes the right interval does not contain 0 (does not check)
    template <typename Type>
    Interval operator/(const Type &value,Interval interval)
    {
        double input = (double)value;
        
        // Broken into if statements to reduce division and (hopefully) branching.
        // If the input is nonnegative, reverse the product.
        if(input>=0)
        {
            double newlower;
            newlower = (-input)/interval.upper;
            interval.upper = input/(-interval.lower);
            interval.lower = newlower;
        }
        else
        {
            interval.lower = input/interval.lower;
            interval.upper = input/interval.upper;
        }
        return interval;
    }
    
    ///////////////////////
    // Comparison Operators
    ///////////////////////
    
    // Equality operator.  True if and only if both upper and lower values are equal.
    bool operator==(const Interval& interval1, const Interval& interval2)
    {
        return (interval1.upper == interval2.upper) && (interval1.lower == interval2.lower);
    }
    
    // Equality operator.
    // Should be slightly faster than pass by reference for constants.
    bool operator==(const Interval & interval, int value)
    {
        return (-interval.lower == value) && (interval.upper == value);
    }
    
    bool operator==(int value, const Interval & interval)
    {
        return interval==value;
    }
    
    bool operator==(const Interval & interval, unsigned int value)
    {
        return (-interval.lower == value) && (interval.upper == value);
    }
    
    bool operator==(unsigned int value, const Interval & interval)
    {
        return interval==value;
    }
    
    bool operator==(const Interval & interval, float value)
    {
        return (-interval.lower == value) && (interval.upper == value);
    }
    
    bool operator==(float value, const Interval & interval)
    {
        return interval==value;
    }
    
    bool operator==(const Interval & interval, double value)
    {
        return (-interval.lower == value) && (interval.upper == value);
    }
    
    bool operator==(double value, const Interval & interval)
    {
        return interval==value;
    }
    
    // Equality operator.  Assumes that the input can be cast to a double.
    // Can only return true when interval is a point.
    template <typename Type>
    bool operator==(const Interval &interval, const Type &value)
    {
        double input = (double)value;
        return input == interval;
    }
    
    template <typename Type>
    bool operator==(const Type &value,const Interval &interval)
    {
        return interval==value;
    }
    
    // Inequality operator.  False if and only if both upper and lower values are equal.
    bool operator!=(const Interval& interval1, const Interval& interval2)
    {
        return !(interval1==interval2);
    }
    
    // Inquality operator.
    // Should be slightly faster than pass by reference for constants.
    bool operator!=(const Interval &interval, int value)
    {
        return !(interval==value);
    }
    
    bool operator!=(int value, const Interval &interval)
    {
        return !(interval==value);
    }
    
    bool operator!=(const Interval &interval, unsigned int value)
    {
        return !(interval==value);
    }
    
    bool operator!=(unsigned int value, const Interval &interval)
    {
        return !(interval==value);
    }
    
    bool operator!=(const Interval &interval, float value)
    {
        return !(interval==value);
    }
    
    bool operator!=(float value, const Interval &interval)
    {
        return !(interval==value);
    }
    
    bool operator!=(const Interval &interval, double value)
    {
        return !(interval==value);
    }
    
    bool operator!=(double value, const Interval &interval)
    {
        return !(interval==value);
    }
    
    // Inequality operator.  Assumes that the input can be cast to a double.
    // Always returns false when interval has width.
    template <typename Type>
    bool operator!=(const Interval &interval ,const Type &value)
    {
        return !(interval==value);
    }
    
    template <typename Type>
    bool operator!=(const Type &value,const Interval &interval)
    {
        return !(interval==value);
    }
    
    // Less than operator.  True if the left interval is completely less than the right interval.
    bool operator<(const Interval &interval1, const Interval &interval2)
    {
        return (interval1.upper<-interval2.lower);
    }
    
    // Less than operator.  True if the interval is completely less than the point.
    // Should be slightly faster than pass by reference for constants.
    bool operator<(const Interval &interval, int value)
    {
        return (interval.upper<value);
    }
    
    bool operator<(int value, const Interval &interval)
    {
        return (-interval.lower>value);
    }
    
    bool operator<(const Interval &interval, unsigned int value)
    {
        return (interval.upper<value);
    }
    
    bool operator<(unsigned int value, const Interval &interval)
    {
        return (-interval.lower>value);
    }
    
    bool operator<(const Interval &interval, float value)
    {
        return (interval.upper<value);
    }
    
    bool operator<(float value, const Interval &interval)
    {
        return (-interval.lower>value);
    }
    
    bool operator<(const Interval &interval, double value)
    {
        return (interval.upper<value);
    }
    
    bool operator<(double value, const Interval &interval)
    {
        return (-interval.lower>value);
    }
    
    // Less than operator.  True if the interval is completely less than the point.
    // Assumes that the input can be cast to a double.
    template <typename Type>
    bool operator<(const Interval &interval,const Type &value)
    {
        double input = (double)value;
        return (interval < input);
    }
    
    // Less than operator.  True if the interval is completely greater than the point.
    // Assumes that the input can be cast to a double.
    template <typename Type>
    bool operator<(const Type &value,const Interval &interval)
    {
        double input = (double)value;
        return (input < interval);
    }
    
    // Less than or equal operator.  True if the left interval is completely less than or equal the right interval.
    bool operator<=(const Interval &interval1, const Interval &interval2)
    {
        return (interval1.upper<=-interval2.lower);
    }
    
    // Less than or equal to operator.  True if the interval is completely less than the point.
    // Should be slightly faster than pass by reference for constants.
    // Always returns true when interval has width.
    bool operator<=(const Interval &interval, int value)
    {
        return (interval.upper<=value);
    }
    
    bool operator<=(int value, const Interval &interval)
    {
        return (-interval.lower>=value);
    }
    
    bool operator<=(const Interval &interval, unsigned int value)
    {
        return (interval.upper<=value);
    }
    
    bool operator<=(unsigned int value, const Interval &interval)
    {
        return (-interval.lower>=value);
    }
    
    bool operator<=(const Interval &interval, float value)
    {
        return (interval.upper<=value);
    }
    
    bool operator<=(float value, const Interval &interval)
    {
        return (-interval.lower>=value);
    }
    
    bool operator<=(const Interval &interval, double value)
    {
        return (interval.upper<=value);
    }
    
    bool operator<=(double value, const Interval &interval)
    {
        return (-interval.lower>=value);
    }
    
    // Less than or equal operator.  True if the interval is completely less than or equal the point.
    // Assumes that the input can be cast to a double.
    template <typename Type>
    bool operator<=(const Interval &interval,const Type &value)
    {
        double input = (double)value;
        return (interval <= input);
    }
    
    // Less or equal than operator.  True if the interval is completely greater than or equal the point.
    // Assumes that the input can be cast to a double.
    template <typename Type>
    bool operator<=(const Type &value,const Interval &interval)
    {
        double input = (double)value;
        return (input <= interval);
    }
    
    // Greater than operator.  True if the left interval is completely greater than the right interval.
    bool operator>(const Interval &interval1, const Interval &interval2)
    {
        return (interval2 < interval1);
    }
    
    // Greater than operator.  True if the interval is completely greater than the point.
    // Should be slightly faster than pass by reference for constants.
    // Always returns true when interval has width.
    bool operator>(const Interval &interval, int value)
    {
        return (value < interval);
    }
    
    bool operator>(int value, const Interval &interval)
    {
        return (interval < value);
    }
    
    bool operator>(const Interval &interval, unsigned int value)
    {
        return (value < interval);
    }
    
    bool operator>(unsigned int value, const Interval &interval)
    {
        return (interval < value);
    }
    
    bool operator>(const Interval &interval, float value)
    {
        return (value < interval);
    }
    
    bool operator>(float value, const Interval &interval)
    {
        return (interval < value);
    }
    
    bool operator>(const Interval &interval, double value)
    {
        return (value < interval);
    }
    
    bool operator>(double value, const Interval &interval)
    {
        return (interval < value);
    }
    
    // Greater than operator.  True if the interval is completely greater than the point.
    // Assumes that the input can be cast to a double.
    template <typename Type>
    bool operator>(const Interval &interval,const Type &value)
    {
        double input = (double)value;
        return (input < interval);
    }
    
    // Greater than operator.  True if the interval is completely less than the point.
    // Assumes that the input can be cast to a double.
    template <typename Type>
    bool operator>(const Type &value,const Interval &interval)
    {
        double input = (double)value;
        return (interval < input);
    }
    
    // Greater than or equal operator.  True if the left interval is completely greater than or equal the right interval.
    bool operator>=(const Interval &interval1, const Interval &interval2)
    {
        return (interval2 <= interval1);
    }
    
    // Greater than operator.  True if the interval is completely greater than the point.
    // Should be slightly faster than pass by reference for constants.
    // Always returns true when interval has width.
    bool operator>=(const Interval &interval, int value)
    {
        return (value <= interval);
    }
    
    bool operator>=(int value, const Interval &interval)
    {
        return (interval <= value);
    }
    
    bool operator>=(const Interval &interval, unsigned int value)
    {
        return (value <= interval);
    }
    
    bool operator>=(unsigned int value, const Interval &interval)
    {
        return (interval <= value);
    }
    
    bool operator>=(const Interval &interval, float value)
    {
        return (value <= interval);
    }
    
    bool operator>=(float value, const Interval &interval)
    {
        return (interval <= value);
    }
    
    bool operator>=(const Interval &interval, double value)
    {
        return (value <= interval);
    }
    
    bool operator>=(double value, const Interval &interval)
    {
        return (interval <= value);
    }
    
    // Greater or equal than operator.  True if the interval is completely greater than or equal the point.
    // Assumes that the input can be cast to a double.
    template <typename Type>
    bool operator>=(const Interval &interval,const Type &value)
    {
        double input = (double)value;
        return (input <= interval);
    }
    
    // Greater than or equal operator.  True if the interval is completely less than or equal the point.
    // Assumes that the input can be cast to a double.
    template <typename Type>
    bool operator>=(const Type &value,const Interval &interval)
    {
        double input = (double)value;
        return (interval <= input);
    }
    
    ///////////////////////
    // Friend Inclusion Operators
    ///////////////////////
    
    // Returns true if the first value is contained in the interval
    bool contains(int value, const Interval &interval)
    {
        return (value <= interval.upper) && (value>= -interval.lower);
    }
    
    bool contains(unsigned int value, const Interval &interval)
    {
        return (value <= interval.upper) && (value>= -interval.lower);
    }
    
    bool contains(float value, const Interval &interval)
    {
        return (value <= interval.upper) && (value>= -interval.lower);
    }
    
    bool contains(double value, const Interval &interval)
    {
        return (value <= interval.upper) && (value>= -interval.lower);
    }
    
    // Templated version
    template <typename Type>
    bool contains(const Type &value, const Interval &interval)
    {
        return (value <= interval.upper) && (value>= -interval.lower);
    }
    
    // Returns true if 0 is in the interval
    bool containsZero(const Interval &interval)
    {
        return (0 <= interval.upper) && (0 <=interval.lower);
    }
}

#endif /* Interval_tcc */
