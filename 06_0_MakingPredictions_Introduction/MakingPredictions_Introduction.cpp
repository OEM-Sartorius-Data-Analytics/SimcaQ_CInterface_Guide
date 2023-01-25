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

////////////////////////////////////////////////////////////////////////
////////////// FUNCTION FOR READING INPUT DATA
//////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////
////////////// MAIN FUNCTION
//////////////////////////////////////////////////////////////////////////

int main(int argc,char* argv[])
{
  // Check that all input parameters have been passed
  if(argc!=4)
    {
      std::cout<<"\nYou need to pass 1) a SIMCA file, 2) a model name and 3) the name of an input file\n";
      return -1;
    }
  
  SQ_ErrorCode eError; // handler for SIMCA-Q errors
  char szError[256]; // C-string for handling SIMCA-Q error descriptions
  char szBuffer[256]; // general C-string handle

  ////////////////////////////////////////////////////////////////////////
  //////////// LOAD PROJECT
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
  //////////// LOAD MODEL
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
  //////////// PREPARE PREDICTION
  ////////////////////////////////////////////////////////////////////////

  SQ_PreparePrediction hPreparePrediction = NULL;
  SQ_GetPreparePrediction(hModel, &hPreparePrediction);
  
  ////////////////////////////////////////////////////////////////////////
  //////////// RETRIEVE HANDLE FOR PREDICTION VARIABLES
  ////////////////////////////////////////////////////////////////////////

  SQ_VariableVector hPredictionVariables = NULL;
  SQ_GetVariablesForPrediction(hPreparePrediction, &hPredictionVariables);

  int numPredSetVariables;
  SQ_GetNumVariablesInVector(hPredictionVariables, &numPredSetVariables);
  
  /*
  // The following code block can be used to create a vector populated
  // with the names of the variables managed by the hPreparePrediction handle
  char szVariableName[100];
  std::vector<std::string> vVariableNames;
  SQ_Variable hVariable = NULL;
  for(int iVar=1;iVar<=numPredSetVariables;iVar++){
    SQ_GetVariableFromVector(hPredictionVariables, iVar, &hVariable);
    SQ_GetVariableName (hVariable, 1, szVariableName, sizeof(szVariableName));
    vVariableNames.push_back(szVariableName);
  }
  */

  ////////////////////////////////////////////////////////////////////////
  //////////// RETRIEVE VARIABLE-POSITION DICTIONARY
  ////////////////////////////////////////////////////////////////////////

  char szVariableName[100];
  std::map<std::string, int> DataLookup;
  SQ_Variable hVariable = NULL;
  for(int iVar=1;iVar<=numPredSetVariables;iVar++){
    SQ_GetVariableFromVector(hPredictionVariables, iVar, &hVariable);
    SQ_GetVariableName(hVariable, 1, szVariableName, sizeof(szVariableName));
    DataLookup[szVariableName] = iVar;
  }

  ////////////////////////////////////////////////////////////////////////
  //////////// GET INPUT DATA FOR PREDICTION
  ////////////////////////////////////////////////////////////////////////

  std::vector<float> fQuantitativeData;
  std::vector<std::string> inputVariables;
  std::string fileName = argv[3];
  ReadInputFile(fileName, inputVariables, fQuantitativeData);

  ////////////////////////////////////////////////////////////////////////
  //////////// POPULATE SQ_PREPAREPREDICTION WITH INPUT DATA
  ////////////////////////////////////////////////////////////////////////

  for (auto const& [key, val] : DataLookup){
    auto res = std::find(inputVariables.begin(), inputVariables.end(), key);
    if(res!=inputVariables.end()){
      int position = res - inputVariables.begin();
      SQ_SetQuantitativeData(hPreparePrediction, 1, val, fQuantitativeData[position]);
    }
  }
  
  ////////////////////////////////////////////////////////////////////////
  //////////// RETRIEVE PREDICTION HANDLE
  ////////////////////////////////////////////////////////////////////////  

  SQ_Prediction hPredictionHandle = NULL;
  SQ_GetPrediction(hPreparePrediction, &hPredictionHandle);

  ////////////////////////////////////////////////////////////////////////
  //////////// RETRIEVE SCORES FOR ALL PREDICTIVE COMPONENTS
  ////////////////////////////////////////////////////////////////////////  

  std::cout << "--------------------------------------" << std::endl;
  std::cout << "--------------------------------------" << std::endl;
  std::cout << "---Predicting Predictive Components---" << std::endl;

  SQ_VectorData hPredictedPredictiveComponents = NULL;
  SQ_GetTPS(hPredictionHandle, NULL, &hPredictedPredictiveComponents);

  // Retrieve observations
  SQ_StringVector hObservationNamesPredComp;
  SQ_GetRowNames(hPredictedPredictiveComponents, &hObservationNamesPredComp);
  int numObservationsPredComp;
  SQ_GetNumStringsInVector(hObservationNamesPredComp, &numObservationsPredComp);
  std::cout << "Number of observations (Predicting Predictive Components Code Section): " << numObservationsPredComp << std::endl;
  for(int i=1;i<=numObservationsPredComp;i++){
    SQ_GetStringFromVector(hObservationNamesPredComp, i, szBuffer, sizeof(szBuffer));
    std::cout << "Name of observation #" << i << " (Predicting Predictive Components Code Section): " << szBuffer << std::endl;
  }

  // Retrieve names of predictive components
  SQ_StringVector hPredictiveComponentNames;
  SQ_GetColumnNames(hPredictedPredictiveComponents, &hPredictiveComponentNames);
  int numPredictiveComponents;
  SQ_GetNumStringsInVector(hPredictiveComponentNames, &numPredictiveComponents);
  std::cout << "Number of predictive components (Predicting Predictive Components Code Section): " << numPredictiveComponents << std::endl;
  for(int i=1;i<=numPredictiveComponents;i++){
    SQ_GetStringFromVector(hPredictiveComponentNames, i, szBuffer, sizeof(szBuffer));
    std::cout << "Name of predictive component #" << i << " (Predicting Predictive Components Code Section): " << szBuffer << std::endl;
  }

  SQ_FloatMatrix hPredictedPredictiveComponentsDataMatrix = NULL;
  SQ_GetDataMatrix(hPredictedPredictiveComponents, &hPredictedPredictiveComponentsDataMatrix);

  // Print scores for predicted component
  float fScoreValue;
  for(int iObsPredComp=1; iObsPredComp<=numObservationsPredComp;iObsPredComp++){
    for(int iPredComp=1;iPredComp<=numPredictiveComponents;iPredComp++){
      SQ_GetDataFromFloatMatrix(hPredictedPredictiveComponentsDataMatrix, iObsPredComp, iPredComp, &fScoreValue);
      SQ_GetStringFromVector(hPredictiveComponentNames, iPredComp, szBuffer, sizeof(szBuffer));
      std::cout << szBuffer << " for observation #" << iObsPredComp << ": " << fScoreValue << std::endl;
    }
  }

  ////////////////////////////////////////////////////////////////////////
  //////////// RETRIEVE ALL PREDICTED Y QUANTITIES
  ////////////////////////////////////////////////////////////////////////  

  std::cout << "----------------------------" << std::endl;
  std::cout << "----------------------------" << std::endl;
  std::cout << "---Predicting Y Variables---" << std::endl;

  int numPredictiveScores;
  SQ_GetNumberOfPredictiveComponents(hModel, &numPredictiveScores);

  SQ_VectorData hPredictedYs = NULL;
  SQ_GetYPredPS(hPredictionHandle, numPredictiveScores, SQ_Unscaled_True, SQ_Backtransformed_True, NULL, &hPredictedYs);

  // Retrieve observations
  SQ_StringVector hObservationNames;
  SQ_GetRowNames(hPredictedYs, &hObservationNames);
  int numObservations;
  SQ_GetNumStringsInVector(hObservationNames, &numObservations);
  std::cout << "Number of observations (Predicting Y Values Code Section): " << numObservations << std::endl;
  for(int i=1;i<=numObservations;i++){
    SQ_GetStringFromVector(hObservationNames, i, szBuffer, sizeof(szBuffer));
    std::cout << "Name of observation #" << i << " (Predicting Y Values Code Section): " << szBuffer << std::endl;
  }

  // Retrieve names of Y variables
  SQ_StringVector hYVariableNames;
  SQ_GetColumnNames(hPredictedYs, &hYVariableNames);
  int numYVariables;
  SQ_GetNumStringsInVector(hYVariableNames, &numYVariables);
  std::cout << "Number of Y variables (Predicting Y Values Code Section): " << numYVariables << std::endl;
  for(int i=1;i<=numYVariables;i++){
    SQ_GetStringFromVector(hYVariableNames, i, szBuffer, sizeof(szBuffer));
    std::cout << "Name of Y variable #" << i << " (Predicting Y Values Code Section): " << szBuffer << std::endl;
  }

  SQ_FloatMatrix hPredictedYsMatrix = NULL;
  SQ_GetDataMatrix(hPredictedYs, &hPredictedYsMatrix);

  // Print predicted Y values
  float fYValue;
  for(int iObs=1; iObs<=numObservations;iObs++){
    for(int iYVar=1;iYVar<=numYVariables;iYVar++){
      SQ_GetDataFromFloatMatrix(hPredictedYsMatrix, iObs, iYVar, &fYValue);
      SQ_GetStringFromVector(hYVariableNames, iYVar, szBuffer, sizeof(szBuffer));
      std::cout << szBuffer << " for observation #" << iObs << ": " << fYValue << std::endl;
    }    
  }  

  ////////////////////////////////////////////////////////////////////////
  //////////// CLEAR HANDLES
  ////////////////////////////////////////////////////////////////////////

  SQ_ClearPreparePrediction(&hPreparePrediction);
  hPreparePrediction = NULL;
  SQ_ClearPrediction (&hPredictionHandle);
  hPredictionHandle = NULL;
  SQ_ClearVectorData (&hPredictedYs);
  hPredictedYs = NULL;
  SQ_ClearFloatMatrix(&hPredictedYsMatrix);
  hPredictedYsMatrix = NULL;
  SQ_ClearStringVector(&hObservationNamesPredComp);
  hObservationNamesPredComp = NULL;
  SQ_ClearStringVector(&hPredictiveComponentNames);
  hPredictiveComponentNames = NULL;
  SQ_ClearFloatMatrix(&hPredictedPredictiveComponentsDataMatrix);
  hPredictedPredictiveComponentsDataMatrix = NULL;


  ////////////////////////////////////////////////////////////////////////
  //////////// CLOSE MODEL AND PROJECT
  ////////////////////////////////////////////////////////////////////////

  eError = SQ_CloseProject(&hProject);
  hProject = NULL;

  return 0;
}
