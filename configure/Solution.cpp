/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%  Copyright 2014-2021 ImageMagick Studio LLC, a non-profit organization      %
%  dedicated to making software imaging solutions freely available.           %
%                                                                             %
%  You may not use this file except in compliance with the License.  You may  %
%  obtain a copy of the License at                                            %
%                                                                             %
%    http://www.imagemagick.org/script/license.php                            %
%                                                                             %
%  Unless required by applicable law or agreed to in writing, software        %
%  distributed under the License is distributed on an "AS IS" BASIS,          %
%  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   %
%  See the License for the specific language governing permissions and        %
%  limitations under the License.                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/
#include "stdafx.h"
#include "Solution.h"
#include "Shared.h"
#include "VersionInfo.h"

Solution::Solution()
{
}

int Solution::loadProjectFiles(const ConfigureWizard &wizard)
{
  int
    count;

  count=0;
  foreach (Project*,p,_projects)
  {
    if (!(*p)->isSupported(wizard.visualStudioVersion()))
      continue;

    if (!(*p)->loadFiles(wizard))
      continue;

    foreach (ProjectFile*,pf,(*p)->files())
    {
      (*pf)->loadConfig();
      count++;
    }

    (*p)->checkFiles(wizard.visualStudioVersion());

    (*p)->mergeProjectFiles(wizard);
  }

  return(count);
}

void Solution::loadProjects()
{
  HANDLE
    fileHandle;

  WIN32_FIND_DATA
    data;

  fileHandle=FindFirstFile(L"..\\*.*",&data);
  do
  {
    if (fileHandle == INVALID_HANDLE_VALUE)
      return;

    if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
      continue;

    if (_wcsicmp(data.cFileName,L".") == 0)
      continue;

    if (_wcsicmp(data.cFileName,L"..") == 0)
      continue;

     Project* project = Project::create(data.cFileName);
     if (project != (Project *) NULL)
       _projects.push_back(project);
  } while (FindNextFile(fileHandle,&data));

  FindClose(fileHandle);
}

void Solution::write(const ConfigureWizard &wizard,WaitDialog &waitDialog)
{
  int
    steps;

  VersionInfo
    versionInfo;

  wofstream
    file;

  steps=loadProjectFiles(wizard);
  /* write solution, configuration, MakeFile.PL and version */
  waitDialog.setSteps(steps+4);

  file.open(getFileName(wizard));
  if (!file)
    return;

  waitDialog.nextStep(L"Writing solution");

  write(wizard,file);

  file.close();

  foreach (Project*,p,_projects)
  {
    foreach (ProjectFile*,pf,(*p)->files())
    {
      waitDialog.nextStep(L"Writing: " + (*pf)->fileName());
      (*pf)->write(_projects);
    }
  }

  waitDialog.nextStep(L"Writing configuration");
  writeMagickBaseConfig(wizard);

  waitDialog.nextStep(L"Writing threshold-map.h");
  writeThresholdMap(wizard);

  waitDialog.nextStep(L"Writing Makefile.PL");
  writeMakeFile(wizard);

  if (!versionInfo.load())
    return;

  waitDialog.nextStep(L"Writing version");
  writeVersion(wizard,versionInfo);

  waitDialog.nextStep(L"Writing NOTICE.txt");
  writeNotice(wizard,versionInfo);
}

wstring Solution::getFileName(const ConfigureWizard &wizard)
{
  wstring
    fileName;

  fileName=L"..\\Visual" + wizard.solutionName();

  return(fileName+L".sln");
}

wstring Solution::getFolder()
{
  wstring
    folder;

  folder=L"MagickCore";
  if (!PathFileExists((L"..\\..\\ImageMagick\\" + folder).c_str()))
    folder=L"magick";
  return(folder);
}

