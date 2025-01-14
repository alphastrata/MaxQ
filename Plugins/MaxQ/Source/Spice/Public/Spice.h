// Copyright 2021 Gamergenic.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be included in all copies
// or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

// Author: chucknoble@gamergenic.com | https://www.gamergenic.com
// 
// Project page:
// https://www.gamergenic.com/project/maxq/
//
// Documentation site:
// https://maxq.gamergenic.com/
// 
// Unreal Engine Plugin Source Code on GitHub:
// https://github.com/Gamergenic1/MaxQ
//
// Implementation walkthrough:
// "Adding Third-Party Libraries to Unreal Engine : NASA's SPICE Toolkit"
// https://gamedevtricks.com/post/third-party-libs-1/


//------------------------------------------------------------------------------
// Spice.h
// 
// API Comments
//
// USpice : public UBlueprintFunctionLibrary
// 
// Purpose:  Blueprint implementation of a CSPICE wrapper.
// 
// MaxQ:
// * Base API
// * Refined API
//    * C++
//    * Blueprints
//
// MaxQ: Three APIs
// * Base API :  Initial Harness for studying integration with UE
//    ~ USpice, USpiceTypes, etc
// * Refined API
//    * C++ :  C++-friendly
//      ~ SpiceMath.h, SpiceOperators.h, etc
//    * Blueprints :  C++-friendly
//      ~ USpiceK2, UK2Node_unorm, etc
// 
// USpice is part of the base API, where CSPICE functionality is initially
// exposed.
// It is maintained for backwards compatibility, when possible deprecation is
// preferred to a breaking change.
// What is here is usable from either blueprints or c++, but the Refined C++/
// Blueprint APIs will be more convenient to the user when functionality is
// migrated there.
// 
// See implementation comments in Spice.cpp.
//------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpiceConstants.h"
#include "SpiceTypes.h"
#include "SpiceCore.h"
#include "SpiceMath.h"
#include "SpiceData.h"
#include "SpiceOperators.h"
#include "Spice.generated.h"


DECLARE_DYNAMIC_DELEGATE_TwoParams(FDerivativeDelegate, double, et, double&, value);

