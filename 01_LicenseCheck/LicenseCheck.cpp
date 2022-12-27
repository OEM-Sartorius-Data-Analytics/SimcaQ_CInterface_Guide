#include <iostream>
#include <cmath>
#include "SQ.h"


int main()
{
  std::cout<<"Hi! "<<sqrt(10000)<<std::endl;

  SQ_Bool bValid;

  if (SQ_IsLicenseFileValid(&bValid) != SQ_E_OK)
   {
     std::cout<<"Could not read license"<<std::endl;
     std::cout<<bValid<<std::endl;
   }
  else
    {
      if (!bValid)
       {
         std::cout<<"Invalid license"<<std::endl;
       }
      else
       {
         std::cout<<"Valid license"<<std::endl;
       }
  
    }
  

  return 0;
}