void Solution::writeMagickBaseConfig(const ConfigureWizard &wizard)
{
  wstring
    folder,
    line;

  wifstream
    configIn;

  wofstream
    config;

  folder=getFolder();

  configIn.open(L"..\\" + folder + L"\\magick-baseconfig.h.in");
  if (!configIn)
    return;

  config.open(L"..\\..\\ImageMagick\\" + folder + L"\\magick-baseconfig.h");
  if (!config)
    return;

  while (getline(configIn,line))
  {
    if (trim(line).compare(L"$$CONFIG$$") != 0)
    {
      config << line << endl;
      continue;
    }

    config << "/*" << endl;
    config << "  Define to build a ImageMagick which uses registry settings or" << endl;
    config << "  hard-coded paths to locate installed components.  This supports" << endl;
    config << "  using the \"setup.exe\" style installer, or using hard-coded path" << endl;
    config << "  definitions (see below).  If you want to be able to simply copy" << endl;
    config << "  the built ImageMagick to any directory on any directory on any machine," << endl;
    config << "  then do not use this setting." << endl;
    config << "*/" << endl;
    if (wizard.installedSupport())
      config << "#define MAGICKCORE_INSTALLED_SUPPORT" << endl;
    else
      config << "//#define MAGICKCORE_INSTALLED_SUPPORT" << endl;
    config << endl;

    config << "/*" << endl;
    config << "  Specify size of PixelPacket color Quantums (8, 16, or 32)." << endl;
    config << "  A value of 8 uses half the memory than 16 and typically runs 30% faster," << endl;
    config << "  but provides 256 times less color resolution than a value of 16." << endl;
    config << "*/" << endl;
    if (wizard.quantumDepth() == Q8)
      config << "#define MAGICKCORE_QUANTUM_DEPTH 8" << endl;
    else if (wizard.quantumDepth() == Q16)
      config << "#define MAGICKCORE_QUANTUM_DEPTH 16" << endl;
    else if (wizard.quantumDepth() == Q32)
      config << "#define MAGICKCORE_QUANTUM_DEPTH 32" << endl;
    else if (wizard.quantumDepth() == Q64)
      config << "#define MAGICKCORE_QUANTUM_DEPTH 64" << endl;
    config << endl;

    config << "/*" << endl;
    config << "  Define to enable high dynamic range imagery (HDRI)" << endl;
    config << "*/" << endl;
    if (wizard.useHDRI())
      config << "#define MAGICKCORE_HDRI_ENABLE 1" << endl;
    else
      config << "#define MAGICKCORE_HDRI_ENABLE 0" << endl;
    config << endl;

    config << "/*" << endl;
    config << "  Define to enable OpenCL" << endl;
    config << "*/" << endl;
    if (wizard.useOpenCL())
      config << "#define MAGICKCORE__OPENCL\n#define MAGICKCORE_HAVE_CL_CL_H" << endl;
    else
      config << "#undef MAGICKCORE__OPENCL\n#undef MAGICKCORE_HAVE_CL_CL_H" << endl;
    config << endl;

    config << "/*" << endl;
    config << "  Exclude deprecated methods in MagickCore API" << endl;
    config << "*/" << endl;
    if (wizard.excludeDeprecated())
      config << "#define MAGICKCORE_EXCLUDE_DEPRECATED" << endl;
    else
      config << "//#define MAGICKCORE_EXCLUDE_DEPRECATED" << endl;
    config << endl;

    config << "/*" << endl;
    config << "  Define to only use the built-in (in-memory) settings." << endl;
    config << "*/" << endl;
    if (wizard.zeroConfigurationSupport())
      config << "#define MAGICKCORE_ZERO_CONFIGURATION_SUPPORT 1" << endl;
    else
      config << "#define MAGICKCORE_ZERO_CONFIGURATION_SUPPORT 0" << endl;

    foreach (Project*,p,_projects)
    {
      if ((*p)->files().size() == 0)
        continue;

      if ((*p)->configDefine().empty())
        continue;

      config << endl;
      config << (*p)->configDefine();
    }
  }
}

void Solution::writeMakeFile(const ConfigureWizard &wizard)
{
  wifstream
    makeFileIn,
    zipIn;

  wofstream
    lib,
    makeFile,
    zip;

  wstring
    libName,
    line;

  libName=L"CORE_RL_" + getFolder()+ L"_";

  lib=wofstream(L"..\\..\\ImageMagick\\PerlMagick\\" + libName + L".a");
  if (!lib)
    return;
  lib.close();

  zipIn=wifstream(L"..\\PerlMagick\\Zip.ps1", std::ios::binary);
  if (!zipIn)
    return;
  zip=wofstream(L"..\\..\\ImageMagick\\PerlMagick\\Zip.ps1", std::ios::binary);
  zip << zipIn.rdbuf();
  zip.close();

  makeFileIn.open(L"..\\PerlMagick\\Makefile.PL.in");
  if (!makeFileIn)
    return;

  makeFile.open(L"..\\..\\ImageMagick\\PerlMagick\\Makefile.PL");
  if (!makeFile)
    return;

  while (getline(makeFileIn,line))
  {
    line=replace(line,L"$$LIB_NAME$$",libName);
    line=replace(line,L"$$PLATFORM$$",wizard.targetCpu());
    makeFile << line << endl;
  }
  makeFile.close();
}

