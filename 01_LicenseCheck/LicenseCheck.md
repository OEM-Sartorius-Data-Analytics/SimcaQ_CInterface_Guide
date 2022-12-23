# A script to check the SIMCA-Q license

```
#include <iostream>
#include "SQ.h"

int main()
{
  SQ_Bool bValid;

  if (SQ_IsLicenseFileValid(&bValid) != SQ_E_OK)
   {
     std::cout<<"Could not read license"<<std::endl;
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

```