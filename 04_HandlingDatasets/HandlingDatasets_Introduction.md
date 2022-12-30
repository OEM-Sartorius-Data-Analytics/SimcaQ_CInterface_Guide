# Handling datasets

The C interface of SIMCA-Q offers a handler for SIMCA datasets, the structure *tagSQ_Dataset*, as well as a typedef-name for pointers to this structure, *SQ_Dataset*. SIMCA-Q also offers several functions, defined in *SQDataset.h*, that can be used to retrieve information on the datasets included in SIMCA projects.

If we have previously [loaded a SIMCA project into a SQ_Project pointer](../02_HandlingProjects/HandlingProjects.md), let's name it hProject, we can initialize a pointer to a tagSQ_ModelHandle structure, let's name it hDataset, by:
```
SQ_Dataset hDataset = NULL;
```

Similarly to the way that SIMCA-Q handles models, in order to load a dataset we need to know a number the dataset is associate with. However, it might be not straightforward to know this number. Nevertheless, one can get it from the dataset index. All datasets withon a SIMCA project/file are associated to indices starting from 1 in the same order as they appear in the SIMCA project/file. Provided that we know the index of the dataset of interest, we can get the associated dataset number by calling the function *SQ_GetDatasetNumberFromIndex()* which receives as input parameters the SQ_Project pointer, the model index and the address of the model number that we want to retrieve. For instance, provided that we have already [loaded a SIMCA project onto a *SQ_Project* handle](../02_HandlingProjects/HandlingProjects.md), let's name it *hProject*,we can retrieve the number of the dataset with index equal to 1 by:
```
int iDatasetIndex = 1;
int iDatasetNumber;
SQ_GetDatasetNumberFromIndex(hProject, iDatasetIndex, &iDatasetNumber);
```

Once we know the number of the dataset of interest, we can retrieve the corresponding dataset model handler by:
```
SQ_GetDataset (hProject, iDatasetNumber, &hDataset);
```

Once the dataset is loaded onto the SQ_Dataset handle, we can use the methods declared in *SQDataset.h* to access all the information contained in the dataset.

We can retrieve the name of the dataset by:
```
char szBuffer[256];
SQ_GetDataSetName(hDataset, szBuffer, sizeof(szBuffer));
```

The number of Observation IDs can be retrieved by:
```
int nrObsIDs;
SQ_GetNumberOfObservationIDs(hDataset, &nrObsIDs);
```

and the labels of these Observation IDs can be retrieved by:
```
char szBuffer[256];
for(int iObsID = 1; iObsID<= nrObsIDs; iObsID++){
  SQ_GetDataSetObservationIDName(hDataset, iObsID, szBuffer, sizeof(szBuffer));
} 
```

For a given observation ID, we can retrieve the values for all observations as follows. First, we need to declare which Observation ID we want to focus on. Then, for this Observation ID we retrieve a pointer to a tagSQ_StringVector structure. Then, we can retrieve the specific value of this Observation ID for a specific Observation number (previously declared) by using the function SQ_GetStringFromVector(). In order to retrieve the value of the first Observation ID for the first Observation:
```
int iObsID = 1;
SQ_StringVector pObservationNames = NULL;
SQ_GetDataSetObservationNames(hDataset, iObsID, &pObservationNames);

char szBuffer[256];
int iObs = 1;
SQ_GetStringFromVector(pObservationNames, iObs, szBuffer, sizeof(szBuffer));
```

By means of the function *SQ_GetNumStringsinVector* we can also use this pointer i.e., the SQ_StringVector pObservationNames to retrieve the total number of Observations:
```
int nrObs;
SQ_GetNumStringsInVector (pObservationNames, &nrObs);
```

The number of Variable IDs can be retrieved by:
```
int nrVarIDs;
SQ_GetNumberOfVariableIDs(hDataset, &nrVarIDs);
```

In order to work with the Variable labels we need first to retrieve a pointer to a *tagSQ_VariableVector* structure:
```
SQ_VariableVector pVariableVector = NULL;
SQ_GetDataSetVariableNames(hDataset, &pVariableVector);
```

This pointer allows to retrieve the number of Variables in the dataset:
```
int variableVectorSize;
SQ_GetNumVariablesInVector(pVariableVector, &variableVectorSize);
```

and also the name of each of the Variable labels. For instance, for the Variable label #1 (and also Variable ID #1):
```
int variableIndex=1;
SQ_Variable pVariable;
SQ_GetVariableFromVector(pVariableVector, variableIndex, &pVariable);
int variableNameID = 1;
SQ_GetVariableName (pVariable, variableNameID, szBuffer, sizeof(szBuffer));
```

For accessing the actual Observation values in the dataset, we need first to retrieve a pointer to a *tagSQ_VectorData* structure by using the function *SQ_GetDataSetObservations()*, which takes as input arguments the handle to the Dataset, then either *NULL* if we want to retrieve all observations or a pointer to a *tagSQ_IntVector* structure if we want just specific observations, and the address of the pointer to the *tagSQ_VectorData* structure. For example, if we want to retrieve all observations:
```
SQ_VectorData pVectorData;
SQ_GetDataSetObservations(hDataset, NULL, &pVectorData);
```

This structure does not only allow the Observation values, but also the labels of all Observations and Variables.

To retrieve the labels of the Observations we need to use the loaded SQ_VectorData pVectorData handle to retrieve a pointer to a *tagSQ_StringVector* structure for handling the name of the Observations (columns according to SIMCA-Q) in the dataset e.g.:
```
SQ_StringVector pColumnNames;
SQ_GetColumnNames(pVectorData, &pColumnNames);
```

We can then use the obtained SQ_StringVector handle to find the number of columns/observations in the dataset:
```
int nrColumns;
SQ_GetNumStringsInVector(pColumnNames, &nrColumns);
```

and also the label for a specific Observation. For instance, for Observation #1:
```
int iCol = 1;
SQ_GetStringFromVector(pColumnNames, iCol, szBuffer, sizeof(szBuffer));
```

We can follow the same proccess to obtain the Variable labels. First we retrieve a pointer to a *tagSQ_StringVector* structure for handling the name of the Variables (rows according to SIMCA-Q) in the dataset e.g.:
```
SQ_StringVector pRowNames;
SQ_GetRowNames (pVectorData, &pRowNames);
```

We can then use the obtained SQ_StringVector handle to find the number of rows/variables in the dataset:
```
int nrRows;
SQ_GetNumStringsInVector(pRowNames, &nrRows);
```

and also the label for a specific Variable. For instance, for Variable #1:
```
int iRow = 1;
SQ_GetStringFromVector(pRowNames, iRow, szBuffer, sizeof(szBuffer));
```

Finally, to retrieve the actual values of the Observations. For this, we need first to load the Observations data into a pointer to a *tagSQ_FloatMatrix* structure:
```
SQ_FloatMatrix pMatrix;
SQ_GetDataMatrix(pVectorData, &pMatrix);
```

Then, to retrieve the value for e.g. Variable 1 and Observation 1:
```
int var=1;
int obs=1;
float val;
SQ_GetDataFromFloatMatrix (pMatrix, var, obs, &val);
```

All this is combines in the [example](HandlingDatasets_Introduction.cpp) below, a console script that takes as an input parameter the name of a SIMCA project and prints to the screen info on the dataset with index 1:
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
```