void Solution::writeNotice(const ConfigureWizard &wizard,const VersionInfo &versionInfo)
{
  wofstream
    notice;

  notice.open(L"..\\..\\VisualMagick\\NOTICE.txt");
  if (!notice)
    return;

  notice << "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *" << endl << endl;
  notice << "[ Imagemagick " << versionInfo.version() << versionInfo.libAddendum() << "] copyright:" << endl << endl;
  notice << readLicense(L"..\\..\\ImageMagick\\LICENSE");
  notice << endl << "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *" << endl << endl;

  foreach (Project*,p,_projects)
  {
    if (((*p)->license() == L"") || ((*p)->shouldSkip(wizard)))
      continue;

    notice << "[ " << (*p)->name() << " " << (*p)->version() << " ] copyright:" << endl << endl;
    notice << (*p)->license() << endl;
    notice << endl << "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *" << endl << endl;
  }

  notice.close();
}

void Solution::writeThresholdMap(const ConfigureWizard &wizard)
{
  wifstream
    inputStream;

  wofstream
    outputStream;

  wstring
    line;

  if (!wizard.zeroConfigurationSupport())
    return;

  inputStream.open(L"..\\bin\\thresholds.xml");
  if (!inputStream)
    return;

  outputStream.open(L"..\\..\\ImageMagick\\MagickCore\\threshold-map.h");
  if (!outputStream)
    {
      inputStream.close();
      return;
    }

  outputStream << "static const char *const BuiltinMap=" << endl;

  while (getline(inputStream,line))
  {
    if (line.length() == 0)
      continue;

    line=replace(line,L"\"",L"\\\"");
    outputStream << "\"" << line << "\"" << endl;
  }

  outputStream << ";";

  inputStream.close();
  outputStream.close();
}

void Solution::writeVersion(const ConfigureWizard &wizard,const VersionInfo &versionInfo)
{

  wstring
    folder,
    line;

  folder=getFolder();

  writeVersion(wizard,versionInfo,L"..\\..\\ImageMagick\\"+folder+L"\\version.h.in",L"..\\..\\ImageMagick\\"+folder+L"\\version.h");
  writeVersion(wizard,versionInfo,L"..\\..\\ImageMagick\\config\\configure.xml.in",L"..\\bin\\configure.xml");
  writeVersion(wizard,versionInfo,L"..\\installer\\inc\\version.isx.in",L"..\\installer\\inc\\version.isx");
  writeVersion(wizard,versionInfo,L"..\\utilities\\ImageMagick.version.h.in",L"..\\utilities\\ImageMagick.version.h");
}

void Solution::writeVersion(const ConfigureWizard &wizard,const VersionInfo &versionInfo,wstring input,wstring output)
{
  size_t
    start,
    end;

  wifstream
    inputStream;

  wofstream
    outputStream;

  wstring
    line;

  inputStream.open(input);
  if (!inputStream)
    return;

  outputStream.open(output);
  if (!outputStream)
    {
      inputStream.close();
      return;
    }

  while (getline(inputStream,line))
  {
    line=replace(line,L"@CC@",wizard.visualStudioVersionName());
    line=replace(line,L"@CXX@",wizard.visualStudioVersionName());
    line=replace(line,L"@DOCUMENTATION_PATH@",L"unavailable");
    line=replace(line,L"@LIB_VERSION@",versionInfo.version());
    line=replace(line,L"@MAGICK_LIB_VERSION_NUMBER@",versionInfo.libVersionNumber());
    line=replace(line,L"@MAGICK_LIB_VERSION_TEXT@",versionInfo.version());
    line=replace(line,L"@MAGICK_LIBRARY_CURRENT@",versionInfo.interfaceVersion());
    line=replace(line,L"@MAGICK_LIBRARY_CURRENT_MIN@",versionInfo.interfaceVersion());
    line=replace(line,L"@MAGICK_TARGET_CPU@",wizard.targetCpu());
    line=replace(line,L"@MAGICK_TARGET_OS@",L"Windows");
    line=replace(line,L"@MAGICKPP_LIB_VERSION_TEXT@",versionInfo.version());
    line=replace(line,L"@MAGICKPP_LIBRARY_CURRENT@",versionInfo.ppInterfaceVersion());
    line=replace(line,L"@MAGICKPP_LIBRARY_CURRENT_MIN@",versionInfo.ppInterfaceVersion());
    line=replace(line,L"@MAGICKPP_LIBRARY_VERSION_INFO@",versionInfo.ppLibVersionNumber());
    line=replace(line,L"@MAGICKPP_LIBRARY_VERSION_TEXT@",versionInfo.version());
    line=replace(line,L"@PACKAGE_BASE_VERSION@",versionInfo.version());
    line=replace(line,L"@PACKAGE_FULL_VERSION@",versionInfo.fullVersion());
    line=replace(line,L"@PACKAGE_LIB_VERSION@",versionInfo.libVersion());
    line=replace(line,L"@PACKAGE_LIB_VERSION_NUMBER@",versionInfo.versionNumber());
    line=replace(line,L"@PACKAGE_NAME@",L"ImageMagick");
    line=replace(line,L"@PACKAGE_VERSION_ADDENDUM@",versionInfo.libAddendum());
    line=replace(line,L"@PACKAGE_RELEASE_DATE@",versionInfo.releaseDate());
    line=replace(line,L"@QUANTUM_DEPTH@",to_wstring(wizard.quantumDepth()));
    line=replace(line,L"@RELEASE_DATE@",versionInfo.releaseDate());
    line=replace(line,L"@TARGET_OS@",L"Windows");
    start=line.find(L"@");
    if (start != string::npos)
    {
      end=line.find(L"@",start+1);
      if (end != string::npos)
        checkKeyword(line.substr(start+1,end-start-1));
      continue;
    }
    outputStream << line << endl;
  }

  inputStream.close();
  outputStream.close();
}

