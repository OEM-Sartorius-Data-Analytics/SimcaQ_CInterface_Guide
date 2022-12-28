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

  SQ_ErrorCode eError; // handler for SIMCA-Q erros
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

  // Find out and print the name of the project
  char projectName [256];
  eError = SQ_GetProjectName(hProject, projectName, sizeof(projectName));
  std::cout<<"The name of the project is: "<<projectName<<"."<<std::endl;

  // Find out and print the number of models in the project
  int iNumModels;
  eError = SQ_GetNumberOfModels(hProject, &iNumModels);
  std::cout<<"It contains "<<iNumModels<<" models."<<std::endl;

  // Find out and print the number of datasets in the project
  int iNumDatasets;  
  eError = SQ_GetNumberOfDatasets(hProject, &iNumDatasets);
  std::cout<<"and "<<iNumDatasets<<" datasets."<<std::endl;

  // If we would have made changes to the project and wished to save them:
  // eError = SQ_Save(hProject);

  // Close the project
  eError = SQ_CloseProject(&hProject);
  hProject = NULL;

  return 0;
}
