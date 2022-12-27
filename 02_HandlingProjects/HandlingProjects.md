# Handling SIMCA projects

A crucial step in almost any SIMCA-Q application will consist in handling SIMCA projects. Examples include opening and saving the project, accessing datasets or models within the project, etc. For this purpose, the C interface of SIMCA-Q offers a C structure to handle SIMCA projects/files as well as several functions, defined in *SQProject.h*, to retrieve information on the project as well as other SIMCA-Q structure references like e.g., SQ_Dataset, SQ_Models, etc.

Explicetely, we would initiate such a structure pointer by:
```
SQ_Project hProject = NULL;
```

To open the project we would use the *SQ_OpenProject()* that receives as input parameters the name of the SIMCA project, its password (if any, otherwise pass NULL, and a reference to the project pointer e.g.:
```
const char * szUSPFile = argv[1];
const char * szPassword = NULL;
SQ_OpenProject(szUSPFile, szPassword, &hProject);
```

It is then straightforward to retrieve some information about the project. To retrieve the project name we can use the *SQ_GetProjectName()* that takes as input parameters the project pointer, a pointer to an array and the size of that array e.g.:
```
char projectName [256];
SQ_GetProjectName(hProject, projectName, sizeof(projectName));
```

Below you can find an [example](HandlingProjects.cpp) where all this commands are combined into a script that accepts as an input parameter the relative path to a SIMCA file and prints the name of the project/file as well as its number of models and datasets:
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
  
  // Initiate a reference to a structure for handling SIMCA projects
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
  eError = SQ_GetProjectName (hProject, projectName, sizeof(projectName));
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
```