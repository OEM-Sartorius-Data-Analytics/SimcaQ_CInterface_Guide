# Handling SIMCA projects

A crucial step in almost any SIMCA-Q application will consist in handling SIMCA projects. Examples include opening and saving the project, accessing datasets or models within the project, etc. For this purpose, the C interface of SIMCA-Q offers a C structure to handle SIMCA projects/files, *tagSQ_ProjectHandle*, as well as a typedef-name for pointers to this structure, *SQ_Project*. SIMCA-Q also offers several functions, defined in *SQProject.h*, to retrieve information on the project as well as other SIMCA-Q structures for handling datasets, models, etc.

Explicetely, we would initiate such a pointer to the *tagSQ_ProjectHandle* structure by:
```
SQ_Project hProject = NULL;
```

To open the project we would use the *SQ_OpenProject()* that receives as input parameters the name of the SIMCA project, its password (if any, otherwise pass NULL), and the address of the *SQ_Project* pointer e.g.:
```
const char * szUSPFile = argv[1];
const char * szPassword = NULL;
SQ_OpenProject(szUSPFile, szPassword, &hProject);
```

It is then straightforward to retrieve some information about the project. To retrieve the project name we can use the *SQ_GetProjectName()* that takes as input parameters the SQ_Project pointer, a pointer to a char array and the size of that char array e.g.:
```
char projectName [256];
SQ_GetProjectName(hProject, projectName, sizeof(projectName));
```

To find out the number of models in the SIMCA project we can use the *SQ_GetNumberOfModels()* that takes as input parameters the SQ_Project pointer and the address of an interger accounting for the number of models e.g.:
```
int iNumModels;
SQ_GetNumberOfModels(hProject, &iNumModels);
```

To find out the numbe of datasetss in the SIMCA project we can use the *SQ_GetNumberOfDatasets()* that takes as input parameters the SQ_Project pointer and the address of an interger accounting for the number of datasets e.g.:
```
int iNumDatasets;  
SQ_GetNumberOfDatasets(hProject, &iNumDatasets);
```

In case you would have made changes to the project (not yet in this example) you could use the *SQ_Save()* function that takes as an input parameter the SQ_Project pointer:
```
SQ_Save(hProject);
```

When you are done you should close the project and assign the SQ_Project pointer to NULL:
```
SQ_CloseProject(&hProject);
hProject = NULL;
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
```