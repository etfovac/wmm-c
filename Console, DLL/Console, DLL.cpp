// Console, DLL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <windows.h>
#include <string>
#include <iostream>
using namespace std;
#include "pathAppend.h"
#include "check_ptr.h"
extern "C" {
#include "..\code base\GeomagnetismHeader.h"
#include "..\code base\EGM9615.h"
}

int main()
{
    MAGtype_MagneticModel* MagneticModels[1] , * TimedMagneticModel;
    //MAGtype_MagneticModel* MagneticModel ;
    MAGtype_Ellipsoid Ellip;
    MAGtype_CoordSpherical CoordSpherical;
    MAGtype_CoordGeodetic CoordGeodetic;
    MAGtype_Date UserDate;
    MAGtype_GeoMagneticElements GeoMagneticElements, Errors;
    MAGtype_Geoid Geoid;
    char ans[20], b;
    char filename[] = "WMM.COF";
    char VersionDate_Large[] = "$Date: 2014-11-21 10:40:43 -0700 (Fri, 21 Nov 2014) $";
    char VersionDate[12];
    int NumTerms, Flag = 1, nMax = 0;
    int epochs = 1;
    /* Memory allocation */

    typedef int(_cdecl*  ptr_f)();
    typedef int(_cdecl*  ptr_f_MV)(MAGtype_MagneticModel*, char*);
    typedef MAGtype_MagneticModel* (_cdecl*  ptr_f_Mi)(int);
    typedef void(_cdecl*  ptr_f_vi)(int);
    typedef int(_cdecl*  ptr_f_cMi)(char*, MAGtype_MagneticModel* (*)[], int); // problem, use cast
    typedef int(_cdecl*  ptr_f_cii)(char*, uintptr_t, int); //fix
    typedef int(_cdecl*  ptr_f_cMpi)(char*, MAGtype_MagneticModel, int); //too many changes
    typedef int(_cdecl*  ptr_f_EG)(MAGtype_Ellipsoid*, MAGtype_Geoid* );
    typedef int(_cdecl*  ptr_f_M)(MAGtype_MagneticModel*);
    typedef int(_cdecl*  ptr_f_MGCD)(MAGtype_MagneticModel* , MAGtype_Geoid* , MAGtype_CoordGeodetic* , MAGtype_Date* );
    typedef int(_cdecl*  ptr_f_ECS)(MAGtype_Ellipsoid , MAGtype_CoordGeodetic , MAGtype_CoordSpherical* );
    typedef int(_cdecl*  ptr_f_DMM)(MAGtype_Date , MAGtype_MagneticModel * , MAGtype_MagneticModel * );
    typedef int(_cdecl*  ptr_f_ESGMG)(MAGtype_Ellipsoid ,
        MAGtype_CoordSpherical ,
        MAGtype_CoordGeodetic ,
        MAGtype_MagneticModel * ,
        MAGtype_GeoMagneticElements * );
    typedef int(_cdecl*  ptr_f_LE)(MAGtype_CoordGeodetic , MAGtype_GeoMagneticElements * );
    typedef int(_cdecl*  ptr_f_HU)(double , MAGtype_GeoMagneticElements * );
    typedef int(_cdecl* ptr_f_EESTMG)(MAGtype_GeoMagneticElements ,
        MAGtype_GeoMagneticElements ,
        MAGtype_CoordGeodetic ,
        MAGtype_Date ,
        MAGtype_MagneticModel * ,
        MAGtype_Geoid * );

     // put DLL in folder with EXE
    wchar_t Dir[MAX_PATH] = {};
    GetCurrentDirectory(MAX_PATH, Dir);
    string file = "DLL C.dll";
    char path[MAX_PATH];
    sprintf_s(path, "%ls", Dir);
    string FilePath = pathAppend(path, file);
    wstring wsFilePath = wstring(FilePath.begin(), FilePath.end());
    const wchar_t* wcFilePath = wsFilePath.c_str();
    HMODULE hDLL = LoadLibrary(wcFilePath);

    //check_ptr(NULL);
    if (check_ptr(hDLL, path)) { return EXIT_FAILURE; };
    //function address
    LPCSTR id = "MAG_Geomag";
    ptr_f_ESGMG MAG_Geomag_ptr = (ptr_f_ESGMG)GetProcAddress(hDLL, id);
    if (check_ptr(MAG_Geomag_ptr, id)) { return EXIT_FAILURE; };
    id = "MAG_GeomagIntroduction_WMM";
    ptr_f_MV MAG_GeomagIntroduction_WMM_ptr = (ptr_f_MV)GetProcAddress(hDLL, id);
    if (check_ptr(MAG_GeomagIntroduction_WMM_ptr, id)) { return EXIT_FAILURE; };
    id = "MAG_Error";
    ptr_f_vi MAG_Error_ptr = (ptr_f_vi)GetProcAddress(hDLL, id);
    if (check_ptr(MAG_Error_ptr, id)) { return EXIT_FAILURE; };
    id = "MAG_robustReadMagModels"; // !
    ptr_f_cMi MAG_robustReadMagModels_ptr = (ptr_f_cMi)GetProcAddress(hDLL, id);
    if (check_ptr(MAG_robustReadMagModels_ptr, id)) { return EXIT_FAILURE; };
    id = "MAG_robustReadMagModels_1"; // test fix
    ptr_f_cii MAG_robustReadMagModels_1_ptr = (ptr_f_cii)GetProcAddress(hDLL, id);
    if (check_ptr(MAG_robustReadMagModels_1_ptr, id)) { return EXIT_FAILURE; };
    id = "MAG_robustReadMagModels_2"; // test fix
    ptr_f_cMpi MAG_robustReadMagModels_2_ptr = (ptr_f_cMpi)GetProcAddress(hDLL, id);
    if (check_ptr(MAG_robustReadMagModels_2_ptr, id)) { return EXIT_FAILURE; };
    id = "MAG_AllocateModelMemory"; // mem alloc
    ptr_f_Mi MAG_AllocateModelMemory_ptr = (ptr_f_Mi)GetProcAddress(hDLL, id);
    if (check_ptr(MAG_AllocateModelMemory_ptr, id)) { return EXIT_FAILURE; };
    id = "MAG_SetDefaults"; // def
    ptr_f_EG MAG_SetDefaults_ptr = (ptr_f_EG)GetProcAddress(hDLL, id);
    if (check_ptr(MAG_SetDefaults_ptr, id)) { return EXIT_FAILURE; };
    id = "MAG_FreeMagneticModelMemory"; // free
    ptr_f_M MAG_FreeMagneticModelMemory_ptr = (ptr_f_M)GetProcAddress(hDLL, id);
    if (check_ptr(MAG_FreeMagneticModelMemory_ptr, id)) { return EXIT_FAILURE; };
    id = "MAG_GetUserInput"; // 
    ptr_f_MGCD MAG_GetUserInput_ptr = (ptr_f_MGCD)GetProcAddress(hDLL, id);
    if (check_ptr(MAG_GetUserInput_ptr, id)) { return EXIT_FAILURE; };
    id = "MAG_GeodeticToSpherical"; // 
    ptr_f_ECS MAG_GeodeticToSpherical_ptr = (ptr_f_ECS)GetProcAddress(hDLL, id);
    if (check_ptr(MAG_GeodeticToSpherical_ptr, id)) { return EXIT_FAILURE; };
    id = "MAG_TimelyModifyMagneticModel"; // 
    ptr_f_DMM MAG_TimelyModifyMagneticModel_ptr = (ptr_f_DMM)GetProcAddress(hDLL, id);
    if (check_ptr(MAG_TimelyModifyMagneticModel_ptr, id)) { return EXIT_FAILURE; };
    id = "MAG_CalculateGridVariation"; // 
    ptr_f_LE MAG_CalculateGridVariation_ptr = (ptr_f_LE)GetProcAddress(hDLL, id);
    if (check_ptr(MAG_CalculateGridVariation_ptr, id)) { return EXIT_FAILURE; };
    id = "MAG_WMMErrorCalc"; // 
    ptr_f_HU MAG_WMMErrorCalc_ptr = (ptr_f_HU)GetProcAddress(hDLL, id);
    if (check_ptr(MAG_WMMErrorCalc_ptr, id)) { return EXIT_FAILURE; };
    id = "MAG_PrintUserDataWithUncertainty"; // 
    ptr_f_EESTMG MAG_PrintUserDataWithUncertainty_ptr = (ptr_f_EESTMG)GetProcAddress(hDLL, id);
    if (check_ptr(MAG_PrintUserDataWithUncertainty_ptr, id)) { return EXIT_FAILURE; };
    
    strncpy_s(VersionDate, sizeof(VersionDate_Large), VersionDate_Large + 39, 11);
    // strncpy_s umesto strncpy, 
    // Error	2	error C4996: 'strncpy': This function or variable may be unsafe. Consider using strncpy_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. 
    // See online help for details.	d:\google drive - shared\programiranje\wmm2015 win - deklinacija\wmm_point.c	48	1	c exe konzola
    VersionDate[11] = '\0';
        
    int CoefsLoaded = MAG_robustReadMagModels_ptr(filename, (MAGtype_MagneticModel * (*)[]) &MagneticModels, epochs);
    // printf("\n CoefsLoaded: %i \n ", CoefsLoaded); // check, returns 1 for OK
    // MAG_robustReadMagModels_1_ptr(filename, (uintptr_t)&MagneticModels, epochs)
    // use alternative function which is callable from C++ (2nd param cast)
    if (!CoefsLoaded) {
        printf("\n WMM.COF not found.  Press enter to exit... \n ");
        fgets(ans, 20, stdin);
        return EXIT_FAILURE;
    }

    if (nMax < MagneticModels[0]->nMax) nMax = MagneticModels[0]->nMax;
    NumTerms = ((nMax + 1) * (nMax + 2) / 2);
    TimedMagneticModel = MAG_AllocateModelMemory_ptr(NumTerms); /* For storing the time modified WMM Model parameters */
    if (MagneticModels[0] == NULL || TimedMagneticModel == NULL)
    {
        MAG_Error_ptr(2);
    }

    MAG_SetDefaults_ptr(&Ellip, &Geoid); /* Set default values and constants */
  /* Check for Geographic Poles */

    /* Set EGM96 Geoid parameters */
    Geoid.GeoidHeightBuffer = GeoidHeightBuffer;
    Geoid.Geoid_Initialized = 1;
    /* Set EGM96 Geoid parameters END */

    b = MAG_GeomagIntroduction_WMM_ptr(MagneticModels[0], VersionDate);
    //std::cout << "b " << b << std::endl;

    while (Flag == 1 && b != 'x')
    {
        if (MAG_GetUserInput_ptr(MagneticModels[0], &Geoid, &CoordGeodetic, &UserDate) == 1) /*Get User Input */
        {
            MAG_GeodeticToSpherical_ptr(Ellip, CoordGeodetic, &CoordSpherical); /*Convert from geodetic to Spherical Equations: 17-18, WMM Technical report*/
            MAG_TimelyModifyMagneticModel_ptr(UserDate, MagneticModels[0], TimedMagneticModel); /* Time adjust the coefficients, Equation 19, WMM Technical report */
            MAG_Geomag_ptr(Ellip, CoordSpherical, CoordGeodetic, TimedMagneticModel, &GeoMagneticElements); /* Computes the geoMagnetic field elements and their time change*/
            MAG_CalculateGridVariation_ptr(CoordGeodetic, &GeoMagneticElements);
            MAG_WMMErrorCalc_ptr(GeoMagneticElements.H, &Errors);
            MAG_PrintUserDataWithUncertainty_ptr(GeoMagneticElements, Errors, CoordGeodetic, UserDate, TimedMagneticModel, &Geoid); /* Print the results */
        }
        printf("\n\n Do you need more point data ? (y or n) \n ");
        fgets(ans, 20, stdin);
        switch (ans[0]) {
        case 'Y':
        case 'y':
            Flag = 1;
            break;
        case 'N':
        case 'n':
            Flag = 0;
            break;
        default:
            Flag = 0;
            break;
        }
    }

    MAG_FreeMagneticModelMemory_ptr(TimedMagneticModel);
    MAG_FreeMagneticModelMemory_ptr(MagneticModels[0]);

    return EXIT_SUCCESS;
}