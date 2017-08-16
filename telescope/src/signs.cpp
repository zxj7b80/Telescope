//
// C++ Implementation: signs
//
// Description: 
//
//
// Author:  <>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//


int sgn(double x)
{
    int result;

    if(x>0)
        result=1;
    else
        if(x==0)
            result=0;
        else
            result=-1;

    return(result);
}