UCLASS(Category="MaxQ")
class SPICE_API USpice : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Utility|Kernel",
        meta = (
            Keywords = "UTILITY",
            AdvancedDisplay = "ErrorIfNoFilesFound",
            ExpandEnumAsExecs = "ResultCode",
            ToolTip = "List files (path relative to /Content directory)"
            ))
    static void enumerate_kernels(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FString>& kernelFileRelativePaths,
        const FString& relativeDirectory = TEXT("NonAssetData/kernels"),
        bool ErrorIfNoFilesFound = true
        );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Kernel",
        meta = (
            Keywords = "UTILITY",
            ExpandEnumAsExecs = "ResultCode",
            ShortToolTip = "Load kernel file",
            ToolTip = "Load a kernel file\n\nThe path relative to the /Content directory,\ne.g. \"kernels/gm_de431.tpc\", where this is a subdirectory of your Content directory."
            ))
    static void furnsh(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FString& relativePath = TEXT("NonAssetData/kernels/gm_de431.tpc")
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Kernel",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "UTILITY",
            ShortToolTip = "Load kernel file list",
            ToolTip = "Load a list of kernel files (paths relative to /Content directory)"
            ))
    static void furnsh_list(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const TArray<FString>& relativePaths
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Utility|Kernel",
        meta = (
            Keywords = "UTILITY",
            ToolTip = "Join a path to a list of paths"
            ))
    static void combine_paths(
        const FString& basePath,
        const TArray<FString>& relativePaths,
        TArray<FString>& joinedPaths
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Kernel",
        meta = (
            AdvancedDisplay = "PrintCallstack",
            Keywords = "UTILITY",
            ToolTip = "Clear all kernel file / kernel pool"
            ))
    static void clear_all();

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Kernel",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "UTILITY",
            ToolTip = "Unload kernel file"
            ))
    static void unload(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FString& relativeDirectory
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Kernel",
        meta = (
            AdvancedDisplay = "PrintCallstack",
            Keywords = "UTILITY",
            ToolTip = "reset/init cspice"
            ))
    static void init_all(
        bool PrintCallstack = false
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Error",
        meta = (
            Keywords = "ERROR",
            ToolTip = "Get Default Error Action"
            ))
    static void get_erract(ES_ErrorAction& Result);

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Error",
        meta = (
            Keywords = "ERROR",
            ToolTip = "Get Error Output Device Name"
            ))
    static void get_errdev(ES_ErrorDevice& device);

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Error",
        meta = (
            Keywords = "ERROR",
            ToolTip = "Get Error Output Items"
            ))
    static void get_errprt(FString& OutMessage);

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Error",
        meta = (
            Keywords = "ERROR",
            ToolTip = "Set Default Error"
            ))
    static void set_erract(ES_ErrorAction actionToSet);

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Error",
        meta = (
            Keywords = "ERROR",
            ToolTip = "Set Error Output Device Name"
            ))
    static void set_errdev(ES_ErrorDevice Device, const FString& LogFilePath);

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Error",
        meta = (
            Keywords = "ERROR",
            ToolTip = "Set Error Output Items"
            ))
    static void set_errprt(
        UPARAM(meta = (Bitmask, BitmaskEnum = "ES_Items")) int32 items =0x10
        );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Error",
        meta = (
            Keywords = "ERROR",
            ToolTip = "Reset Error Status"
            ))
    static void reset();

    /// <summary>Construct a rotation matrix that rotates vectors by a specified angle about a specified axis</summary>
    /// <param name="axis">[in] Rotation axis</param>
    /// <param name="angle">[in] Rotation angle, in radians</param>
    /// <param name="r">[out] Rotation matrix corresponding to axis and angle</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintPure,
        Category = "MaxQ|Rotation",
        meta = (
            Keywords = "MATRIX, ROTATION",
            ShortToolTip = "Axis and angle to rotation",
            ToolTip = "Construct a rotation matrix that rotates vectors by a specified angle about a specified axis"
            ))
    static void axisar(
        const FSDimensionlessVector& axis,
        const FSAngle& angle,
        FSRotationMatrix& r
    );


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Coordinates",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "FRAMES, PCK, SPK, TIME",
            ShortToolTip = "AZ/EL, constant position observer state",
            ToolTip = "Return the azimuth/elevation coordinates of a specified target relative to an observer",
            AutoCreateRefTerm = "obspos"
            ))
    static void azlcpo(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSDimensionlessStateVector& azlsta,
        FSEphemerisPeriod& lt,
        const FSEphemerisTime& et,
        const FSDistanceVector& obspos,
        const FString& obsctr = FString("EARTH"),
        const FString& obsref = FString("IAU_EARTH"),
        const FString& target = FString("SUN"),
        UPARAM(meta = (DisplayName = "azimuth is counter-clockwise")) bool azccw = true,
        UPARAM(meta = (DisplayName = "elevation increases with positive Z")) bool elplsz = true,
        ES_AberrationCorrectionWithTransmissions abcorr = ES_AberrationCorrectionWithTransmissions::None,
        ES_LocalZenithMethod method = ES_LocalZenithMethod::ELLIPSOID
    );


    UFUNCTION(
        BlueprintPure,
        Category = "MaxQ|Coordinates",
        meta = (
            Keywords = "CONVERSION, COORDINATES",
            ShortToolTip = "AZ/EL to rectangular coordinates",
            ToolTip = "Convert from range, azimuth and elevation of a point to rectangular coordinates"
            ))
    static void azlrec(
        FSDistanceVector& rectan,
        const FSDistance& range,
        const FSAngle& az,
        const FSAngle& el,
        UPARAM(meta = (DisplayName = "azimuth is counter-clockwise")) bool azccw = true,
        UPARAM(meta = (DisplayName = "elevation increases with positive Z")) bool elplsz = true
    );

    /// <summary>Body name to ID code translation</summary>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Kernel",
        meta = (
            Keywords = "CONSTANTS",
            ExpandEnumAsExecs = "FoundCode",
            ToolTip = "Determine whether values exist for some item for any body in the kernel pool"
            ))
    static void bodfnd(
        ES_FoundCode& FoundCode,
        int body = 399,
        const FString& item = TEXT("RADII")
    );

    /// <summary>Body name to ID code translation</summary>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|NAIF IDs",
        meta = (
            Keywords = "BODY, CONVERSION",
            ExpandEnumAsExecs = "FoundCode",
            ToolTip = "Translate the SPICE integer code of a body into a common name for that body"
            ))
    static void bodc2n(
        ES_FoundCode& FoundCode,
        int code,
        FString& name
    );

    /// <summary> Define a body name/ID code pair for later translation via bodn2c_c or bodc2n_c.< / summary>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|NAIF IDs",
        meta = (
            Keywords = "BODY, CONVERSION",
            ToolTip = "Define a body name/ID code pair for later translation via bodn2c_c or bodc2n_c"
            ))
    static void boddef(
        const FString& name = TEXT("OUMUAMUA"),
        int code = 3788040
    );

    /// <summary>Body name to ID code translation</summary>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|NAIF IDs",
        meta = (
            ExpandEnumAsExecs = "FoundCode",
            Keywords = "BODY, CONVERSION, ID, NAME, UTILITY",
            ShortToolTip = "Body string to ID code translation",
            ToolTip = "Translate a string containing a body name or ID code to an integer code"
            ))
    static void bods2c(
        ES_FoundCode& FoundCode,
        int& code,
        const FString& name = TEXT("EARTH")
    );

    /// <summary>Body name to ID code translation</summary>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|NAIF IDs",
        meta = (
            ExpandEnumAsExecs = "FoundCode",
            Keywords = "BODY, CONVERSION, ID, NAME",
            ShortToolTip = "Body name to ID code translation",
            ToolTip = "Translate the name of a body or object to the corresponding SPICE integer ID code"
            ))
        static void bodn2c(
            ES_FoundCode& FoundCode,
            int& code,
            const FString& name = TEXT("EARTH")
        );

    // Bodvrd, etc, are not deprecated for C++ usage, only Blueprints.  They're still handy from C++.

    /// <summary>Fetch from the kernel pool the double precision values of an item associated with a body</summary>
    /// <param name="bodyid">[in] Body ID code</param>
    /// <param name="item">[in] Item for which values are desired. ("RADII", "NUT_PREC_ANGLES", etc. )</param>
    /// <param name="maxn">[in] Maximum number of values that may be returned</param>
    /// <param name="dim">[out] Number of values returned</param>
    /// <param name="values">[out] Values</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly,
        Category = "MaxQ|Deprecated",
        meta = (
            DeprecatedFunction,
            DeprecationMessage = "Bodvcd is obolete, use the K2Node_bodvcd (bodcrd) instead",
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONSTANTS",
            ShortToolTip = "Return d.p. values from the kernel pool",
            ToolTip = "(double) Fetch from the kernel pool the double precision values of an item associated with a body, where the body is specified by an integer ID code"
            ))
    static void bodvcd_scalar(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        double& ReturnValue,
        int bodyid = 399,
        const FString& item = TEXT("RADII")
    );

    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly,
        Category = "MaxQ|Deprecated",
        meta = (
            DeprecatedFunction,
            DeprecationMessage = "Bodvcd is obolete, use the K2Node_bodvcd (bodvcd) instead",
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONSTANTS",
            ShortToolTip = "Return d.p. values from the kernel pool",
            ToolTip = "(double vector3) Fetch from the kernel pool the double precision values of an item associated with a body, where the body is specified by an integer ID code"
            ))
    static void bodvcd_vector(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSDimensionlessVector& ReturnValue,
        int bodyid = 399,
        const FString& item = TEXT("RADII")
    );

    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly,
        Category = "MaxQ|Deprecated",
        meta = (
            DeprecatedFunction,
            DeprecationMessage = "Bodvcd is obolete, use the K2Node_bodvcd (bodvcd) instead",
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONSTANTS",
            ShortToolTip = "Return d.p. values from the kernel pool",
            ToolTip = "(double) Fetch from the kernel pool the double precision values of an item associated with a body, where the body is specified by an integer ID code"
            ))
    static void bodvcd_mass(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSMassConstant& ReturnValue,
        int bodyid = 399,
        const FString& item = TEXT("GM")
    );

    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly,
        Category = "MaxQ|Deprecated",
        meta = (
            DeprecatedFunction,
            DeprecationMessage = "Bodvcd is obolete, use the K2Node_bodcrd (bodcrd) instead",
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONSTANTS",
            ShortToolTip = "Return d.p. values from the kernel pool",
            ToolTip = "(double) Fetch from the kernel pool the double precision values of an item associated with a body, where the body is specified by an integer ID code"
            ))
    static void bodvcd_distance_vector(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSDistanceVector& ReturnValue,
        int bodyid = 399,
        const FString& item = TEXT("GM")
    );


    /// <summary>Fetch from the kernel pool the double precision values of an item associated with a body</summary>
    /// <param name="bodynm">[in] Body name</param>
    /// <param name="item">[in] Item for which values are desired. ("RADII", "NUT_PREC_ANGLES", etc. )</param>
    /// <param name="maxn">[in] Maximum number of values that may be returned</param>
    /// <param name="dim">[out] Number of values returned</param>
    /// <param name="values">[out] Values</param>
    /// <returns></returns>
    // Bodvrd is obolete, in favor of K2Node_bodvrd
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Deprecated", BlueprintInternalUseOnly,
        meta = (
            DeprecatedFunction,
            DeprecationMessage = "Bodvrd is obolete, use the K2Node_bodvrd (bodvrd) instead",
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONSTANTS",
            ShortToolTip = "Return d.p. values from the kernel pool",
            ToolTip = "(double) Fetch from the kernel pool the double precision values of an item associated with a body"
            ))
    static void bodvrd_scalar(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        double& ReturnValue,
        const FString& bodynm = TEXT("EARTH"),
        const FString& item = TEXT("RADII")
    );

    UFUNCTION(BlueprintCallable, 
        Category = "MaxQ|Deprecated", BlueprintInternalUseOnly,
        meta = (
            DeprecatedFunction,
            DeprecationMessage = "Bodvrd is obolete, use the K2Node_bodvrd (bodvrd) instead",
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONSTANTS",
            ShortToolTip = "Return d.p. values from the kernel pool",
            ToolTip = "(double vector3) Fetch from the kernel pool the double precision values of an item associated with a body"
            ))
    // Bodvrd is obolete, in favor of K2Node_bodvrd
    static void bodvrd_vector(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSDimensionlessVector& ReturnValue,
        const FString& bodynm = TEXT("EARTH"),
        const FString& item = TEXT("RADII")
    );

    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly,
        Category = "MaxQ|Deprecated",
        meta = (
            DeprecatedFunction,
            DeprecationMessage = "Bodvrd is obolete, use the K2Node_bodvrd (bodvrd) instead",
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONSTANTS",
            ShortToolTip = "Return d.p. values from the kernel pool",
            ToolTip = "(double) Fetch from the kernel pool the double precision values of an item associated with a body"
            ))
    // Bodvrd is obolete, in favor of K2Node_bodvrd
    static void bodvrd_mass(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSMassConstant& ReturnValue,
        const FString& bodynm = TEXT("EARTH"),
        const FString& item = TEXT("GM")
    );
    
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly,
        Category = "MaxQ|Deprecated",
        meta = (
            DeprecatedFunction,
            DeprecationMessage = "Bodvrd is obolete, use the K2Node_bodvrd (bodvrd) instead",
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONSTANTS",
            ShortToolTip = "Return d.p. values from the kernel pool",
            ToolTip = "(double) Fetch from the kernel pool the double precision values of an item associated with a body"
            ))
    // Bodvrd is obolete, in favor of K2Node_bodvrd
    static void bodvrd_distance_vector(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSDistanceVector& ReturnValue,
        const FString& bodynm = TEXT("EARTH"),
        const FString& item = TEXT("RADII")
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Search",
        meta = (
            Keywords = "ARRAY, SEARCH",
            ShortToolTip = "Binary search for a double precision value",
            ToolTip = "Do a binary search for a given value within a double precision array, assumed to be in nondecreasing order.Return the index of the matching array entry, or -1 if the key value is not found"
            ))
    static int bsrchd(
        double value,
        const TArray<double>& valueArray
    );

    /// <summary>CK, Close file</summary>
    /// <param name="handle">[in] Handle of the CK file to be closed</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Ellipses",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "ELLIPSE, GEOMETRY",
            ShortToolTip = "Center and generating vectors to ellipse",
            ToolTip = "Form a CSPICE ellipse from a center vector and two generating vectors"
            ))
    static void cgv2el(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSDistanceVector& center,
        const FSDistanceVector& vec1,
        const FSDistanceVector& vec2,
        FSEllipse& ellipse
    );


    /// <summary>CK, Close file</summary>
    /// <param name="handle">[in] Handle of the CK file to be closed</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|CK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CK",
            ShortToolTip = "CK, Close file",
            ToolTip = "Close an open CK file"
            ))
    static void ckcls(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int handle
    );

    /// <summary>Find the coverage window for a specified object in a specified CK file</summary>
    /// <param name="ck">[in] Name of CK file</param>
    /// <param name="idcode">[in] ID code of object</param>
    /// <param name="needav">[in] Flag indicating whether angular velocity is needed</param>
    /// <param name="level">[in] Coverage level:  "SEGMENT" OR "INTERVAL"</param>
    /// <param name="tol">[in] Tolerance in ticks</param>
    /// <param name="timsys">[in] Time system used to represent coverage</param>
    /// <param name="cover">[out] Window giving coverage for `idcode'</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|CK",
        meta = (
            AutoCreateRefTerm = "merge_to",
            Keywords = "POINTING, TIME, UTILITY",
            ExpandEnumAsExecs = "ResultCode",
            ShortToolTip = "CK coverage",
            ToolTip = "Find the coverage window for a specified object in a specified CK file"
            ))
    static void ckcov(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        UPARAM(meta = (DisplayName = "project-relative file path")) const FString& ck_relative_path,
        int idcode,
        bool need_av,
        double tol,
        UPARAM(meta = (DisplayName = "(optional) existing window to merge results with")) const TArray<FSWindowSegment>& merge_to,
        TArray<FSWindowSegment>& coverage,
        ES_CoverageLevel level = ES_CoverageLevel::Interval,
        ES_TimeSystem    timsys = ES_TimeSystem::SCLK
    );



    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|CK",
        meta = (
            Keywords = "POINTING",
            ExpandEnumAsExecs = "FoundCode",
            ShortToolTip = "CK frame, find position rotation",
            ToolTip = "Find the position rotation matrix from a C-kernel (CK) frame with the specified frame class ID(CK ID) to the base frame of the highest priority CK segment containing orientation data for this CK"
            ))
    static void ckfrot(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        ES_FoundCode& FoundCode,
        FSRotationMatrix& rotationMatrix,
        int& ref,
        int inst,
        const FSEphemerisTime& et
    );


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|CK",
        meta = (
            Keywords = "POINTING",
            ExpandEnumAsExecs = "FoundCode",
            ShortToolTip = "CK frame, find state transformation",
            ToolTip = "Find the state transformation matrix from a C-kernel (CK) frame with the specified frame class ID(CK ID) to the base frame of the highest priority CK segment containing orientation and angular velocity"
            ))
    static void ckfxfm(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        ES_FoundCode& FoundCode,
        FSStateTransform& xform,
        int& ref,
        int inst,
        const FSEphemerisTime& et
    );


    /// <summary>C-kernel, get pointing</summary>
    /// <param name="inst">[in] NAIF ID of instrument, spacecraft, or structure</param>
    /// <param name="sclkdp">[in] Encoded spacecraft clock time</param>
    /// <param name="tol">[in] Time tolerance</param>
    /// <param name="ref">[in] Reference frame</param>
    /// <param name="cmat">[out] C-matrix pointing data</param>
    /// <param name="clkout">[out] Output encoded spacecraft clock time</param>
    /// <param name="found">[out] True when requested pointing is available</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|CK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "POINTING",
            ShortToolTip = "C-kernel, get pointing",
            ToolTip = "Get pointing (attitude) for a specified spacecraft clock time"
            ))
    static void ckgp(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int inst,
        double sclkdp,
        double tol,
        const FString& ref,
        FSRotationMatrix& cmat,
        double& clkout,
        bool& bFound
    );

    /// <summary>C-kernel, get pointing and angular velocity</summary>
    /// <param name="inst">[in] NAIF ID of instrument, spacecraft, or structure</param>
    /// <param name="sclkdp">[in] Encoded spacecraft clock time</param>
    /// <param name="tol">[in] Time tolerance</param>
    /// <param name="ref">[in] Reference frame</param>
    /// <param name="cmat">[in] C-matrix pointing data</param>
    /// <param name="av">[in] Angular velocity vector</param>
    /// <param name="clkout">[out] Output encoded spacecraft clock time</param>
    /// <param name="found">[out] True when requested pointing is available</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|CK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "POINTING",
            ShortToolTip = "C-kernel, get pointing and angular velocity",
            ToolTip = "Get pointing (attitude) and angular velocity for a specified spacecraft clock time"
            ))
    static void ckgpav(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int inst,
        double sclkdp,
        double tol,
        const FString& ref,
        FSRotationMatrix& cmat,
        FSAngularVelocity& av,
        double& clkout,
        bool& bFound
    );


    /// <summary>C-kernel, load pointing file</summary>
    /// <param name="filename">[in] Name of the CK file to be loaded</param>
    /// <param name="handle">[out] Loaded file's handle</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|CK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "POINTING",
            ShortToolTip = "C-kernel, load pointing file",
            ToolTip = "Load a CK pointing file for use by the CK readers"
            ))
    static void cklpf(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        UPARAM(meta = (DisplayName = "project-relative file path")) const FString& ck_relative_path,
        int& handle
    );

    /// <summary>CK objects</summary>
    /// <param name="ck">[in] Name of CK file</param>
    /// <param name="ids">[in/out] Set of ID codes of objects in CK file</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|CK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "POINTING, UTILITY",
            ShortToolTip = "CK objects",
            ToolTip = "Find the set of ID codes of all objects in a specified CK file"
            ))
    static void ckobj(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        UPARAM(meta = (DisplayName = "project-relative file path")) const FString& relative_path,
        TArray<int>& ids
    );

    /// <summary>CK, open new file</summary>
    /// <param name="fname">[in] The name of the CK file to be opened</param>
    /// <param name="ifname">[in] The internal filename for the CK</param>
    /// <param name="ncomch">[in] The number of characters to reserve for comments</param>
    /// <param name="handle">[out] The handle of the opened CK file</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|CK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "POINTING",
            ShortToolTip = "CK, open new file",
            ToolTip = "Open a new CK file, returning the handle of the opened file"
            ))
    static void ckopn(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        UPARAM(meta = (DisplayName = "project-relative file path")) const FString& relativePath,
        UPARAM(meta = (DisplayName = "internal filename")) const FString& ifname,
        UPARAM(meta = (DisplayName = "number of characters to reserve for comments")) const int ncomch,
        int& handle
    );

    /// <summary>C-kernel, Unload pointing file</summary>
    /// <param name="handle">[in] Handle of CK file to be unloaded</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|CK",
        meta = (
            Keywords = "POINTING",
            ShortToolTip = "C-kernel, Unload pointing file",
            ToolTip = "Unload a CK pointing file so that it will no longer be searched by the readers"
            ))
    static void ckupf(
        int handle
    );

    /// <summary>C-Kernel, write segment to C-kernel, data type 1</summary>
    /// <param name="handle">[in] Handle of an open CK file</param>
    /// <param name="begtim">[in] The beginning encoded SCLK of the segment</param>
    /// <param name="endtim">[in] The ending encoded SCLK of the segment</param>
    /// <param name="inst">[in] The NAIF instrument ID code</param>
    /// <param name="ref">[in]  The reference frame of the segment</param>
    /// <param name="avflag">[in] True if the segment will contain angular velocity</param>
    /// <param name="segid">[in] Segment identifier</param>
    /// <param name="nrec">[in] Number of pointing records</param>
    /// <param name="sclkdp">[in] Encoded SCLK times</param>
    /// <param name="quats">[in] Quaternions representing instrument pointing</param>
    /// <param name="avvs">[in] Angular velocity vectors</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|CK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "POINTING, UTILITY",
            ShortToolTip = "C-Kernel, write segment to C-kernel, data type 1",
            ToolTip = "Add a type 1 segment to a C-kernel"
            ))
    static void ckw01(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int handle,
        double begtim,
        double endtim,
        int inst,
        const FString& ref,
        bool        avflag,
        const FString& segid,
        const TArray<FSPointingType1Observation>& records
    );


    /// <summary>C-Kernel, write segment to C-kernel, data type 2</summary>
    /// <param name="handle">[in] Handle of an open CK file</param>
    /// <param name="begtim">[in] The beginning encoded SCLK of the segment</param>
    /// <param name="endtim">[in] The ending encoded SCLK of the segment</param>
    /// <param name="inst">[in] The NAIF instrument ID code</param>
    /// <param name="ref">[in] The reference frame of the segment</param>
    /// <param name="segid">[in] Segment identifier</param>
    /// <param name="nrec">[in] Number of pointing records</param>
    /// <param name="start">[in] Encoded SCLK interval start times</param>
    /// <param name="stop">[in] Encoded SCLK interval stop times</param>
    /// <param name="quats">[in] Quaternions representing instrument pointing</param>
    /// <param name="avvs">[in] Angular velocity vectors</param>
    /// <param name="rates">[in] Number of seconds per tick for each interval</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|CK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "POINTING, UTILITY",
            ShortToolTip = "C-Kernel, write segment to C-kernel, data type 2",
            ToolTip = "Write a type 2 segment to a C-kernel"
            ))
    static void ckw02(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int handle,
        double begtim,
        double endtim,
        int inst,
        const FString& ref,
        const FString& segid,
        const TArray<FSPointingType2Observation>& records
    );

    /// <summary>C-Kernel, write segment to C-kernel, data type 3</summary>
    /// <param name="handle">[in] Handle of an open CK file</param>
    /// <param name="begtim">[in] The beginning encoded SCLK of the segment</param>
    /// <param name="endtim">[in] The ending encoded SCLK of the segment</param>
    /// <param name="inst">[in] The NAIF instrument ID code</param>
    /// <param name="ref">[in] The reference frame of the segment</param>
    /// <param name="avflag">[in] True if the segment will contain angular velocity</param>
    /// <param name="segid">[in] Segment identifier</param>
    /// <param name="nrec">[in] Number of pointing records</param>
    /// <param name="sclkdp">[in] Encoded SCLK times</param>
    /// <param name="quats">[in] Quaternions representing instrument pointing</param>
    /// <param name="avvs">[in] Angular velocity vectors</param>
    /// <param name="nints">[in] Number of intervals</param>
    /// <param name="starts">[in] Encoded SCLK interval start times</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|CK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "POINTING, UTILITY",
            ShortToolTip = "C-Kernel, write segment to C-kernel, data type 3",
            ToolTip = "Add a type 3 segment to a C-kernel"
            ))
    static void ckw03(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int handle,
        double begtim,
        double endtim,
        int inst,
        const FString& ref,
        bool        avflag,
        const FString& segid,
        const TArray<FSPointingType1Observation>& records,
        const TArray<double>& starts
    );


    /// <summary>Write CK segment, type 5</summary>
    /// <param name="handle">[in] Handle of an open CK file</param>
    /// <param name="subtyp">[in] CK type 5 subtype code</param>
    /// <param name="degree">[in] Degree of interpolating polynomials</param>
    /// <param name="begtim">[in] The beginning encoded SCLK of the segment</param>
    /// <param name="endtim">[in] The ending encoded SCLK of the segment</param>
    /// <param name="inst">[in] The NAIF instrument ID code</param>
    /// <param name="ref">[in] The reference frame of the segment</param>
    /// <param name="avflag">[in] True if the segment will contain angular velocity</param>
    /// <param name="segid">[in] Segment identifier</param>
    /// <param name="n">[in] Number of packets</param>
    /// <param name="sclkdp">[in] Encoded SCLK times</param>
    /// <param name="packts">[in] Array of packets</param>
    /// <param name="rate">[in] Nominal SCLK rate in seconds per tick</param>
    /// <param name="nints">[in] Number of intervals</param>
    /// <param name="starts">[in] Encoded SCLK interval start times</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|CK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "POINTING, UTILITY",
            ShortToolTip = "Write CK segment, type 5",
            ToolTip = "Write a type 5 segment to a CK file"
            ))
    static void ckw05(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int handle,
        ES_CK05Subtype      subtyp,
        int                 degree,
        double begtim,
        double endtim,
        int inst,
        const FString& ref,
        bool  avflag,
        const FString& segid,
        const TArray<FSPointingType5Observation>& records,
        double rate,
        const TArray<double>& starts
    );

    /// <summary>Return the speed of light in a vacuum (IAU official value, in km / sec)</summary>
    /// <returns>The function returns the IAU official value for the speed of light in vacuo : 299792.458 km / sec</returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Constants",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "C, Speed of light in a vacuum",
            ToolTip = "Return the speed of light in a vacuum (IAU official value, in km / sec)"
            ))
    static void clight(FSSpeed& c);

    /**
      Determine the state (position, velocity) of an orbiting body
      from a set of elliptic, hyperbolic, or parabolic orbital
      elements.  See:
       https://naif.jpl.nasa.gov/pub/naif/toolkit_docs/C/cspice/conics_c.html
       Google: cspice conics_c
      */
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|Ephemeris",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONIC, EPHEMERIS",
            ShortToolTip = "Determine state from conic elements",
            ToolTip = "Determine the state (position, velocity) of an orbiting body from a set of elliptic, hyperbolic, or parabolic orbital elements"
            ))
    static void conics(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSConicElements& elts,
        const FSEphemerisTime& et,
        FSStateVector& state
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Coordinates",
        meta = (
            Keywords = "CONVERSION, COORDINATES",
            ShortToolTip = "Cylindrical to latitudinal",
            ToolTip = "Convert from cylindrical to latitudinal coordinates"
            ))
    static void cyllat(
        const FSCylindricalVector& cylvec,
        FSLatitudinalVector& latvec
    );

    /// <summary>Take a measurement X, the units associated with X, and units to which X should be converted; return Y-- -the value of the measurement in the output units.  Google cspice+convrt for units</summary>
    /// <param name="x">[in] Number representing a measurement in some units</param>
    /// <param name="in">[in] The units in which x is measured</param>
    /// <param name="out">[in] Desired units for the measurement</param>
    /// <param name="y">[out] The measurment in the desired units</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|Units",
        meta = (
            Keywords = "CONVERSION, UNITS",
            ExpandEnumAsExecs = "ResultCode",
            ToolTip = "Convert Units"
            ))
    static void convrt(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        double in_value,
        ES_Units            in,
        ES_Units            out,
        double& out_value
    );

    /// <summary>DAF add comments</summary>
    /// <param name="handle">[in] handle of a DAF opened with write access</param>
    /// <param name="n">[in] Number of comments to put into the comment area</param>
    /// <param name="lenvals">[in] Length of elements</param>
    /// <param name="buffer">[in] Buffer of comments to put into the comment area</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|DAF",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "FILES, UTILITY",
            ShortToolTip = "DAF add comments",
            ToolTip = "Add comments from a buffer of character strings to the comment area of a binary DAF file, appending them to any comments which are already present in the file's comment area"
            ))
    static void dafac(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int handle,                       // Todo, make a blue printable abstraction instead of handles!
        const TArray<FString>& comments
    );

    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|DSK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "COVERAGE, TOPOGRAPHY",
            ShortToolTip = "DSK, get object IDs",
            ToolTip = "Find the set of body ID codes of all objects for which topographic data are provided in a specified DSK file"
            ))
    static void dskobj(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<int>& bodids,
        const FString& fileRelativePath = TEXT("NonAssetData/kernels/phobos512.bds")
    );

    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|DSK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "COVERAGE, SURFACE, TOPOGRAPHY",
            ShortToolTip = "DSK, get surface IDs for body",
            ToolTip = "Find the set of surface ID codes for all surfaces associated with a given body in a specified DSK file"
            ))
    static void dsksrf(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<int>& srfids,
        const FString& fileRelativePath = TEXT("NonAssetData/kernels/phobos512.bds"),
        int bodyid = 401
    );

    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|DSK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "DAS, DSK, FILES",
            ShortToolTip = "DSK, fetch type 2 model size parameters",
            ToolTip = "Return plate model size parameters---plate count and vertex count-- - for a type 2 DSK segment"
            ))
    static void dskz02(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int& nv,
        int& np,
        int handle,
        const FSDLADescr& dladsc
    );

    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|DSK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "DAS, DSK, FILES",
            ShortToolTip = "DSK, fetch type 2 plate data",
            ToolTip = "Fetch triangular plates from a type 2 DSK segment"
            ))
    static void dskp02(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FSPlateIndices>& plates,
        int               handle,
        const FSDLADescr& dladsc,
        int               count,
        int               start = 1
    );

    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|DSK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "DAS, DSK, FILES",
            ShortToolTip = "DSK, type 2, compute normal vector for plate",
            ToolTip = "Compute the unit normal vector for a specified plate from a type 2 DSK segment"
            ))
    static void dskn02(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSDimensionlessVector& normal,
        int               handle,
        const FSDLADescr& dladsc,
        int               plid
    );

    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|DSK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "DAS, DSK, FILES",
            ShortToolTip = "DSK, fetch type 2 vertex data",
            ToolTip = "Fetch vertices from a type 2 DSK segment"
            ))
    static void dskv02(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FSDistanceVector>& vrtces,
        int               handle,
        const FSDLADescr& dladsc,
        int               count,
        int               start = 1
    );

    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|DSK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "GEOMETRY, INTERCEPT, SURFACE, TOPOGRAPHY",
            ShortToolTip = "DSK, ray-surface intercept, vectorized",
            ToolTip = "Compute ray-surface intercepts for a set of rays, using data provided by multiple loaded DSK segments"
            ))
    static void dskxv(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FSDistanceVector>& xptarr,
        TArray<bool>& fndarr,
        const TArray<int>& srflst,
        const FSEphemerisTime& et,
        const TArray <FSRay>& rayarray,
        const FString& target = TEXT("PHOBOS"),
        const FString& fixref = TEXT("IAU_PHOBOS")
    );

    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|DSK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "GEOMETRY, INTERCEPT, SURFACE, TOPOGRAPHY",
            ShortToolTip = "DSK, ray-surface intercept with source information",
            ToolTip = "Compute a ray-surface intercept using data provided by multiple loaded DSK segments. Return information about the source of the data defining the surface on which the intercept was found : DSK handle, DLA and DSK descriptors, and DSK data type-dependent parameters"
            ))
    static void dskxsi(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSDistanceVector& xpt,
        int& handle,
        FSDLADescr& dladsc,
        FSDSKDescr& dskdsc,
        TArray<double> dc,
        TArray<int> ic,
        bool& bFound,
        TArray<int> srflst,
        const FSEphemerisTime& et,
        const FSRay& ray,
        const FString& target,
        const FString& fixref
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Coordinates",
        meta = (
            Keywords = "CONVERSION, COORDINATES",
            ShortToolTip = "Cylindrical to rectangular",
            ToolTip = "Convert from cylindrical to rectangular coordinates"
            ))
    static void cylrec(
        const FSCylindricalVector& cylvec,
        FSDistanceVector& rectan
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Coordinates",
        meta = (
            Keywords = "CONVERSION, COORDINATES",
            ShortToolTip = "Cylindrical to spherical",
            ToolTip = "Convert from cylindrical to spherical coordinates"
            ))
    static void cylsph(
        const FSCylindricalVector& cylvec,
        FSSphericalVector& sphvec
    );

    /// <summary>DAF, close</summary>
    /// <param name="handle">[in] Handle of DAF to be closed</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|DAF",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "DAF, FILES",
            ShortToolTip = "DAF, close",
            ToolTip = "Close the DAF associated with a given handle"
            ))
        static void dafcls(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            int handle
        );

    /// <summary>DAF extract comments</summary>
    /// <param name="handle">[in] Handle of binary DAF opened with read access</param>
    /// <param name="bufsiz">[in] Maximum size, in lines, of buffer</param>
    /// <param name="lenout">[in] Length of strings in output buffer</param>
    /// <param name="n">[out] Number of extracted comment lines</param>
    /// <param name="buffer">[out] Buffer where extracted comment lines are placed</param>
    /// <param name="done">[out] Indicates whether all comments have been extracted</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|DAF",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "FILES, UTILITY",
            ShortToolTip = "Extract comments from the comment area of a binary DAF",
            ToolTip = "Close the DAF associated with a given handle"
            ))
        static void dafec(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            int handle,
            TArray<FString>& comments
        );

    /// <summary>DAF, open for read</summary>
    /// <param name="fname">[in] Name of DAF to be opened</param>
    /// <param name="handle">[in] Handle assigned to DAF</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|DAF",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "DAF, FILES",
            ShortToolTip = "DAF, open for read",
            ToolTip = ""
            ))
        static void dafopr(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            const FString& relativePath,
            int& handle
        );

     /// <summary>DAF, open for write</summary>
    /// <param name="fname">[in] Name of DAF to be opened</param>
    /// <param name="handle">[in] Handle assigned to DAF</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|DAF",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "DAF, FILES",
            ShortToolTip = "DAF, open for write",
            ToolTip = "Open a DAF file for writing"
            ))
    static void dafopw(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FString& relativePath,
        int& handle
    );

    /// <summary>DAF, open for write</summary>
    /// <param name="fname">[in] Name of DAF to be opened</param>
    /// <param name="handle">[in] Handle assigned to DAF</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|DAS",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "DAS, FILES",
            ShortToolTip = "DAS, open for read",
            ToolTip = "Open a DAS file for reading"
            ))
    static void dasopr(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FString& relativePath,
        int& handle
    );

    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|DAS",
        meta = (
            Keywords = "DAS, FILES",
            ToolTip = "Close a DAS file"
            ))
    static void dascls(
        int handle
    );


    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|DAS",
        meta = (
            ExpandEnumAsExecs = "FoundCode",
            Keywords = "DAS, DLA, FILES",
            ShortToolTip = "DLA, begin forward search",
            ToolTip = "Begin a forward segment search in a DLA file"
            ))
    static void dlabfs(
        int          handle,
        FSDLADescr& dladsc,
        ES_FoundCode& FoundCode
    );

    /// <summary>Return the value of Delta ET (ET-UTC) for an input epoch</summary>
    /// <param name="epoch">[in] Input epoch (seconds past J2000)</param>
    /// <param name="eptype">[in] Type of input epoch ("UTC" or "ET")</param>
    /// <param name="delta">[out] Delta ET (ET-UTC) at input epoch</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|Time",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "TIME",
            ShortToolTip = "Delta ET, ET - UTC",
            ToolTip = "Return the value of Delta ET (ET-UTC) for an input epoch"
            ))
    static void deltet(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        double epoch,
        ES_EpochType            eptype,
        FSEphemerisPeriod& delta
    );

    /// <summary>Compute the determinant of a double precision 3x3 matrix</summary>
    /// <param name="m1">Matrix whose determinant is to be found</param>
    /// <returns>the value of the determinant found by direct application of the definition of the determinan</returns>
    UFUNCTION(
        BlueprintPure,
        Category = "MaxQ|Math",
        meta = (
            ShortToolTip = "Determinant of a double precision 3x3 matrix",
            Keywords = "MATH, MATRIX",
            ToolTip = "Compute the determinant of a double precision 3x3 matrix"
            ))
    static void det(const FSRotationMatrix& m1, double& ReturnValue);

    /// <summary>Return the value of the largest (positive) number representable in a double precision variable</summary>
    /// <returns>largest (positive) number that can be represented in a double precision variable</returns>
    UFUNCTION(
        BlueprintPure,
        Category = "MaxQ|Constants",
        meta = (
            ShortToolTip = "Largest DP number",
            Keywords = "CONSTANTS",
            ToolTip = "Return the value of the largest (positive) number representable in a double precision variable"
            ))
    static void dpmax(double& doublePrecisionMax);

    /// <summary>Return the value of the smallest (negative) number representable in a double precision variable</summary>
    /// <returns>smallest (negative) number that can be represented in a double precision variable</returns>
    UFUNCTION(
        BlueprintPure,
        Category = "MaxQ|Constants",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Smallest DP number",
            ToolTip = "Return the value of the smallest (negative) number representable in a double precision variable"
            ))
    static void dpmin(double& doublePrecisionMin);

    /// <summary>Return the number of degrees per radian</summary>
    /// <returns>The value of pi is determined by the ACOS function</returns>
    UFUNCTION(
        BlueprintPure,
        Category = "MaxQ|Constants",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Degrees per radian",
            ToolTip = "Return the number of degrees per radian"
            ))
    static void dpr(double& degreesPerRadian);

    // https://naif.jpl.nasa.gov/pub/naif/toolkit_docs/C/cspice/eqncpv_c.html
    /// <summary>Compute the state (position and velocity of an object whose trajectory is described via equinoctial elements relative to some fixed plane(usually the equatorial plane of some planet) </summary>
    /// <param name="et">Epoch in seconds past J2000 to find state</param>
    /// <param name="epoch">Epoch of elements in seconds past J2000</param>
    /// <param name="eqel">Array of equinoctial elements</param>
    /// <param name="rapol">Right Ascension of the pole of the reference plane</param>
    /// <param name="decpol">Declination of the pole of the reference plane</param>
    /// <param name="state">State of the object described by eqel</param>
    /// <returns></returns>
    /// 
    UFUNCTION(
        BlueprintPure,
        Category = "MaxQ|SPK",
        meta = (
            Keywords = "EPHEMERIS",
            ShortToolTip = "Equinoctial Elements to position and velocity",
            ToolTip = "Compute the state (position and velocity of an object whose trajectory is described via equinoctial elements relative to some fixed plane(usually the equatorial plane of some planet)"
            ))
    static void eqncpv(
        const FSEphemerisTime& et,
        const FSEphemerisTime& epoch,
        const FSEquinoctialElements& eqel,
        const FSAngle& rapol,
        const FSAngle& decpol,
        FSStateVector& state
    );


    /// <summary>Given an ephemeris epoch, compute the local solar time for an object on the surface of a body at a specified longitude</summary>
    /// <param name="et">[in] Epoch in seconds past J2000 epoch</param>
    /// <param name="body">[in] ID-code of the body of interest</param>
    /// <param name="lon">[in] Longitude of surface point (RADIANS)</param>
    /// <param name="type">[in] Type of longitude "PLANETOCENTRIC", etc</param>
    /// <param name="timlen">[in] Available room in output time string</param>
    /// <param name="ampmlen">[in] Available room in output `ampm' string</param>
    /// <param name="hr">[out] Local hour on a "24 hour" clock</param>
    /// <param name="mn">[out] Minutes past the hour</param>
    /// <param name="sc">[out] Seconds past the minute</param>
    /// <param name="time">[out] String giving local time on 24 hour clock</param>
    /// <param name="ampm">[out] String giving time on A.M./ P.M. scale</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|Time",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "TIME",
            ShortToolTip = "ET to Local Solar Time",
            ToolTip = "Given an ephemeris epoch, compute the local solar time for an object on the surface of a body at a specified longitude"
            ))
    static void et2lst(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSEphemerisTime& et,
        int body,
        const FSAngle& lon,
        ES_LongitudeType type,
        int& hr,
        int& mn,
        int& sc,
        FString& time,
        FString& ampm
    );


    /// <summary>Convert an input time from ephemeris seconds past J2000 to Calendar, Day - of - Year, or Julian Date format, UTC</summary>
    /// <param name="et">[in] Input epoch, given in ephemeris seconds past J2000</param>
    /// <param name="format">[in] Format of output epoch</param>
    /// <param name="prec">[in] Digits of precision in fractional seconds or days</param>
    /// <param name="lenout">[in] The length of the output string plus 1</param>
    /// <param name="utcstr">[out] Output time string, UTC</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|Time",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "TIME",
            ShortToolTip = "Ephemeris Time to UTC",
            ToolTip = "Convert an input time from ephemeris seconds past J2000 to Calendar, Day - of - Year, or Julian Date format, UTC",
            AdvancedDisplay = "prec"
            ))
    static void et2utc(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSEphemerisTime& et,
        ES_UTCTimeFormat format,
        FString& utcstr,
        int prec = 4
    );


    /// <summary>Convert ET to Calendar format</summary>
    /// <param name="et">Equivalent UTC seconds past J2000</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Time",
        meta = (
            Keywords = "TIME",
            ShortToolTip = "Convert ET to Calendar format",
            ToolTip = "Convert from an ephemeris epoch measured in seconds past the epoch of J2000 to a calendar string format using a formal calendar free of leapseconds"
            ))
    static void etcal(const FSEphemerisTime& et, FString& ReturnValue);

    /// <summary>Construct a rotation matrix from a set of Euler angles</summary>
    /// <param name="angle3, angle2, angle1">[in] Rotation angles about third, second, and first rotation axes(radians)</param>
    /// <param name="axis3, axis2, axis1">[in] Axis numbers of third, second, and first rotation axes</param>
    /// <param name="o">Product of the 3 rotations</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|Rotation",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "MATRIX, ROTATION, TRANSFORMATION",
            ShortToolTip = "Euler angles to matrix",
            ToolTip = "Construct a rotation matrix from a set of Euler angles"
            ))
    static void eul2m(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSRotationMatrix& r,
        const FSAngle& angle3,
        const FSAngle& angle2,
        const FSAngle& angle1,
        ES_Axis axis3 = ES_Axis::X,
        ES_Axis axis2 = ES_Axis::Y,
        ES_Axis axis1 = ES_Axis::Z
    );

    /// <summary>This routine computes a state transformation from an Euler angle factorization of a rotationand the derivatives of those Euler angles</summary>
    /// <param name="eulang">An array of Euler angles and their derivatives</param>
    /// <param name="axisa">Axis A of the Euler angle factorization</param>
    /// <param name="axisb">Axis B of the Euler angle factorization</param>
    /// <param name="axisc">Axis C of the Euler angle factorization</param>
    /// <param name="xform">A state transformation matrix</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|Rotation",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "ANGLES, DERIVATIVES, STATE",
            ShortToolTip = "Euler angles and derivative to transformation",
            ToolTip = "computes a state transformation from an Euler angle factorization of a rotation and the derivatives of those Euler angles"
            ))
    static void eul2xf(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSEulerAngularState& eulang,
        FSEulerAngularTransform& xform
    );

    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|CK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EVENT, FOV, GEOMETRY, INSTRUMENT",
            ShortToolTip = "Is target in FOV at time?",
            ToolTip = "Determine if a specified ray is within the field-of-view (FOV) of a specified instrument at a given time"
            ))
    static void fovray(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        bool& visible,
        const FSEphemerisTime& et,
        const FSDimensionlessVector& raydir,
        const FString& inst = TEXT("CASSINI_UVIS_FUV_OCC"),
        const FString& rframe = TEXT("J2000"),
        ES_AberrationCorrectionFov abcorr = ES_AberrationCorrectionFov::S,
        const FString& observer = TEXT("Cassini")
    );

    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|CK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EVENT, FOV, GEOMETRY, INSTRUMENT",
            ShortToolTip = "Is target in FOV at time?",
            ToolTip = "Determine if a specified ephemeris object is within the field - of - view(FOV) of a specified instrument at a given time"
            ))
    static void fovtrg(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        bool& visible,
        const FSEphemerisTime& et,
        const FString& inst = TEXT("CASSINI_ISS_NAC"),
        const FString& target = TEXT("ENCELADUS"),
        ES_GeometricModel tshape = ES_GeometricModel::ELLIPSOID,
        const FString& tframe = TEXT("IAU_ENCELADUS"),
        ES_AberrationCorrectionWithTransmissions abcorr = ES_AberrationCorrectionWithTransmissions::CN_S,
        const FString& obsrvr = TEXT("Cassini")
    );

    /// <summary>Given a vector x, this routine builds a right handed orthonormal frame x, y, z where the output x is parallel to the input x</summary>
    /// <param name="x">[in/out]Input vector. A parallel unit vector on output</param>
    /// <param name="y">[out]Unit vector in the plane orthogonal to x</param>
    /// <param name="z">[out]Unit vector given by x X y</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Frames",
        meta = (
            Keywords = "AXES, FRAMES",
            ShortToolTip = "Build a right handed coordinate frame",
            ToolTip = "Given a vector x, this routine builds a right handed orthonormal frame x, y, z where the output x is parallel to the input x"
            ))
    static void frame(
        const FSDimensionlessVector& x_in,
        FSDimensionlessVector& x,
        FSDimensionlessVector& y,
        FSDimensionlessVector& z
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Frames",
        meta = (
            ExpandEnumAsExecs = "FoundCode",
            Keywords = "FRAMES",
            ShortToolTip = "Frame Information",
            ToolTip = "Retrieve the minimal attributes associated with a frame needed for converting transformations to and from it"
            ))
    static void frinfo(
        int  frcode,
        int& cent,
        int& frclss,
        int& clssid,
        ES_FoundCode& FoundCode
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Frames",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "FRAMES",
            ShortToolTip = "Frame to Name",
            ToolTip = "Retrieve the name of a reference frame associated with a SPICE ID code"
            ))
    static void frmnam(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int      frcode,
        FString& frname
    );

    /// <summary>Get character data from the kernel pool</summary>
    /// <param name="name">[in] Name of the variable whose value is to be returned</param>
    /// <param name="start">[in] Which component to start retrieving for name</param>
    /// <param name="room">[in] The largest number of values to return</param>
    /// <param name="lenout">[in] The length of the output string</param>
    /// <param name="n">[out] Number of values returned for name</param>
    /// <param name="cvals">[out] Values associated with name</param>
    /// <param name="found">[out] True if variable is in pool</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Kernel",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONSTANTS, FILES",
            ShortToolTip = "Get character data from the kernel pool",
            ToolTip = "Return the character value of a kernel variable from the kernel pool"
            ))
    static void gcpool(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FString>& cvals,
        bool& bFound,
        const FString& name = TEXT("PATH_VALUES"),
        int                 start = 0,
        int                 room = 1
    );

    /// <summary>Get d.p. values from the kernel pool</summary>
    /// <param name="name">[in] Name of the variable whose value is to be returned</param>
    /// <param name="start">[in] Which component to start retrieving for name</param>
    /// <param name="room">[in] The largest number of values to return</param>
    /// <param name="n">[out] Number of values returned for name</param>
    /// <param name="values">[out] Values associated with name</param>
    /// <param name="found">[out] True if variable is in pool</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly,
        Category = "MaxQ|Kernel",
        meta = (
            DeprecationMessage = "gdpool is obolete, use the K2Node_gdpool (gdpool) instead",
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONSTANTS, FILES",
            ShortToolTip = "Get d.p. values from the kernel pool",
            ToolTip = "Return the d.p. value of a kernel variable from the kernel pool"
            ))
    static void gdpool(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<double>& values,
        bool& bFound,
        const FString& name = TEXT("BODY514_NUT_PREC_RA"),
        int                 start = 0,
        int                 room = 7
    );

    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly,
        Category = "MaxQ|Deprecated",
        meta = (
            DeprecationMessage = "gdpool is obolete, use the K2Node_gdpool (gdpool) instead",
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONSTANTS, FILES",
            ShortToolTip = "Get SDistance values from the kernel pool",
            ToolTip = "Return the SDistance value of a kernel variable from the kernel pool"
            ))
    static void gdpool_scalar(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        double& value,
        bool& bFound,
        const FString& name = TEXT("BODY514_LONG_AXIS")
    );


    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly,
        Category = "MaxQ|Deprecated",
        meta = (
            DeprecationMessage = "gdpool is obolete, use the K2Node_gdpool (gdpool) instead",
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONSTANTS, FILES",
            ShortToolTip = "Get SDistance values from the kernel pool",
            ToolTip = "Return the SDistance value of a kernel variable from the kernel pool"
            ))
    static void gdpool_distance(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSDistance& value,
        bool& bFound,
        const FString& name = TEXT("BODY514_LONG_AXIS")
    );


    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly,
        Category = "MaxQ|Deprecated",
        meta = (
            DeprecationMessage = "gdpool is obolete, use the K2Node_gdpool (gdpool) instead",
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONSTANTS, FILES",
            ShortToolTip = "Get SDistanceVector values from the kernel pool",
            ToolTip = "Return the SDistanceVector value of a kernel variable from the kernel pool"
            ))
    static void gdpool_vector(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSDistanceVector& value,
        bool& bFound,
        const FString& name
    );


    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly,
        Category = "MaxQ|Deprecated",
        meta = (
            DeprecationMessage = "gdpool is obolete, use the K2Node_gdpool (gdpool) instead",
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONSTANTS, FILES",
            ShortToolTip = "Get SMassConstant values from the kernel pool",
            ToolTip = "Return the SMassConstant value of a kernel variable from the kernel pool"
            ))
    static void gdpool_mass(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSMassConstant& value,
        bool& bFound,
        const FString& name = TEXT("BODY399_GM")
    );


    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Ephemeris",
        meta = (
            Keywords = "EPHEMERIS",
            ToolTip = "Get geophysical constants for TLE"
            ))
    static void getgeophs(FSTLEGeophysicalConstants& geophs, const FString& id = TEXT("EARTH"));


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Parsing",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "PARSING, EPHEMERIS",
            AdvancedDisplay = "frstyr",
            ShortToolTip = "Get the components from two-line elements",
            ToolTip = "Given a the \"lines\" of a two-line element set, parse the lines and return the elements in units suitable for use in SPICE software"
            ))
    static void getelm(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSEphemerisTime& epoch,
        FSTwoLineElements& elems,
        const FString& firstLine,
        const FString& secondLine,
        int         frstyr = 1957
    );


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Ephemeris",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EPHEMERIS",
            ShortToolTip = "Evaluate \"two - line\" element data",
            ToolTip = "Evaluate NORAD two-line element data for earth orbiting spacecraft",
            AdvancedDisplay = "IgnoreBadMeanEccentricity"
            ))
    static void evsgp4(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSStateVector& state,
        const FSEphemerisTime& et,
        const FSTLEGeophysicalConstants& geophs,
        const FSTwoLineElements& elems,
        bool IgnoreBadMeanEccentricity = true
    );

    [[deprecated("Use evsgp4()")]]
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Ephemeris",
        meta = (
            DeprecatedFunction,
            DeprecationMessage = "Use evsgp4",
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EPHEMERIS",
            ToolTip = "Get state from TLE"
            ))
    static int ev2lin(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            const FSEphemerisTime& et,
            const FSTLEGeophysicalConstants& geophs,
            const FSTwoLineElements& elems,
            FSStateVector& state
        );

    /// <summary>Convert geodetic coordinates to rectangular coordinates</summary>
    /// <param name="lon">[in] Geodetic longitude of point (radians)</param>
    /// <param name="lat">[in] Geodetic latitude  of point (radians)</param>
    /// <param name="alt">[in] Altitude of point above the reference spheroid</param>
    /// <param name="re">[in] Equatorial radius of the reference spheroid</param>
    /// <param name="f">[in] Flattening coefficient</param>
    /// <param name="rectan">[out] Rectangular coordinates of point</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Coordinates",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONVERSION, COORDINATES",
            ShortToolTip = "Geodetic to rectangular coordinates",
            ToolTip = "Convert geodetic coordinates to rectangular coordinates"
            ))
    static void georec(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSGeodeticVector& geovec,
        const FSDistance& re,
        FSDistanceVector& rectan,
        double f = 0.00335281066474748071984552861852
    );


    /// <summary>
    //   Return the field-of-view (FOV) parameters for a specified 
    //   instrument.The instrument is specified by its NAIF ID code.
    /// </summary>
    /// <param name="instid"></param>
    /// <param name="shape"></param>
    /// <param name="frame"></param>
    /// <param name="bsight"></param>
    /// <param name="bounds"></param>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|NAIF IDs",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "FOV, INSTRUMENT",
            ToolTip = "Get instrument FOV parameters"
            ))
    static void getfov(
        ES_ResultCode&  ResultCode,
        FString&        ErrorMessage,
        int           instid,
        FString&        shape,
        FString&        frame,
        FSDimensionlessVector&          bsight,
        TArray<FSDimensionlessVector>&   bounds
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Kernel",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "KERNEL, UTILITY",
            ShortToolTip = "Get file architecture and type",
            ToolTip = "Determine the file architecture and file type of most SPICE kernel files"
            ))
    static void getfat(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FString& arch,
        FString& fype,
        const FString& fileRelativePath = TEXT("NonAssetData/kernels/pck00010.tpc")
    );


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Geometry Finder",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EPHEMERIS, EVENT, GEOMETRY, SEARCH, WINDOW",
            AutoCreateRefTerm = "adjust, refval",
            ShortToolTip = "GF, distance search",
            ToolTip = "Return the time window over which a specified constraint on observer - target distance is met"
            ))
    static void gfdist(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FSEphemerisTimeWindowSegment>& results,
        const TArray<FSEphemerisTimeWindowSegment>& cnfine,
        const FSEphemerisPeriod& step,
        const FSDistance& refval,
        const FSDistance& adjust,
        const FString& target = TEXT("MOON"),
        ES_AberrationCorrectionWithTransmissions abcorr = ES_AberrationCorrectionWithTransmissions::None,
        const FString& obsrvr = TEXT("EARTH"),
        ES_RelationalOperator relate = ES_RelationalOperator::GreaterThan
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Geometry Finder",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "ANGLE, EPHEMERIS, ILLUMINATION, LIGHTING, SEARCH",
            AutoCreateRefTerm = "adjust, refval",
            ShortToolTip = "GF, illumination angle search",
            ToolTip = "Return the time window over which a specified constraint on the observed phase, solar incidence, or emission angle at a specified target body surface point is met"
            ))
    static void gfilum(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FSEphemerisTimeWindowSegment>& results,
        const TArray<FSEphemerisTimeWindowSegment>& cnfine,
        const FSDistanceVector& spoint,
        const FSAngle& refval,
        const FSAngle& adjust,
        const FSEphemerisPeriod& step,
        ES_IlluminationAngleType angtyp = ES_IlluminationAngleType::INCIDENCE,
        const FString& target = TEXT("MARS"),
        const FString& illmn = TEXT("SUN"),
        const FString& fixref = TEXT("IAU_MARS"),
        ES_AberrationCorrectionWithNewtonians abcorr = ES_AberrationCorrectionWithNewtonians::CN_S,
        const FString& obsrvr = TEXT("MRO"),
        ES_RelationalOperator relate = ES_RelationalOperator::LessThan
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Geometry Finder",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EPHEMERIS, EVENT, GEOMETRY, SEARCH, WINDOW",
            ShortToolTip = "GF, phase angle search",
            ToolTip = "Determine time intervals for which a specified constraint on the phase angle between an illumination source, a target, and observer body centers is met",
            AutoCreateRefTerm = "refval, adjust"
            ))
    static void gfpa(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FSEphemerisTimeWindowSegment>& results,
        const TArray<FSEphemerisTimeWindowSegment>& cnfine,
        const FSAngle& refval,
        const FSAngle& adjust,
        const FSEphemerisPeriod& step,
        const FString& target = TEXT("MOON"),
        const FString& illmn = TEXT("SUN"),
        ES_AberrationCorrectionWithNewtonians abcorr = ES_AberrationCorrectionWithNewtonians::LT_S,
        const FString& obsrvr = TEXT("EARTH"),
        ES_RelationalOperator relate = ES_RelationalOperator::Equal
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Geometry Finder",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EVENT, GEOMETRY, SEARCH, WINDOW",
            ShortToolTip = "GF, find occultation",
            ToolTip = "Determine time intervals when an observer sees one target occulted by, or in transit across, another",
            AutoCreateRefTerm = "frontShapeSurfaces, backShapeSurfaces"
            ))
    static void gfoclt(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FSEphemerisTimeWindowSegment>& results,
        const TArray<FSEphemerisTimeWindowSegment>& cnfine,
        const FSEphemerisPeriod& step,
        const TArray<FString>& frontShapeSurfaces,
        const TArray<FString>& backShapeSurfaces,
        ES_OccultationType occtyp = ES_OccultationType::ANY,
        const FString& front = TEXT("MOON"),
        ES_GeometricModel frontShape = ES_GeometricModel::ELLIPSOID,
        const FString& frontframe = TEXT("IAU_MOON"),
        const FString& back = TEXT("SUN"),
        ES_GeometricModel backShape = ES_GeometricModel::ELLIPSOID,
        const FString& backFrame = TEXT("IAU_SUN"),
        ES_AberrationCorrectionForOccultation abcorr = ES_AberrationCorrectionForOccultation::CN,
        const FString& obsrvr = TEXT("EARTH")
    );


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Geometry Finder",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EVENT, GEOMETRY, SEARCH, SEPARATION",
            AdvancedDisplay = "nintvls",
            ShortToolTip = "GF, observer-target vector coordinate search",
            ToolTip = "Determine time intervals for which a coordinate of an observer - target position vector satisfies a numerical constraint"
            ))
    static void gfposc(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FSEphemerisTimeWindowSegment>& results,
        const FSEphemerisPeriod& step,
        const TArray<FSEphemerisTimeWindowSegment>& cnfine,
        const FString& target = TEXT("SUN"),
        const FString& frame = TEXT("IAU_EARTH"),
        ES_AberrationCorrectionWithTransmissions abcorr = ES_AberrationCorrectionWithTransmissions::None,
        const FString& obsrvr = TEXT("EARTH"),
        ES_CoordinateSystemInclRadec crdsys = ES_CoordinateSystemInclRadec::LATITUDINAL,
        ES_CoordinateName coord = ES_CoordinateName::LATITUDE,
        ES_RelationalOperator relate = ES_RelationalOperator::ABSMAX,
        double          refval = 0.,
        double          adjust = 0.,
        int nintvls = 750
    );


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Geometry Finder",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EVENT, FOV, GEOMETRY, INSTRUMENT, SEARCH, WINDOW",
            ShortToolTip = "GF, is target in FOV?",
            ToolTip = "Determine time intervals when a specified ephemeris object intersects the space bounded by the field - of - view(FOV) of a specified instrument."
            ))
    static void gftfov(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FSEphemerisTimeWindowSegment>& results,
        const FSEphemerisPeriod& step,
        const TArray<FSEphemerisTimeWindowSegment>& cnfine,
        const FString& inst = TEXT("CASSINI_ISS_NAC"),
        const FString& target = TEXT("PHOEBE"),
        ES_GeometricModel tshape = ES_GeometricModel::ELLIPSOID,
        const FString& tframe = TEXT("IAU_PHOEBE"),
        ES_AberrationCorrectionWithTransmissions abcorr = ES_AberrationCorrectionWithTransmissions::LT_S,
        const FString& obsrvr = TEXT("CASSINI")
    );


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Geometry Finder",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EVENT, FOV, GEOMETRY, INSTRUMENT, SEARCH, WINDOW",
            ShortToolTip = "GF, is ray in FOV?",
            ToolTip = "Determine time intervals when a specified ray intersects the space bounded by the field - of - view(FOV) of a specified instrument."
            ))
    static void gfrfov(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FSEphemerisTimeWindowSegment>& results,
        const FSDimensionlessVector& raydir,
        const FSEphemerisPeriod& step,
        const TArray<FSEphemerisTimeWindowSegment>& cnfine,
        const FString& inst = TEXT("CASSINI_ISS_NAC"),
        const FString& rframe = TEXT("IAU_PHOEBE"),
        ES_AberrationCorrectionFov abcorr = ES_AberrationCorrectionFov::S,
        const FString& obsrvr = TEXT("CASSINI")
    );


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Geometry Finder",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            AutoCreateRefTerm = "adjust, refval",
            Keywords = "EPHEMERIS, EVENT, GEOMETRY, SEARCH, WINDOW",
            ShortToolTip = "GF, range rate search",
            ToolTip = "Determine time intervals for which a specified constraint on the observer - target range rate is met"
            ))
    static void gfrr(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FSEphemerisTimeWindowSegment>& results,
        const TArray<FSEphemerisTimeWindowSegment>& cnfine,
        const FSEphemerisPeriod& step,
        const FSSpeed& refval,
        const FSSpeed& adjust,
        const FString& target = TEXT("MOON"),
        ES_AberrationCorrectionWithTransmissions abcorr = ES_AberrationCorrectionWithTransmissions::None,
        const FString& obsrvr = TEXT("EARTH"),
        ES_RelationalOperator relate = ES_RelationalOperator::GreaterThan
    );


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Geometry Finder",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            AutoCreateRefTerm = "adjust, refval",
            Keywords = "EVENT, GEOMETRY, SEARCH, SEPARATION",
            ShortToolTip = "GF, angular separation search",
            ToolTip = "Determine time intervals when the angular separation between the position vectors of two target bodies relative to an observer satisfies a numerical relationship"
            ))
    static void gfsep(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FSEphemerisTimeWindowSegment>& result,
        const TArray<FSEphemerisTimeWindowSegment>& cnfine,
        const FSAngle& refval,
        const FSAngle& adjust,
        const FSEphemerisPeriod& step,
        const FString& targ1 = TEXT("SUN"),
        ES_OtherGeometricModel shape1 = ES_OtherGeometricModel::POINT,
        const FString& targ2 = TEXT("MOON"),
        ES_OtherGeometricModel shape2 = ES_OtherGeometricModel::POINT,
        ES_AberrationCorrectionWithTransmissions abcorr = ES_AberrationCorrectionWithTransmissions::LT,
        const FString& obsrvr = TEXT("EARTH"),
        ES_RelationalOperator relate = ES_RelationalOperator::LessThan
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Geometry Finder",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EVENT, GEOMETRY, SEARCH, SEPARATION",
            ShortToolTip = "GF, surface intercept vector coordinate search",
            ToolTip = "Determine time intervals for which a coordinate of an surface intercept position vector satisfies a numerical constraint"
            ))
    static void gfsntc(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FSEphemerisTimeWindowSegment>& results,
        const TArray<FSEphemerisTimeWindowSegment>& cnfine,
        const FSDimensionlessVector& dvec,
        const FSEphemerisPeriod& step,
        double refval = 0.,
        double adjust = 0.,
        const FString& target = TEXT("EARTH"),
        const FString& fixref = TEXT("IAU_EARTH"),
        ES_AberrationCorrectionWithTransmissions abcorr = ES_AberrationCorrectionWithTransmissions::None,
        const FString& obsrvr = TEXT("SUN"),
        const FString& dref = TEXT("SEM"),
        ES_CoordinateSystemInclRadec crdsys = ES_CoordinateSystemInclRadec::LATITUDINAL,
        ES_CoordinateName coord = ES_CoordinateName::LATITUDE,
        ES_RelationalOperator relate = ES_RelationalOperator::Equal
    );


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Geometry Finder",
        meta = (
            Keywords = "GEOMETRY",
            ShortToolTip = "GF, set a tolerance value for GF",
            ToolTip = "Override the default GF convergence value used in the high level GF routines."
            ))
    static void gfstol(double value);


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Geometry Finder",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            AdvancedDisplay = "nintvls",
            Keywords = "COORDINATE, EVENT, GEOMETRY, SEARCH",
            ShortToolTip = "GF, subpoint vector coordinate search",
            ToolTip = "Determine time intervals for which a coordinate of an subpoint position vector satisfies a numerical constraint"
            ))
    static void gfsubc(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FSEphemerisTimeWindowSegment>& results,
        const FSEphemerisPeriod& step,
        const TArray<FSEphemerisTimeWindowSegment>& cnfine,
        double     refval = 0.,
        double     adjust = 0.,
        const FString& target = TEXT("EARTH"),
        const FString& fixref = TEXT("IAU_EARTH"),
        ES_SubpointComputationMethod method = ES_SubpointComputationMethod::NEAR_POINT_ELLIPSOID,
        ES_AberrationCorrectionWithTransmissions abcorr = ES_AberrationCorrectionWithTransmissions::None,
        const FString& obsrvr = TEXT("SUN"),
        ES_CoordinateSystemInclRadec crdsys = ES_CoordinateSystemInclRadec::GEODETIC,
        ES_CoordinateName coord = ES_CoordinateName::LATITUDE,
        ES_RelationalOperator relate = ES_RelationalOperator::GreaterThan,
        int nintvls = 100
    );

    /// <summary>Get integers from the kernel pool</summary>
    /// <param name="name">[in] Name of the variable whose value is to be returned</param>
    /// <param name="start">[in] Which component to start retrieving for `name'</param>
    /// <param name="room">[in] The largest number of values to return</param>
    /// <param name="n">[out] Number of values returned for `name'</param>
    /// <param name="ivals">[out] Values associated with `name'</param>
    /// <param name="found">[out] True if variable is in pool</param>
    /// <returns></returns>

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Kernel",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONSTANTS, FILES",
            ShortToolTip = "Get integers from the kernel pool",
            ToolTip = "Return the integer value of a kernel variable from the kernel pool"
            ))
    static void gipool(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<int>& ivals,
        bool& bFound,
        const FString& name,
        int             start,
        int             room
    );

    /// <summary>Get names of kernel pool variables</summary>
    /// <param name="name">[in] Template that names should match</param>
    /// <param name="start">[in] Index of first matching name to retrieve</param>
    /// <param name="room">[in] The largest number of values to return</param>
    /// <param name="lenout">[in] Length of strings in output array kvars</param>
    /// <param name="n">[out] Number of values returned for name</param>
    /// <param name="kvars">[out] Kernel pool variables whose names match name</param>
    /// <param name="found">[out] True if there is at least one match</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Kernel",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONSTANTS, FILES",
            ShortToolTip = "Get names of kernel pool variables",
            ToolTip = "Return names of kernel variables matching a specified template"
            ))
    static void gnpool(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FString>& kvars,
        bool& bFound,
        const FString& name = TEXT("BODY%%%_*"),
        int                 start = 0,
        int                 room = 100
    );

    /// <summary>Evaluate a Hermite interpolating polynomial at a specified abscissa value</summary>
    /// <param name="n">Number of points defining the polynomial</param>
    /// <param name="xvals">Abscissa values</param>
    /// <param name="yvals">Ordinate and derivative values</param>
    /// <param name="x">Point at which to interpolate the polynomial</param>
    /// <param name="work">Work space array, generally sized at number of elements in yvals times two</param>
    /// <param name="f">Interpolated function value at x</param>
    /// <param name="df">Interpolated function's derivative at x</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Polynomial",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "INTERPOLATION, POLYNOMIAL",
            ShortToolTip = "Hermite polynomial interpolation",
            ToolTip = "Evaluate a Hermite interpolating polynomial at a specified abscissa value"
            ))
    static void hrmint(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const TArray<double>& xvals,
        const TArray<double>& yvals,
        double x,
        double& f,
        double& df
    );

    /// <summary>Return half the value of pi</summary>
    /// <returns>half the value of pi</returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Constants", Exec,
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Half the value of pi",
            ToolTip = "Return half the value of pi"
            ))
    static void halfpi(double& half_pi);

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Constants",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Angle of half the value of pi",
            ToolTip = "Return angle of half the value of pi"
            ))
    static void halfpi_angle(FSAngle& half_pi);


    /// <summary>This routine returns the 3x3 identity matrix</summary>
    /// <returns>the 3x3 Identity matrix</returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Matrix",
        meta = (
            Keywords = "MATRIX",
            ShortToolTip = "Return the 3x3 identity matrix",
            ToolTip = "Returns the 3x3 identity matrix"
            ))
    static void ident(
        FSRotationMatrix& identity
    );


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Illumination",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            AutoCreateRefTerm = "surfaces",
            Keywords = "ANGLES, GEOMETRY, ILLUMINATION",
            ShortToolTip = "Illumination angles, general source, return flags",
            ToolTip = "Compute the illumination angles -phase, incidence, and emission- at a specified point on a target body.Return logical flags indicating whether the surface point is visible from the observer's position and whether the surface point is illuminated"
            ))
    static void illumf(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSEphemerisTime& trgepc,
        FSDistanceVector& srfvec,
        FSAngle& phase,
        FSAngle& incdnc,
        FSAngle& emissn,
        bool& visibl,
        bool& lit,
        const FSEphemerisTime& et,
        const FSDistanceVector& spoint,
        const TArray<FString>& surfaces,
        ES_GeometricModel method = ES_GeometricModel::ELLIPSOID,
        const FString& target = TEXT("MARS"),
        const FString& ilusrc = TEXT("SUN"),
        const FString& fixref = TEXT("IAU_MARS"),
        ES_AberrationCorrectionWithTransmissions abcorr = ES_AberrationCorrectionWithTransmissions::CN_S,
        const FString& obsrvr = TEXT("MGS")
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Illumination",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            AutoCreateRefTerm = "surfaces",
            Keywords = "ANGLES, GEOMETRY, ILLUMINATION",
            ShortToolTip = "Illumination angles, general source",
            ToolTip = "Find the illumination angles (phase, incidence, and emission) at a specified surface point of a target body"
            ))
    static void illumg(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSEphemerisTime& trgepc,
        FSDistanceVector& srfvec,
        FSAngle& phase,
        FSAngle& incdnc,
        FSAngle& emissn,
        const FSEphemerisTime& et,
        const FSDistanceVector& spoint,
        const TArray<FString>& surfaces,
        ES_GeometricModel method = ES_GeometricModel::ELLIPSOID,
        const FString& target = TEXT("MARS"),
        const FString& ilusrc = TEXT("SUN"),
        const FString& fixref = TEXT("IAU_MARS"),
        ES_AberrationCorrectionWithTransmissions abcorr = ES_AberrationCorrectionWithTransmissions::CN_S,
        const FString& obsrvr = TEXT("MGS")
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Illumination",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "ANGLES, GEOMETRY, ILLUMINATION",
            ShortToolTip = "Illumination angles",
            ToolTip = "Find the illumination angles (phase, solar incidence, and emission) at a specified surface point of a target body"
            ))
    static void ilumin(
        ES_ResultCode&          ResultCode,
        FString&                ErrorMessage,
        const FSDistanceVector& spoint,
        const FSEphemerisTime&  et,
        FSEphemerisTime&        trgepc,
        FSDistanceVector&       srfvec,
        FSAngle&                phase,
        FSAngle&                incdnc,
        FSAngle&                emissn,
        const FString&          method = TEXT("ELLIPSOID"),
        const FString&          target = TEXT("EARTH"),
        const FString&          fixref = TEXT("IAU_EARTH"),
        ES_AberrationCorrectionWithTransmissions abcorr = ES_AberrationCorrectionWithTransmissions::None,
        const FString&          obsrvr = TEXT("EARTH")
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Ellipses",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "ELLIPSE, GEOMETRY, MATH",
            ShortToolTip = "Intersection of ellipse and plane",
            ToolTip = "Find the intersection of an ellipse and a plane"
            ))
    static void inelpl(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSEllipse& ellips,
        const FSPlane& plane,
        TArray<FSDistanceVector>& xpts,
        bool& coplanar
    );

    /// <summary>Return the value of the largest (positive) number representable in a SpiceInt variable</summary>
    /// <returns>the largest (positive) number that can be represented in a SpiceInt variable</returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Constants",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Largest integer number",
            ToolTip = "Value of the largest (positive) number representable in a SpiceInt variable"
            ))
    static void intmax(int& int_max);

    /// <summary>Return the value of the smallest (negative) number representable in a SpiceInt variable</summary>
    /// <returns>the smallest (negative) number that can be represented in a SpiceInt variable</returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Constants",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Smallest integer number",
            ToolTip = "Value of the smallest(negative) number representable in a SpiceInt variable"
            ))
    static void intmin(int& int_min);

    /// <summary>Generate the inverse of a 3x3 matrix</summary>
    /// <param name="m1">[in] Matrix to be inverted</param>
    /// <param name="mout">[out] Inverted matrix (m1)**-1.  If m1 is singular, then mout will be the zero matrix.mout can overwrite m1</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|Matrix",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "MATH, MATRIX",
            ShortToolTip = "Invert a 3x3 matrix",
            ToolTip = "Generate the inverse of a 3x3 matrix"
            ))
    static void invert(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSRotationMatrix& m1,
        FSRotationMatrix& mout
    );


    /// <summary>Given a matrix, construct the matrix whose rows are the columns of the first divided by the length squared of the the corresponding columns of the input matrix</summary>
    /// <param name="m">A 3x3 matrix</param>
    /// <param name="mit">m after transposition and scaling of rows</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|Matrix",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "MATRIX",
            ShortToolTip = "Invert nearly orthogonal matrices",
            ToolTip = "Given a matrix, construct the matrix whose rows are the columns of the first divided by the length squared of the the corresponding columns of the input matrix"
            ))
    static void invort(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSRotationMatrix& m,
        FSRotationMatrix& mit
    );

    UFUNCTION(
        BlueprintPure,
        Category = "MaxQ|Matrix",
        meta = (
            Keywords = "MATRIX",
            ShortToolTip = "Inverse of state transformation matrix",
            ToolTip = "Return the inverse of a state transformation matrix"
            ))
    static void invstm(
        const FSStateTransform& xform,
        FSStateTransform& inverseXform
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Constants|Besselian Dates",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Besselian Date 1900.0",
            ToolTip = "Return the Julian Date corresponding to Besselian Date 1900.0"
            ))
    static void b1900(
        double& Seconds
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Constants|Besselian Dates",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Besselian Date 1950.0",
            ToolTip = "Return the Julian Date corresponding to Besselian Date 1950.0 (double seconds)"
            ))
    static void b1950(
        double& Seconds
    );

    /// <summary>Return the Julian Date of 1899 DEC 31 12:00:00 (1900 JAN 0.5)</summary>
    /// <returns>the Julian Date of 1899 DEC 31 12:00:00 (1900 JAN 0.5)</returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Constants|Julian Dates",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Julian Date of 1900.0 JAN 0.5",
            ToolTip = "Return the Julian Date of 1899 DEC 31 12:00:00 (1900 JAN 0.5) (double seconds)"
            ))
    static void j1900(
        double& Seconds
    );

    /// <summary>Return the Julian Date of 1950 JAN 01 00:00:00 (1950 JAN 1.0)</summary>
    /// <returns>the Julian Date of 1950 JAN 01 00:00:00 (1950 JAN 1.0) </returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Constants|Julian Dates",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Julian Date of 1950.0 JAN 1.0",
            ToolTip = "Return the Julian Date of 1950 JAN 01 00:00:00 (1950 JAN 1.0) (double seconds)"
            ))
    static void j1950(double& Seconds);

    /// <summary>Return the Julian Date of 2000 JAN 01 12:00:00 (2000 JAN 1.5)</summary>
    /// <returns>the Julian Date of 2000 JAN 01 12:00:00 (2000 JAN 1.5)</returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|CONSTANTS|Julian Dates",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Julian Date of 2000 JAN 1.5",
            ToolTip = "Return the Julian Date of 2000 JAN 01 12:00:00 (2000 JAN 1.5)"
            ))
    static void j2000(double& Seconds);

    /// <summary>Return the Julian Date of 2100 JAN 01 12:00:00 (2100 JAN 1.5)</summary>
    /// <returns>the Julian Date of 2100 JAN 01 12:00:00 (2100 JAN 1.5)</returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Constants|Julian Dates",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Julian Date of 2100 JAN 1.5",
            ToolTip = "Return the Julian Date of 2100 JAN 01 12:00:00 (2100 JAN 1.5)"
            ))
    static void j2100(double& Seconds);

    /// <summary>Return the number of seconds in a julian year</summary>
    /// <returns>The number of seconds/julian year</returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Constants|Julian Dates",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Seconds per julian year",
            ToolTip = "Return the number of seconds in a julian year"
            ))
    static void jyear(double& secondsPerJulianYear);

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Constants|Julian Dates",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Seconds per tropical year",
            ToolTip = "Return the number of seconds in a tropical year"
            ))
    static void tyear(double& secondsPerTropicalYear);

    [[deprecated("Use SEphemerisPeriod JulianYear")]]
    UFUNCTION(BlueprintPure,
        DeprecatedFunction,
        DeprecationMessage = "Use SEphemerisPeriod JulianYear",
        ExpandEnumAsExecs = "ResultCode",
        Category = "MaxQ|Constants",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Ephemeris Period of one julian year",
            ToolTip = "Return the number of seconds in a julian year"
            ))
    static void jyear_period(FSEphemerisPeriod& oneJulianYear);

    [[deprecated("Use SEphemerisPeriod TropicalYear")]]
    UFUNCTION(BlueprintPure,
        DeprecatedFunction,
        DeprecationMessage = "Use SEphemerisPeriod TropicalYear",
        Category = "MaxQ|Constants",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Ephemeris Period of one tropical year",
            ToolTip = "Return the number of seconds in a tropical year"
            ))
    static void tyear_period(FSEphemerisPeriod& oneTropicalYear);

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Kernel",
        meta = (
            ExpandEnumAsExecs = "FoundCode",
            ShortToolTip = "Kernel Data",
            ToolTip = "Return data for the nth kernel that is among a list of specified kernel types"
            ))
    static void kdata(
        ES_FoundCode& FoundCode,
        UPARAM(Meta = (ToolTip = "The relative path of the kernel file")) FString& file,
        UPARAM(Meta = (ToolTip = "The type of the kernel")) ES_KernelType& filtyp,
        UPARAM(Meta = (ToolTip = "Name of the source file used to load 'file'")) FString& srcfil,
        UPARAM(Meta = (ToolTip = "The handle attached to 'file'")) int& handle,
        UPARAM(Meta = (ToolTip = "The kinds of kernels to count", Bitmask, BitmaskEnum = "ES_KernelType")) int32 kind = 0x7f,
        UPARAM(Meta = (ToolTip = "Index of kernel to fetch from the list of kernels")) int which = 0
        );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Kernel",
        meta = (
            ExpandEnumAsExecs = "FoundCode",
            ShortToolTip = "Kernel Information",
            ToolTip = "Return information about a loaded kernel specified by name"
            ))
    static void kinfo(
        UPARAM(Meta = (ToolTip = "The type of the kernel")) ES_KernelType& filtyp,
        UPARAM(Meta = (ToolTip = "Name of the source file used to load 'file'")) FString& srcfil,
        UPARAM(Meta = (ToolTip = "The handle attached to 'file'")) int& handle,
        ES_FoundCode& FoundCode,
        UPARAM(Meta = (ToolTip = "Name of a kernel to fetch information for")) const FString& file = TEXT("NonAssetData/kernels/pck00010.tpc")
        );

    UFUNCTION(BlueprintCallable,
    Category = "MaxQ|Kernel",
    meta = (
        ShortToolTip = "Kernel Totals",
        ToolTip = "Return the number of kernels of a specified type that are currently loaded via the Furnsh action."
        ))
    static void ktotal(
        UPARAM(Meta = (ToolTip = "The number of kernels of type 'kind'")) int& count,
        UPARAM(Meta = (ToolTip = "The kinds of kernels to count", Bitmask, BitmaskEnum = "ES_KernelType")) int32 kind = 0x7f
        );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Coordinates",
        meta = (
            Keywords = "CONVERSION, COORDINATES",
            ShortToolTip = "Latitudinal to cylindrical coordinates",
            ToolTip = "Convert from latitudinal coordinates to cylindrical coordinates"
            ))
    static void latcyl(
        const FSLatitudinalVector& latvec,
        FSCylindricalVector& cylvec
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Coordinates",
        meta = (
            Keywords = "CONVERSION, COORDINATES",
            ShortToolTip = "Latitudinal to rectangular coordinates",
            ToolTip = "Convert from latitudinal coordinates to rectangular coordinates"
            ))
    static void latrec(
        const FSLatitudinalVector& vec,
        FSDistanceVector& rectan
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Coordinates",
        meta = (
            Keywords = "CONVERSION, COORDINATES",
            ShortToolTip = "Latitudinal to spherical coordinates",
            ToolTip = "Convert from latitudinal coordinates to spherical coordinates"
            ))
    static void latsph(
        const FSLatitudinalVector& latvec,
        FSSphericalVector& sphVec
    );


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|DSK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "COORDINATES, DSK, GEOMETRY, SURFACE",
            AutoCreateRefTerm = "shapeSurfaces, et",
            ShortToolTip = "Latitudinal grid to surface points",
            ToolTip = "Map array of planetocentric longitude/latitude coordinate pairs to surface points on a specified target body. The surface of the target body may be represented by a triaxial ellipsoid or by topographic data provided by DSK files."
            ))
    static void latsrf(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FSDistanceVector>& srfpts,
        const FSEphemerisTime& et,
        const TArray<FSLonLat>& lonlat,
        const TArray<FString>& shapeSurfaces,
        ES_GeometricModel method = ES_GeometricModel::DSK,
        const FString& target = TEXT("PHOBOS"),
        const FString& fixref = TEXT("IAU_PHOBOS")
    );


    /// <summary>Evaluate a Lagrange interpolating polynomial for a specified set of coordinate pairs, at a specified abscissa value. Return the value of both polynomialand derivative</summary>
    /// <param name="n">[in] Number of points defining the polynomial</param>
    /// <param name="[in] xvals">Abscissa values</param>
    /// <param name="[in] yvals">Ordinate values</param>
    /// <param name="[in] work">Work space array, n x 2 work space array, where n is the same dimension as xvals and yvals</param>
    /// <param name="x">[in] Point at which to interpolate the polynomial</param>
    /// <param name="p">[out] Polynomial value at x</param>
    /// <param name="dp">[out] Polynomial derivative at x</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Polynomial",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "INTERPOLATION, POLYNOMIAL",
            ShortToolTip = "Lagrange polynomial interpolation with derivative",
            ToolTip = "Evaluate a Lagrange interpolating polynomial for a specified set of coordinate pairs, at a specified abscissa value.Return the value of both polynomialand derivative"
            ))
    static void lgrind(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const TArray<double>& xvals,
        const TArray<double>& yvals,
        double x,
        double& p,
        double& dp
    );


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|DSK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            AdvancedDisplay = "maxn",
            Keywords = "GEOMETRY",
            ShortToolTip = "Limb points on an extended object",
            ToolTip = "Find limb points on a target body. The limb is the set of points of tangency on the target of rays emanating from the observer. The caller specifies half - planes bounded by the observer - target center vector in which to search for limb points. The surface of the target body may be represented either by a triaxial ellipsoid or by topographic data."
            ))
    static void limbpt(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FSLimptCut>& cuts,
        const FSEphemerisTime& et,
        const FSDimensionlessVector& refvec,
        const FSAngle& rolstp,
        int        ncuts,
        const FSAngle& schstp,
        const FSAngle& soltol,
        const TArray<FString>& shapeSurfaces,
        ES_LimbComputationMethod method = ES_LimbComputationMethod::TANGENT_DSK,
        const FString& target = TEXT("PHOBOS"),
        const FString& fixref = TEXT("IAU_PHOBOS"),
        ES_AberrationCorrectionWithNewtonians abcorr = ES_AberrationCorrectionWithNewtonians::CN_S,
        ES_AberrationCorrectionLocus corloc = ES_AberrationCorrectionLocus::CENTER,
        const FString& obsrvr = TEXT("MARS"),
        int maxn = 10000
    );

    /// <summary>Compute L_s, the planetocentric longitude of the sun, as seen from a specified body</summary>
    /// <param name="body">[in] Name of central body</param>
    /// <param name="et">[in] Epoch in seconds past J2000 TDB</param>
    /// <param name="abcorr">[in] Aberration correction ["NONE", "LT", "LT+S"]</param>
    /// <returns>the value of L_s for the specified body at the specified time</returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SPK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "GEOMETRY, TIME",
            ShortToolTip = "Longitude of the sun, planetocentric",
            ToolTip = "Compute L_s, the planetocentric longitude of the sun, as seen from a specified body"
            ))
    static void lspcn(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FString& body,
        const FSEphemerisTime& et,
        ES_AberrationCorrection abcorr,
        FSAngle& lon
    );

    /// <summary>Set one double precision 3x3 matrix equal to another</summary>
    /// <param name="m1">[in] Input matrix</param>
    /// <param name="mout">[out] Output matrix equal to m1</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintPure,
        Category = "MaxQ|Matrix",
        meta = (
            Keywords = "ASSIGNMENT, MATRIX",
            ShortToolTip = "Matrix equal to another, 3x3",
            ToolTip = "Set one double precision 3x3 matrix equal to another"
            ))
    static void mequ(
        const FSRotationMatrix& m1,
        FSRotationMatrix& mout
    );

