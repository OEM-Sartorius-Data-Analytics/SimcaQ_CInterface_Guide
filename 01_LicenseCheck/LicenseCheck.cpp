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
