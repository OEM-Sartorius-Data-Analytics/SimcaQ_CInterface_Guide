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
  int nComponents;
  SQ_GetNumStringsInVector(pQ2CumComponentNames, &nComponents);
  std::cout<<"Number of componentss: "<<nComponents<<std::endl;

  // Find and print the names of the components for which Q2(cum) was calculated
  for(int i=1;i<=nComponents;i++){
    SQ_GetStringFromVector(pQ2CumComponentNames, i, szBuffer, sizeof(szBuffer));
    std::cout<<"Component "<<i<<": "<<szBuffer<<std::endl;
  }
  
  // Handle for the calculated quantity (trivial, only Q2(cum))
  SQ_StringVector pQ2CumColumnNames = NULL;
  SQ_GetColumnNames(pQ2Cum, &pQ2CumColumnNames);


  // Find the number of calculated quantities. Trivial, only 1, Q2(cum)
  int nColumns;
  SQ_GetNumStringsInVector(pQ2CumColumnNames, &nColumns);
  std::cout<<"Number of columns: "<<nColumns<<std::endl;

  // Find and print the names of the calculated quantities.
  // It will be <name_of_model>.Q2(cum)
  for(int i=1;i<=nColumns;i++){
    SQ_GetStringFromVector(pQ2CumColumnNames, i, szBuffer, sizeof(szBuffer));
    std::cout<<"Column "<<i<<": "<<szBuffer<<std::endl;
  }
  
  // Handle for Q2(cum) values
  SQ_FloatMatrix pQ2CumMatrix = NULL;
  SQ_GetDataMatrix(pQ2Cum, &pQ2CumMatrix);

  float val;
  for(int iComp=1;iComp<=nComponents;iComp++){
    SQ_GetDataFromFloatMatrix(pQ2CumMatrix, iComp, 1, &val);
    std::cout<<"Q2(cum) for Component "<<iComp<<": "<<val<<std::endl;
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
  for(int i=1;i<=nComponentsR2X;i++){
    SQ_GetStringFromVector(pR2XCumComponentNames, i, szBuffer, sizeof(szBuffer));
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

  float valR2X;
  for(int iComp=1;iComp<=nComponentsR2X;iComp++){
    SQ_GetDataFromFloatMatrix(pR2XCumMatrix, iComp, 1, &valR2X);
    std::cout<<"R2X(cum) for Component "<<iComp<<": "<<valR2X<<std::endl;
  }



  // Clear structures
  SQ_ClearFloatMatrix(&pQ2CumMatrix);
  SQ_ClearStringVector(&pQ2CumComponentNames);
  SQ_ClearStringVector(&pQ2CumColumnNames);
  SQ_ClearVectorData(&pQ2Cum);

  SQ_ClearFloatMatrix(&pR2XCumMatrix);
  SQ_ClearStringVector(&pR2XCumComponentNames);
  SQ_ClearStringVector(&pR2XCumColumnNames);
  SQ_ClearVectorData(&pR2XCum);
  

  ////////////////////////////////////////////////////////////////////////
  //////////// CLOSE THE MODEL AND PROJECT
  ////////////////////////////////////////////////////////////////////////

  // Close the model and project
  eError = SQ_CloseProject(&hProject);
  hProject = NULL;

  return 0;
}
