
void rootlogon()
{
    std::cout << "RUNNING LOCAL LOGON" << std::endl;
    gSystem->AddDynamicPath("$PWD/Core/SharedLibs");
    gInterpreter->AddIncludePath("$PWD/Core/Headers");
    gSystem->Load("libUtilsRectifyTracks.so");
    gSystem->Load("libGeneralUtils.so");
    gSystem->Load("libUtilsCreateTree.so");
    gSystem->Load("libUtilsRetina.so");

}