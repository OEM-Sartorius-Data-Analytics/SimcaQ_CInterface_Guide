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