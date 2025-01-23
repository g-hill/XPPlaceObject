#include "XPLMPlugin.h"
#include "XPLMProcessing.h"
#include "XPLMUtilities.h"
#include "XPLMScenery.h"
#include "XPLMInstance.h"
#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMDataAccess.h"
#include <string>
#include <cstring>

// Global variables
static float lat = 0.0f;
static float lon = 0.0f;
static float elev = 0.0f;
static XPLMWindowID gWindow = NULL;
static char latStr[16] = "0.0", lonStr[16] = "0.0", elevStr[16] = "0.0";
static int selectedField = 0;  // Track selected input field

// Callback function for object placement
void ObjectLoadedCallback(XPLMObjectRef obj, void* refcon) {
    if (obj != NULL) {
        const char* dataRefs[] = { nullptr };
        XPLMInstanceRef gInstance = XPLMCreateInstance(obj, dataRefs);

        XPLMDrawInfo_t pos;
        pos.structSize = sizeof(XPLMDrawInfo_t);
        pos.x = lon;
        pos.y = elev;
        pos.z = lat;

        XPLMInstanceSetPosition(gInstance, &pos, nullptr);

        XPLMDebugString("Object placed successfully at specified coordinates.\n");
    }
    else {
        XPLMDebugString("Failed to load object.\n");
    }
}

// Function to place the object
void PlaceObject(void*) {
    const char path[] = "Resources/plugins/PlaceObject/sample.obj";
    XPLMLoadObjectAsync(path, ObjectLoadedCallback, nullptr);
}

// Key handler function
void HandleKeyCallback(XPLMWindowID inWindowID, char inKey, XPLMKeyFlags inFlags, char inVirtualKey, void* inRefcon, int losingFocus) {
    if (inKey == '\t') {
        selectedField = (selectedField + 1) % 3;
    }
    else if (inKey == '\r') {
        PlaceObject(nullptr);
    }
    else {
        size_t len;
        if (selectedField == 0) {
            len = strlen(latStr);
            if (len < sizeof(latStr) - 1) {
                latStr[len] = inKey;
                latStr[len + 1] = '\0';
            }
        }
        if (selectedField == 1) {
            len = strlen(lonStr);
            if (len < sizeof(lonStr) - 1) {
                lonStr[len] = inKey;
                lonStr[len + 1] = '\0';
            }
        }
        if (selectedField == 2) {
            len = strlen(elevStr);
            if (len < sizeof(elevStr) - 1) {
                elevStr[len] = inKey;
                elevStr[len + 1] = '\0';
            }
        }
    }
}

// Window draw callback
void DrawWindowCallback(XPLMWindowID inWindowID, void* inRefcon) {
    int left, top, right, bottom;
    XPLMGetWindowGeometry(inWindowID, &left, &top, &right, &bottom);

    float white[] = { 1.0f, 1.0f, 1.0f };
    float green[] = { 0.0f, 1.0f, 0.0f };

    XPLMDrawString(white, left + 20, top - 30, (char*)"Latitude:", NULL, xplmFont_Proportional);
    XPLMDrawString(white, left + 100, top - 30, latStr, NULL, xplmFont_Proportional);

    XPLMDrawString(white, left + 20, top - 60, (char*)"Longitude:", NULL, xplmFont_Proportional);
    XPLMDrawString(white, left + 100, top - 60, lonStr, NULL, xplmFont_Proportional);

    XPLMDrawString(white, left + 20, top - 90, (char*)"Elevation:", NULL, xplmFont_Proportional);
    XPLMDrawString(white, left + 100, top - 90, elevStr, NULL, xplmFont_Proportional);

    XPLMDrawString(green, left + 20, top - 130, (char*)"[ Press ENTER to PLACE ]", NULL, xplmFont_Proportional);
}

// Plugin start function
PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc) {
    std::strcpy(outName, "SimpleObjectPlacer");
    std::strcpy(outSig, "xplane.plugin.objectplacer");
    std::strcpy(outDesc, "Places an object at specified coordinates.");

    XPLMCreateWindow_t params;
    params.structSize = sizeof(params);
    params.left = 200;
    params.top = 600;
    params.right = 500;
    params.bottom = 400;
    params.visible = 1;
    params.drawWindowFunc = DrawWindowCallback;
    params.handleKeyFunc = HandleKeyCallback;
    params.layer = xplm_WindowLayerFloatingWindows;
    params.decorateAsFloatingWindow = xplm_WindowDecorationRoundRectangle;
    params.refcon = NULL;

    gWindow = XPLMCreateWindowEx(&params);
    XPLMSetWindowPositioningMode(gWindow, xplm_WindowPositionFree, -1);
    XPLMSetWindowResizingLimits(gWindow, 300, 200, 600, 400);

    XPLMDebugString("SimpleObjectPlacer Plugin Started.\n");
    return 1;
}

// Plugin stop function
PLUGIN_API void XPluginStop(void) {
    if (gWindow) {
        XPLMDestroyWindow(gWindow);
        gWindow = NULL;
    }
    XPLMDebugString("SimpleObjectPlacer Plugin Stopped.\n");
}

// Plugin enable function
PLUGIN_API int XPluginEnable(void) {
    XPLMDebugString("SimpleObjectPlacer Plugin Enabled.\n");
    return 1;
}

// Plugin disable function
PLUGIN_API void XPluginDisable(void) {
    XPLMDebugString("SimpleObjectPlacer Plugin Disabled.\n");
}
