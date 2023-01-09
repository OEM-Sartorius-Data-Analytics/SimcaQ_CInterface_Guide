# Making Predictions: Introduction

One of the main uses of SIMCA-Q is to make predictions from new data based on a SIMCA model.

To illustrate how predictions work in SIMCA-Q, we will start from a point where we will assume that handles for a SIMCA project and for the model within that project that will be used to make predictions, let's name them *hProject* and *hModel*, have already been created.

From this point, the first step to make a prediction consist in creating a pointer to a *tagSQ_PreparePrediction* structure that we will use for all preparatory steps of the prediction process:
```
SQ_PreparePrediction hPreparePrediction = NULL;
SQ_GetPreparePrediction(hModel, &hPreparePrediction);
```

You can now get the number of variables required for making predictions by:
```
int numPredSetVariables;
SQ_GetNumVariablesInVector(hVariableVector, &numPredSetVariables);
```

We can use the *SQ_GetVariableFromVector* to retrieve a handle for specific variables. For instance to get a handle for the first variable:
```
iVar = 1;
SQ_Variable hVariable = NULL;
SQ_GetVariableFromVector(hVariables, iVar, &hVariable);
```

This allows us to retrieve e.g., the name of the variable by using the *SQ_GetVariableName()* function:
```
char szVariableName[100];
SQ_GetVariableName(hVariable, 1, szVariableName, sizeof(szVariableName));
```

The next step will be to populate the SQ_PreparePrediction structure pointer with the data from which we will predict a desired quantity. Let's imagine that we have the input data in the form of a std::vector<float>, let's name it fQuantitativeData, with a size equal to that of the number of variables needed for prediction, containing the variable values for just one observation, also in the same order than that for the observations used to build the model. In this case, we could populate the *SQ_PreparePrediction* structure pointer by:

```
iObs = 1;
for (int iVar = 1; iVar <= numPredSetVariables; ++iVar){
  SQ_SetQuantitativeData(hPreparePrediction, iObs, iVar, fQuantitativeData[iVar-1]);
}
```

We could actually populate this handle with variable values from many predictions, we would just need to e.g., have the input vector in a 2D vector and iterate as well over the number of observations.

