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
  
  // Initialization of the tagSQ_Dataset pointer
  SQ_Dataset hDataset = NULL;

  // Retrieval of the handle for the dataset with index = 1
  int iDatasetIndex = 1;
  int iDatasetNumber;
  SQ_GetDatasetNumberFromIndex(hProject, iDatasetIndex, &iDatasetNumber);
  SQ_GetDataset (hProject, iDatasetNumber, &hDataset);

  // // C-string for handling output strings
  char szBuffer[256];

  // Retrieve and print the name of the dataset
  SQ_GetDataSetName(hDataset, szBuffer, sizeof(szBuffer));
  std::cout<<"The dataset name is: "<< szBuffer <<","<<std::endl;

  // Retrieve and print the number of observation IDs in the dataset
  int nrObsIDs;
  SQ_GetNumberOfObservationIDs(hDataset, &nrObsIDs);
  std::cout<<"Number of Observation IDs: "<< nrObsIDs <<std::endl;

  // Retrieve and print all names of observation IDs in the dataset
  std::cout<<"DataSet Observation ID Names: ";
  for(int iObsID = 1; iObsID<= nrObsIDs; iObsID++){
    SQ_GetDataSetObservationIDName(hDataset, iObsID, szBuffer, sizeof(szBuffer));
    std::cout<< szBuffer <<", ";
  }
  std::cout<<"\n";

  // For the observation with ID = 1, retrieve a pointer to a tagSQ_StringVector structure
  int iObsID = 1;
  SQ_StringVector pObservationNames = NULL;
  SQ_GetDataSetObservationNames(hDataset, iObsID, &pObservationNames);  

  // use this pointer to retrieve (and print) the observation ID value for the first
  // observation of the dataset
  int iObs = 1;
  SQ_GetStringFromVector(pObservationNames, iObs, szBuffer, sizeof(szBuffer));
  std::cout<<"Observation Name(2): "<< szBuffer <<","<<std::endl;

  // this pointer can also be used to retrieve the number of observations
  int nrObs;
  SQ_GetNumStringsInVector (pObservationNames, &nrObs);
  std::cout<<"Number of Observations in the dataset: "<< nrObs << std::endl;
  
  // Retrieve and print the number of variable IDs
  int nrVarIDs;
  SQ_GetNumberOfVariableIDs(hDataset, &nrVarIDs);
  std::cout<<"Number of Variable IDs: "<< nrVarIDs <<std::endl;

  // Retrieve a pointer to a tagSQ_VariableVector structure for handling
  // the variable names
  SQ_VariableVector pVariableVector = NULL;
  SQ_GetDataSetVariableNames(hDataset, &pVariableVector);

  // Retrieve and print the number of variables in the dataset
  int variableVectorSize;
  SQ_GetNumVariablesInVector(pVariableVector, &variableVectorSize);
  std::cout<<"Number of variables in the dataset: "<< variableVectorSize <<std::endl;

  // Use this pointer to retrieve and print the name of the first variable (ID also equal to 1)
  int variableIndex=1;
  SQ_Variable pVariable;
  SQ_GetVariableFromVector(pVariableVector, variableIndex, &pVariable);
  int variableNameID = 1;
  SQ_GetVariableName (pVariable, variableNameID, szBuffer, sizeof(szBuffer));
  std::cout<<"Variable #4 name: "<< szBuffer <<std::endl;

  // Retrieve a pointer to a tagSQ_VectorData structure for handling the actual observation
  // values in the dataset
  SQ_VectorData pVectorData;
  // Using NULL instead of a SQ_IntVector in order to retrieve all observations
  SQ_GetDataSetObservations(hDataset, NULL, &pVectorData);

  ////////////////////////
  // Use the SQ_VectorData pVectorData handle to retrieve a pointer to a tagSQ_StringVector
  // structure for handling the name of the columns/observations in the dataset
  SQ_StringVector pColumnNames;
  SQ_GetColumnNames(pVectorData, &pColumnNames);

  // use the obtained SQ_StringVector handle to find the number of columns/observations
  // in the dataset
  int nrColumns;
  SQ_GetNumStringsInVector(pColumnNames, &nrColumns);
  std::cout<<"Number of Columns in the SQ_GetDataSetObservations: "<< nrColumns << std::endl;

  // print the name of all columns/observations
  for(int iCol=1; iCol<=nrColumns; iCol++){
    SQ_GetStringFromVector(pColumnNames, iCol, szBuffer, sizeof(szBuffer));
    //std::cout<<"Column/Observation Name("<<iCol<<"): "<< szBuffer <<","<<std::endl;
  }

  ////////////////////////////////
  // Use the SQ_VectorData pVectorData handle to retrieve a pointer to a tagSQ_StringVector
  // structure for handling the name of the rows/variables in the dataset
  SQ_StringVector pRowNames;
  SQ_GetRowNames (pVectorData, &pRowNames);

  // use the obtained SQ_StringVector handle to find the number of rows/variables
  // in the dataset
  int nrRows;
  SQ_GetNumStringsInVector(pRowNames, &nrRows);
  std::cout<<"Number of Rows in the SQ_GetDataSetObservations: "<< nrRows << std::endl;

  // print the name of all row/variables
  for(int iRow=1; iRow<=nrRows; iRow++){
    SQ_GetStringFromVector(pRowNames, iRow, szBuffer, sizeof(szBuffer));
    //std::cout<<"Row/Variable Name("<<iRow<<"): "<< szBuffer <<","<<std::endl;
  }

  // Retrieve the value for observation =1 and variable = 1
  SQ_FloatMatrix pMatrix;
  SQ_GetDataMatrix(pVectorData, &pMatrix);
  int var=1;
  int obs=1;
  float val;
  SQ_GetDataFromFloatMatrix (pMatrix, var, obs, &val);
  std::cout<<"Val: "<< val <<","<<std::endl;

  // Close the project
  eError = SQ_CloseProject(&hProject);
  hProject = NULL;
  return 0;
}
