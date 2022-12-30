# Handling models: Retrieving the scores

*SQ_Model* handles allow retrieving the scores for the model worksheet.

Given that a [model has been loaded onto a *SQ_Model* handle](../05_0_HandlingModels_Introduction/HandlingModels_Introduction.md), let's name it *hModel* as in the previous example, we can retrieve a pointer to a *tagSQ_VectorData* structure, that we can use to handle the model scores, by using the function SQ_GetT():
```
SQ_VectorData scoresVectorData=NULL;
SQ_GetT(m_hModel,NULL,&scoresVectorData);
```

Here we used NULL t get al scroes from the model. If we would like to select specific scores we would need to use a *SQ_IntVector* handle.

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