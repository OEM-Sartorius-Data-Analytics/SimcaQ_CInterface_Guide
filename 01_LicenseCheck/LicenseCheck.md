# A simple SIMCA-Q script: Check your license

In order to get started we will go through a simple script for checking the validity and charateristics of your SIMCA-Q license. In order for your app to work, you should have a SIMCA-Q license in the same directory as your executable (other options will be discussed later on).

To begin with, we can check the validity (or existence) of the license file by using the function *SQ_IsLicenseValid()*, which takes as an argument a reference to a SIMCA-Q enumeration type named *SQ_Bool* (*{ SQ_False = 0, SQ_True }*). Actually, most SIMCA-Q functions return a SIMCA-Q enumeration type named *SQ_ErrorCode*, which can have values of either *Success* (*SQ_E_OK*) or of almost any possible error type. SIMCA-Q also offers the possibility to get a description of a *SQ_ErrorCode*. For this we can use the function *SQ_GetErrorDescription()*, which receives as input parameters a *SQ_ErrorCode* type, a pointer to an array and the size of that array. Thus, a useful way of invoking *SQ_IsLicenseValid()* could be e.g.:
```
SQ_Bool bValid;
SQ_ErrorCode eError;
char szError[256];

eError = SQ_IsLicenseFileValid(&bValid);
if (eError != SQ_E_OK)
  {
    SQ_GetErrorDescription(eError, szError, sizeof(szError));
    std::cout << szError << std::endl;
  }
else
  {
  // Code that provides information on the license ...
  }
```

```
#include <iostream>
#include "SIMCAQP.h"


int main()
{
  SQ_Bool bValid; // SIMCA-Q enumeration { SQ_False = 0, SQ_True }. Used here for determining license validity.
  SQ_ErrorCode eError; // handler for SIMCA-Q errors
  char szError[256]; // C-string for handling SIMCA-Q error descriptions

  // Determine license validity
  eError = SQ_IsLicenseFileValid(&bValid);

  if (eError != SQ_E_OK)
   {
     SQ_GetErrorDescription(eError, szError, sizeof(szError));
     std::cout << szError << std::endl;
   }
  else
    {
      if (!bValid)
       {
         std::cout<<"You have an invalid license"<<std::endl;	 
       }
      else
       {
         std::cout<<"You have a valid license,"<<std::endl;
	 
	 char szBuffer[256];
	 eError = SQ_GetLicenseFileExpireDate(szBuffer,sizeof(szBuffer));
	 if (eError != SQ_E_OK)
	   {
	     SQ_GetErrorDescription(eError, szError, sizeof(szError));
             std::cout << szError << std::endl;
             return 0;
	   }
	 else
	   {
	     std::cout << "which will expire on " << szBuffer << std::endl;
	   }
	     

	 SQ_Product sqProduct; // SIMCA-Q enumeration for determining type.
	 eError = SQ_GetLicenseFileProduct (&sqProduct);
	 if (eError != SQ_E_OK)
	   {
	     SQ_GetErrorDescription(eError, szError, sizeof(szError));
             std::cout << szError << std::endl;
             return 0;
	   }
	 else
	   {
	     switch(sqProduct)
	       {
	       case 0:
		 std::cout << "Yout license is of type SQP." << std::endl;
		 break;
	       case 1:
		 std::cout << "Yout license is of type SQPPlus." << std::endl;
		 break;
	       case 2:
		 std::cout << "Yout license is of type SQM." << std::endl;
		 break;
	       case 3:
		 std::cout << "Yout license is of type SQAll." << std::endl;
		 break;
	       default:
		 std::cout << "An unknown error occurred when determining your type of license." << std::endl;
	       }
	   }	 
       }  
    }  

  return 0;
}
```