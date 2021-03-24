# WMM Point Calculation C++ console uses C DLL  

[![DOI](https://zenodo.org/badge/286341041.svg)](https://zenodo.org/badge/latestdoi/286341041)  [![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://github.com/etfovac/wmm-c/blob/master/LICENSE)
 WMM Point Calculation C++ console (based on C console) uses WMM's C DLL instead of WMM's <em>GeomagnetismLibrary.c</em>   
 (outdated, uses 2014/2015 release from <a href="https://ngdc.noaa.gov/geomag/WMM/soft.shtml">The World Magnetic Model and Associated Software</a>  )

### Keywords:

> WMM,	World Magnetic Model  
> Magnetic Declination Estimation  

<em>GeomagnetismHeader.h</em> is header for both DLL and C Library.  
```c++
extern "C" {
#include "..\code base\GeomagnetismHeader.h"
#include "..\code base\EGM9615.h"
}
``` 
Note: <em>WMM.COF</em> file should be in the same folder with <em>DLL C.dll</em> and <em>Console, DLL.exe</em>.  

```c++
    // DLL is in folder with EXE
    wchar_t Dir[MAX_PATH] = {};
    GetCurrentDirectory(MAX_PATH, Dir);
    string file = "DLL C.dll";
    char path[MAX_PATH];
    sprintf_s(path, "%ls", Dir);
    string FilePath = pathAppend(path, file);
    wstring wsFilePath = wstring(FilePath.begin(), FilePath.end());
    const wchar_t* wcFilePath = wsFilePath.c_str();
    // LoadLibrary
    HMODULE hDLL = LoadLibrary(wcFilePath);
    if (check_ptr(hDLL, path)) { return EXIT_FAILURE; };  
```  
Some of pointers to functions from DLL:
```c++
    LPCSTR id = "MAG_Geomag";
    ptr_f_ESGMG MAG_Geomag_ptr = (ptr_f_ESGMG)GetProcAddress(hDLL, id);
    if (check_ptr(MAG_Geomag_ptr, id)) { return EXIT_FAILURE; };
 //...
    id = "MAG_GeomagIntroduction_WMM";
    ptr_f_MV MAG_GeomagIntroduction_WMM_ptr = (ptr_f_MV)GetProcAddress(hDLL, id);
    if (check_ptr(MAG_GeomagIntroduction_WMM_ptr, id)) { return EXIT_FAILURE; };
```  
Some TypeDef allocations:
```c++
    typedef int(_cdecl*  ptr_f)();
    typedef int(_cdecl*  ptr_f_MV)(MAGtype_MagneticModel*, char*);
    typedef MAGtype_MagneticModel* (_cdecl*  ptr_f_Mi)(int);
    typedef void(_cdecl*  ptr_f_vi)(int);
    typedef int(_cdecl*  ptr_f_cMi)(char*, MAGtype_MagneticModel* (*)[], int); // problem, use cast
    typedef int(_cdecl*  ptr_f_cii)(char*, uintptr_t, int); //fix
    typedef int(_cdecl*  ptr_f_MGCD)(MAGtype_MagneticModel* , MAGtype_Geoid* , MAGtype_CoordGeodetic* , MAGtype_Date* );
```  
Calling functions:
```c++
    b = MAG_GeomagIntroduction_WMM_ptr(MagneticModels[0], VersionDate);
//...
    MAG_FreeMagneticModelMemory_ptr(TimedMagneticModel);
    MAG_FreeMagneticModelMemory_ptr(MagneticModels[0]);
```  
### References
<a href="https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml#declination">Magnetic Field Calculators - Magnetic Declination Estimated Value (NOAA > NESDIS > NCEI (formerly NGDC) > Geomagnetism)</a>  
<a href="https://ngdc.noaa.gov/geomag/WMM/soft.shtml">The World Magnetic Model and Associated Software</a>  
<a href="https://ngdc.noaa.gov/geomag/WMM/thirdpartycontributions.shtml">WMM program ports to other languages</a>

### License
This repository includes portions of the WMM source code distributed by <a href="https://www.ngdc.noaa.gov/ngdc.html">NOAA National Centers for Environmental Information (NCEI)</a>.

<a href="https://ngdc.noaa.gov/geomag/WMM/license.shtml">The World Magnetic Model - License and copyright information</a>  
>>The WMM source code is in the public domain and not licensed or under copyright. The information and software may be used freely by the public. As required by 17 U.S.C. 403, third parties producing copyrighted works consisting predominantly of the material produced by U.S. government agencies must provide notice with such work(s) identifying the U.S. Government material incorporated and stating that such material is not subject to copyright protection.

The remaining source code is available under <a href="https://github.com/etfovac/wmm-c/blob/master/LICENSE">the MIT license</a>
