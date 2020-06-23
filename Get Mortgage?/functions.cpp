//
//  functions.cpp
//  Get Mortgage?
//
//  Created by Erik Amundson on 6/22/20.
//  Copyright © 2020 Erik Amundson. All rights reserved.
//

#include "functions.hpp"
#include <iostream>
using namespace std;
#include <iomanip>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>

string separate( double x, int p, int group, char separator )
{
   const string digits = "0123456789";

   stringstream ss;                                                      // put output into string s via a stringstream
   ss << showpoint << setprecision( p ) << x;
   string s = ss.str();

   // Number of separators
   int posPoint = (int)s.find( '.' );                                                         // position of decimal point in s
   int digitsFront = posPoint - (int)s.find_first_of( digits );                               // # digits before decimal point
   int digitsBack  = (int)s.find_last_of( digits ) - posPoint;                                // # digits after decimal point
   int sepFront = digitsFront / group;   if ( digitsFront % group == 0 ) sepFront--;     // # separators before decimal point
   int sepBack  = digitsBack  / group;   if ( digitsBack  % group == 0 ) sepBack--;      // # separators after decimal point
   
   // Compose separated string
   for ( int i = 1; i <= sepFront; i++ ) s.insert( posPoint - i *   group      , 1, separator );   // separators before point
   posPoint += sepFront;                                                                           // adjust decimal point
   for ( int i = 1; i <= sepBack ; i++ ) s.insert( posPoint + i * ( group + 1 ), 1, separator );   // separators after point

   return s;
}


double get_monthly_payment(double down, double apr, double mortgage, int years){
    double monthly_payment = 0;
    double r = apr/12;
    int n = years * 12;
    double discount_factor = ((1+r)* n - 1) / (r * (1 + r) * n);
    monthly_payment = mortgage/discount_factor;
    return monthly_payment;
}

struct finals;

finals calculate_mortgage(int years, double down, double mortgage, double apr, double capital, double annual_return, double yearly_dividends, double quarterly_dividends, double biannual_dividends, double monthly_dividends){
    //declare some variables
    double monthly_return = annual_return/12;
    int months = years*12;
    double monthly_payment = get_monthly_payment(down, apr, mortgage, years);
    double house_price = down + mortgage;
    double no_mortgage = capital - house_price;
    double yes_mortgage = capital - down;
    double dividends;
    //
    
    for (int i = 1; i<= months; i++){
        if(i%12 == 0){
            dividends = yearly_dividends + monthly_dividends + quarterly_dividends + biannual_dividends;
            no_mortgage += dividends;
            yes_mortgage += dividends;
        }
        else if (i%6 == 0){
            dividends = biannual_dividends + quarterly_dividends + monthly_dividends;
            no_mortgage += dividends;
            yes_mortgage += dividends;
        }
        else if (i%3 ==0){
            dividends = quarterly_dividends + monthly_dividends;
            no_mortgage += dividends;
            yes_mortgage += dividends;
        } else {
            no_mortgage += monthly_dividends;
            yes_mortgage += monthly_dividends;
        }
        yes_mortgage -= monthly_payment;
        yes_mortgage *= (1 + monthly_return);
        no_mortgage *= (1 + monthly_return);
    }
    struct finals x = {no_mortgage, yes_mortgage};
    return x;
}

void do_mortgage(){
    //input variables
    double house_price;
    double down;
    double mortgage;
    int years;
    double capital;
    double apr;
    double annual_return;
    double yearly_dividends;
    double biannual_dividends;
    double quarterly_dividends;
    double monthly_dividends;
    cout << "\nWhat is the house price? $";
    cin >> house_price;
    cout << "\nWhat is your down payment? $";
    cin >> down;
    mortgage = house_price-down;
    cout << "\nHow many years is your mortgage? ";
    cin >> years;
    cout << "\nWhat is the APR on your mortgage in % ?  ";
    cin >> apr;
    apr = apr/100;
    cout << "\nWhat is your starting capital? $";
    cin >> capital;
    cout << "\nWhat is your expected annual return in % ? ";
    cin >> annual_return;
    annual_return = annual_return/100;
    cout << "\nWhat is your expected total yearly dividend? $";
    cin >> yearly_dividends;
    cout << "\nWhat is your expected total biannual dividend? $";
    cin >> biannual_dividends;
    cout << "\nWhat is your expected total quarterly dividend? $";
    cin >> quarterly_dividends;
    cout << "\nWhat is your expected total monthly dividend? $";
    cin >> monthly_dividends;
    
    //get number values
    struct finals x = calculate_mortgage(years, down, mortgage, apr, capital, annual_return, yearly_dividends, quarterly_dividends, biannual_dividends, monthly_dividends);
    
    double no_mortgage = x.no_mortgage;
    double yes_mortgage = x.yes_mortgage;
    
    bool get_mortgage = yes_mortgage >= no_mortgage;
    double bigger_number;
    string to_get_mortgage;
    if (get_mortgage){
        to_get_mortgage = "You should get a mortgage.";
        bigger_number = yes_mortgage;
    } else {
        bigger_number = no_mortgage;
        to_get_mortgage = "You should not get a mortgage.";
    }
    int precision = trunc(log10(bigger_number)) + 3;
    string yes_string = separate(yes_mortgage, precision, 3, ',');
    string no_string = separate(no_mortgage, precision, 3, ',');

    cout << "\n\nAfter " << years << " years, your capital will be $" << yes_string << " if you take out a mortgage and $" << no_string << " if you don't. " << to_get_mortgage << "\n\n";
}
