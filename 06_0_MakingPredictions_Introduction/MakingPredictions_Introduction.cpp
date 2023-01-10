#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "SIMCAQP.h"

void ReadInputFile(std::string fileName, std::vector<std::string>& inputVariables, std::vector<float>& fQuantitativeData)
{
  std::ifstream file;

  file.open(fileName);

  /*
  if(!file.is_open()){
    std::cout << "Could not open input file" << std::endl;
    return -1;
  }
  */

  std::string line, word;

  for(int i=1;i<=2;i++){
    if(i==1){
      std::getline(file, line);
      std::stringstream s(line);
      while (std::getline(s, word, ',')) {
	//std::cout<<word<<std::endl;
	inputVariables.push_back(word);
      }
    }
    else if(i==2){
      std::getline(file, line);
      std::stringstream s(line);
      while (std::getline(s, word, ',')) {
	//std::cout<<std::stof(word)<<std::endl;
	fQuantitativeData.push_back(std::stof(word));
      }
    }
  }
}

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
  //////////// ACCESSING PREDICTION VARIABLES
  ////////////////////////////////////////////////////////////////////////

  SQ_PreparePrediction hPreparePrediction = NULL;
  SQ_GetPreparePrediction(hModel, &hPreparePrediction);
  
  SQ_VariableVector hVariableVector = NULL;
  SQ_GetVariablesForPrediction(hPreparePrediction, &hVariableVector);

  int numPredSetVariables;
  SQ_GetNumVariablesInVector(hVariableVector, &numPredSetVariables);
  std::cout<<"Number of variables required for prediction: " << numPredSetVariables << std::endl;

  char szVariableName[100];
  std::vector<std::string> vVariableNames;
  SQ_Variable hVariable = NULL;
  for(int iVar=1;iVar<=numPredSetVariables;iVar++){
    SQ_GetVariableFromVector(hVariableVector, iVar, &hVariable);
    SQ_GetVariableName (hVariable, 1, szVariableName, sizeof(szVariableName));
    vVariableNames.push_back(szVariableName);
    //std::cout<<"Variable Name [" << iVar << "]: " << szVariableName << std::endl;
  }

  ////////////////////////////////////////////////////////////////////////
  //////////// VARIABLE-POSITION DICTIONARY
  ////////////////////////////////////////////////////////////////////////

  std::map<std::string, int> DataLookup;
  for(int iVar=1;iVar<=numPredSetVariables;iVar++){
    SQ_GetVariableFromVector(hVariableVector, iVar, &hVariable);
    SQ_GetVariableName (hVariable, 1, szVariableName, sizeof(szVariableName));
    DataLookup[szVariableName] = iVar;
  }

  ////////////////////////////////////////////////////////////////////////
  //////////// GET INPUT DATA FOR PREDICTION
  ////////////////////////////////////////////////////////////////////////

  std::vector<float> fQuantitativeData;
  std::vector<std::string> inputVariables;

  std::string fileName = "predictionDataFile.csv";
  ReadInputFile(fileName, inputVariables, fQuantitativeData);

  /*

  for(std::string s: inputVariables)
    std::cout << "s = " << s << std::endl;

  for(float f: fQuantitativeData)
    std::cout << "f = " << f << std::endl;

  */
  



  ////////////////////////////////////////////////////////////////////////
  //////////// POPULATING SQ_PREPAREPREDICTION WITH INPUT DATA
  ////////////////////////////////////////////////////////////////////////

  std::vector<int>::iterator it;

  //auto it = find(inputVariables.begin(), inputVariables.end(), 30);

  /*

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
    }*/

  

  for (auto const& [key, val] : DataLookup){
    std::cout << key        // string (key)
              << ':'  
              << val        // string's value
              << std::endl;
    auto res = find(inputVariables.begin(), inputVariables.end(), key);

    if(res!=inputVariables.end()){
      std::cout << "found" << std::endl;
      int position = res - inputVariables.begin();
      SQ_SetQuantitativeData(hPreparePrediction, 1, position, fQuantitativeData[position]);
    }
    }

  ////////////////////////////////////////////////////////////////////////
  //////////// GET THE PREDICTION
  ////////////////////////////////////////////////////////////////////////

  

  // Retrieve prrediction handle
  SQ_Prediction hPredictionHandle = NULL;
  SQ_GetPrediction(hPreparePrediction, &hPredictionHandle);

  SQ_VectorData hScoresHandle = NULL;
  SQ_GetTPS(hPredictionHandle, NULL, &hScoresHandle);

  /*

  int numberComponentsForPredcition = 1;
  SQ_IntVector m_vector = NULL;
  SQ_InitIntVector(&m_vector, iSize);
  SQ_SetDataInIntVector(m_vector, 1, 1);
  SQ_GetTPS(hPredictionHandle, &m_vector, &hScoresHandle);

  */

  SQ_FloatMatrix hScoresMatrix = NULL;
  SQ_GetDataMatrix(hScoresHandle, &hScoresMatrix);

  float fScoreValue;
  int iObs = 1;
  int iComp = 1;
  SQ_GetDataFromFloatMatrix(hScoresMatrix, iObs, iComp, &fScoreValue);

  std::cout << "score value: " << fScoreValue << std::endl;


  
  int numPredictiveScores = 1;
  //SQ_GetNumberOfPredictiveComponents(hModel, &numPredictiveScores);

  SQ_VectorData hPredictedYs = NULL;

  SQ_GetYPredPS(hPredictionHandle, numPredictiveScores, SQ_Unscaled_True, SQ_Backtransformed_True, NULL, &hPredictedYs);

  SQ_FloatMatrix hYMatrix = NULL;
  SQ_GetDataMatrix(hPredictedYs, &hYMatrix);

  float fYValue;
  //int iObs = 1;
  //int iComp = 1;
  SQ_GetDataFromFloatMatrix(hYMatrix, iObs, iComp, &fYValue);


  std::cout << "predicted y: " << fYValue << std::endl;

  


  ////////////////////////////////////////////////////////////////////////
  //////////// CLEAR HANDLES
  ////////////////////////////////////////////////////////////////////////

  SQ_ClearPreparePrediction(&hPreparePrediction);
  hPreparePrediction = NULL;

  ////////////////////////////////////////////////////////////////////////
  //////////// CLOSE THE MODEL AND PROJECT
  ////////////////////////////////////////////////////////////////////////

  // Close the project
  eError = SQ_CloseProject(&hProject);
  hProject = NULL;

  return 0;
}