void Solution::checkKeyword(const wstring keyword)
{
  vector<wstring> skipableKeywords={
    L"CODER_PATH",L"CONFIGURE_ARGS",L"CONFIGURE_PATH",L"CXXFLAGS",L"DEFS",L"DISTCHECK_CONFIG_FLAGS",
    L"EXEC_PREFIX_DIR",L"EXECUTABLE_PATH",L"FILTER_PATH",L"host",L"INCLUDE_PATH",L"LIBRARY_PATH",
    L"MAGICK_CFLAGS",L"MAGICK_CPPFLAGS",L"MAGICK_DELEGATES",L"MAGICK_FEATURES",L"MAGICK_GIT_REVISION",
    L"MAGICK_LDFLAGS",L"MAGICK_LIBS",L"MAGICK_PCFLAGS",L"MAGICK_TARGET_VENDOR",L"PREFIX_DIR",
    L"SHARE_PATH",L"SHAREARCH_PATH"
  };

  if (contains(skipableKeywords,keyword))
   return;

  throw exception();
}

void Solution::write(const ConfigureWizard &wizard,wofstream &file)
{
  file << "Microsoft Visual Studio Solution File, Format Version 12.00" << endl;
  if (wizard.visualStudioVersion() == VS2012)
    file << "# Visual Studio 2012" << endl;
  else if (wizard.visualStudioVersion() == VS2013)
    file << "# Visual Studio 2013" << endl;
  else if (wizard.visualStudioVersion() == VS2015)
    file << "# Visual Studio 2015" << endl;
  else if (wizard.visualStudioVersion() == VS2017)
    file << "# Visual Studio 2017" << endl;
  else if (wizard.visualStudioVersion() == VS2019)
    file << "# Visual Studio 2019" << endl;
  else if (wizard.visualStudioVersion() == VS2022)
    file << "# Visual Studio 2022" << endl;

  foreach (Project*,p,_projects)
  {
    foreach (ProjectFile*,pf,(*p)->files())
    {
      file << "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"" << (*pf)->name() << "\", ";
      file << "\".\\" << (*p)->name() << "\\" << (*pf)->fileName() << "\", \"{" << (*pf)->guid() << "}\"" << endl;
      file << "EndProject" << endl;
    }
  }
  file << "EndProject" << endl;

  file << "Global" << endl;
  file << "\tGlobalSection(SolutionConfigurationPlatforms) = preSolution" << endl;
  file << "\t\tDebug|" << wizard.platform() << " = Debug|" << wizard.platform() << endl;
  file << "\t\tRelease|" << wizard.platform() << " = Release|" << wizard.platform() << endl;
  file << "\tEndGlobalSection" << endl;

  file << "\tGlobalSection(ProjectConfigurationPlatforms) = postSolution" << endl;
  foreach (Project*,p,_projects)
  {
    foreach (ProjectFile*,pf,(*p)->files())
    {
      file << "\t\t{" << (*pf)->guid() << "}.Debug|" << wizard.platform() << ".ActiveCfg = Debug|" << wizard.platform() << endl;
      file << "\t\t{" << (*pf)->guid() << "}.Debug|" << wizard.platform() << ".Build.0 = Debug|" << wizard.platform() << endl;
      file << "\t\t{" << (*pf)->guid() << "}.Release|" << wizard.platform() << ".ActiveCfg = Release|" << wizard.platform() << endl;
      file << "\t\t{" << (*pf)->guid() << "}.Release|" << wizard.platform() << ".Build.0 = Release|" << wizard.platform() << endl;
    }
  }
  file << "\tEndGlobalSection" << endl;
  file << "EndGlobal" << endl;
}
