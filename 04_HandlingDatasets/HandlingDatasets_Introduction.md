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

[example](HandlingDatasets_Introduction.cpp)