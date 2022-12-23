# Getting started with the SIMCA-Q C interface

## Windows

When building in Windows an application that communicates with SIMCA-Q through its C interface, copy all files from the *include* folder under *%ProgramFiles%\Umetrics\SIMCA-Q 17* (or similar depending on your version) to the same directory where you have put the source files of your application.

For running the application, files from the lib folder (dll), under under %ProgramFiles%\Umetrics\SIMCA-Q 17, must be copied to the same directory as the executable. In that same directory you also need a valid license file.

## Linux

Instructions on how to build in Linux an application that communicates with SIMCA-Q through its C interface can be found in this [repo](https://github.com/OEM-Sartorius-Data-Analytics/SIMCAQ-GUIDE).

For running the application, you will need to set the environmental variable LD_LIBRARY_PATH variable to the path of the SIMCA-Q libraries. You will need to chack their location, but probably you will find them in /usr/lib64. Then, you can just run:
```
export LD_LIBRARY_PATH=/usr/lib64
```

and then run the executable of your application.

## Include files

In order to use the SIMCA-Q C interface, your script needs to include relevant headers. Which one you will need to include will depend on your license and appplication.

In case your application will use SIMCA-Q for predictions using standard (non-batch) models, you will need to include the *SIMCAQP.h* header:
```
#include "SIMCAQP.h"
```

In case you will use batch models, include the *SIMCAQPPlus.h* header:
```
#include "SIMCAQPPlus.h"
```

If your application will build models, you will need to include the *SIMCAQM.h* header:
```
#include "SIMCAQM.h"
```

## Help Files

Please, refer to the [Help Files](https://www.sartorius.com/download/961736/simca-q-17-0-1-help-files-en-b-00260-sartorius-zip-data.zip) available from [Sartorius SIMCA-Q web](https://www.sartorius.com/en/products/oem/oem-data-analytics/simca-q) for a complete list of the classes and methods available from the headers listed above.