#include <iostream>
#include <vector>
#include <string>
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
  char szBuffer[256]; // C-string for handling output strings

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
  //////////// SCORES
  ////////////////////////////////////////////////////////////////////////
  
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

  // Clear structures
  SQ_ClearFloatMatrix(&scoresDatamatrix);
  SQ_ClearVectorData(&scoresVectorData);

  ////////////////////////////////////////////////////////////////////////
  //////////// SUMMARY OF FIT PARAMETERS
  ////////////////////////////////////////////////////////////////////////

  // Handle for Q2(cum)
  SQ_VectorData pQ2Cum = NULL;
  SQ_GetQ2Cum(hModel, &pQ2Cum);

  // Handle for the names of the components for which Q2(cum) was calculated
  SQ_StringVector pQ2CumComponentNames = NULL;
  SQ_GetRowNames(pQ2Cum, &pQ2CumComponentNames);

  // Find the number of components for which Q2(cum) was calculated
  int nComponentsQ2;
  SQ_GetNumStringsInVector(pQ2CumComponentNames, &nComponentsQ2);
  std::cout<<"Number of Q2(cum) components: "<<nComponentsQ2<<std::endl;

  // Find and print the names of the components for which Q2(cum) was calculated
  std::vector<std::string> vQ2ComponentNames;
  for(int i=1;i<=nComponentsQ2;i++){
    SQ_GetStringFromVector(pQ2CumComponentNames, i, szBuffer, sizeof(szBuffer));
    vQ2ComponentNames.push_back(szBuffer);
    std::cout<<"Q2 Component "<<i<<": "<<szBuffer<<std::endl;
  }

  for(auto it = vQ2ComponentNames.begin(); it != vQ2ComponentNames.end(); it++){
    std::cout<< *it << std::endl;
  }

  // Handle for the calculated quantity (trivial, only Q2(cum))
  SQ_StringVector pQ2CumColumnNames = NULL;
  SQ_GetColumnNames(pQ2Cum, &pQ2CumColumnNames);


  // Find the number of calculated quantities. Trivial, only 1, Q2(cum)
  int nColumnsQ2;
  SQ_GetNumStringsInVector(pQ2CumColumnNames, &nColumnsQ2);
  std::cout<<"Number of Q2 columns: "<<nColumnsQ2<<std::endl;

  // Find and print the names of the calculated quantities.
  // It will be <name_of_model>.Q2(cum)
  for(int i=1;i<=nColumnsQ2;i++){
    SQ_GetStringFromVector(pQ2CumColumnNames, i, szBuffer, sizeof(szBuffer));
    std::cout<<"Q2 Column "<<i<<": "<<szBuffer<<std::endl;
  }
  
  // Handle for Q2(cum) values
  SQ_FloatMatrix pQ2CumMatrix = NULL;
  SQ_GetDataMatrix(pQ2Cum, &pQ2CumMatrix);

  // Find and print Q2(cum) values for all components for which this quantity was calculated
  // Also populate a vector with these values
  float valQ2Cum;
  std::vector<float> Q2CumDataValues;
  for(int iComp=1;iComp<=nComponentsQ2;iComp++){
    SQ_GetDataFromFloatMatrix(pQ2CumMatrix, iComp, 1, &valQ2Cum);
    Q2CumDataValues.push_back(valQ2Cum);
    std::cout<<"Q2(cum) for Component "<<iComp<<": "<<valQ2Cum<<std::endl;
  }

  for(auto it = Q2CumDataValues.begin(); it != Q2CumDataValues.end(); it++){
    std::cout<< *it << std::endl;
  }


  // Handle for R2X(cum)
  SQ_VectorData pR2XCum = NULL;
  SQ_GetR2XCum(hModel, &pR2XCum);

  // Handle for the names of the components for which R2X(cum) was calculated
  SQ_StringVector pR2XCumComponentNames = NULL;
  SQ_GetRowNames(pR2XCum, &pR2XCumComponentNames);

  // Find the number of components for which R2X(cum) was calculated
  int nComponentsR2X;
  SQ_GetNumStringsInVector(pR2XCumComponentNames, &nComponentsR2X);
  std::cout<<"R2X Number of components: "<<nComponentsR2X<<std::endl;

  // Find and print the names of the components for which R2X(cum) was calculated
  // Also populate a vector with these names
  std::vector<std::string> vR2XComponentNames;
  for(int i=1;i<=nComponentsR2X;i++){
    SQ_GetStringFromVector(pR2XCumComponentNames, i, szBuffer, sizeof(szBuffer));
    vR2XComponentNames.push_back(szBuffer);
    std::cout<<"R2X Component "<<i<<": "<<szBuffer<<std::endl;
  }
  
  // Handle for the calculated quantity (trivial, only R2X(cum))
  SQ_StringVector pR2XCumColumnNames = NULL;
  SQ_GetColumnNames(pR2XCum, &pR2XCumColumnNames);


  // Find the number of calculated quantities. Trivial, only 1, R2X(cum)
  int nColumnsR2X;
  SQ_GetNumStringsInVector(pR2XCumColumnNames, &nColumnsR2X);
  std::cout<<"R2X Number of columns: "<<nColumnsR2X<<std::endl;

  // Find and print the names of the calculated quantities.
  // It will be <name_of_model>.R2X(cum)
  for(int i=1;i<=nColumnsR2X;i++){
    SQ_GetStringFromVector(pR2XCumColumnNames, i, szBuffer, sizeof(szBuffer));
    std::cout<<"R2X Column "<<i<<": "<<szBuffer<<std::endl;
  }
  
  // Handle for R2X(cum) values
  SQ_FloatMatrix pR2XCumMatrix = NULL;
  SQ_GetDataMatrix(pR2XCum, &pR2XCumMatrix);

  // Find and print R2X(cum) values for all components for which this quantity was calculated
  // Also populate a vector with these values
  float valR2XCum;
  std::vector<float> R2XCumDataValues;
  for(int iComp=1;iComp<=nComponentsR2X;iComp++){
    SQ_GetDataFromFloatMatrix(pR2XCumMatrix, iComp, 1, &valR2XCum);
    R2XCumDataValues.push_back(valR2XCum);
    std::cout << "R2X(cum) for Component " << iComp << ": " << valR2XCum << std::endl;
  }



  // Clear structure pointers
  SQ_ClearFloatMatrix(&pQ2CumMatrix);
  SQ_ClearStringVector(&pQ2CumComponentNames);
  SQ_ClearStringVector(&pQ2CumColumnNames);
  SQ_ClearVectorData(&pQ2Cum);

  SQ_ClearFloatMatrix(&pR2XCumMatrix);
  SQ_ClearStringVector(&pR2XCumComponentNames);
  SQ_ClearStringVector(&pR2XCumColumnNames);
  SQ_ClearVectorData(&pR2XCum);
  

  ////////////////////////////////////////////////////////////////////////
  //////////// LOADINGS
  ////////////////////////////////////////////////////////////////////////

  // Handle for all model loadings
  SQ_VectorData hLoadingsVectorData = NULL;
  SQ_GetP(hModel, NULL, SQ_Reconstruct_False, &hLoadingsVectorData);

  // Handle for loadings names
  SQ_StringVector hVariablesLoadingsVectorData;
  SQ_GetRowNames(hLoadingsVectorData, &hVariablesLoadingsVectorData);

  int numLoadings;
  SQ_GetNumStringsInVector(hVariablesLoadingsVectorData, &numLoadings);
  std::cout<<"Num Loadings: "<< numLoadings << std::endl;

  // Find all loading names and populate a string vector with them
  std::vector<std::string> vLoadingNames;
  for(int iLoading=1; iLoading<=numLoadings ; iLoading++){
    SQ_GetStringFromVector(hVariablesLoadingsVectorData, iLoading, szBuffer, sizeof(szBuffer));
    vLoadingNames.push_back(szBuffer);
  }  

  // Print all loadings names
  for(std::vector<std::string>::size_type iLoading = 0; iLoading < vLoadingNames.size(); iLoading++){
    std::cout << "Loading(" << iLoading + 1 << "): " << vLoadingNames.at(iLoading) << std::endl;
  }
  
  // Handle for the names of components for the retrieved loadings
  SQ_StringVector hComponentsLoadingsVectorData;
  SQ_GetColumnNames(hLoadingsVectorData, &hComponentsLoadingsVectorData);

  // Find number of components for the retrieved loadings 
  int numComponentsLoadings;
  SQ_GetNumStringsInVector(hComponentsLoadingsVectorData, &numComponentsLoadings);
  std::cout<<"Num Components for which loadings have been retrieved: "<< numComponentsLoadings << std::endl;

  // Find component names for the retrieved loadings and populate a string vector with them
  std::vector<std::string> vComponentLoadingNames;
  for(int iComponentLoading=1; iComponentLoading<=numComponentsLoadings ; iComponentLoading++){
    SQ_GetStringFromVector(hComponentsLoadingsVectorData, iComponentLoading, szBuffer, sizeof(szBuffer));
    vComponentLoadingNames.push_back(szBuffer);
  }

  // Print all component names for which loadings were calculated
  for(std::vector<std::string>::size_type iComponentLoading = 0; iComponentLoading < vComponentLoadingNames.size(); iComponentLoading++){
    std::cout << "Component Loading(" << iComponentLoading + 1 << "): " << vComponentLoadingNames.at(iComponentLoading) << std::endl;
  }

  // Handle for the Loadings Datamatrix
  SQ_FloatMatrix hLoadingsDataMatrix = NULL;
  SQ_GetDataMatrix(hLoadingsVectorData, &hLoadingsDataMatrix);
  

  // Retrieve the loading value for specific variable and component
  int iVar = 47;
  int iComp =1;
  float pfVal;
  SQ_GetDataFromFloatMatrix(hLoadingsDataMatrix, iVar, iComp, &pfVal);
  std::cout<<"Val: "<< pfVal << std::endl;
  


  ////////////////////////////////////////////////////////////////////////
  //////////// CLOSE THE MODEL AND PROJECT
  ////////////////////////////////////////////////////////////////////////

  // Close the model and project
  eError = SQ_CloseProject(&hProject);
  hProject = NULL;

  return 0;
}
