# Handling datasets

The C interface of SIMCA-Q offers a handler for SIMCA datasets, the structure *tagSQ_Dataset*, as well as a typedef-name for pointers to this structure, *SQ_Dataset*. SIMCA-Q also offers several functions, defined in *SQDataset.h*, that can be used to retrieve information on the datasets included in SIMCA projects.

If we have previously [loaded a SIMCA project into a SQ_Project pointer](../02_HandlingProjects/HandlingProjects.md), let's name it hProject, we can initialize a pointer to a tagSQ_ModelHandle structure, let's name it hDataset, by:
```
SQ_Dataset hDataset = NULL;
```