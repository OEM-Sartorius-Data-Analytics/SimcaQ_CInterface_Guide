# Handling models: Retrieving properties and parameters of models

*SQ_Model* handles allow retrieving multiple properties and parameters of the models included in SIMCA project/files. Here we will go through the following examples that will show how to work with:

- [Summary of Fit Parameters](#Summary).
- [Scores](#Scores).
- [Loadings](#Loadings).

For these examples we will assume that a [model has already been loaded onto a *SQ_Model* handle](../05_0_HandlingModels_Introduction/HandlingModels_Introduction.md), let's name it *hModel*.

## <a name="Summary">Summary of Fit Parameters</a>

*SQ_Model* handles can be used to e.g., retrieve parameters that summarize the quality of the fit like R2(cum) and Q2(cum).

To obtain Q2(cum) values, start by retrieving a pointer to a *tagSQ_VectorData* structure, which we can use to handle the Q2(cum) parameter, by using the function *SQ_GetQ2Cum()*:
```
SQ_VectorData pQ2Cum = NULL;
SQ_GetQ2Cum(hModel, &pQ2Cum);
```

We can use this handle to get the number and names of the components for which Q2(cum) was calculated. The first step in this direction is to retrieve a pointer to a *SQ_StringVector* structure to handle these names:
```
SQ_StringVector pQ2CumComponentNames = NULL;
SQ_GetRowNames(pQ2Cum, &pQ2CumComponentNames);
```

To find the number of components we will use the *SQ_GetRowNames()* function:
```
int nComponentsQ2;
SQ_GetNumStringsInVector(pQ2CumComponentNames, &nComponentsQ2);
```

and their names can be retrieved by using the *SQ_GetStringFromVector()* function. An example where these names would be stored in a vector of strings would be:
```
char szBuffer[256];
std::vector<std::string> vQ2ComponentNames;
for(int i=1;i<=nComponentsQ2;i++){
  SQ_GetStringFromVector(pQ2CumComponentNames, i, szBuffer, sizeof(szBuffer));
  vQ2ComponentNames.push_back(szBuffer);  
```

To access the actual *cumulative Q2* values we need first to retrieve a pointer ro a *tagSQ_FloatMatrix* structure:
```
SQ_FloatMatrix pQ2CumMatrix = NULL;
SQ_GetDataMatrix(pQ2Cum, &pQ2CumMatrix);
```

Then, we can access these values by using the *SQ_GetDataFromFloatMatrix()* function. An example where these values would be stored in a vector of floats would be:
```
float val;
std::vector<float> Q2CumDataValues;
for(int iComp=1;iComp<=nComponentsQ2;iComp++){
  SQ_GetDataFromFloatMatrix(pQ2CumMatrix, iComp, 1, &val);
  Q2CumDataValues.push_back(val);
}
```

It is a good practise in SIMCA-Q to clear the structure pointers once they are not needed:
```
SQ_ClearFloatMatrix(&pQ2CumMatrix);
SQ_ClearStringVector(&pQ2CumComponentNames);
SQ_ClearStringVector(&pQ2CumColumnNames);
SQ_ClearVectorData(&pQ2Cum);
```

We can use exactly the same approach to retrieve e.g. R2X(cum) component labels and values:
```
// Handle for R2X(cum)
SQ_VectorData pR2XCum = NULL;
SQ_GetR2XCum(hModel, &pR2XCum);

// Handle for the names of the components for which R2X(cum) was calculated
SQ_StringVector pR2XCumComponentNames = NULL;
SQ_GetRowNames(pR2XCum, &pR2XCumComponentNames);

// Find the number of components for which R2X(cum) was calculated
int nComponentsR2X;
SQ_GetNumStringsInVector(pR2XCumComponentNames, &nComponentsR2X);

// Find and the names of the components for which R2X(cum) was calculated
// Also populate a vector with these names
std::vector<std::string> vR2XComponentNames;
for(int i=1;i<=nComponentsR2X;i++){
  SQ_GetStringFromVector(pR2XCumComponentNames, i, szBuffer, sizeof(szBuffer));
  vR2XComponentNames.push_back(szBuffer);
}

// Find R2X(cum) values for all components for which this quantity was calculated
// Also populate a vector with these values
float valR2XCum;
std::vector<float> R2XCumDataValues;
for(int iComp=1;iComp<=nComponentsR2X;iComp++){
  SQ_GetDataFromFloatMatrix(pR2XCumMatrix, iComp, 1, &valR2XCum);
  R2XCumDataValues.push_back(valR2XCum);
}

// Clear structure pointers
SQ_ClearFloatMatrix(&pR2XCumMatrix);
SQ_ClearStringVector(&pR2XCumComponentNames);
SQ_ClearStringVector(&pR2XCumColumnNames);
SQ_ClearVectorData(&pR2XCum);
```

## <a name="Scores">Scores</a>

*SQ_Model* handles allow retrieving the scores for the observations used to train a model.

We can retrieve a pointer to a *tagSQ_VectorData* structure, which we can use to handle the model scores, by using the function *SQ_GetT()*:
```
SQ_VectorData scoresVectorData=NULL;
SQ_GetT(m_hModel,NULL,&scoresVectorData);
```

Here we used NULL to get al scores from the model. If we would like to select specific scores we would need to use a *SQ_IntVector* handle.

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

## <a name="Loadings">Loadings</a>

*SQ_Model* handles allow retrieving the loadings for the a model variables. The process is similar to that shown above for obtaining the scores of a model. However, in this case we will use the *SQ_GetP()*. This function receives as input parameters 1) the pointer to the structure encapsulating the model, 2) a pointer to a *tagSQ_IntVector* structure with the variable indices to retrieve (or NULL if want want to retrieve all of them), 3) a *SQ_TriStateReconstructState* SIMCA-Q enumeration indicating whether we are working with a wavelet spectral compressed project and 4) the address of a pointer to a *tagSQ_VectorData* structure that we will use to handle the loading. For example, to retrieve such a pointer for all loadings:
```
SQ_VectorData hLoadingsVectorData = NULL;
SQ_GetP(hModel, NULL, SQ_Reconstruct_False, &hLoadingsVectorData);
```

We can get a handle for the loadings that were calculated by creating a pointer to a *tagSQ_StringVector* structure:
```
SQ_StringVector hVariablesLoadingsVectorData;
SQ_GetRowNames(hLoadingsVectorData, &hVariablesLoadingsVectorData);
```

We can use these pointer to find the number of loadings that were calculated:
```
int numLoadings;
SQ_GetNumStringsInVector(hVariablesLoadingsVectorData, &numLoadings);
```

as well as the actual loading names. Below is an example where a string vector is populated with these names:
```
std::vector<std::string> vLoadingNames;
for(int iLoading=1; iLoading<=numLoadings ; iLoading++){
  SQ_GetStringFromVector(hVariablesLoadingsVectorData, iLoading, szBuffer, sizeof(szBuffer));
  vLoadingNames.push_back(szBuffer);
}  
```



## <a name="ExampleScript">Example Script</a>

In this [link](HandlingModels_GettingScores.cpp) you can find an example where all this is combined into a stand alone console script.
