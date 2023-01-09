#include <iostream>
#include <cassert>
#include <vector>
#include <map>
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

  ////////////////////////////////////////////////////////////////////////
  //////////// VARIABLE-POSITION DICTIONARY
  ////////////////////////////////////////////////////////////////////////

  std::map<std::string, int> DataLookup;

  SQ_Dataset hDataset = NULL;
  SQ_GetDataset(hProject, 1, &hDataset);

  SQ_VariableVector hVariables = NULL;
  SQ_GetDataSetVariableNames(hDataset, &hVariables);

  int NumVar;
  SQ_GetNumVariablesInVector(hVariables, &NumVar);

  char szName[100];
  for (int iVar = 1; iVar <= NumVar; ++iVar){
    SQ_Variable hVariable = NULL;
    SQ_GetVariableFromVector(hVariables, iVar, &hVariable);
    SQ_GetVariableName(hVariable, 1, szName, sizeof(szName));
    DataLookup[szName] = iVar;
  }

  ////////////////////////////////////////////////////////////////////////
  //////////// VARIABLE-POSITION DICTIONARY
  ////////////////////////////////////////////////////////////////////////

  SQ_PreparePrediction hPreparePrediction = NULL;
  SQ_GetPreparePrediction(hModel, &hPreparePrediction);
  
  SQ_VariableVector hVariableVector = NULL;
  SQ_GetVariablesForPrediction(hPreparePrediction, &hVariableVector);

  int numPredSetVariables;
  SQ_GetVariablesForPrediction(hPreparePrediction, &hVariableVector);

  //std::vector<float> fQuantitativeData = CreateFakeData();

  
  ////////////////////////////////////////////////////////////////////////
  //////////// CLOSE THE MODEL AND PROJECT
  ////////////////////////////////////////////////////////////////////////

  // Close the project
  eError = SQ_CloseProject(&hProject);
  hProject = NULL;

  return 0;
}
