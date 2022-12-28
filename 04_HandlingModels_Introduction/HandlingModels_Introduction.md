# Handling models: An introduction

The C interface of SIMCA-Q offers a handler for SIMCA models, the structure *tagSQ_ModelHandle*, as well as a typedef-name for pointers to this structure, *SQ_Model*. SIMCA-Q also offers several functions, defined in SQModel.h, that can be used to retrieve information on the model as well as for other purpose e.g., create further SIMCA-Q structures to make predictions.

If we have previously [loaded a SIMCA project into a SQ_Project pointer](../02_HandlingProjects/HandlingProjects.md), let's name it hProject, we can initialize a pointer to a *tagSQ_ModelHandle* structure, let's name it hModel, by:
```
SQ_Model hModel = NULL;
```

The next step would consist in loading the model. For this we need to know the number associated with the model of interest.