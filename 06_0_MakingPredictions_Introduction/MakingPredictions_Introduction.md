# Making Predictions: Introduction

One of the main uses of SIMCA-Q is to make predictions from new data based on a SIMCA model.

In SIMCA-Q, predictions are inferred by retrieving a sequence of handles (pointers to structures). This typically consists in:

1. A handle for all the preparations needed before making the actual prediction.
2. A handle for the general predictions from a specific model.
3. A handle for the specific quantity to be predicted e.g. predictive components, Y variables, etc.
4. A handle for the matrix of predicted values. From this matirx handle it is then possible to retrieve actual predicted values.

To illustrate how predictions work in SIMCA-Q, we will start from a point where we will assume that handles for a SIMCA project and for the model within that project that will be used to make predictions, let's name them *hProject* and *hModel*, have already been created. From this point, we will go through the following steps:

- [Prepare Predictions](#prepare-predictions).
- [Make Predictions](#make-predictions).
    - [Predictive Components](#predictive-components).
    - [Y Variables](#y-variables).

## <a name="prepare-predictions">Prepare Predictions</a>

The first step to make a prediction consist in creating a pointer to a *tagSQ_PreparePrediction* structure that we will use for all preparatory steps of the prediction process:
```
SQ_PreparePrediction hPreparePrediction = NULL;
SQ_GetPreparePrediction(hModel, &hPreparePrediction);
```

Among the possibilities offered by *PreparePrediction*, one is to retrieve a handle for the variables that it manages, which is of the form of a pointer to a *tagSQ_VariableVector* structure, and that can be retrieved eg.g., by:
```
SQ_VariableVector hPredictionVariables = NULL;
SQ_GetVariablesForPrediction(hPreparePrediction, &hPredictionVariables);
```

We can get the total number of variables managed by this handle by:
```
int numPredictionVariables;
SQ_GetNumVariablesInVector(hPredictionVariables, &numPredictionVariables);
```

We can also use the function *SQ_GetVariableFromVector* to retrieve a handle for a specific variable. This handle will be of the form of a pointer to a *tagSQ_Variable* structure. For instance to get a handle for the first variable:
```
iVar = 1;
SQ_Variable hVariable = NULL;
SQ_GetVariableFromVector(hVariables, iVar, &hVariable);
```

We can use this handle to retrieve e.g., the name of the variable by using the *SQ_GetVariableName()* function:
```
char szVariableName[100];
SQ_GetVariableName(hVariable, 1, szVariableName, sizeof(szVariableName));
```

The next step will be to populate the *SQ_PreparePrediction* structure pointer with the data from which we will predict a desired quantity. Let's first focus on the simplest case. In this case, the variables managed by the *SQ_PreparePrediction* handle will be only those strictely needed for making a prediction. It is important to note that this will not be always the case as discussed below. Moreover, the input data will be in the form of an array-like structure, let's say a std::vector<float> named *fQuantitativeData* for this example, with a size equal to that of the number of variables needed by the PreparePrediction handle and in the same order. Finally, the input data structure will only contain the variable values for just one observation (it is possible to make predictions for several observations within the same call to SIMCA-Q. In this simple case, we could populate the *SQ_PreparePrediction* handle by:

```
iObs = 1; // we are just dealing with one observation
for (int iVar = 1; iVar <= numPredSetVariables; ++iVar){
  SQ_SetQuantitativeData(hPreparePrediction, iObs, iVar, fQuantitativeData[iVar-1]);
}
```

As mentioned, we could populate this handle with variable values from many predictions. We would just need to e.g., have the input vector in a 2D vector and iterate as well over the number of observations.

However, in many cases this will be a buggy approach. For prediction, SIMCA-Q requires only the data of the X variables used for building the model. Moreover, SIMCA-Q requires that these values are provided in the correct order i.e., same order as the variables follow in the corresponding workset for the relevant SIMCA model. But it is not uncommon that a *SQ_PreparePrediction* handle will manage more variable that those strictly needed for making predictions. And also that the position of the variables needed for prediction within the whole set managed by *SQ_PreparePrediction* is unknown a priori.

If you know in advance the structure of your worksetset, you can hard-code the script in order to place the input data in the correct positions when using *SQ_SetQuantitativeData()* (or e.g., *SQ_SetQualitativeData()* in case your model requires qualitative data). Just adjust the *iVar* variable in the above code accordingly. Actually, this would be the only option if you data file i.e., the file containing the data used for prediction, does not include the names of the variables these data corresponds to, or if these names do not coincide with those of the dataset used to build the model.

In case your input file contains the variable names along witht the variable values, and that these names coincide with those used for variables in the SIMCA model, one can think in workarouds to make the code more robust.

One of this workarounds would be as follows:

- The first step would consist in creating a dictionary/map with the names of the variables managed by the *SQ_PreparePrediction* handle as keys, and the position of these variables as values. We could get this dictionary, let's name it *DataLookup*, as follows:
```
std::map<std::string, int> DataLookup;
for(int iVar=1;iVar<=numPredSetVariables;iVar++){
  SQ_GetVariableFromVector(hPredictionVariables, iVar, &hVariable);
  SQ_GetVariableName (hVariable, 1, szVariableName, sizeof(szVariableName));
  DataLookup[szVariableName] = iVar;
}
```

Now suppose that we made functions that read the input data and return the names of the variables within these files as an std::vector\<std::string\> structure (e.g., with the name *inputVariables*), and the values of these variables as a std::vector\<float\> structure (e.g., with the name *fQuantitativeData*). Once the above dictionary is created, we can iterate over it, and then check if the variable name is also present in the std::vector<std::string> structure *inputVariables*. If that is the case, we can populate the correct position of the *SQ_PreparePrediction* handle with the corresponding value of the std::vector<float> structure *fQuantitativeData*:
```
for (auto const& [key, val] : DataLookup){
  auto res = std::find(inputVariables.begin(), inputVariables.end(), key);
  if(res!=inputVariables.end()){
    int position = res - inputVariables.begin();
    SQ_SetQuantitativeData(hPreparePrediction, 1, val, fQuantitativeData[position]);
  }
}
```

## <a name="make-predictions">Make Predictions</a>

Once we have feed the *SQ_PreparePrediction* handle with the correct data and in the correct order, we can create a *SQ_Prediction* handle that will allow us to handle predicted quantities:
```
SQ_Prediction hPredictionHandle = NULL;
SQ_GetPrediction(hPreparePrediction, &hPredictionHandle);
```

At this stage we can access all possible predicted quantities using the fuctions declared in *SQPrediction.h*.

### <a name="predictive-components">Predictive Components</a>

To obtain scores for predictive components, we will first need to retrieve a handle for them. For this we need to use the *SQ_GetTPS()* function. This function receives as input arguments:
- The *SQ_PreparePrediction* handle.
- The address of a *SQ_IntVector* structure pointer that will act as a handle for the components that will be predicted. However, you can set it to NULL/nullptr if you want to predict scores for all components.
- The address of a *SQ_VectorData* structure pointer that will be used to handle the scores.

For instance, if we want to predict scores for all components:
```
SQ_VectorData hPredictiveComponents = NULL;
SQ_GetTPS(hPredictionHandle, NULL, &hPredictiveComponents);
```

If for instance we would like to predict the score only the first component:
```
int iSize = 1; // Number of predictive components
SQ_IntVector hIntVector = NULL;
SQ_InitIntVector(&hIntVector, iSize);
SQ_SetDataInIntVector(hIntVector, 1, 1);
SQ_GetTPS(hPredictionHandle, &hIntVector, &hPredictiveComponents);
```

Next, we need to retrieve a handle for the matrix of the scores to be predicted:
```
SQ_FloatMatrix hScoresMatrix = NULL;
SQ_GetDataMatrix(hPredictiveComponents, &hScoresMatrix);
```

Finally, we can retrieve the score values using the *SQ_GetDataFromFloatMatrix()* function, which receives as input parameters 1) the handle for the matrix of the scores to be predicted, 2) the observation index, 3) the component index and 4) the address of the retrieved float value (i.e., the score value). For instance, to retrieve the score value of the first predicted observation and first predictive component:
```
float fScoreValue;
int iObs = 1;
int iComp = 1;
SQ_GetDataFromFloatMatrix(hScoresMatrix, iObs, iComp, &fScoreValue);
```

### <a name="y-variables">Y variables</a>

In the same way we could access the predicted Y values inn PLS or OPLS models. For this, we can use the function *SQ_GetYPredPS()*. This function receives as input parameters:
- The handle for the predictions.
- The number of model components we want the results from. For an OPLS model, the last predictive component is the only valid one.
- A *SQ_UnscaledState* handle, *True* if the function will return the y-values in the (unscaled) metric of the dataset. If *False*, the returned y-values will be in the scaled and centered metric of the workset.
- A *SQ_BacktransformedState* handle, *True* if the function will return the y-values in the unscaled untransformed metric of the workset. If *False* the returned y-values will be transformed in the same way as the workset.
- The address of a *SQ_IntVector* structure pointer to handle the indices of Y variables/columns to predict. If we set it to *NULL*, we will predict all variables/columns in the model.
- The address of a *SQ_VectorData* structure pointer that will be used to handle the actual predicted Y values.

For instance, if we want to retrieve predicted unscaled untransformed values for all Y variables using all predictive components the first step would consist in retrieving a *tagSQ_VectorData* handle for these predicted quantitites:

```
int numPredictiveComponents;
SQ_GetNumberOfPredictiveComponents(hModel, &numPredictiveComponents);
SQ_VectorData hPredictedYs;
SQ_GetYPredPS(hPredictionHandle, numPredictiveComponents, SQ_Unscaled_True, SQ_Backtransformed_True, NULL, &hPredictedYs);
```

As in the previous case, we need to retrieve a handle for the matrix of the predicted Y values:
```
SQ_FloatMatrix hYMatrix = NULL;
SQ_GetDataMatrix(hPredictedYs, &hYMatrix);
```

Now we are finally ready to retrieve the actual predicted Y values. For instance, we can retrieve the value of the first Y variable of the model and the first observation within the new set of data for prediction by:
```
float fYValue;
int iObs = 1;
int iYVar = 1;
SQ_GetDataFromFloatMatrix(hYMatrix, iObs, iComp, &fYValue);
```

## Example Script

In this [link](https://github.com/OEM-Sartorius-Data-Analytics/SimcaQ_CInterface_Guide/blob/main/06_0_MakingPredictions_Introduction/MakingPredictions_Introduction.cpp) you can find an example where all this is combined into a stand alone console script. The script will take as input parameters:

1. The name of a SIMCA project that will be loaded.
2. The name of a model within that SIMCA project.
3. The name of a file with data to make predictions

The script will print in the terminal the values of all predicted predictive components and Y variables.

The script also contains a simple function named *ReadInputFile()* tailored to read input data files like the [one provided in this repo](https://github.com/OEM-Sartorius-Data-Analytics/SimcaQ_CInterface_Guide/blob/main/06_0_MakingPredictions_Introduction/sampleSpectrum.csv). This data file contains variable names and values for a NIR spectrum of a sample of beer like the ones used to build the models within the SIMCA project included also in this repository,

Data files like this one could
that takes as input parameters 1) the name of an input file with the names and values of X variabled needed to make the predictions
