# Handling models: Retrieving the scores

- <a name="Summary">Summary of Fit Parameters</a>
- <a name="Scores">Scores</a>

## [Summary of Fit Parameters](#Summary)

...

## [Scores](#Scores)

*SQ_Model* handles allow retrieving the scores for the model worksheet.

Given that a [model has been loaded onto a *SQ_Model* handle](../05_0_HandlingModels_Introduction/HandlingModels_Introduction.md), let's name it *hModel* as in the previous example, we can retrieve a pointer to a *tagSQ_VectorData* structure, that we can use to handle the model scores, by using the function *SQ_GetT()*:
```
SQ_VectorData scoresVectorData=NULL;
SQ_GetT(m_hModel,NULL,&scoresVectorData);
```

Here we used NULL t get al scroes from the model. If we would like to select specific scores we would need to use a *SQ_IntVector* handle.

From the *SQ_VectorData* handle we can retrieve a handle for the matrix that contains the score values themselves. This new handle will be a pointer to the *tagSQ_FloatMatrix* structure and we can  get it by:
```
SQ_FloatMatrix scoresDatamatrix=NULL;
SQ_GetDataMatrix(scoresVectorData, &scoresDatamatrix);
```

Now we are ready to retrieve the scores. We can do it by using the function *SQ_GetDataFromFloatMatrix()*, which receives as input parameters the SQ_FloatMatrix handle, the observation index (row in SIMCA-Q), the score index (column in SIMCA-Q) and the address of the float value that would be retrieved. For example, to retrieve the score of the second component for the third observation we would call:
```
int iObs = 3;
int iScore = 2;
float value;
SQ_GetDataFromFloatMatrix(scoresDatamatrix, iObs, iScore, &value);
```

We can also get the number of rows and columns of the matrix using the *SQ_GetNumRowsInFloatMatrix()* and *SQ_GetNumColumnsInFloatMatrix()* functions. Using them can retrieve all scores for all observations and e.g. print them:
```
int nObs, nScores;
float value;
SQ_GetNumRowsInFloatMatrix(scoresDatamatrix, &nObs);
SQ_GetNumColumnsInFloatMatrix(scoresDatamatrix, &nScores);

for(int iObs=1;iObs<=nObs;iObs++){
  for(int iScore=1;iScore<=nScores;iScore++){
    SQ_GetDataFromFloatMatrix(scoresDatamatrix, iObs, iScore, &value);
    std::cout<<value;
  }
  std::cout<<"\n";
}
```

Below you can find an [example](HandlingModels_GettingScores.cpp) where all this is combined into a stand alone console script:
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

  // Load the handles for the model scores
  SQ_VectorData scoresVectorData=NULL;
  SQ_GetT(hModel,NULL,&scoresVectorData);
  SQ_FloatMatrix scoresDatamatrix=NULL;
  SQ_GetDataMatrix(scoresVectorData, &scoresDatamatrix);

  // Print the model scores
  int nObs, nScores;
  SQ_GetNumRowsInFloatMatrix(scoresDatamatrix, &nObs);
  SQ_GetNumColumnsInFloatMatrix(scoresDatamatrix, &nScores);
  float value;
  for(int iObs=1;iObs<=nObs;iObs++){
    for(int iScore=1;iScore<=nScores;iScore++){
      SQ_GetDataFromFloatMatrix(scoresDatamatrix, iObs, iScore, &value);
      std::cout<<value<<"\t";
    }
    std::cout<<"\n";
  }


  // Close the project
  eError = SQ_CloseProject(&hProject);
  hProject = NULL;

  return 0;
}
```
  