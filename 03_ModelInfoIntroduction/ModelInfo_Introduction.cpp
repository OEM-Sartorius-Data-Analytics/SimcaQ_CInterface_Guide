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
