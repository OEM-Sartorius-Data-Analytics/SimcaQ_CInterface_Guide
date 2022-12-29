# Handling datasets

The C interface of SIMCA-Q offers a handler for SIMCA datasets, the structure *tagSQ_Dataset*, as well as a typedef-name for pointers to this structure, *SQ_Dataset*. SIMCA-Q also offers several functions, defined in *SQDataset.h*, that can be used to retrieve information on the datasets included in SIMCA projects.

If we have previously [loaded a SIMCA project into a SQ_Project pointer](../02_HandlingProjects/HandlingProjects.md), let's name it hProject, we can initialize a pointer to a tagSQ_ModelHandle structure, let's name it hDataset, by:
```
SQ_Dataset hDataset = NULL;
```

Similarly to the way that SIMCA-Q handles models, in order to load a dataset we need to know a number the dataset is associate with. However, it might be not straightforward to know this number. Nevertheless, one can get it from the dataset index. All datasets withon a SIMCA project/file are associated to indices starting from 1 in the same order as they appear in the SIMCA project/file. Provided that we know the index of the dataset of interest, we can get the associated dataset number by calling the function *SQ_GetDatasetNumberFromIndex()* which receives as input parameters the SQ_Project pointer, the model index and the address of the model number that we want to retrieve. For instance, we can retrieve the number of the dataset with index equal to 1 by:
```
int iDatasetIndex = 1;
int iDatasetNumber;
SQ_GetDatasetNumberFromIndex(hProject, iDatasetIndex, &iDatasetNumber);
```

Once we know the number of the dataset of interest, we can retrieve the corresponding dataset model handler by:
```
SQ_GetDataset (hProject, iDatasetNumber, &hDataset);
```

[example](HandlingDatasets_Introduction.cpp)