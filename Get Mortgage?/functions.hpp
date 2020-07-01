//
//  functions.hpp
//  Get Mortgage?
//
//  Created by Erik Amundson on 6/22/20.
//  Copyright © 2020 Erik Amundson. All rights reserved.
//

#ifndef functions_hpp
#define functions_hpp

#include <stdio.h>
#include <iostream>
using namespace std;
#include <string>

string separate( double x, int p, int group, char separator );
double get_monthly_payment(double down, double apr, double mortgage, int years);


struct finals{
    double no_mortgage;
    double yes_mortgage;
};

finals calculate_mortgage(int years, double down, double mortgage, double apr, double capital, double annual_return2);

void do_mortgage();
#endif /* functions_hpp */