#if 0
    /// <summary>Multiply a matrix and a vector of arbitrary size</summary>
    /// <param name="m1">[in] Left-hand matrix to be multiplied</param>
    /// <param name="v2">[in] Right-hand vector to be multiplied</param>
    /// <param name="nrow1">[in] Row dimension of m1 and length of vout</param>
    /// <param name="nc1r2">[in] Column dimension of m1 and length of v2</param>
    /// <param name="vout">[out] Product vector m1*v2</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable, Category = "MaxQ", meta = (ToolTip = ""))
        static void mxvg(
            const void* m1,
            const void* v2,
            SpiceInt       nrow1,
            SpiceInt       nc1r2,
            void* vout
        );
#endif


    UFUNCTION(BlueprintPure, Category = "MaxQ|Math", meta = (ToolTip = "multiplies state by state transform"))
    static void mxv_state(
        const FSStateTransform& m,
        const FSStateVector& statein,
        FSStateVector&  stateout
    );


    UFUNCTION(BlueprintPure, Category = "MaxQ|Math", meta = (ToolTip = "multiplies state by state transform transpose"))
    static void mtxv_state(
        const FSStateTransform& m,
        const FSStateVector& statein,
        FSStateVector& stateout
    );

    /// <summary>Find a unit quaternion corresponding to a specified rotation matrix</summary>
    /// <param name="r">[in] Rotation matrix</param>
    /// <param name="q">[out] unit quaternion representing 'r'</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Matrix",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "MATRIX, VECTOR",
            ShortToolTip = "Matrix to quaternion",
            ToolTip = "Find a unit quaternion corresponding to a specified rotation matrix"
            ))
        static void m2q(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            const FSRotationMatrix& r,
            FSQuaternion& q
        );

    /// <summary>Multiply two 3x3 matrices</summary>
    /// <param name="m1">[in] 3x3 double precision matrix</param>
    /// <param name="m2">[in] 3x3 double precision matrix</param>
    /// <param name="mout">[out] 3x3 double precision matrix. mout is the product m1* m2</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Matrix",
        meta = (
            Keywords = "MATRIX",
            ShortToolTip = "Matrix times matrix, 3x3",
            ToolTip = "Multiply two 3x3 matrices"
            ))
    static void mxm(
        const FSRotationMatrix& m1,
        const FSRotationMatrix& m2,
        FSRotationMatrix& mout
    );


    /// <summary"Matrix times matrix transpose, 3x3</summary>
    /// <param name="m1">[in] 3x3 double precision matrix</param>
    /// <param name="m2">[in] 3x3 double precision matrix</param>
    /// <param name="mout">[out] The product m1 times m2 transpose</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Matrix",
        meta = (
            Keywords = "MATRIX",
            ShortToolTip = "Matrix times matrix transpose, 3x3",
            ToolTip = "multiplies a 3x3 matrix and the transpose of another 3x3 matrix"
            ))
    static void mxmt(
        const FSRotationMatrix& m1,
        const FSRotationMatrix& m2,
        FSRotationMatrix& mout
    );


    /// <summary>Matrix transpose times vector, 3x3</summary>
    /// <param name="m1">[in] 3x3 double precision matrix</param>
    /// <param name="vin">[in] 3-dimensional double precision vector</param>
    /// <param name="vout">[out] -dimensional double precision vector. vout is the product m1** t* vin</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintPure,
        Category = "MaxQ|Matrix",
        meta = (
            Keywords = "MATRIX, VECTOR",
            ShortToolTip = "Matrix transpose times vector, 3x3",
            ToolTip = "mtxv multiplies the transpose of a 3x3 matrix on the left with a vector below"
            ))
    static void mtxv(
        const FSRotationMatrix& m1,
        const FSDimensionlessVector& vin,
        FSDimensionlessVector& vout
    );


    /// <summary>Matrix transpose times vector, 3x3</summary>
    /// <param name="m1">[in] 3x3 double precision matrix</param>
    /// <param name="vin">[in] 3-dimensional double precision vector</param>
    /// <param name="vout">[out] -dimensional double precision vector. vout is the product m1** t* vin</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintPure,
        Category = "MaxQ|Matrix",
        meta = (
            Keywords = "MATRIX, VECTOR",
            ShortToolTip = "Matrix transpose times vector, 3x3",
            ToolTip = "mtxv multiplies the transpose of a 3x3 matrix on the left with a vector below"
            ))
    static void mtxv_distance(
        const FSRotationMatrix& m1,
        const FSDistanceVector& vin,
        FSDistanceVector& vout
    );


    /// <summary>Matrix transpose times vector, 3x3</summary>
    /// <param name="m1">[in] 3x3 double precision matrix</param>
    /// <param name="vin">[in] 3-dimensional double precision vector</param>
    /// <param name="vout">[out] -dimensional double precision vector. vout is the product m1** t* vin</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintPure,
        Category = "MaxQ|Matrix",
        meta = (
            Keywords = "MATRIX, VECTOR",
            ShortToolTip = "Matrix transpose times vector, 3x3",
            ToolTip = "mtxv multiplies the transpose of a 3x3 matrix on the left with a vector below"
            ))
    static void mtxv_velocity(
        const FSRotationMatrix& m1,
        const FSVelocityVector& vin,
        FSVelocityVector& vout
    );

    /// <summary>Matrix transpose times vector, 3x3</summary>
    /// <param name="m1">[in] 3x3 double precision matrix</param>
    /// <param name="vin">[in] 3-dimensional double precision vector</param>
    /// <param name="vout">[out] -dimensional double precision vector. vout is the product m1** t* vin</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintPure,
        Category = "MaxQ|Matrix",
        meta = (
            Keywords = "MATRIX, VECTOR",
            ShortToolTip = "Matrix transpose times vector, 3x3",
            ToolTip = "mtxv multiplies the transpose of a 3x3 matrix on the left with a vector below"
            ))
    static void mtxv_angular(
        const FSRotationMatrix& m1,
        const FSAngularVelocity& vin,
        FSAngularVelocity& vout
    );

    UFUNCTION(
        BlueprintPure,
        Category = "MaxQ|Matrix",
        meta = (
            Keywords = "MATRIX, VECTOR",
            ShortToolTip = "Matrix times vector, 3x3",
            ToolTip = "mtxv multiplies a 3x3 matrix on the left with a vector below"
            ))
    static void mxv_angular(
        const FSRotationMatrix& m1,
        const FSAngularVelocity& vin,
        FSAngularVelocity& vout
    );

    /// <summary>Matrix transpose times matrix, 3x3</summary>
    /// <param name="m1">[in] 3x3 double precision matrix</param>
    /// <param name="m2">[in] 3x3 double precision matrix</param>
    /// <param name="mout">[out] The produce m1 transpose times m2</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Matrix",
        meta = (
            Keywords = "MATRIX",
            ShortToolTip = "Matrix transpose times matrix, 3x3",
            ToolTip = "Multiply the transpose of a 3x3 matrix and a 3x3 matrix"
            ))
    static void mtxm(
        const FSRotationMatrix& m1,
        const FSRotationMatrix& m2,
        FSRotationMatrix& mout
    );

    /// <summary>Factor a rotation matrix as a product of three rotations about specified coordinate axes</summary>
    /// <param name="r">[in] Rotation matrix to be factored</param>
    /// <param name="[out] axis1, axis2, axis3">Numbers of third, second, and first rotation axes</param>
    /// <param name="[out] angle1, angle2, angle3">Third, second, and first Euler angles, in radians</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Rotation",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "ANGLE, MATRIX, ROTATION, TRANSFORMATION",
            ShortToolTip = "Matrix to Euler angles",
            ToolTip = "Factor a rotation matrix as a product of three rotations about specified coordinate axes"
            ))
        static void m2eul(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            FSEulerAngles& eulers,
            const FSRotationMatrix& r,
            ES_Axis axis3 = ES_Axis::X,
            ES_Axis axis2 = ES_Axis::Y,
            ES_Axis axis1 = ES_Axis::Z
        );


    /// <summary>Multiply a 3x3 double precision matrix with a 3-dimensional double precision vector</summary>
    /// <param name="m1">[in] 3x3 double precision matrix</param>
    /// <param name="vin">[in] 3-dimensional double precision vector</param>
    /// <param name="vout">[out] 3-dimensinoal double precision vector. vout is the product m1* vin</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintPure,
        Category = "MaxQ|Matrix",
        meta = (
            Keywords = "MATRIX, VECTOR",
            ShortToolTip = "Matrix times vector, 3x3",
            ToolTip = "Multiply a 3x3 double precision matrix with a 3-dimensional double precision vector"
            ))
        static void mxv_distance(
            const FSRotationMatrix& m1,
            const FSDistanceVector& vin,
            FSDistanceVector& vout
        );

    /// <summary>Multiply a 3x3 double precision matrix with a 3-dimensional double precision vector</summary>
    /// <param name="m1">[in] 3x3 double precision matrix</param>
    /// <param name="vin">[in] 3-dimensional double precision vector</param>
    /// <param name="vout">[out] 3-dimensinoal double precision vector. vout is the product m1* vin</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintPure,
        Category = "MaxQ|Matrix",
        meta = (
            Keywords = "MATRIX, VECTOR",
            ShortToolTip = "Matrix times vector, 3x3",
            ToolTip = "Multiply a 3x3 double precision matrix with a 3-dimensional double precision vector"
            ))
    static void mxv_velocity(
        const FSRotationMatrix& m1,
        const FSVelocityVector& vin,
        FSVelocityVector& vout
    );

    /// <summary>Multiply a 3x3 double precision matrix with a 3-dimensional double precision vector</summary>
    /// <param name="m1">[in] 3x3 double precision matrix</param>
    /// <param name="vin">[in] 3-dimensional double precision vector</param>
    /// <param name="vout">[out] 3-dimensinoal double precision vector. vout is the product m1* vin</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintPure,
        Category = "MaxQ|Matrix",
        meta = (
            Keywords = "MATRIX, VECTOR",
            ShortToolTip = "Matrix times vector, 3x3",
            ToolTip = "Multiply a 3x3 double precision matrix with a 3-dimensional double precision vector"
            ))
        static void mxv(
            const FSRotationMatrix& m1,
            const FSDimensionlessVector& vin,
            FSDimensionlessVector& vout
        );


    /// <summary>
    /// Look up the frame ID code associated with a string.
    /// </summary>
    /// <param name="frname">[in] The name of some reference frame</param>
    /// <param name="frcode">[out] The SPICE ID code of the frame</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Frames",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "FRAMES",
            ShortToolTip = "Name to frame",
            ToolTip = "Look up the frame ID code associated with a string"
            ))
        static void namfrm(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            const FString& frname,
            int& frcode
        );

    /// <summary>Find the nearest point on an ellipse to a specified point, both in three - dimensional space, and find the distance between the ellipseand the point</summary>
    /// <param name="point">[in] Point whose distance to an ellipse is to be found</param>
    /// <param name="ellips">[in] A CSPICE ellipse</param>
    /// <param name="pnear">[out] Nearest point on ellipse to input point</param>
    /// <param name="dist">[out] Distance of input point to ellipse</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Ellipses",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONIC, ELLIPSE, GEOMETRY, MATH",
            ShortToolTip = "Nearest point on ellipse to point",
            ToolTip = "Find the nearest point on an ellipse to a specified point, both in three - dimensional space, and find the distance between the ellipse and the point"
            ))
        static void npelpt(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            const FSDistanceVector& point,
            const FSEllipse& ellips,
            FSDistanceVector& pnear,
            FSDistance& dist
        );

    /// <summary>This routine locates the point on the surface of an ellipsoid that is nearest to a specified position.It also returns the altitude of the position above the ellipsoid</summary>
    /// <param name="positn">Position of a point in bodyfixed frame</param>
    /// <param name="a">[in] Length of semi-axis parallel to x-axis</param>
    /// <param name="b">[in] Length of semi-axis parallel to y-axis</param>
    /// <param name="c">[in] Length on semi-axis parallel to z-axis</param>
    /// <param name="npoint">[out] Point on the ellipsoid closest to positn</param>
    /// <param name="alt">[out] Altitude of positn above the ellipsoid</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Ellipsoid",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "ELLIPSOID, GEOMETRY",
            ShortToolTip = "Nearest point on an ellipsoid",
            ToolTip = "Locates the point on the surface of an ellipsoid that is nearest to a specified position.It also returns the altitude of the position above the ellipsoid"
            ))
        static void nearpt(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            const FSDistanceVector& positn,
            const FSDistance& a,
            const FSDistance& b,
            const FSDistance& c,
            FSDistanceVector& npoint,
            FSDistance& alt
        );

    /// <summary>Find nearest point on a triaxial ellipsoid to a specified line, and the distance from the ellipsoid to the line</summary>
    /// <param name="a">[in] Length of ellipsoid's semi-axis in the x direction</param>
    /// <param name="b">[in] Length of ellipsoid's semi-axis in the y direction</param>
    /// <param name="c">[in] Length of ellipsoid's semi-axis in the z direction</param>
    /// <param name="linept">[in] Point on line</param>
    /// <param name="linedr">[in] Direction vector of line</param>
    /// <param name="pnear">[out] Nearest point on ellipsoid to line</param>
    /// <param name="dist">[out] Distance of ellipsoid from line</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Ellipses",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "ELLIPSOID, GEOMETRY, MATH",
            ShortToolTip = "Nearest point on ellipsoid to line",
            ToolTip = "Find nearest point on a triaxial ellipsoid to a specified line, and the distance from the ellipsoid to the line"
            ))
        static void npedln(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            const FSDistance& a,
            const FSDistance& b,
            const FSDistance& c,
            const FSDistanceVector& linept,
            const FSDimensionlessVector& linedr,
            FSDistanceVector& pnear,
            FSDistance& dist
        );

    /// <summary>Find the nearest point on a line to a specified point, and find the distance between the two points</summary>
    /// <param name="linpt">[in] Point on a line</param>
    /// <param name="lindir">[in] The line's direction vector</param>
    /// <param name="point">[in] A second point</param>
    /// <param name="pnear">[out] Nearest point on the line to point</param>
    /// <param name="dist">[out] Distance between point and pnear</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Math",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "GEOMETRY, MATH, VECTOR",
            ShortToolTip = "Nearest point on line to point",
            ToolTip = "Find the nearest point on a line to a specified point, and find the distance between the two points"
            ))
        static void nplnpt(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            const FSDistanceVector& linpt,
            const FSDimensionlessVector& lindir,
            const FSDistanceVector& point,
            FSDistanceVector& pnear,
            double& dist
        );

    /// <summary>Make a CSPICE plane from a normal vector and a constant</summary>
    /// <param name="normal">[in] A normal vector</param>
    /// <param name="constant">[in] constant defining a plane</param>
    /// <param name="plane">[out] A CSPICE plane structure representing the plane</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Planes",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "GEOMETRY, MATH, PLANE",
            ShortToolTip = "Normal vector and constant to plane",
            ToolTip = "Make a plane from a normal vector and a constant"
            ))
        static void nvc2pl(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            const FSDimensionlessVector& normal,
            const FSDistance& constant,
            FSPlane& plane
        );

    /// <summary>Make a CSPICE plane from a normal vector and a point</summary>
    /// <param name="">[in] A normal vector</param>
    /// <param name="">[in] a point defining a plane</param>
    /// <param name="">[out] A CSPICE plane structure representing the plane</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Planes",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "GEOMETRY, MATH, PLANE",
            ShortToolTip = "Normal vector and point to plane",
            ToolTip = "Make a plane from a normal vector and a point"
            ))
    static void nvp2pl(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSDimensionlessVector& normal,
        const FSDistanceVector& point,
        FSPlane& plane
    );


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SPK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "ELLIPSOID, GEOMETRY, OCCULTATION",
            ShortToolTip = "Normal vector and point to plane",
            ToolTip = "Make a plane from a normal vector and a point",
            AutoCreateRefTerm = "shape1Surfaces, shape2Surfaces"
            ))
    static void occult(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        ES_OccultationType& ocltid,
        FString& front,
        FString& back,
        const FSEphemerisTime& et,
        const TArray<FString>& shape1Surfaces,
        const TArray<FString>& shape2Surfaces,
        const FString& targ1 = TEXT("SATURN"),
        ES_GeometricModel shape1 = ES_GeometricModel::ELLIPSOID,
        const FString& frame1 = TEXT("IAU_SATURN"),
        const FString& targ2 = TEXT("SUN"),
        ES_GeometricModel shape2 = ES_GeometricModel::ELLIPSOID,
        const FString& frame2 = TEXT("IAU_SUN"),
        ES_AberrationCorrectionForOccultation abcorr = ES_AberrationCorrectionForOccultation::CN,
        const FString& obsrvr = TEXT("CASSINI")
        );


    // https://naif.jpl.nasa.gov/pub/naif/toolkit_docs/C/cspice/oscelt_c.html
    /// <summary>Determine the set of osculating conic orbital elements that corresponds to the state(position, velocity) of a body at some epoch</summary>
    /// <param name="state">[in] State of body at epoch of elements</param>
    /// <param name="et">[in] Epoch of elements</param>
    /// <param name="mu">[in] Gravitational parameter (GM) of primary body</param>
    /// <param name="elts">[out] Equivalent conic elements</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|Ephemeris",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONIC, ELEMENTS, EPHEMERIS",
            ShortToolTip = "Determine conic elements from state",
            ToolTip = "Determine the set of osculating conic orbital elements that corresponds to the state(position, velocity) of a body at some epoch"
            ))
    static void oscelt(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSStateVector& state,
        const FSEphemerisTime& et,
        const FSMassConstant& mu,
        FSConicElements& elts
    );

    // https://naif.jpl.nasa.gov/pub/naif/toolkit_docs/C/cspice/oscltx_c.html
    /// <summary>Determine the set of osculating conic orbital elements that corresponds to the state(position, velocity) of a body at some epoch. In additional to the classical elements, return the true anomaly, semi - major axis, and period, if applicable</summary>
    /// <param name="state">[in] State of body at epoch of elements</param>
    /// <param name="et">[in] Epoch of elements</param>
    /// <param name="mu">[in] Gravitational parameter (GM) of primary body</param>
    /// <param name="elts">[out] Extended set of classical conic elements</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|Ephemeris",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONIC, ELEMENTS, EPHEMERIS",
            ShortToolTip = "Extended osculating elements from state",
            ToolTip = "Determine the set of osculating conic orbital elements that corresponds to the state(position, velocity) of a body at some epoch.  In additional to the classical elements, return the true anomaly, semi - major axis, and period, if applicable"
            ))
    static void oscltx(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSStateVector& state,
        const FSEphemerisTime& et,
        const FSMassConstant& mu,
        FSConicElements& elts,
        FSAngle& nu,
        FSDistance& a,
        FSEphemerisPeriod& tau
    );


    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|PCK",
        meta = (
            Keywords = "ORIENTATION, TIME, UTILITY",
            ExpandEnumAsExecs = "ResultCode",
            ShortToolTip = "PCK coverage",
            ToolTip = "Find the coverage window for a specified reference frame in a specified binary PCK file"
            ))
    static void pckcov(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FString& pckFileRelativePath,
        int           idcode,
        const TArray<FSWindowSegment>& mergeWith,
        TArray<FSWindowSegment>& coverage
    );

    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|PCK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "ORIENTATION, UTILITY",
            ShortToolTip = "PCK reference frame class ID set",
            ToolTip = "Find the set of reference frame class ID codes of all frames in a specified binary PCK file"
            ))
    static void pckfrm(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FString& pckRelativePath,
        TArray<int>& ids
    );


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Kernel",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "POOL",
            ShortToolTip = "Put character strings into the kernel pool",
            ToolTip = "Provides a method for programmatically inserting character data into the kernel pool"
            ))
    static void pcpool_list(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FString& name,
        const TArray<FString>& cvals
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Kernel",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "POOL",
            ShortToolTip = "Put character strings into the kernel pool",
            ToolTip = "Provides a method for programmatically inserting character data into the kernel pool"
            ))
    static void pcpool(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FString& name,
        const FString& cval
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Kernel",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "POOL",
            ShortToolTip = "Put d.p.'s into the kernel pool",
            ToolTip = "Provides a method for programmatically inserting double precision data into the kernel pool"
            ))
    static void pdpool_list(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FString& name,
        const TArray<double>& dvals
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Kernel",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "POOL",
            ShortToolTip = "Put d.p.'s into the kernel pool",
            ToolTip = "Provides a method for programmatically inserting double precision data into the kernel pool"
            ))
    static void pdpool(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FString& name,
        double dval
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Coordinates",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONVERSION, COORDINATES, GEOMETRY, MATH",
            ShortToolTip = "Planetographic to rectangular",
            ToolTip = "Convert planetographic coordinates to rectangular coordinates"
            ))
    static void pgrrec(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSPlanetographicVector& planetographicVec,
        const FSDistance& re,
        FSDistanceVector& rectan,
        const FString& body = TEXT("EARTH"),
        double f = 0.00335281066474748071984552861852
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Ephemeris",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EPHEMERIS, GEOMETRY, PHASE ANGLE, SEARCH",
            ShortToolTip = "Phase angle quantity between bodies centers",
            ToolTip = "Compute the apparent phase angle for a target, observer, illuminator set of ephemeris objects"
            ))
    static FSAngle phaseq(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSEphemerisTime& et,
        const FString& target = TEXT("MOON"),
        const FString& illmn = TEXT("SUN"),
        const FString& obsrvr = TEXT("EARTH"),
        ES_AberrationCorrectionWithNewtonians abcorr = ES_AberrationCorrectionWithNewtonians::LT_S
    );

    /// <summary>the value of pi</summary>
    /// <returns>pi</returns>
    UFUNCTION(
        BlueprintPure,
        Category = "MaxQ|Constants",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Value of pi",
            ToolTip = "Return the value of pi (the ratio of the circumference of a circle to its diameter)"
            ))
    static void pi(double& pi);


    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Constants",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Angle of value pi",
            ToolTip = "Return angle with the value of pi"
            ))
    static void pi_angle(FSAngle& pi);


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Kernel",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "POOL",
            ShortToolTip = "Put integers into the kernel pool",
            ToolTip = "Provides a method for programmatically inserting integer data into the kernel pool"
            ))
    static void pipool_list(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FString& name,
        const TArray<int>& ivals
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Kernel",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "POOL",
            ShortToolTip = "Put integers into the kernel pool",
            ToolTip = "Provides a method for programmatically inserting integer data into the kernel pool"
            ))
    static void pipool(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FString& name,
        int ival
    );



    /// <summary>Project an ellipse onto a plane, orthogonally</summary>
    /// <param name="elin">A CSPICE ellipse to be projected</param>
    /// <param name="plane">A plane onto which elin is to be projected</param>
    /// <param name="elout">A CSPICE ellipse resulting from the projection</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Ellipses",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "ELLIPSE, GEOMETRY, MATH",
            ShortToolTip = "Project ellipse onto plane",
            ToolTip = "Project an ellipse onto a plane, orthogonally"
            ))
    static void pjelpl(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSEllipse& elin,
        const FSPlane& plane,
        FSEllipse& elout
    );


    /// <summary>Return a unit normal vector and constant that define a specified plane</summary>
    /// <param name="plane">[in] A CSPICE plane</param>
    /// <param name="normal, constant">[out] A normal vector and constant defining the geometric plane represented by plane</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Planes",
        meta = (
            Keywords = "GEOMETRY, MATH, PLANE",
            ShortToolTip = "Plane to normal vector and constant",
            ToolTip = "Return a unit normal vector and constant that define a specified plane"
            ))
        static void pl2nvc(
            const FSPlane& plane,
            FSDimensionlessVector& normal,
            FSDistance& constant
        );

    /// <summary>Return a unit normal vector and point that define a specified plane</summary>
    /// <param name="plane">[in] A CSPICE plane</param>
    /// <param name="normal, point">[out] A unit normal vector and point that define plane</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Planes",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "GEOMETRY, MATH, PLANE",
            ShortToolTip = "Plane to normal vector and point",
            ToolTip = "Return a unit normal vector and point that define a specified plane"
            ))
        static void pl2nvp(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            const FSPlane& plane,
            FSDistanceVector& normal,
            FSDistanceVector& point
        );

    /// <summary>Return a point and two orthogonal spanning vectors that generate a specified plane</summary>
    /// <param name="plane">[in] A CSPICE plane</param>
    /// <param name="point>[out] A point in the input plane</param>
    /// <param name="span1>[out] A vector spanning the input plane</param>
    /// <param name="span2>[out] A vector spanning the input plane</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Planes",
        meta = (
            Keywords = "GEOMETRY, MATH, PLANE",
            ShortToolTip = "Plane to point and spanning vectors",
            ToolTip = "Return a point and two orthogonal spanning vectors that generate a specified plane"
            ))
        static void pl2psv(
            const FSPlane& plane,
            FSDistanceVector& point,
            FSDistanceVector& span1,
            FSDistanceVector& span2
        );

    /// <summary>Given a central mass and the state of massless body at time t_0, this routine determines the state as predicted by a two - body force model at time t_0 + dt</summary>
    /// <param name="gm">[in] Gravity of the central mass (G times the mass M)</param>
    /// <param name="pvinit">[in] Initial state from which to propagate a state</param>
    /// <param name="dt">[in] Time offset from initial state to propagate to</param>
    /// <param name="pvprop">[out] The propagated state</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Ephemeris",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONIC, EPHEMERIS, UTILITY",
            ShortToolTip = "Propagate a two-body solution",
            ToolTip = "Given a central mass and the state of massless body at time t_0, this routine determines the state as predicted by a two - body force model at time t_0 + dt"
            ))
        static void prop2b(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            const FSMassConstant& gm,
            const FSStateVector& pvinit,
            const FSEphemerisPeriod& dt,
            FSStateVector& pvprop
        );

    /// <summary>Make a plane from a point and two spanning vectors</summary>
    /// <param name="point">[in] A point</param>
    /// <param name="point">[in] Spanning vector</param>
    /// <param name="point">[in] Spanning vector</param>
    /// <param name="plane">[out] A CSPICE plane representing the plane</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Planes",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "GEOMETRY, MATH, PLANE",
            ShortToolTip = "Point and spanning vectors to plane",
            ToolTip = "Make a plane from a point and two spanning vectors"
            ))
        static void psv2pl(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            const FSDistanceVector& point,
            const FSDistanceVector& span1,
            const FSDistanceVector& span2,
            FSPlane& plane
        );

    /// <summary>Position Transformation Matrix</summary>
    /// <param name="from">[in] Name of the frame to transform from</param>
    /// <param name="to">[in] Name of the frame to transform to</param>
    /// <param name="et">[in] Epoch of the rotation matrix</param>
    /// <param name="rotate">[out] A rotation matrix</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|Frames",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "FRAMES, TRANSFORM",
            ShortToolTip = "Position Transformation Matrix",
            ToolTip = "Return the matrix that transforms position vectors from one specified frame to another at a specified epoch"
            ))
        static void pxform(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            FSRotationMatrix& rotate,
            const FSEphemerisTime& et,
            const FString& from = TEXT("J2000"),
            const FString& to = TEXT("ECLIPJ2000")
        );

    /// <summary>Position Transform Matrix, Different Epochs</summary>
    /// <param name="from">[in] Name of the frame to transform from</param>
    /// <param name="to">[in] Name of the frame to transform to</param>
    /// <param name="et">[in] Epoch of the rotation matrix</param>
    /// <param name="etto">[in] Evaluation time of `to' frame</param>
    /// <param name="rotate">[out] A rotation matrix</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|Frames",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "FRAMES, TRANSFORM",
            ShortToolTip = "Position Transform Matrix, Different Epochs",
            ToolTip = "Return the 3x3 matrix that transforms position vectors from one specified frame at a specified epoch to another specified frame at another specified epoch"
            ))
        static void pxfrm2(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            const FString& from,
            const FSEphemerisTime& etfrom,
            const FSEphemerisTime& etto,
            FSRotationMatrix& rotate,
            const FString& to = TEXT("ECLIPJ2000")
        );

    /// <summary>Find a unit quaternion corresponding to a specified rotation matrix</summary>
    /// <param name="q">[in] unit quaternion</param>
    /// <param name="r">[out] rotation matrix corresponding to 'q'</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Rotation",
        meta = (
            Keywords = "MATH, MATRIX, ROTATION",
            ShortToolTip = "Quaternion to matrix",
            ToolTip = "Find the rotation matrix corresponding to a specified unit quaternion"
            ))
        static void q2m(
            const FSQuaternion& q,
            FSRotationMatrix& r
        );


    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Rotation",
        meta = (
            Keywords = "MATH, MATRIX, ROTATION",
            ShortToolTip = "Quaternion and quaternion derivative to a.v.",
            ToolTip = "Derive angular velocity from a unit quaternion and its derivative with respect to time"
            ))
    static void qdq2av(
        const FSQuaternion& q,
        const FSQuaternionDerivative& dq,
        FSAngularVelocity& av
    );


    /// <summary>Multiply two quaternions</summary>
    /// <param name="q1">[in] First SPICE quaternion factor</param>
    /// <param name="q2">[in] Second SPICE quaternion factor</param>
    /// <param name="qout">[out] Product of 'q1' and 'q2'</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Rotation",
        meta = (
            Keywords = "MATH, POINTING, ROTATION",
            ShortToolTip = "Quaternion times quaternion",
            ToolTip = "Multiply two quaternions"
            ))
        static void qxq(
            const FSQuaternion& q1,
            const FSQuaternion& q2,
            FSQuaternion& qout
        );

    /// <summary>Convert from range, right ascension, and declination to rectangular coordinates</summary>
    /// <param name="range">[in] Distance of a point from the origin</param>
    /// <param name="ra">[in] Right ascension of point in radians</param>
    /// <param name="dec">[in] Declination of point in radians</param>
    /// <param name="rectan">[out] Rectangular coordinates of the point</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintPure,
        Category = "MaxQ|Coordinates",
        meta = (
            Keywords = "CONVERSION, COORDINATES",
            ShortToolTip = "Convert from range, right ascension, and declination to rectangular coordinates",
            ToolTip = "Convert from range, right ascension, and declination to rectangular coordinates"
            ))
        static void radrec(
            const FSDistance& range,
            const FSAngle& ra,
            const FSAngle& dec,
            FSDistanceVector& rectan
        );

    /// <summary>This routine determines a state transformation matrix from a rotation matrixand the angular velocity of the rotation</summary>
    /// <param name="rot">[in] Rotation matrix</param>
    /// <param name="av">[in] Angular velocity vector</param>
    /// <param name="xform">[out] State transformation associated with rot and av</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintPure,
        Category = "MaxQ|Rotation",
        meta = (
            Keywords = "FRAMES",
            ShortToolTip = "Rotation and angular velocity to transform",
            ToolTip = "Determines a state transformation matrix from a rotation matrix and the angular velocity of the rotation"
            ))
        static void rav2xf(
            const FSRotationMatrix& rot,
            const FSAngularVelocity& av,
            FSEulerAngularTransform& xform
        );

    /// <summary>Compute the axis of the rotation given by an input matrix and the angle of the rotation about that axis</summary>
    /// <param name="matrix">[in] 3x3 rotation matrix in double precision</param>
    /// <param name="axis">[out] Axis of the rotation</param>
    /// <param name="angle">[out] Angle through which the rotation is performed</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Rotation",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "ANGLE, MATRIX, ROTATION",
            ShortToolTip = "Rotation axis of a matrix",
            ToolTip = "Compute the axis of the rotation given by an input matrix and the angle of the rotation about that axis"
            ))
    static void raxisa(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSRotationMatrix& matrix,
        FSDimensionlessVector& axis,
        FSAngle& angle
    );


    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Coordinates",
        meta = (
            Keywords = "CONVERSION, COORDINATES",
            ShortToolTip = "Rectangular coordinates to AZ/EL",
            ToolTip = "Convert rectangular coordinates of a point to range, azimuth and elevation."
            ))
    static void recazl(
        FSDistance& range,
        FSAngle& az,
        FSAngle& el,
        const FSDistanceVector& rectan,
        UPARAM(meta = (DisplayName = "azimuth is counter-clockwise")) bool azccw = true,
        UPARAM(meta = (DisplayName = "elevation increases with positive Z")) bool elplsz = true
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Coordinates",
        meta = (
            Keywords = "CONVERSION, COORDINATES",
            ShortToolTip = "Rectangular to cylindrical coordinates",
            ToolTip = "Convert from rectangular to cylindrical coordinates"
            ))
    static void reccyl(
        const FSDistanceVector& rectan,
        FSCylindricalVector& vec
    );

    /// <summary>Convert from rectangular coordinates to geodetic coordinates</summary>
    /// <param name="rectan">[in] Rectangular coordinates of a point</param>
    /// <param name="re">[in] Equatorial radius of the reference spheroid</param>
    /// <param name="f">[in] Flattening coefficient</param>
    /// <param name="lon">[out] Geodetic longitude of the point (radians)</param>
    /// <param name="lat">[out] Geodetic latitude  of the point (radians)</param>
    /// <param name="alt">[out] Altitude of the point above reference spheroid</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Coordinates",
        meta = (
            Keywords = "CONVERSION, COORDINATES",
            ShortToolTip = "Rectangular to geodetic",
            ToolTip = "Convert from rectangular coordinates to geodetic coordinates"
            ))
    static void recgeo(
        const FSDistanceVector& rectan,
        const FSDistance& re,
        FSGeodeticVector& vec,
        double f = 0.00335281066474748071984552861852
    );

    /// <summary>Convert from rectangular coordinates to latitudinal coordinates</summary>
    /// <param name="rectan">[in] Rectangular coordinates of a point</param>
    /// <param name="radius">[out] Distance of the point from the origin</param>
    /// <param name="longitude">[out] Longitude of the point in radians</param>
    /// <param name="latitude">[out] Latitude of the point in radians</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Coordinates",
        meta = (
            Keywords = "CONVERSION, COORDINATES",
            ShortToolTip = "Rectangular to latitudinal coordinates",
            ToolTip = "Convert from rectangular coordinates to latitudinal coordinates"
            ))
    static void reclat(
        const FSDistanceVector& rectan,
        FSLatitudinalVector& latvec
    );


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Coordinates",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONVERSION, COORDINATES, GEOMETRY, MATH",
            ShortToolTip = "Rectangular to planetographic",
            ToolTip = "Convert rectangular coordinates to planetographic coordinates"
            ))
    static void recpgr(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSDistanceVector& rectan,
        const FSDistance& re,
        FSPlanetographicVector& vec,
        const FString& body = TEXT("EARTH"),
        double f = 0.00335281066474748071984552861852
    );



    /// <summary>Convert rectangular coordinates to range, right ascension, and declination</summary>
    /// <param name="rectan">[in] Rectangular coordinates of a point</param>
    /// <param name="range">[out] Distance of the point from the origin</param>
    /// <param name="ra">[out] Right ascension in radians</param>
    /// <param name="dec">[out] Declination in radians</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Coordinates",
        meta = (
            Keywords = "CONVERSION, COORDINATES",
            ShortToolTip = "Rectangular coordinates to RA and DEC",
            ToolTip = "Convert rectangular coordinates to range, right ascension, and declination"
            ))
        static void recrad(
            const FSDistanceVector& rectan,
            FSDistance& range,
            FSAngle& ra,
            FSAngle& dec
        );

    /// <summary>Convert from rectangular coordinates to spherical coordinates</summary>
    /// <param name="rectan">[in] Rectangular coordinates of a point</param>
    /// <param name="r">[out] Distance of the point from the origin</param>
    /// <param name="colat">[out] Angle of the point from the Z-axis in radians</param>
    /// <param name="lon">[out] Longitude of the point in radians</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Coordinates",
        meta = (
            Keywords = "CONVERSION, COORDINATES",
            ShortToolTip = "Rectangular to spherical coordinates",
            ToolTip = "Convert from rectangular coordinates to spherical coordinates"
            ))
        static void recsph(
            const FSDistanceVector& rectan,
            FSSphericalVector& vec
        );

    /// <summary>Generate a rotation matrix</summary>
    /// <param name="angle">[in] Angle of rotation (radians)</param>
    /// <param name="iaxis">[in] Axis of rotation (X=1, Y=2, Z=3)</param>
    /// <param name="mout">[out] Resulting rotation matrix [angle]</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Rotation",
        meta = (
            Keywords = "MATRIX, ROTATION",
            ShortToolTip = "Generate a rotation matrix",
            ToolTip = "Calculate the 3x3 rotation matrix generated by a rotation of a specified angle about a specified axis. This rotation is thought of as rotating the coordinate system"
            ))
        static void rotate(
            const FSAngle& angle,
            ES_Axis         iaxis,
            FSRotationMatrix& mout
        );

    /// <summary>Rotate a matrix. This rotation is thought of as rotating the coordinate system</summary>
    /// <param name="m1">[in] Matrix to be rotated</param>
    /// <param name="angle">[in] Angle of rotation (radians)</param>
    /// <param name="iaxis">[in] Axis of rotation (X=1, Y=2, Z=3)</param>
    /// <param name="mout">[out] Resulting rotated matrix</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Rotation",
        meta = (
            Keywords = "MATRIX, ROTATION",
            ShortToolTip = "Rotate a matrix",
            ToolTip = "Applies a rotation of angle radians about axis iaxis to a matrix.This rotation is thought of as rotating the coordinate system."
            ))
        static void rotmat(
            const FSRotationMatrix& m1,
            const FSAngle& angle,
            ES_Axis                 iaxis,
            FSRotationMatrix& mout
        );

    /// <summary>Transform a vector via a rotation.  Transform a vector to a new coordinate system rotated by angle radians about axis iaxis.This transformation rotates v1 by - angle radians about the specified axis</summary>
    /// <param name="v1">[in] Vector whose coordinate system is to be rotated</param>
    /// <param name="angle">[in] Angle of rotation in radians</param>
    /// <param name="iaxis">[in] Axis of rotation (X=1, Y=2, Z=3)</param>
    /// <param name="vout">[out] Resulting vector</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Rotation",
        meta = (
            Keywords = "ROTATION, VECTOR",
            ShortToolTip = "Transform a vector via a rotation",
            ToolTip = "Transform a vector to a new coordinate system rotated by angle radians about axis iaxis.This transformation rotates v1 by - angle radians about the specified axis"
            ))
    static void rotvec(
        const FSDistanceVector& v1,
        const FSAngle& angle,
        ES_Axis                 iaxis,
        FSDistanceVector& vout
    );

    /// <summary>Radians per degree</summary>
    /// <returns>Radians.  Per degree.</returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Constants",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Radians per degree",
            ToolTip = "Return the number of radians per degree"
            ))
    static void rpd(
        // Hmm... scalar value?
        double& radiansPerDegree
    );

    /// <summary>Roots of a quadratic equation</summary>
    /// <param name="a">[in] Coefficient of quadratic term</param>
    /// <param name="b">[in] Coefficient of linear term</param>
    /// <param name="c">[in] Constant</param>
    /// <param name="root1">[out] Root built from positive discriminant term</param>
    /// <param name="root2">[out] Root built from negative discriminant term</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Polynomial",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "MATH, POLYNOMIAL, ROOT",
            ShortToolTip = "Roots of a quadratic equation",
            ToolTip = "Find the roots of a quadratic equation"
            ))
    static void rquad(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        double a,
        double b,
        double c,
        FSComplexScalar& root1,
        FSComplexScalar& root2
    );

    /// <summary>Decode spacecraft clock</summary>
    /// <param name="sc">[in] NAIF spacecraft identification code</param>
    /// <param name="sclkdp">[in] Encoded representation of a spacecraft clock count</param>
    /// <param name="lenout">[in] Maximum allowed length of output SCLK string</param>
    /// <param name="sclkch">[out] Character representation of a clock count</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SCLK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONVERSION, TIME",
            ShortToolTip = "Decode spacecraft clock",
            ToolTip = "Convert double precision encoding of spacecraft clock time into a character representation."
            ))
    static void scdecd(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int sc,
        double sclkdp,
        FString& sclkch
    );

    /// <summary>ET to continuous SCLK ticks</summary>
    /// <param name="sc">[in] NAIF spacecraft identification code</param>
    /// <param name="et">[in] Ephemeris time, seconds past J2000</param>
    /// <param name="sclkdp">[out] SCLK, encoded as ticks since spacecraft clock start. sclkdp need not be integral</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SCLK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONVERSION, TIME",
            ShortToolTip = "ET to continuous SCLK ticks",
            ToolTip = "Convert ephemeris seconds past J2000 (ET) to continuous encoded spacecraft clock(`ticks'). Non - integral tick values may be returned"
            ))
    static void sce2c(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int sc,
        const FSEphemerisTime& et,
        double& sclkdp
    );

    /// <summary>ET to SCLK string</summary>
    /// <param name="sc">[in] NAIF spacecraft identification code</param>
    /// <param name="et">[in] Ephemeris time, seconds past J2000</param>
    /// <param name="lenout">[in] Maximum length of output string</param>
    /// <param name="b">[out] An SCLK string</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SCLK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONVERSION, TIME",
            ShortToolTip = "ET to SCLK string",
            ToolTip = "Convert an epoch specified as ephemeris seconds past J2000 (ET) to a character string representation of a spacecraft clock value(SCLK)"
            ))
    static void sce2s(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int sc,
        const FSEphemerisTime& et,
        FString& sclkch
    );

    /// <summary>ET to SCLK ticks</summary>
    /// <param name="sc">[in] NAIF spacecraft identification code</param>
    /// <param name="et">[in] Ephemeris time, seconds past J2000</param>
    /// <param name="sclkdp">[out] SCLK, encoded as ticks since spacecraft clock start</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SCLK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONVERSION, TIME",
            ShortToolTip = "ET to SCLK ticks",
            ToolTip = "Convert ephemeris seconds past J2000 (ET) to integral encoded spacecraft clock('ticks').For conversion to fractional ticks, (required for C - kernel production), see the function 'sce2c'"
            ))
        static void sce2t(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            int sc,
            const FSEphemerisTime& et,
            double& sclkdp
        );

    /// <summary>Encode spacecraft clock</summary>
    /// <param name="sc">[in] NAIF spacecraft identification code</param>
    /// <param name="sclkch">[in] Character representation of a spacecraft clock</param>
    /// <param name="sclkdp">[out] Encoded representation of the clock count</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SCLK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONVERSION, TIME",
            ShortToolTip = "Encode spacecraft clock",
            ToolTip = "Encode character representation of spacecraft clock time into a double precision number"
            ))
    static void scencd(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int sc,
        const FString& sclkch,
        double& sclkdp
    );

    /// <summary>Convert SCLK "ticks" to character clock format</summary>
    /// <param name="sc">[in] NAIF spacecraft identification code</param>
    /// <param name="ticks">[in] Encoded representation of a spacecraft clock count</param>
    /// <param name="lenout">[in] Maximum length of output string</param>
    /// <param name="clkstr">[out] Character representation of a clock count</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SCLK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONVERSION, TIME",
            ShortToolTip = "Convert SCLK \"ticks\" to character clock format",
            ToolTip = "Convert encoded spacecraft clock ticks to character clock format"
            ))
    static void scfmt(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int sc,
        double ticks,
        FString& clkstr
    );

    /// <summary>Spacecraft Clock Partition Information</summary>
    /// <param name="sc">[in] NAIF spacecraft identification code</param>
    /// <param name="nparts">[out] The number of spacecraft clock partitions</param>
    /// <param name="pstart">[out] Array of partition start times</param>
    /// <param name="pstop">[out] Array of partition stop times</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SCLK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "TIME",
            ShortToolTip = "Spacecraft Clock Partition Information",
            ToolTip = "Get spacecraft clock partition information from a spacecraft clock kernel file"
            ))
    static void scpart(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int sc,
        TArray<double>& pstart,
        TArray<double>& pstop
    );

    /// <summary>SCLK string to ET</summary>
    /// <param name="sc">[in] NAIF spacecraft identification code</param>
    /// <param name="sclkch">[in] An SCLK string</param>
    /// <param name="et">[out] Ephemeris time, seconds past J2000</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SCLK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONVERSION, TIME",
            ShortToolTip = "SCLK string to ET",
            ToolTip = "Convert a spacecraft clock string to ephemeris seconds past J2000(ET)"
            ))
    static void scs2e(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int sc,
        const FString& sclkch,
        FSEphemerisTime& et
    );

    /// <summary>SCLK ticks to ET</summary>
    /// <param name="sc">[in] NAIF spacecraft identification code</param>
    /// <param name="sclkdp">[in] SCLK, encoded as ticks since spacecraft clock start</param>
    /// <param name="et">[out] Ephemeris time, seconds past J2000</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SCLK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONVERSION, TIME",
            ShortToolTip = "SCLK ticks to ET",
            ToolTip = "Convert encoded spacecraft clock (`ticks') to ephemeris seconds past J2000(ET)"
            ))
    static void sct2e(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int sc,
        double sclkdp,
        FSEphemerisTime& et
    );

    /// <summary>Convert spacecraft clock string to ticks</summary>
    /// <param name="sc">[in] NAIF spacecraft identification code</param>
    /// <param name="clkstr">[in] Character representation of a spacecraft clock</param>
    /// <param name="ticks">[out] Number of ticks represented by the clock string</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SCLK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONVERSION, TIME",
            ShortToolTip = "Convert spacecraft clock string to ticks",
            ToolTip = "Convert a spacecraft clock format string to number of \"ticks\""
            ))
    static void sctiks(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int sc,
        const FString& clkstr,
        double& ticks
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Sort",
        meta = (
            Keywords = "ARRAY, SORT",
            ShortToolTip = "Shell sort a double precision array",
            ToolTip = "Sort a double precision array using the Shell Sort algorithm.  Sorts DoubleArray in place (passed by reference, modified)"
            ))
    static void shelld(
        UPARAM(ref) TArray<double>& DoubleArray
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Sort",
        meta = (
            Keywords = "ARRAY, SORT",
            ShortToolTip = "Shell sort a double precision array",
            ToolTip = "Sort a double precision array using the Shell Sort algorithm, returns array of int indices, does not modify original Array.  If the array has duplicates the results at those indices may be the indices of any of the matches or 0."
            ))
    static void shelld_ByIndex(
        const TArray<double>& DoubleArray,
        TArray<int>& Order
    );


    /// <summary>Convert spacecraft clock string to ticks</summary>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|DSK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            AutoCreateRefTerm = "shapeSurfaces",
            Keywords = "GEOMETRY",
            ShortToolTip = "Surface intercept",
            ToolTip = "Given an observer and a direction vector defining a ray, compute the surface intercept of the ray on a target body at a specified epoch, optionally corrected for light timeand stellar aberration"
            ))
    static void sincpt(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSEphemerisTime& et,
        const FString& dref,
        const FSDimensionlessVector& dvec,
        FSDistanceVector& spoint,
        FSEphemerisTime& trgepc,
        FSDistanceVector& srfvec,
        bool& bFound,
        const TArray<FString>& shapeSurfaces,
        ES_GeometricModel method = ES_GeometricModel::ELLIPSOID,
        const FString& target = TEXT("EARTH"),
        const FString& fixref = TEXT("IAU_EARTH"),
        const FString& obsrvr = TEXT("EARTH"),
        ES_AberrationCorrectionWithTransmissions abcorr = ES_AberrationCorrectionWithTransmissions::None
    );

    /// <summary>SPK, Close file</summary>
    /// <param name="handle">[in] Handle of the SPK file to be closed</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SPK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "SPK",
            ShortToolTip = "SPK, Close file",
            ToolTip = "The handle of the SPK file that is to be closed"
            ))
    static void spkcls(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int handle
    );


    /// <summary>SPK coverage</summary>
    /// <param name="spk">[in] Name of SPK file</param>
    /// <param name="idcode">[in] ID code of ephemeris object</param>
    /// <param name="cover">[out] Window giving coverage in `spk' for `idcode''</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SPK",
        meta = (
            AutoCreateRefTerm = "merge_to",
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EPHEMERIS, TIME, UTILITY",
            ShortToolTip = "SPK coverage",
            ToolTip = "Find the coverage window for a specified ephemeris object in a specified SPK file"
            ))
    static void spkcov(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,

        UPARAM(meta = (DisplayName = "project-relative file path"))
        const FString& spk_relative_path,

        UPARAM(meta = (DisplayName = "is the integer ID code of an object in the ephemeris file"))
        int idcode,

        UPARAM(meta = (DisplayName = "(optional) existing window to merge results with"))
        const TArray<FSWindowSegment>& merge_to,

        TArray<FSWindowSegment>& coverage
    );

    /// <summary>SPK, constant position observer state</summary>
    /// <param name="target">[in] Name of target ephemeris object</param>
    /// <param name="et">    [in] Observation epoch</param>
    /// <param name="outref">[in] Reference frame of output state</param>
    /// <param name="refloc">[in] Output reference frame evaluation locus</param>
    /// <param name="abcorr">[in] Aberration correction</param>
    /// <param name="obspos">[in] Observer position relative to center of motion</param>
    /// <param name="obsctr">[in] Center of motion of observer</param>
    /// <param name="obsref">[in] Frame of observer position</param>
    /// <param name="state">[out] State of target with respect to observer</param>
    /// <param name="lt">   [out] One way light time between target and observer</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SPK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EPHEMERIS",
            ShortToolTip = "SPK, constant position observer state",
            ToolTip = "Return the state of a specified target relative to an 'observer', where the observer has constant position in a specified referencevframe.The observer's position is provided by the calling program rather than by loaded SPK files."
            ))
    static void spkcpo(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSStateVector& state,
        FSEphemerisPeriod& lt,
        const FSEphemerisTime& et,
        const FSDistanceVector& obspos,
        const FString& target = TEXT("SUN"),
        const FString& outref = TEXT("DSS-14_TOPO"),
        ES_ReferenceFrameLocus refloc = ES_ReferenceFrameLocus::OBSERVER,
        const FString& obsctr = TEXT("EARTH"),
        const FString& obsref = TEXT("ITRF93"),
        ES_AberrationCorrectionWithNewtonians
        abcorr = ES_AberrationCorrectionWithNewtonians::CN_S
    );

    /// <summary>SPK, constant position target state</summary>
    /// <param name="trgpos">[in] Target position relative to center of motion</param>
    /// <param name="trgctr">[in] Center of motion of target</param>
    /// <param name="trgref">[in] Frame of target position</param>
    /// <param name="et">    [in] Observation epoch</param>
    /// <param name="outref">[in] Reference frame of output state</param>
    /// <param name="refloc">[in] Output reference frame evaluation locus</param>
    /// <param name="abcorr">[in] Aberration correction</param>
    /// <param name="obsrvr">[in] Name of observing ephemeris object</param>
    /// <param name="state">[out] State of target with respect to observer</param>
    /// <param name="lt">   [out] One way light time between target and observer</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SPK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EPHEMERIS",
            ShortToolTip = "SPK, constant position target state",
            ToolTip = "Return the state, relative to a specified observer, of a target having constant position in a specified reference frame. The target's position is provided by the calling program rather than by loaded SPK files"
            ))
    static void spkcpt(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSStateVector& state,
        FSEphemerisPeriod& lt,
        const FSDistanceVector& trgpos,
        const FSEphemerisTime& et,
        const FString& trgctr = TEXT("EARTH"),
        const FString& trgref = TEXT("ITRF93"),
        const FString& outref = TEXT("ITRF93"),
        ES_ReferenceFrameLocus refloc = ES_ReferenceFrameLocus::TARGET,
        const FString& obsrvr = TEXT("MGS"),
        ES_AberrationCorrectionWithNewtonians
        abcorr = ES_AberrationCorrectionWithNewtonians::CN_S
    );

    /// <summary>SPK, constant velocity observer state</summary>
    /// <param name="target">[in] Name of target ephemeris object</param>
    /// <param name="et">    [in] Observation epoch</param>
    /// <param name="outref">[in] Reference frame of output state</param>
    /// <param name="refloc">[in] Output reference frame evaluation locus</param>
    /// <param name="abcorr">[in] Aberration correction</param>
    /// <param name="obssta">[in] Observer position relative to center of motion</param>
    /// <param name="obsepc">[in] Epoch of observer state</param>
    /// <param name="obsctr">[in] Center of motion of observer</param>
    /// <param name="obsref">[in] Frame of observer position</param>
    /// <param name="state">[out] State of target with respect to observer</param>
    /// <param name="lt">   [out] One way light time between target and observer</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SPK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EPHEMERIS",
            ShortToolTip = "SPK, constant velocity observer state",
            ToolTip = "Return the state, relative to a specified observer, of a target having constant position in a specified reference frame.The target's position is provided by the calling program rather than by loaded SPK files."
            ))
    static void spkcvo(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSStateVector& state,
        FSEphemerisPeriod& lt,
        const FSEphemerisTime& et,
        const FSStateVector& obssta,
        const FSEphemerisTime& obsepc,
        const FString& target = TEXT("MGS"),
        const FString& outref = TEXT("ITRF93"),
        ES_ReferenceFrameLocus refloc = ES_ReferenceFrameLocus::OBSERVER,
        const FString& obsctr = TEXT("EARTH"),
        const FString& obsref = TEXT("ITRF93"),
        ES_AberrationCorrectionWithNewtonians
        abcorr = ES_AberrationCorrectionWithNewtonians::CN_S
    );

    /// <summary>SPK, constant velocity target state</summary>
    /// <param name="trgsta">[in] Target state relative to center of motion</param>
    /// <param name="trgepc">[in] Epoch of target state</param>
    /// <param name="trgctr">[in] Center of motion of target</param>
    /// <param name="trgref">[in] Frame of target position</param>
    /// <param name="et">    [in] Observation epoch</param>
    /// <param name="outref">[in] Reference frame of output state</param>
    /// <param name="refloc">[in] Output reference frame evaluation locus</param>
    /// <param name="abcorr">[in] Aberration correction</param>
    /// <param name="obsrvr">[in] Name of observing ephemeris object</param>
    /// <param name="state">[out] State of target with respect to observer</param>
    /// <param name="lt">   [out] One way light time between target and observer</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SPK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EPHEMERIS",
            ShortToolTip = "SPK, constant velocity target state",
            ToolTip = "Return the state of a specified target relative to an 'observer', where the observer has constant velocity in a specified reference frame.The observer's state is provided by the calling program rather than by loaded SPK files"
            ))
    static void spkcvt(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSStateVector& state,
        FSEphemerisPeriod& lt,
        const FSStateVector& trgsta,
        const FSEphemerisTime& trgepc,
        const FSEphemerisTime& et,
        const FString& trgctr = TEXT("MARS"),
        const FString& trgref = TEXT("IAU_MARS"),
        const FString& outref = TEXT("ITRF93"),
        ES_ReferenceFrameLocus refloc = ES_ReferenceFrameLocus::OBSERVER,
        const FString& obsrvr = TEXT("MGS"),
        ES_AberrationCorrectionWithNewtonians
        abcorr = ES_AberrationCorrectionWithNewtonians::CN_S
    );

    /// <summary>S/P Kernel, easy position</summary>
    /// <param name="targ">[in] Target body NAIF ID code</param>
    /// <param name="et">[in] Observer epoch</param>
    /// <param name="ref">[in] Reference frame of output state vector</param>
    /// <param name="abcorr">[in] Aberration correction flag ["NONE"]</param>
    /// <param name="obs">[in] Observing body NAIF ID code</param>
    /// <param name="ptarg">[out] Position of target</param>
    /// <param name="lt">[out] One way light time between observer and target</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SPK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EPHEMERIS",
            ShortToolTip = "S/P Kernel, easy position",
            ToolTip = "Return the position of a target body relative to an observing body, optionally corrected for light time(planetary aberration) and stellar aberration"
            ))
    static void spkezp(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSEphemerisTime& et,
        FSDistanceVector& ptarg,
        FSEphemerisPeriod& lt,
        int targ,
        int obs,
        const FString& ref,
        ES_AberrationCorrectionWithNewtonians abcorr = ES_AberrationCorrectionWithNewtonians::None
    );

    /// <summary>S/P Kernel, easier reader</summary>
