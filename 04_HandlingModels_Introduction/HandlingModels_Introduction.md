# Handling models: An introduction

The C interface of SIMCA-Q offers a handler for SIMCA models, the structure *tagSQ_ModelHandle*, as well as a typedef-name for pointers to this structure, *SQ_Model*. SIMCA-Q also offers several functions, defined in SQModel.h, that can be used to retrieve information on the model as well as for other purpose e.g., create further SIMCA-Q structures to make predictions.

If we have previously [loaded a SIMCA project into a SQ_Project pointer](../02_HandlingProjects/HandlingProjects.md), let's name it hProject, we can initialize a pointer to a *tagSQ_ModelHandle* structure, let's name it hModel, by:
```
SQ_Model hModel = NULL;
```

The next step would consist in loading the model. For this we need to know the number associated with the model of interest. However, it might be not straightforward to know this number. Nevertheless, one can get it from the model index. All models withon a SIMCA project/file are associated to indices starting from 1 in the same order as they appear in the SIMCA project/file. Provided that we know the index of the model of interest, we can get the associated model number by calling the function SQ_GetModelNumberFromIndex() which receives as input parameters the SQ_Project pointer, the model index and the address of the model number that we want to retrieve. For instance, for retrieving the model win index equal to 1:
```
int iModelIndex = 1;
int iModelNumber;
SQ_GetModelNumberFromIndex(hProject, iModelIndex, &iModelNumber);
```

Once we know the number of the model of interest, we can retrieve the SIMCA model handler by:
```
SQ_GetModel(hProject, iModelNumber, &hModel);
```

At this stage we are ready to retrieve information on the project. For instance, to retrieve the model name:
```
char szBuffer[256];
SQ_GetModelName(hModel, szBuffer, sizeof(szBuffer));
```

To retrieve the model type:
```
char szBuffer[256];
SQ_GetModelTypeString(hModel, szBuffer, sizeof(szBuffer));
```

To retrieve the number of components of the model:
```
int iNumComponents;
SQ_GetNumberOfComponents(hModel, &iNumComponents);
```

To recap, find below a script that combines all of this. Specifically, the script will receive as an input a SIMCA project (usp file) and print out information on the model with index equal to 1:
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

  SQ_ErrorCode eError; // handler for SIMCA-Q errors
  char szError[256]; // C-string for handling SIMCA-Q error descriptions

  // Load the project
  SQ_Project hProject = NULL;
  const char * szUSPFile = argv[1];
  const char * szPassword = NULL;
  eError = SQ_OpenProject(szUSPFile, szPassword, &hProject);
  if (eError != SQ_E_OK)
    {            
      SQ_GetErrorDescription(eError, szError, sizeof(szError));
      std::cout << szError << std::endl;
      return -1;
    }
  
  // Load the model with index = 1
  SQ_Model hModel = NULL;
  int iModelIndex = 1;
  int iModelNumber;
  SQ_Bool bIsFitted;
  eError = SQ_GetModelNumberFromIndex(hProject, iModelIndex, &iModelNumber);
  eError = SQ_GetModel(hProject, iModelNumber, &hModel);
  // Check if model is correct (=fitted) 
  if (SQ_IsModelFitted(hModel, &bIsFitted) != SQ_E_OK || bIsFitted != SQ_True)
    return -1;

  // Print model properties

  char szBuffer[256];
  int iNumComponents;

  // Model name
  eError = SQ_GetModelName(hModel, szBuffer, sizeof(szBuffer));
  std::cout<<"The name of the model is: "<< szBuffer <<"."<<std::endl;

  // Model type 
  eError = SQ_GetModelTypeString(hModel, szBuffer, sizeof(szBuffer));
  std::cout<<"The model type is: "<< szBuffer <<"."<<std::endl;
  
  // Number of components
  eError = SQ_GetNumberOfComponents(hModel, &iNumComponents);
  std::cout<<"The number of components in the model is: "<< iNumComponents <<"."<<std::endl;

  // Close the project
  eError = SQ_CloseProject(&hProject);
  hProject = NULL;

  return 0;
}
```