#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "SIMCAQP.h"

void ReadInputFile(std::string fileName, std::vector<std::string>& inputVariables, std::vector<float>& fQuantitativeData)
{
  std::ifstream file;

  file.open(fileName);

  std::string line, word;

  int j = 0;
  int k = 0;

  for(int iRow=1;iRow<=2;iRow++){
    if(iRow==1){
      std::getline(file, line);
      std::stringstream s(line);
      while (std::getline(s, word, ',')) {
	inputVariables.push_back(word);
      }
    }
    else if(iRow==2){
      std::getline(file, line);
      std::stringstream s(line);
      while (std::getline(s, word, ',')) {
	fQuantitativeData.push_back(std::stof(word));
      }
    }
  }
}

void PrintToFile(std::string fileName, float yValue)
{
  std::ofstream file (fileName);
  if (file.is_open())
  {
    file << yValue << "\n";
    file.close();
  }
}


int main(int argc,char* argv[])
{
  if(argc!=5)
    {
      std::cout<<"\nYou need to pass 1) a SIMCA file, 2) a model name, 3) the name of an iput file and 4) the name of an output file as arguments\n";
      return -1;
    }
  
  SQ_ErrorCode eError; // handler for SIMCA-Q errors
  char szError[256]; // C-string for handling SIMCA-Q error descriptions

  char szBuffer[256]; // general C-string handle

  ////////////////////////////////////////////////////////////////////////
  //////////// LOADING PROJECT
  ////////////////////////////////////////////////////////////////////////

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
  
  ////////////////////////////////////////////////////////////////////////
  //////////// LOADING MODEL
  ////////////////////////////////////////////////////////////////////////

  // Get number of models
  int numModels;
  int modelNumber;
  SQ_Model hModel = NULL;

  SQ_GetNumberOfModels(hProject, &numModels);

  for(int iModelIndex=1;iModelIndex<=numModels;iModelIndex++){
    hModel = NULL;
    eError = SQ_GetModelNumberFromIndex(hProject, iModelIndex, &modelNumber);
    eError = SQ_GetModel(hProject, modelNumber, &hModel);
    SQ_GetModelName(hModel, szBuffer, sizeof(szBuffer));
    if(strcmp(szBuffer,argv[2])==0){
      break;
    }
  }

  SQ_Bool bIsFitted;
  if (SQ_IsModelFitted(hModel, &bIsFitted) != SQ_E_OK || bIsFitted != SQ_True)
    return -1;

  
  ////////////////////////////////////////////////////////////////////////
  //////////// ACCESSING PREDICTION VARIABLES
  ////////////////////////////////////////////////////////////////////////

  SQ_PreparePrediction hPreparePrediction = NULL;
  SQ_GetPreparePrediction(hModel, &hPreparePrediction);
  
  SQ_VariableVector hPredictionVariables = NULL;
  SQ_GetVariablesForPrediction(hPreparePrediction, &hPredictionVariables);

  int numPredSetVariables;
  SQ_GetNumVariablesInVector(hPredictionVariables, &numPredSetVariables);
  
  char szVariableName[100];
  std::vector<std::string> vVariableNames;
  SQ_Variable hVariable = NULL;
  for(int iVar=1;iVar<=numPredSetVariables;iVar++){
    SQ_GetVariableFromVector(hPredictionVariables, iVar, &hVariable);
    SQ_GetVariableName (hVariable, 1, szVariableName, sizeof(szVariableName));
    vVariableNames.push_back(szVariableName);
  }

  ////////////////////////////////////////////////////////////////////////
  //////////// VARIABLE-POSITION DICTIONARY
  ////////////////////////////////////////////////////////////////////////

  std::map<std::string, int> DataLookup;
  for(int iVar=1;iVar<=numPredSetVariables;iVar++){
    SQ_GetVariableFromVector(hPredictionVariables, iVar, &hVariable);
    SQ_GetVariableName (hVariable, 1, szVariableName, sizeof(szVariableName));
    DataLookup[szVariableName] = iVar;
  }

  ////////////////////////////////////////////////////////////////////////
  //////////// GET INPUT DATA FOR PREDICTION
  ////////////////////////////////////////////////////////////////////////

  std::vector<float> fQuantitativeData;
  std::vector<std::string> inputVariables;

  std::string fileName2 = argv[3];
  ReadInputFile(fileName2, inputVariables, fQuantitativeData);


  ////////////////////////////////////////////////////////////////////////
  //////////// POPULATING SQ_PREPAREPREDICTION WITH INPUT DATA
  ////////////////////////////////////////////////////////////////////////

  for (auto const& [key, val] : DataLookup){
    auto res = std::find(inputVariables.begin(), inputVariables.end(), key);
    if(res!=inputVariables.end()){
      int position = res - inputVariables.begin();
      SQ_SetQuantitativeData(hPreparePrediction, 1, val, fQuantitativeData[position]);
    }
  }
  
  ////////////////////////////////////////////////////////////////////////
  //////////// GET THE PREDICTION
  ////////////////////////////////////////////////////////////////////////  

  // Retrieve prediction handle
  SQ_Prediction hPredictionHandle = NULL;
  SQ_GetPrediction(hPreparePrediction, &hPredictionHandle);

  float fYValue;
  int iObs = 1;
  int iComp = 1;

  int numPredictiveScores;
  SQ_GetNumberOfPredictiveComponents(hModel, &numPredictiveScores);

  SQ_VectorData hPredictedYs = NULL;

  SQ_GetYPredPS(hPredictionHandle, numPredictiveScores, SQ_Unscaled_True, SQ_Backtransformed_True, NULL, &hPredictedYs);

  SQ_FloatMatrix hYMatrix = NULL;
  SQ_GetDataMatrix(hPredictedYs, &hYMatrix);

  SQ_GetDataFromFloatMatrix(hYMatrix, iObs, iComp, &fYValue);


  std::cout << "------------------------" << std::endl;
  std::cout << "predicted y: " << fYValue << std::endl;

  std::string fileName = argv[4]; // output file
  PrintToFile(fileName, fYValue);

  ////////////////////////////////////////////////////////////////////////
  //////////// CLEAR HANDLES
  ////////////////////////////////////////////////////////////////////////

  SQ_ClearPreparePrediction(&hPreparePrediction);
  hPreparePrediction = NULL;
  SQ_ClearPrediction (&hPredictionHandle);
  hPredictionHandle = NULL;
  SQ_ClearVectorData (&hPredictedYs);
  hPredictedYs = NULL;
  SQ_ClearFloatMatrix(&hYMatrix);
  hYMatrix = NULL;

  ////////////////////////////////////////////////////////////////////////
  //////////// CLOSE THE MODEL AND PROJECT
  ////////////////////////////////////////////////////////////////////////

  // Close the project
  eError = SQ_CloseProject(&hProject);
  hProject = NULL;

  return 0;
}