/// <param name="targ">[in] Target body name</param>
/// <param name="et">[in] Observer epoch</param>
/// <param name="ref">[in] Reference frame of output state vector</param>
/// <param name="abcorr">[in] Aberration correction flag ["NONE"]</param>
/// <param name="obs">[in] Observing body name</param>
/// <param name="starg">[out] State of target</param>
/// <param name="lt">[out] One way light time between observer and target</param>
/// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SPK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EPHEMERIS",
            ShortToolTip = "S/P Kernel, easier reader",
            ToolTip = "Return the state (position and velocity) of a target body relative to an observing body, optionally corrected for light time(planetary aberration) and stellar aberration"
            ))
    static void spkezr(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSEphemerisTime& et,
        FSStateVector& state,
        FSEphemerisPeriod& lt,
        const FString& targ = TEXT("MOON"),
        const FString& obs = TEXT("EARTH BARYCENTER"),
        const FString& ref = TEXT("ECLIPJ2000"),
        ES_AberrationCorrectionWithNewtonians abcorr = ES_AberrationCorrectionWithNewtonians::None
    );


    /// <summary>S/P Kernel, geometric state</summary>
    /// <param name="targ">[in] Target body</param>
    /// <param name="et">[in] Target epoch</param>
    /// <param name="ref">[in] Target reference frame</param>
    /// <param name="obs">[in] Observing body</param>
    /// <param name="state">[out] State of target</param>
    /// <param name="lt">[out] Light time</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SPK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EPHEMERIS",
            ShortToolTip = "S/P Kernel, geometric state",
            ToolTip = "Compute the geometric state (position and velocity) of a target body relative to an observing body"
            ))
    static void spkgeo(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int targ,
        const FSEphemerisTime& et,
        int obs,
        FSStateVector& state,
        FSEphemerisPeriod& lt,
        const FString& ref = TEXT("ECLIPJ2000")
    );


    /// <summary>S/P Kernel, geometric position</summary>
    /// <param name="targ">[in] Target body</param>
    /// <param name="et">[in] Target epoch</param>
    /// <param name="ref">[in] Target reference frame</param>
    /// <param name="obs">[in] Observing body</param>
    /// <param name="pos">[out] Position of target</param>
    /// <param name="lt">[out] Light time</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SPK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EPHEMERIS",
            ShortToolTip = "S/P Kernel, geometric position",
            ToolTip = "Compute the geometric position of a target body relative to an observing body"
            ))
        static void spkgps(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            int targ,
            const FSEphemerisTime& et,
            int obs,
            FSDistanceVector& pos,
            FSEphemerisPeriod& lt,
            const FString& ref = TEXT("ECLIPJ2000")
        );


    /// <summary>S/P Kernel, position</summary>
    /// <param name="targ">[in] Target body name</param>
    /// <param name="et">[in] Target epoch</param>
    /// <param name="ref">[in] Target reference frame</param>
    /// <param name="obs">[in] Observing body</param>
    /// <param name="pos">[out] Position of target</param>
    /// <param name="lt">[out] Light time</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SPK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EPHEMERIS",
            ShortToolTip = "S/P Kernel, position",
            ToolTip = "Return the position of a target body relative to an observing body"
            ))
    static void spkpos(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSEphemerisTime& et,
        FSDistanceVector& ptarg,
        FSEphemerisPeriod& lt,
        const FString& targ = TEXT("EARTH"),
        const FString& obs = TEXT("SSB"),
        const FString& ref = TEXT("ECLIPJ2000"),
        ES_AberrationCorrectionWithNewtonians abcorr = ES_AberrationCorrectionWithNewtonians::None
    );


    /// <summary>S/P Kernel, Load ephemeris file</summary>
    /// <param name="filename">[in] Name of the file to be loade</param>
    /// <param name="handle">[out] Loaded file's handle</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SPK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EPHEMERIS, FILES",
            ShortToolTip = "S/P Kernel, Load ephemeris file",
            ToolTip = "Load an ephemeris file for use by the readers"
            ))
    static void spklef(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        UPARAM(meta = (DisplayName = "project-relative file path"))
        const FString& filename,
        int& handle
    );

    /// <summary>Find the set of ID codes of all objects in a specified SPK file</summary>
    /// <param name="spk">[in] Name of SPK file</param>
    /// <param name="ids">[out] Set of ID codes of objects in SPK file</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SPK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EPHEMERIS, UTILITY",
            ShortToolTip = "SPK objects",
            ToolTip = "Find the set of ID codes of all objects in a specified SPK file"
            ))
    static void spkobj(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        UPARAM(meta = (DisplayName = "project-relative file path"))
        const FString& relative_path,
        TArray<int>& ids
    );

    /// <summary>SPK open for addition</summary>
    /// <param name="file">[in] The name of an existing SPK file</param>
    /// <param name="handle">[out] A handle attached to the SPK file opened to append</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SPK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "SPK",
            ShortToolTip = "SPK open for addition",
            ToolTip = "Open an existing SPK file for subsequent write"
            ))
    static void spkopa(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        UPARAM(meta = (DisplayName = "project-relative file path"))
        const FString& file,
        int& handle
    );

    /// <summary>SPK, open new file</summary>
    /// <param name="name">[in] The name of the new SPK file to be created</param>
    /// <param name="ifname">[in] The internal filename for the SPK file</param>
    /// <param name="ncomch">[in] The number of characters to reserve for comments</param>
    /// <param name="handle">[out] The handle of the opened SPK file</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SPK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "SPK",
            ShortToolTip = "SPK, open new file",
            ToolTip = "Create a new SPK file, returning the handle of the opened file"
            ))
    static void spkopn(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FString& name,
        const FString& ifname,
        int         ncomch,
        int& handle
    );

    /// <summary>S/P Kernel, Unload ephemeris file</summary>
    /// <param name="handle">[in] Handle of file to be unloaded</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SPK",
        meta = (
            Keywords = "EPHEMERIS, FILES",
            ShortToolTip = "S/P Kernel, Unload ephemeris file",
            ToolTip = "Unload an ephemeris file so that it will no longer be searched by the readers"
            ))
    static void spkuef(
        int handle
    );

    /// <summary>Write SPK segment, type 5</summary>
    /// <param name="handle">[in] Handle of an SPK file open for writing</param>
    /// <param name="body">[in] Body code for ephemeris object</param>
    /// <param name="center">[in] Body code for the center of motion of the body</param>
    /// <param name="frame">[in] The reference frame of the states</param>
    /// <param name="first">[in] First valid time for which states can be computed</param>
    /// <param name="last">[in] Last valid time for which states can be computed</param>
    /// <param name="segid">[in] Segment identifier</param>
    /// <param name="gm">[in] Gravitational parameter of central body</param>
    /// <param name="n">[in] Number of states and epochs</param>
    /// <param name="states">[in] States</param>
    /// <param name="epochs">[in] Epochs</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SPK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EPHEMERIS",
            ShortToolTip = "Write SPK segment, type 5",
            ToolTip = "Write an SPK segment of type 5 given a time-ordered set of discrete states and epochs, and the gravitational parameter of a central body"
            ))
    static void spkw05(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int handle,
        int body,
        int center,
        const FString& frame,
        const FSEphemerisTime& first,
        const FSEphemerisTime& last,
        const FString& segid,
        const FSMassConstant& gm,
        const TArray<FSPKType5Observation>& states
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|SPK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "EPHEMERIS",
            ShortToolTip = "SPK, write a type 15 segment",
            ToolTip = "Write an SPK segment of type 15 given a type 15 data record"
            ))
    static void spkw15(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        int handle,
        int body,
        int center,
        const FString& frame,
        const FSEphemerisTime& first,
        const FSEphemerisTime& last,
        const FString& segid,
        const FSPKType15Observation& state
    );

    /// <summary>Seconds in a day</summary>
    /// <returns>Seconds.  Per day.</returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Constants",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Seconds per day",
            ToolTip = "The number of seconds in a day"
            ))
    static void spd(double& secondsPerDay);

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Coordinates",
        meta = (
            Keywords = "CONVERSION, COORDINATES",
            ShortToolTip = "Spherical to cylindrical coordinates",
            ToolTip = "This routine converts from spherical coordinates to cylindrical coordinates"
            ))
    static void sphcyl(
        const FSSphericalVector& sphvec,
        FSCylindricalVector& cylvec
    );

    /// <summary>Spherical to latitudinal coordinates</summary>
    /// <param name="r">[in] Distance of the point from the origin</param>
    /// <param name="colat">[in] Angle of the point from positive z axis (radians)</param>
    /// <param name="lons">[in] Angle of the point from the XZ plane (radians)</param>
    /// <param name="radius">[out] Distance of a point from the origin</param>
    /// <param name="lon">[out] Angle of the point from the XZ plane in radians</param>
    /// <param name="lat">[out] Angle of the point from the XY plane in radians</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Coordinates",
        meta = (
            Keywords = "CONVERSION, COORDINATES",
            ShortToolTip = "Spherical to latitudinal coordinates",
            ToolTip = "Convert from spherical coordinates to latitudinal coordinates"
            ))
    static void sphlat(
        const FSSphericalVector& sphvec,
        FSLatitudinalVector& latvec
    );

    /// <summary>Spherical to rectangular coordinates</summary>
    /// <param name="r">[in] Distance of a point from the origin</param>
    /// <param name="colat">[in] Angle of the point from the Z-axis in radians</param>
    /// <param name="lon">[in] Angle of the point from the XZ plane in radians</param>
    /// <param name="rectan">[out] Rectangular coordinates of the point</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Coordinates",
        meta = (
            Keywords = "CONVERSION, COORDINATES",
            ShortToolTip = "Spherical to rectangular coordinates",
            ToolTip = "Convert from spherical coordinates to rectangular coordinates"
            ))
    static void sphrec(
        const FSSphericalVector& sphvec,
        FSDistanceVector& rectan
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Coordinates",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONVERSION, COORDINATES, TRANSFORMATION",
            ShortToolTip = "Surface to rectangular coordinates",
            ToolTip = "Convert planetocentric latitude and longitude of a surface point on a specified body to rectangular coordinates"
            ))
    static void srfrec(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSLonLat& lonlat,
        FSDistanceVector& rectan,
        int           body = 399
        );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|DSK",
        meta = (
            ExpandEnumAsExecs = "isname",
            Keywords = "CONVERSION, DSK, ID, NAME, STRING, SURFACE",
            ShortToolTip = "Surface and body ID codes to surface string",
            ToolTip = "Translate a surface ID code, together with a body ID code, to the corresponding surface name.If no such name exists, return a string representation of the surface ID code"
            ))
    static void srfc2s(
        FString& srfstr,
        ES_FoundCode& isname,
        int   code = 1,
        int   bodyid = 499
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|DSK",
        meta = (
            ExpandEnumAsExecs = "isname",
            Keywords = "CONVERSION, DSK, ID, NAME, STRING, SURFACE",
            ShortToolTip = "Surface ID and body string to surface string",
            ToolTip = "Translate a surface ID code, together with a body string, to the corresponding surface name.If no such surface name exists, return a string representation of the surface ID code"
            ))
    static void srfcss(
        FString& srfstr,
        ES_FoundCode& isname,
        int code = 1,
        const FString& bodstr = TEXT("MARS")
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|DSK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            AutoCreateRefTerm = "shapeSurfaces, et",
            Keywords = "COORDINATES, DSK, GEOMETRY, SURFACE",
            ShortToolTip = "Map surface points to outward normal vectors",
            ToolTip = "Map array of surface points on a specified target body to the corresponding unit length outward surface normal vectors. The surface of the target body may be represented by a triaxial ellipsoid or by topographic data provided by DSK files."
            ))
    static void srfnrm(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FSDimensionlessVector>& normls,
        const FSEphemerisTime& et,
        const TArray<FSDistanceVector>& srfpts,
        const TArray<FString>& shapeSurfaces,
        ES_GeometricModel method = ES_GeometricModel::DSK,
        const FString& target = TEXT("PHOBOS"),
        const FString& fixref = TEXT("IAU_PHOBOS")
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|DSK",
        meta = (
            ExpandEnumAsExecs = "FoundCode",
            Keywords = "CONVERSION, DSK, ID, NAME, STRING, SURFACE",
            ShortToolTip = "Surface and body strings to surface ID code",
            ToolTip = "Translate a surface string, together with a body string, to the corresponding surface ID code.The input strings may contain names or integer ID codes"
            ))
    static void srfs2c(
        int& code,
        ES_FoundCode& FoundCode,
        const FString& srfstr = TEXT("MGS MOLA 128 pixel/deg"),
        const FString& bodstr = TEXT("MARS")
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|DSK",
        meta = (
            ExpandEnumAsExecs = "FoundCode",
            Keywords = "CONVERSION, DSK, ID, NAME, STRING, SURFACE",
            ShortToolTip = "Surface and body strings to surface ID code",
            ToolTip = "Translate a surface string, together with a body ID code, to the corresponding surface ID code.The input surface string may contain a name or an integer ID code."
            ))
    static void srfscc(
        int& code,
        ES_FoundCode& FoundCode,
        const FString& srfstr = TEXT("PHOBOS GASKELL Q512"),
        int bodyid = 401
    );


    /// <summary>String to ET</summary>
    /// <param name="str">[in] A string representing an epoch</param>
    /// <param name="et">[out] The equivalent value in seconds past J2000</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Time",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "TIME",
            ShortToolTip = "String to ET",
            ToolTip = "Convert a string representing an epoch to a double precision value representing the number of TDB seconds past the J2000 epoch corresponding to the input epoch"
            ))
    static void str2et(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSEphemerisTime& et,
        const FString& str = TEXT("2021 October 1  15:37:60.5 (PST)")
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|DSK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            AutoCreateRefTerm = "surfaces",
            Keywords = "GEOMETRY",
            ShortToolTip = "Sub-observer point",
            ToolTip = "Compute the rectangular coordinates of the sub-observer point on a target body at a specified epoch, optionally corrected for light timeand stellar aberration. The surface of the target body may be represented by a triaxial ellipsoid or by topographic data provided by DSK files."
            ))
    static void subpnt(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSDistanceVector& spoint,
        FSEphemerisTime& trgepc,
        FSDistanceVector& srfvec,
        const FSEphemerisTime& et,
        const TArray<FString>& surfaces,
        ES_ComputationMethod method = ES_ComputationMethod::NEAR_POINT_ELLIPSOID,
        const FString& target = TEXT("MARS"),
        const FString& fixref = TEXT("IAU_MARS"),
        ES_AberrationCorrectionWithTransmissions abcorr = ES_AberrationCorrectionWithTransmissions::LT_S,
        const FString& obsrvr = TEXT("MGS")
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|DSK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            AutoCreateRefTerm = "surfaces",
            ShortToolTip = "Sub-solar point",
            Keywords = "GEOMETRY",
            ToolTip = "Compute the rectangular coordinates of the sub-solar point on a target body at a specified epoch, optionally corrected for light timeand stellar aberration.  The surface of the target body may be represented by a triaxial ellipsoid or by topographic data provided by DSK files."
            ))
    static void subslr(
        FSDistanceVector& spoint,
        FSEphemerisTime& trgepc,
        FSDistanceVector& srfvec,
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSEphemerisTime& et,
        const TArray<FString>& surfaces,
        ES_ComputationMethod method = ES_ComputationMethod::NEAR_POINT_ELLIPSOID,
        const FString& target = TEXT("MARS"),
        const FString& fixref = TEXT("IAU_MARS"),
        ES_AberrationCorrectionWithNewtonians abcorr = ES_AberrationCorrectionWithNewtonians::LT_S,
        const FString& obsrvr = TEXT("MGS")
    );


    /// <summary>Map surface points to outward normal vectors</summary>
    /// <param name="a">[in] Length of the ellisoid semi-axis along the x-axis</param>
    /// <param name="b">[in] Length of the ellisoid semi-axis along the y-axis</param>
    /// <param name="c">[in] Length of the ellisoid semi-axis along the z-axis</param>
    /// <param name="point">[in] Body-fixed coordinates of a point on the ellipsoid</param>
    /// <param name="normal">[out] Outward pointing unit normal to ellipsoid at point</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Ellipsoid",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "ELLIPSOID, GEOMETRY",
            ShortToolTip = "Surface normal vector on an ellipsoid",
            ToolTip = "computes the outward-pointing, unit normal vector from a point on the surface of an ellipsoid"
            ))
    static void surfnm(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSDistance& a,
        const FSDistance& b,
        const FSDistance& c,
        const FSDistanceVector& point,
        FSDimensionlessVector& normal
    );

    /// <summary>Determine the intersection of a line-of-sight vector with the surface of an ellipsoid</summary>
    /// <param name="positn">[in] Position of the observer in body-fixed frame</param>
    /// <param name="u">[in] Vector from the observer in some direction</param>
    /// <param name="a">[in] Length of the ellipsoid semi-axis along the x-axis</param>
    /// <param name="b">[in] Length of the ellipsoid semi-axis along the y-axis</param>
    /// <param name="c">[in] Length of the ellipsoid semi-axis along the z-axis</param>
    /// <param name="point">[out] </param>
    /// <param name="found">[out] </param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Ellipsoid",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "ELLIPSOID, GEOMETRY, INTERSECTION",
            ShortToolTip = "Surface point on an ellipsoid",
            ToolTip = "Determine the intersection of a line-of-sight vector with the surface of an ellipsoid"
            ))
    static void surfpt(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSDistanceVector& positn,
        const FSDimensionlessVector& u,
        const FSDistance& a,
        const FSDistance& b,
        const FSDistance& c,
        FSDistanceVector& point,
        bool& bFound
    );

    /// <summary>Return the state transformation matrix from one frame to another at a specified epoch</summary>
    /// <param name="from">[in] Name of the frame to transform from</param>
    /// <param name="to">[in] Name of the frame to transform to</param>
    /// <param name="et">[in] Epoch of the state transformation matrix</param>
    /// <param name="xform">[out] A state transformation matrix</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Frames",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "FRAMES",
            ShortToolTip = "State Transformation Matrix",
            ToolTip = "Return the state transformation matrix from one frame to another at a specified epoch"
            ))
    static void sxform(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSStateTransform& xform,
        const FSEphemerisTime& et,
        const FString& from = TEXT("J2000"),
        const FString& to = TEXT("ECLIPJ2000")
    );


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|DSK",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            AdvancedDisplay = "maxn",
            Keywords = "DSK, GEOMETRY, SHADOW, TERMINATOR",
            ShortToolTip = "Terminator points on an extended object",
            ToolTip = "Find terminator points on a target body. The caller specifies half - planes, bounded by the illumination source center - target center vector, in which to search for terminator points. The terminator can be either umbral or penumbral.The umbral terminator is the boundary of the region on the target surface where no light from the source is visible.The penumbral terminator is the boundary of the region on the target surface where none of the light from the source is blocked by the target itself."
            ))
    void termpt(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        TArray<FSTermptCut>& cuts,
        const FSEphemerisTime& et,
        const FSDimensionlessVector& refvec,
        const FSAngle& rolstp,
        int              ncuts,
        const FSAngle& schstp,
        const FSAngle& soltol,
        const TArray<FString>& shapeSurfaces,
        ES_Shadow shadow = ES_Shadow::UMBRAL,
        ES_CurveType curveType = ES_CurveType::TANGENT,
        ES_GeometricModel method = ES_GeometricModel::ELLIPSOID,
        const FString& ilusrc = TEXT("SUN"),
        const FString& target = TEXT("MARS"),
        const FString& fixref = TEXT("IAU_MARS"),
        ES_AberrationCorrectionWithNewtonians abcorr = ES_AberrationCorrectionWithNewtonians::CN_S,
        ES_AberrationCorrectionLocusTerminator corloc = ES_AberrationCorrectionLocusTerminator::ELLIPSOID_TERMINATOR,
        const FString& obsrvr = TEXT("EARTH"),
        int              maxn = 10000
    );

    /// <summary>Time Output</summary>
    /// <param name="et">[in] An epoch in seconds past the ephemeris epoch J2000</param>
    /// <param name="pictur">[in] A format specification for the output string</param>
    /// <param name="lenout">[in] The length of the output string plus 1</param>
    /// <param name="output">[out] A string representation of the input epoch</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Time",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "TIME",
            ShortToolTip = "Time Output",
            ToolTip = "Converts an input epoch represented in TDB seconds past the TDB epoch of J2000 to a character string formatted to the specifications of a user's format picture"
            ))
        static void timout(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            FString& output,
            const FSEphemerisTime& et,
            const FString& pictur = TEXT("MON DD, YYYY HR:MN:SC.#### (TDB)")
        );


    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Rotation",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "ROTATION, TRANSFORMATION, FRAMES, PCK",
            ShortToolTip = "Transformation, inertial state to bodyfixed",
            ToolTip = "Return a 6x6 matrix that transforms states in inertial coordinates to states in body - equator - and -prime - meridian coordinates"
            ))
    static void tisbod(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSStateTransform& tsipm,
        const FSEphemerisTime& et,
        int           body = 399,
        const FString& ref = TEXT("GALACTIC")
    );

    /// <summary></summary>
    /// <param name="string">Input time string, UTC</param>
    /// <param name="lenout">Available space in output error message string</param>
    /// <param name="ReturnValue">Equivalent UTC seconds past J2000</param>
    /// <param name="errmsg">Descriptive error message</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Time",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "PARSING, TIME",
            ShortToolTip = "Parse a UTC time string",
            ToolTip = "Parse a time string and return seconds past the J2000 epoch on a formal calendar"
            ))
    static void tparse(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FSEphemerisTime& ReturnValue,
        const FString& string = TEXT("2021-10-01T22:46:52.18")
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Time",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "TIME",
            ShortToolTip = "Create a Time Format Picture",
            ToolTip = "Create a time format picture suitable for use by the action timout from a given sample time string"
            ))
    static void tpictr(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        FString& pictur,
        const FString& sample = TEXT("Thu Oct 01 11:11:11 PDT 1111")
    );

    /// <summary>trace of a 3x3 matrix</summary>
    /// <param name="matrix">3x3 matrix of double precision numbers</param>
    /// <returns>trace of matrix, i.e. it is the sum of the diagonal elements of matrix</returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Matrix",
        meta = (
            Keywords = "MATRIX",
            ShortToolTip = "Trace of a 3x3 matrix",
            ToolTip = "Return the trace of a 3x3 matrix"
            ))
    static void trace(
        const FSRotationMatrix& matrix,
        double& trace
    );

    /// <summary>Twice the value of pi</summary>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Constants",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Twice the value of pi",
            ToolTip = "Return twice the value of pi"
            ))
    static void twopi(
        double& two_pi
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Constants",
        meta = (
            Keywords = "CONSTANTS",
            ShortToolTip = "Angle of twice the value pi",
            ToolTip = "Return angle with twice the value of pi"
            ))
    static void twopi_angle(FSAngle& two_pi);

    /// <summary>Two vectors defining an orthonormal frame</summary>
    /// <param name="axdef">[in] Vector defining a principal axis</param>
    /// <param name="indexa">[in] Principal axis number of axdef (X=1, Y=2, Z=3)</param>
    /// <param name="plndef">[in] Vector defining (with axdef) a principal plane</param>
    /// <param name="indexp">[in] Second axis number (with indexa) of principal plane</param>
    /// <param name="mout">[out] Output rotation matrix</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Rotation",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "AXES. FRAME, ROTATION, TRANSFORMATION",
            ShortToolTip = "Two vectors defining an orthonormal frame",
            ToolTip = "Find the transformation to the right-handed frame having a given vector as a specified axis and having a second given vector lying in a specified coordinate plane"
            ))
    static void twovec(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSDimensionlessVector& axdef,
        ES_Axis                         indexa,
        const FSDimensionlessVector& plndef,
        ES_Axis                         indexp,
        FSRotationMatrix& mout
    );

    /// <summary>Compute the normalized cross product of two 3-vectors</summary>
    /// <param name="v1">[in] Left vector for cross product</param>
    /// <param name="v2">[in] Right vector for cross product</param>
    /// <param name="vout">[out] Normalized cross product (v1xv2) / |v1xv2|</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Unitized cross product, 3x3",
            ToolTip = "Compute the normalized cross product of two 3-vectors"
            ))
    static void ucrss(
        const FSDimensionlessVector& v1,
        const FSDimensionlessVector& v2,
        FSDimensionlessVector& vout
    );

    /// <summary>First derivative of a function, df(x)/dx</summary>
    /// <param name="udfunc">[in] Name of the routine</param>
    /// <param name="x">[in] Independent variable of 'udfunc'</param>
    /// <param name="dx">[in] Interval from 'x' for derivative calculation</param>
    /// <param name="deriv">[out] Approximate derivative of 'udfunc' at 'x'</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Math",
        meta = (
            Keywords = "DERIVATIVE, MATH",
            ShortToolTip = "First derivative of a function, df(x)/dx",
            ToolTip = "Calculate the first derivative of a caller-specified function using a three - point estimation"
            ))
        static void uddf(
            const FDerivativeDelegate& udfunc,
            double x,
            double dx,
            double& deriv
        );

    /// <summary>Transform time from one uniform scale to another.  The uniform time scales are TAI, TDT, TDB, ET, JED, JDTDB, JDTDT</summary>
    /// <param name="epoch">[in] An epoch to be converted</param>
    /// <param name="insys">[in] The time scale associated with the input epoch</param>
    /// <param name="outsys">[in] The time scale associated with the function value</param>
    /// <returns> d.p. in outsys that is equivalent to the epoch on the insys time scale</returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Time",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONVERSION, TIME, UTILITY",
            ShortToolTip = "Uniform time scale transformation",
            ToolTip = "Transform time from one uniform scale to another.  The uniform time scales are TAI, TDT, TDB, ET, JED, JDTDB, JDTDT"
            ))
    static void unitim(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        double& out,
        double epoch,
        ES_TimeScale insys = ES_TimeScale::ET,
        ES_TimeScale outsys = ES_TimeScale::ET
    );

    /// <summary>Unit vector and norm, 3 dimensional</summary>
    /// <param name="v1">[in] Vector to be normalized</param>
    /// <param name="vout">[out] Unit vector v1</param>
    /// <param name="vmag">[out] Magnitude of v1</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Unit vector and norm, 3 dimensional",
            ToolTip = "Normalize a double precision 3-vector and return its magnitude"
            ))
    static void unorm_distance(
        const FSDistanceVector& v1,
        FSDimensionlessVector& vout,
        FSDistance& vmag
    );
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Unit vector and norm, 3 dimensional",
            ToolTip = "Normalize a double precision 3-vector and return its magnitude"
            ))
    static void unorm_velocity(
        const FSVelocityVector& v1,
        FSDimensionlessVector& vout,
        FSSpeed& vmag
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Unit vector and norm, 3 dimensional",
            ToolTip = "Normalize a double precision 3-vector and return its magnitude"
            ))
    static void unorm_angular_velocity(
        const FSAngularVelocity& v1,
        FSDimensionlessVector& vout,
        FSAngularRate& vmag
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Unit vector and norm, 3 dimensional",
            ToolTip = "Normalize a double precision 3-vector and return its magnitude"
            ))
    static void unorm(
        const FSDimensionlessVector& v1,
        FSDimensionlessVector& vout,
        double& vmag
    );

    /// <summary>UTC to Ephemeris Time</summary>
    /// <param name="utcstr">[in] Input time string, UTC</param>
    /// <param name="et">[out] Output epoch, ephemeris seconds past J2000</param>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Time",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "TIME",
            ShortToolTip = "UTC to Ephemeris Time",
            ToolTip = "Convert an input time from Calendar or Julian Date format, UTC, to ephemeris seconds past J2000"
            ))
    static void utc2et(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FString& utcstr,
        FSEphemerisTime& et
    );

    /// <summary>Vector addition, 3 dimensional</summary>
    /// <param name="v1">[in] First vector to be added</param>
    /// <param name="v2">[in] Second vector to be added</param>
    /// <param name="vout">[out] Sum vector, v1 + v2</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector addition, 3 dimensional",
            ToolTip = "Add two 3 dimensional vectors"
            ))
    static void vadd_distance(
        const FSDistanceVector& v1,
        const FSDistanceVector& v2,
        FSDistanceVector& vout
    );
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector addition, 3 dimensional",
            ToolTip = "Add two 3 dimensional vectors"
            ))
    static void vadd_velocity(
        const FSVelocityVector& v1,
        const FSVelocityVector& v2,
        FSVelocityVector& vout
    );
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector addition, 3 dimensional",
            ToolTip = "Add two 3 dimensional vectors"
            ))
    static void vadd_angular_velocity(
        const FSAngularVelocity& v1,
        const FSAngularVelocity& v2,
        FSAngularVelocity& vout
    );
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector addition, 3 dimensional",
            ToolTip = "Add two 3 dimensional vectors"
            ))
    static void vadd(
        const FSDimensionlessVector& v1,
        const FSDimensionlessVector& v2,
        FSDimensionlessVector& vout
    );

    /// <summary>Vector cross product, 3 dimensions</summary>
    /// <param name="">[in] Left hand vector for cross product</param>
    /// <param name="">[in] Right hand vector for cross product</param>
    /// <param name="">[out] Cross product v1xv2</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector cross product, 3 dimensions",
            ToolTip = "Compute the cross product of two 3-dimensional vectors"
            ))
    static void vcrss(
        const FSDimensionlessVector& v1,
        const FSDimensionlessVector& v2,
        FSDimensionlessVector& vout
    );

    /// <summary>Vector distance</summary>
    /// <param name="v1, v2">[in] Two 3-vectors</param>
    /// <returns>distance between v1 and v2</returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector distance",
            ToolTip = "Return the distance between two three-dimensional vectors"
            ))
    static void vdist(
        const FSDimensionlessVector& v1,
        const FSDimensionlessVector& v2,
        double& out
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector distance",
            ToolTip = "Return the distance between two three-dimensional vectors"
            ))
    static void vdist_distance(
        const FSDistanceVector& v1,
        const FSDistanceVector& v2,
        FSDistance& out
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector distance",
            ToolTip = "Return the distance between two three-dimensional vectors"
            ))
    static void vdist_velocity(
        const FSVelocityVector& v1,
        const FSVelocityVector& v2,
        FSSpeed& out
    );

    /// <summary>Vector dot product, 3 dimensions</summary>
    /// <param name="v1">First vector in the dot product</param>
    /// <param name="v2">Second vector in the dot product</param>
    /// <returns>dot product of v1 and v2</returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector dot product, 3 dimensions",
            ToolTip = "Compute the dot product of two double precision, 3-dimensional vectors"
            ))
    static void vdot(
        const FSDimensionlessVector& v1,
        const FSDimensionlessVector& v2,
        double& out
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector dot product, 3 dimensions",
            ToolTip = "Compute the dot product of two double precision, 3-dimensional vectors"
            ))
    static void vdot_distance(
        const FSDistanceVector& v1,
        const FSDistanceVector& v2,
        FSDistance& out
    );

    UFUNCTION(BlueprintCallable,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector dot product, 3 dimensions",
            ToolTip = "Compute the dot product of two double precision, 3-dimensional vectors"
            ))
    static void vdot_velocity(
        const FSVelocityVector& v1,
        const FSVelocityVector& v2,
        FSSpeed& out
    );

    /// <summary>Copy one vector to another</summary>
    /// <param name="vin">[in] 3-dimensional double precision vector</param>
    /// <param name="vout">[out] value of vin</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector equality, 3 dimensions",
            ToolTip = "Make one double precision 3-dimensional vector equal to another"
            ))
    static void vequ(
        const FSDimensionlessVector& vin,
        FSDimensionlessVector& vout
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector equality, 3 dimensions",
            ToolTip = "Make one double precision 3-dimensional vector equal to another"
            ))
    static void vequ_distance(
        const FSDistanceVector& vin,
        FSDistanceVector& vout
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector equality, 3 dimensions",
            ToolTip = "Make one double precision 3-dimensional vector equal to another"
            ))
    static void vequ_velocity(
        const FSVelocityVector& vin,
        FSVelocityVector& vout
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector equality, 3 dimensions",
            ToolTip = "Make one double precision 3-dimensional vector equal to another"
            ))
    static void vequ_angular_velocity(
        const FSAngularVelocity& vin,
        FSAngularVelocity& vout
    );

    /// <summary>unit vector</summary>
    /// <param name="v1">[in] Vector to be unitized</param>
    /// <param name="vout">[out] Unit vector</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "\"V - Hat\", unit vector along V, 3 dimensions",
            ToolTip = "Find the unit vector along a double precision 3-dimensional vector"
            ))
    static void vhat(
        const FSDimensionlessVector& v1,
        FSDimensionlessVector& vout
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "\"V - Hat\", unit vector along V, 3 dimensions",
            ToolTip = "Find the unit vector along a double precision 3-dimensional vector"
            ))
    static void vhat_distance(
        const FSDistanceVector& v1,
        FSDimensionlessVector& vout
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "\"V - Hat\", unit vector along V, 3 dimensions",
            ToolTip = "Find the unit vector along a double precision 3-dimensional vector"
            ))
    static void vhat_velocity(
        const FSVelocityVector& v1,
        FSDimensionlessVector& vout
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "\"V - Hat\", unit vector along V, 3 dimensions",
            ToolTip = "Find the unit vector along a double precision 3-dimensional vector"
            ))
    static void vhat_angular_velocity(
        const FSAngularVelocity& v1,
        FSDimensionlessVector& vout
    );

    /// <summary>Vector linear combination, 3 dimensions</summary>
    /// <param name="a">[in] Coefficient of v1</param>
    /// <param name="v1">[in] Vector in 3-space</param>
    /// <param name="b">[in] Coefficient of v2</param>
    /// <param name="v2">[in] Vector in 3-space</param>
    /// <param name="c">[in] Coefficient of v3</param>
    /// <param name="v3">[in] Vector in 3-space</param>
    /// <param name="sum">[out] Linear Vector Combination a*v1 + b*v2 + c*v3</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector linear combination, 3 dimensions",
            ToolTip = "Computes the vector linear combination a * v1 + b * v2 + c * v3 of double precision, 3 - dimensional vectors"
            ))
    static void vlcom3(
        double a,
        const FSDimensionlessVector& v1,
        double b,
        const FSDimensionlessVector& v2,
        double c,
        const FSDimensionlessVector& v3,
        FSDimensionlessVector& sum
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector linear combination, 3 dimensions",
            ToolTip = "Computes the vector linear combination a * v1 + b * v2 + c * v3 of double precision, 3 - dimensional vectors"
            ))
    static void vlcom3_distance(
        double a,
        const FSDistanceVector& v1,
        double b,
        const FSDistanceVector& v2,
        double c,
        const FSDistanceVector& v3,
        FSDistanceVector& sum
    );

    /// <summary>Vector linear combination, 3 dimensions</summary>
    /// <param name="a">[in] Coefficient of v1</param>
    /// <param name="v1">[in] Vector in 3-space</param>
    /// <param name="b">[in] Coefficient of v2</param>
    /// <param name="v2">[in] Vector in 3-space</param>
    /// <param name="sum">[out] Linear Vector Combination a*v1 + b*v2</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector linear combination, 3 dimensions",
            ToolTip = "Compute a vector linear combination of two double precision, 3 - dimensional vectors"
            ))
    static void vlcom(
        double a,
        const FSDimensionlessVector& v1,
        double b,
        const FSDimensionlessVector& v2,
        FSDimensionlessVector& sum
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector linear combination, 3 dimensions",
            ToolTip = "Compute a vector linear combination of two double precision, 3 - dimensional vectors"
            ))
    static void vlcom_distance(
        double a,
        const FSDistanceVector& v1,
        double b,
        const FSDistanceVector& v2,
        FSDistanceVector& sum
    );

    /// <summary>Negation, 3 dimensions</summary>
    /// <param name="v1">[in] Vector to be negated</param>
    /// <param name="vout">[out] Negated vector -v1</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Minus V, \"-V\", 3 dimensions",
            ToolTip = "Negate a double precision 3-dimensional vector"
            ))
    static void vminus(
        const FSDimensionlessVector& v1,
        FSDimensionlessVector& vout
    );


    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Minus V, \"-V\", 3 dimensions",
            ToolTip = "Negate a double precision 3-dimensional distance vector"
            ))
    static void vminus_distance(
        const FSDistanceVector& v1,
        FSDistanceVector& vout
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Minus V, \"-V\", 3 dimensions",
            ToolTip = "Negate a double precision 3-dimensional velocity vector"
            ))
    static void vminus_velocity(
        const FSVelocityVector& v1,
        FSVelocityVector& vout
    );


    /// <summary>Vector norm, 3 dimension</summary>
    /// <param name="v1">[in] Vector whose magnitude is to be found</param>
    /// <returns>magnitude of v1 calculated in a numerically stable way</returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector norm, 3 dimensions",
            ToolTip = "Compute the magnitude of a double precision, 3-dimensional vector"
            ))
    static void vnorm(
        const FSDimensionlessVector& v1,
        double& out
    );

    /// <summary>Vector norm, distance vector</summary>
    /// <param name="v1">[in] Vector whose magnitude is to be found</param>
    /// <returns>magnitude of v1 calculated in a numerically stable way</returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector norm, distance vector",
            ToolTip = "Compute the magnitude of a distance vector"
            ))
    static void vnorm_distance(
        const FSDistanceVector& v1,
        FSDistance& out
    );

    /// <summary>Vector norm, velocity vector</summary>
    /// <param name="v1">[in] Vector whose magnitude is to be found</param>
    /// <returns>magnitude of v1 calculated in a numerically stable way</returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector norm, velocity vector",
            ToolTip = "Compute the magnitude of a velocity vector"
            ))
    static void vnorm_velocity(
        const FSVelocityVector& v1,
        FSSpeed& out
    );


    /// <summary>Pack three scalar components into a vector</summary>
    /// <param name="x">[in] scalar x</param>
    /// <param name="y">[in] scalar y </param>
    /// <param name="z">[in] scalar z </param>
    /// <param name="v">[out] Equivalent 3-vector</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure, Category = "MaxQ|Vector", 
        meta = (
            Keywords = "VECTOR",
            ToolTip = "Pack three RHS scalar components into a RHS dimensionless vector"
            ))
    static void vpack(
        double x,
        double y,
        double z,
        FSDimensionlessVector& v
    );

    UFUNCTION(BlueprintPure, Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ToolTip = "Pack three RHS scalar components into a RHS distance vector"
            ))
    static void vpack_distance(
        double x,
        double y,
        double z,
        FSDistanceVector& km
    );

    UFUNCTION(BlueprintPure, Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ToolTip = "Pack three RHS scalar components into a RHS velocity vector"
            ))
    static void vpack_velocity(
        double x,
        double y,
        double z,
        FSVelocityVector& kmps
    );

    UFUNCTION(BlueprintPure, Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ToolTip = "Pack three RHS scalar components into a RHS State vector"
            ))
    static void vpack_state(
        double x,
        double y,
        double z,
        double dx,
        double dy,
        double dz,
        FSStateVector& km_kmps
    );



    /// <summary>Perpendicular component of a 3-vector</summary>
    /// <param name="">[in] The vector whose orthogonal component is sought</param>
    /// <param name="">[in] The vector used as the orthogonal reference</param>
    /// <param name="">[out] The component of a orthogonal to b</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Perpendicular component of a 3-vector",
            ToolTip = "Find the component of a vector that is perpendicular to a second vector"
            ))
    static void vperp(
        const FSDimensionlessVector& a,
        const FSDimensionlessVector& b,
        FSDimensionlessVector& p
    );

    /// <summary>Project a vector onto a specified plane, orthogonally</summary>
    /// <param name="vin">[in] Vector to be projected</param>
    /// <param name="plane">[in] A CSPICE plane onto which vin is projected</param>
    /// <param name="vout">[out] Vector resulting from projection</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Planes",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "GEOMETRY, MATH, PLAN, VECTOR",
            ShortToolTip = "Vector projection onto plane",
            ToolTip = "Project a vector onto a specified plane, orthogonally"
            ))
    static void vprjp(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSDimensionlessVector& vin,
        const FSPlane& plane,
        FSDimensionlessVector& vout
    );

    /// <summary>Vector projection, 3 dimensions</summary>
    /// <param name="a">[in] The vector to be projected</param>
    /// <param name="b">[in] The vector onto which a is to be projected</param>
    /// <param name="p">[out] The projection of a onto b</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector projection, 3 dimensions",
            ToolTip = "Finds the projection of one vector onto another vector. All vectors are 3 - dimensional"
            ))
    static void vproj(
        const FSDimensionlessVector& a,
        const FSDimensionlessVector& b,
        FSDimensionlessVector& p
    );

    /// <summary>Vector relative difference (scalar)</summary>
    /// <param name="v1, v2">[in] Input vectors</param>
    /// <returns>relative difference between V1 and V2</returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "MATH, VECTOR",
            ShortToolTip = "Vector relative difference, 3 dimensions",
            ToolTip = "Return the relative difference between two 3-dimensional vectors"
            ))
    static void vrel(
        const FSDimensionlessVector& v1,
        const FSDimensionlessVector& v2,
        double& out
    );

    /// <summary>Vector rotation about an axis</summary>
    /// <param name="v">[in] Vector to be rotated</param>
    /// <param name="axis">[in] Axis of the rotation</param>
    /// <param name="theta">[in] Angle of rotation (radians)</param>
    /// <param name="r">[out] Result of rotating v about axis by theta</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Rotation",
        meta = (
            Keywords = "ROTATION, VECTOR",
            ShortToolTip = "Vector rotation about an axis",
            ToolTip = "Rotate a vector about a specified axis vector by a specified angle and return the rotated vector"
            ))
    static void vrotv(
        const FSDimensionlessVector& v,
        const FSDimensionlessVector& axis,
        const FSAngle& theta,
        FSDimensionlessVector& r
    );

    /// <summary>Vector scaling, 3 dimensions</summary>
    /// <param name="s">[in] Scalar to multiply a vector</param>
    /// <param name="v1">[in] Vector to be multiplied</param>
    /// <param name="vout">[out] Product vector, s*v1</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector scaling, 3 dimensions",
            ToolTip = "Multiply a scalar and a 3-dimensional double precision vector"
            ))
    static void vscl(
        double s,
        const FSDimensionlessVector& v1,
        FSDimensionlessVector& vout
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector scaling, 3 dimensions",
            ToolTip = "Multiply a scalar and a 3-dimensional double precision distance vector"
            ))
    static void vscl_distance(
        double s,
        const FSDistanceVector& v1,
        FSDistanceVector& vout
    );


    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Vector scaling, 3 dimensions",
            ToolTip = "Multiply a scalar and a 3-dimensional double precision velocity vector"
            ))
    static void vscl_velocity(
        double s,
        const FSVelocityVector& v1,
        FSVelocityVector& vout
    );


    /// <summary>Angular separation of vectors, 3 dimensions</summary>
    /// <param name="v1">First vector</param>
    /// <param name="v2">Second vector</param>
    /// <returns>is the angle between v1 and v2 expressed in radians</returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "VECTOR",
            ShortToolTip = "Angular separation of vectors, 3 dimensions",
            ToolTip = "Find the separation angle in radians between two double precision, 3 - dimensional vectors.This angle is defined as zero if either vector is zero"
            ))
    static void vsep(
        const FSDimensionlessVector& v1,
        const FSDimensionlessVector& v2,
        FSAngle& out
    );

    /// <summary>Vector subtraction, 3 dimensions</summary>
    /// <param name="v1">[in] First vector (minuend)</param>
    /// <param name="v2">[in] Second vector (subtrahend)</param>
    /// <param name="vout">[out] Difference vector, v1 - v2</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "ANGLE, VECTOR",
            ShortToolTip = "Vector subtraction, 3 dimensions",
            ToolTip = "Compute the difference between two 3-dimensional, double precision vectors"
            ))
    static void vsub(
        const FSDimensionlessVector& v1,
        const FSDimensionlessVector& v2,
        FSDimensionlessVector& vout
    );

    /// <summary>Vector subtraction, 3 dimensions</summary>
    /// <param name="v1">[in] First vector (minuend)</param>
    /// <param name="v2">[in] Second vector (subtrahend)</param>
    /// <param name="vout">[out] Difference vector, v1 - v2</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "ANGLE, VECTOR",
            ShortToolTip = "Vector subtraction, 3 dimensions",
            ToolTip = "Compute the difference between two 3-dimensional, double precision vectors"
            ))
    static void vsub_distance(
        const FSDistanceVector& v1,
        const FSDistanceVector& v2,
        FSDistanceVector& vout
    );

    /// <summary>Vector subtraction, 3 dimensions</summary>
    /// <param name="v1">[in] First vector (minuend)</param>
    /// <param name="v2">[in] Second vector (subtrahend)</param>
    /// <param name="vout">[out] Difference vector, v1 - v2</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "ANGLE, VECTOR",
            ShortToolTip = "Vector subtraction, 3 dimensions",
            ToolTip = "Compute the difference between two 3-dimensional, double precision vectors"
            ))
    static void vsub_velocity(
        const FSVelocityVector& v1,
        const FSVelocityVector& v2,
        FSVelocityVector& vout
    );

    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Matrix",
        meta = (
            Keywords = "MATRIX, VECTOR",
            ShortToolTip = "Vector transpose times matrix times vector, 3 dim",
            ToolTip = "Multiply the transpose of a 3-dimensional column vector, a 3x3 matrix, and a 3 - dimensional column vector"
            ))
    static double vtmv(
        const FSDimensionlessVector& v1,
        const FSRotationMatrix& matrix,
        const FSDimensionlessVector& v2
    );


    /// <summary>Unpack three scalar components from a vector</summary>
    /// <param name="v">[in] 3-vector</param>
    /// <param name="x, y, z">[out] Scalar components of 3-vector</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure, Category = "MaxQ|Vector", meta = (Keywords = "VECTOR", ToolTip = "Unpack three RHS scalar components from a RHS dimensionless vector"))
    static void vupack(
        const FSDimensionlessVector& v,
        double& x,
        double& y,
        double& z
    );

    UFUNCTION(BlueprintPure, Category = "MaxQ|Vector", meta = (Keywords = "VECTOR", ToolTip = "Unpack three RHS scalar components from a RHS distance vector"))
    static void vupack_distance(
        const FSDistanceVector& v,
        double& x,
        double& y,
        double& z
    );


    UFUNCTION(BlueprintPure, Category = "MaxQ|Vector", meta = (Keywords = "VECTOR", ToolTip = "Unpack three RHS scalar components from a RHS velocity vector"))
    static void vupack_velocity(
        const FSVelocityVector& v,
        double& x,
        double& y,
        double& z
    );

    UFUNCTION(BlueprintPure, Category = "MaxQ|Vector", meta = (Keywords = "VECTOR", ToolTip = "Unpack three THS scalar components from a RHS state vector"))
    static void vupack_state(
        const FSStateVector& v,
        double& x,
        double& y,
        double& z,
        double& dx,
        double& dy,
        double& dz
    );



    /// <summary></summary>
    /// <param name="v">[in] Vector to be tested</param>
    /// <returns>The function returns the value SPICETRUE if and only if v is the zero vector</returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Vector",
        meta = (
            Keywords = "MATH, VECTOR",
            ShortToolTip = "Is a vector the zero vector?",
            ToolTip = "Indicate whether a 3-vector is the zero vector"
            ))
    static void vzero(
        const FSDimensionlessVector& v,
        bool& is_zero
    );


    /// <summary>Convert a state transformation matrix to Euler angles and their derivatives with respect to a specified set of axes</summary>
    /// <param name="xform">A state transformation matrix</param>
    /// <param name="axisa">Axis A of the Euler angle factorization</param>
    /// <param name="axisb">Axis B of the Euler angle factorization</param>
    /// <param name="axisc">Axis C of the Euler angle factorization</param>
    /// <param name="eulang">An array of Euler angles and their derivatives</param>
    /// <param name="unique">Indicates if eulang is a unique representation</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|Rotation",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "ANGLES, DERIVATIVES, STATE",
            ShortToolTip = "State transformation to Euler angles",
            ToolTip = "Convert a state transformation matrix to Euler angles and their derivatives with respect to a specified set of axes"
            ))
        static void xf2eul(
            ES_ResultCode& ResultCode,
            FString& ErrorMessage,
            FSEulerAngularState& eulang,
            bool& unique,
            const FSEulerAngularTransform& xform,
            ES_Axis             axis3 = ES_Axis::X,
            ES_Axis             axis2 = ES_Axis::Y,
            ES_Axis             axis1 = ES_Axis::Z
        );

    /// <summary>Transform to rotation and angular velocity</summary>
    /// <param name="xform">[in] state transformation matrix</param>
    /// <param name="rot">[out] rotation associated with xform</param>
    /// <param name="av">[out] angular velocity associated with xform</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Rotation",
        meta = (
            Keywords = "FRAMES",
            ShortToolTip = "Transform to rotation and angular velocity",
            ToolTip = "Determines the rotation matrix and angular velocity of the rotation from a state transformation matrix"
            ))
        static void xf2rav(
            const FSStateTransform& xform,
            FSRotationMatrix& rot,
            FSAngularVelocity& av
        );


    /// <summary>Transform a state between coordinate systems</summary>
    /// <param name="f">[in] Input state</param>
    /// <param name="input_coord_sys">[in] Current (input) coordinate system</param>
    /// <param name="output_coord_sys">[in] Desired (output) coordinate system</param>
    /// <param name="body">[in] Name or NAIF ID of body with which coordinates are associated</param>
    /// <param name="output_state">[out] Converted output state</param>
    /// <returns></returns>
    UFUNCTION(
        BlueprintCallable,
        Category = "MaxQ|Coordinates",
        meta = (
            ExpandEnumAsExecs = "ResultCode",
            Keywords = "CONVERSION, COORDINATE, EPHEMERIS, STATE",
            ToolTip = "Transform a state between coordinate systems"
            ))
    static void xfmsta(
        ES_ResultCode& ResultCode,
        FString& ErrorMessage,
        const FSDimensionlessStateVector& in,
        FSDimensionlessStateVector& out,
        ES_CoordinateSystem input_coord_sys = ES_CoordinateSystem::RECTANGULAR,
        ES_CoordinateSystem output_coord_sys = ES_CoordinateSystem::RECTANGULAR,
        const FString& body = TEXT("")
        );


    /// <summary>Transpose a matrix, 3x3</summary>
    /// <param name="m1">[in] 3x3 matrix to be transposed</param>
    /// <param name="mout">[out] Transpose of m1.  mout can overwrite m1</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Matrix",
        meta = (
            Keywords = "MATRIX",
            ShortToolTip = "Transpose a matrix, 3x3",
            ToolTip = "Indicate whether a 3-vector is the zero vector"
            ))
    static void xpose(
        const FSRotationMatrix& m1,
        FSRotationMatrix& mout
    );


    /// <summary></summary>
    /// <param name="string">Input time string, UTC</param>
    /// <param name="lenout">Available space in output error message string</param>
    /// <param name="sp2000">Equivalent UTC seconds past J2000</param>
    /// <param name="errmsg">Descriptive error message</param>
    /// <returns></returns>
    UFUNCTION(BlueprintPure,
        Category = "MaxQ|Time",
        meta = (
            Keywords = "TIME",
            ShortToolTip = "Approximate current et (suitable for visualizations)",
            ToolTip = "Approximate current ephemeris time, based on clock of local and sketchy CRT conversion"
            ))
    static void et_now(FSEphemerisTime& Now);

    UFUNCTION(BlueprintPure, meta = (ShortToolTip = "Flattening Coefficient", ToolTip = "Flattening Coefficient: (Re-Rp)/Re, where 'rp' is the polar radius of the spheroid"), Category = "MaxQ|Math")
    static void FlatteningCoefficient(const FSDistanceVector& Radii, double& f);

