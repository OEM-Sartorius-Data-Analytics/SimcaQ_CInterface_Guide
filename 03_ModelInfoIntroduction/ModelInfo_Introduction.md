# The ModelInfo structure: Obtaining information about models withouth loading them

SIMCA-Q offers a C structure, *tagSQ_ModelInfo*, which can be accessed as well with the typedef-name *SQ_ModelInfo*, which can be used to retrieve information about a model without actually loading it. This structure can be created using the function *SQ_GetModelInfo()*, which receives as input parameters a [SQ_Project pointer](../02_HandlingProjects/HandlingProjects.md), the model number and the address of the actual *SQ_ModelInfo* structure that we want to retrieve (which we would have previously declared). It is not straightforward to know the model number. However, one can get it from the model index. All models withon a SIMCA project/file are associated to indices starting from 1 in the same order as they appear in the SIMCA project/file. Provided that we know the index of the model of interest, we can get the associated model number by calling the function *SQ_GetModelNumberFromIndex()* which receives as input parameters the SQ_Project pointer, the model index and the address of the model number that we want to retrieve. Thus, if e.g., we want to retireve the *tagSQ_ModelInfo* structure for the model with index equal to 1 of a (already loaded as e.g., *hProject*) SIMCA project handler, we could do it by:
```
int iModelIndex = 1;
int iModelNumber = -1;  
SQ_GetModelNumberFromIndex(hProject, iModelIndex, &iModelNumber);

SQ_ModelInfo oModelInfo;
SQ_GetModelInfo(hProject, iModelNumber, &oModelInfo);
```

From the *SQ_ModelInfo* structure we could already access plenty of model attributes. The following code is an example that would print the model name, its type, the number of observations used to build it as well as its number of X and Y variables:
```
std::cout<<"The name of the model is: "<< oModelInfo.modelName <<","<<std::endl;
std::cout<<"and it is of type: "<< oModelInfo.modelTypeName <<"."<<std::endl;
std::cout<<"It was built from "<< oModelInfo.numberOfObservations <<" observations."<<std::endl;
std::cout<<"including "<< oModelInfo.numberOfXVariables <<" X variables"<<std::endl;
std::cout<<"and "<< oModelInfo.numberOfYVariables <<" Y variables"<<std::endl;
```

Below you can find an [example](ModelInfo_Introduction.cpp) where all this commands are combined into a script that accepts as an input parameter the relative path to a SIMCA file and prints some properties of the model with index number 1:
```
#include <iostream>
#include "SIMCAQP.h"

int main(int argc,char* argv[])
{
  // Check that only one input argument was passed when calling the script
  // i.e., the name of the SIMCA project/file to be opened. Otherwise stop the program
  if(argc==1)
    {
      std::cout<<"\nYou need to pass a SIMCA file as an argument\n";
      return -1;
    }
  else if(argc>2)
    {
      std::cout<<"\nToo many arguments\n";
      return -1;
    }
  
  // Initialize the SQ_Project pointer for handling SIMCA projects
  SQ_Project hProject = NULL;

  SQ_ErrorCode eError; // handler for SIMCA-Q errors
  const char * szUSPFile = argv[1];
  const char * szPassword = NULL;
  eError = SQ_OpenProject(szUSPFile, szPassword, &hProject);
  char szError[256]; // C-string for handling SIMCA-Q error descriptions
  if (eError != SQ_E_OK)
    {            
      SQ_GetErrorDescription(eError, szError, sizeof(szError));
      std::cout << szError << std::endl;
      return -1;
    }

  // Retrieve model number for the model with index = 1
  int iModelIndex = 1;
  int iModelNumber = -1;  
  eError = SQ_GetModelNumberFromIndex(hProject, iModelIndex, &iModelNumber);

  // Retrieve the tagSQ_ModelInfo structure for the model
  SQ_ModelInfo oModelInfo;
  SQ_GetModelInfo(hProject, iModelNumber, &oModelInfo);

  // Print properties of the model
  std::cout<<"The name of the model is: "<< oModelInfo.modelName <<","<<std::endl;
  std::cout<<"and it is of type: "<< oModelInfo.modelTypeName <<"."<<std::endl;
  std::cout<<"It was built from "<< oModelInfo.numberOfObservations <<" observations."<<std::endl;
  std::cout<<"including "<< oModelInfo.numberOfXVariables <<" X variables"<<std::endl;
  std::cout<<"and "<< oModelInfo.numberOfYVariables <<" Y variables"<<std::endl;

  return 0;
}
```