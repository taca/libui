// 28 may 2019

// This file should NOT have include guards as it is intended to be included more than once; see noinitwrongthread.c for details.

allcallsCase(uiMain, /* no arguments */)
allcallsCase(uiQuit, /* no arguments */)
#ifdef allcallsIncludeQueueMain
allcallsCase(uiQueueMain, NULL, NULL)
#endif

allcallsCase(uiControlType, /* no arguments */)

allcallsCase(uiRegisterControlType, NULL, NULL, NULL, 0)
allcallsCase(uiCheckControlType, NULL, 0)

allcallsCase(uiNewControl, 0, NULL)
allcallsCase(uiControlFree, NULL)
allcallsCase(uiControlSetParent, NULL, NULL)
allcallsCase(uiControlImplData, NULL)
allcallsCase(uiControlOnFree, /* no arguments */)