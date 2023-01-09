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

The next step will be to populate the *SQ_PreparePrediction* structure pointer with the data from which we will predict a desired quantity. Let's imagine that we have the input data in the form of a std::vector<float>, let's name it *fQuantitativeData*, with a size equal to that of the number of variables needed for prediction, containing the variable values for just one observation, also in the same order than that for the observations used to build the model. In this case, we could populate the *SQ_PreparePrediction* structure pointer by:

```
iObs = 1;
for (int iVar = 1; iVar <= numPredSetVariables; ++iVar){
  SQ_SetQuantitativeData(hPreparePrediction, iObs, iVar, fQuantitativeData[iVar-1]);
}
```

We could actually populate this handle with variable values from many predictions, we would just need to e.g., have the input vector in a 2D vector and iterate as well over the number of observations.

However, in many cases this will be a buggy approach. For prediction, SIMCA-Q requires only the data of the variables used for building the model, but it *requires that they are provided in the correct order*. And the order is that of the dataset used to build the model. It is not uncommon that datasets include Y variables before the X variables, and even that not all X variables are used to build the model. Even if this is not explicit, it can happen e.g., when the derivates of the data are used instead of the original data. While SIMCA-Q will automatically apply the same preprocessing to the data that was used to build the model, derivating leaves out of the model building the first and last variables.

If you know in advance the structure of your dataset, you can hardcode the script in order to place the input data in the correct positions. Just adjust the iVar variable in the above code accordingly. Actually, this would be the only option if you data file i.e., the file containing the data used for prediction, does not include the names of the variables these data corresponds to, or if these names do not coincide with those of the dataset used to build the model.

However, we propose here a workaround if your input file contains the variable names, and they coincide with those used by the dataset used to build the model. Basically, you can create a dictionary with the names of the variables used to build the models as keys, and the position of these variables, which as shown above can be obtained from *SQ_Variables* handles. We could get this dictionary, let's name it *DataLookup*, as follows:
```
std::map<std::string, int> DataLookup;
for(int iVar=1;iVar<=numPredSetVariables;iVar++){
  SQ_GetVariableFromVector(hVariables, iVar, &hVariable);
  SQ_GetVariableName (hVariable, 1, szVariableName, sizeof(szVariableName));
  DataLookup[szName] = iVar;
}
```

Now suppose that we made functions that read the input data and return the names of the variables within these files as an std::vector<std::string> structure (e.g., with the name *inputVariables*), and the values of these variables as a std::vector<float> structure (e.g., with the name *fQuantitativeData*). Once the above dictionary is created, we can iterate over it, and then check if the variable name is also present in the std::vector<std::string> structure. If that is the case, we can populate the correct position of the *SQ_PreparePrediction* handle with the corresponding value of the std::vector<float> structure:
```
for (auto const& [key, val] : DataLookup){
  auto res = find(inputVariables.begin(), inputVariables.end(), key);
  if(res!=inputVariables.end()){
    int position = res - inputVariables.begin();
    SQ_SetQuantitativeData(hPreparePrediction, 1, position, fQuantitativeData[position]);
  }
}
```