#pragma region deprecated
    // No scaling... Swizzle only!  No other transformations!
    // #todo:  Add generalized swizzle + transform functionality
    [[deprecated("Use USpice::Swizzle()")]]
    static void SwizzleToUE(const double(&v)[3], FVector& ue);
    [[deprecated("Use USpice::Swizzle()")]]
    static void SwizzleToSpice(const FVector& ue, double(&v)[3]);
    [[deprecated("Use USpice::Swazzle")]]
    static void SwizzleToUE(const double(&q)[4], FQuat& ue);
    [[deprecated("Use USpice::Swazzle")]]
    static void SwizzleToSpice(const FQuat& ue, double(&q)[4]);
#pragma endregion deprecated



    // ------------------------------------------------------------------------
    // Unit Test Support
    // Random things that make it easier to validate MaxQ.
    // ------------------------------------------------------------------------
    // Entry point to assist unit testing.
    static void get_implied_result(
        ES_ResultCode& impliedResultCode,
        FString& impliedErrorMessage
    );


    // Entry point to assist unit testing.
    static void raise_spice_error(
        const FString& ErrorMessage = TEXT("This is a test error."),
        const FString& SpiceError = TEXT("SPICE(VALUEOUTOFRANGE)")
    );


    // Furnsh with an absolute path (provides support for unit tests, should not
    // be used otherwise).  This ensures kernel files can be loaded without UE
    // dependencies into the Spice module's memory space.
    static void furnsh_absolute(
        const FString& absolutePath
    );
};


