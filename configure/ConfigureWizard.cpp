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
#include "resource.h"
#include "ConfigureWizard.h"
#include "CommandLineInfo.h"
#include "Shared.h"

IMPLEMENT_DYNAMIC(ConfigureWizard,CPropertySheet)

ConfigureWizard::ConfigureWizard(CWnd* pWndParent)
  : CPropertySheet(IDS_PROPSHT_CAPTION,pWndParent)
{
  AddPage(&_welcomePage);
  AddPage(&_targetPage);
  AddPage(&_systemPage);
  AddPage(&_finishedPage);

  SetWizardMode();
}

ConfigureWizard::~ConfigureWizard()
{
}

wstring ConfigureWizard::binDirectory() const
{
  return(_systemPage.binDirectory());
}

bool ConfigureWizard::build64bit() const
{
  return(_targetPage.build64bit());
}

bool ConfigureWizard::excludeDeprecated() const
{
  return(_targetPage.excludeDeprecated());
}

wstring ConfigureWizard::fuzzBinDirectory() const
{
  return(_systemPage.fuzzBinDirectory());
}

bool ConfigureWizard::includeIncompatibleLicense() const
{
  return(_targetPage.includeIncompatibleLicense());
}

bool ConfigureWizard::includeOptional() const
{
  return(_targetPage.includeOptional());
}

bool ConfigureWizard::installedSupport() const
{
	return(_targetPage.installedSupport());
}

wstring ConfigureWizard::intermediateDirectoryDebug() const
{
  return(_systemPage.intermediateDirectoryDebug());
}

wstring ConfigureWizard::intermediateDirectoryRelease() const
{
  return(_systemPage.intermediateDirectoryRelease());
}

wstring ConfigureWizard::libDirectory() const
{
  return(_systemPage.libDirectory());
}

wstring ConfigureWizard::platform() const
{
  return(_targetPage.build64bit() ? L"x64" : L"Win32");
}

int ConfigureWizard::quantumDepth() const
{
  return(_targetPage.quantumDepth());
}

wstring ConfigureWizard::solutionName() const
{
  if (solutionType() == DYNAMIC_MT)
    return(L"DynamicMT");
  else if (solutionType() == STATIC_MTD)
    return(L"StaticMTD");
  else if (solutionType() == STATIC_MT)
    return(L"StaticMT");
  else
    return(L"ThisShouldNeverHappen");
}

int ConfigureWizard::solutionType() const
{
  return(_targetPage.solutionType());
}

wstring ConfigureWizard::targetCpu() const
{
  return(_targetPage.build64bit() ? L"x64" : L"x86");
}

bool ConfigureWizard::useHDRI() const
{
  return(_targetPage.useHDRI());
}

bool ConfigureWizard::useOpenCL() const
{
  return(_targetPage.useOpenCL());
}

bool ConfigureWizard::useOpenMP() const
{
  return(_targetPage.useOpenMP());
}

int ConfigureWizard::visualStudioVersion() const
{
  return(_targetPage.visualStudioVersion());
}

wstring ConfigureWizard::visualStudioVersionName() const
{
  switch(_targetPage.visualStudioVersion())
  {
    case VS2012: return(L"VS2012");
    case VS2013: return(L"VS2013");
    case VS2015: return(L"VS2015");
    case VS2017: return(L"VS2017");
    case VS2019: return(L"VS2019");
    case VS2022: return(L"VS2022");
    default: return(L"VS");
  }
}

bool ConfigureWizard::zeroConfigurationSupport() const
{
  return(_targetPage.zeroConfigurationSupport());
}

void ConfigureWizard::parseCommandLineInfo(const CommandLineInfo &info)
{
  _targetPage.build64bit(info.build64bit());
  _targetPage.excludeDeprecated(info.excludeDeprecated());
  _targetPage.includeIncompatibleLicense(info.includeIncompatibleLicense());
  _targetPage.includeOptional(info.includeOptional());
  _targetPage.installedSupport(info.installedSupport());
  _targetPage.quantumDepth(info.quantumDepth());
  _targetPage.solutionType(info.solutionType());
  _targetPage.useHDRI(info.useHDRI());
  _targetPage.useOpenCL(info.useOpenCL());
  _targetPage.useOpenMP(info.useOpenMP());
  _targetPage.visualStudioVersion(info.visualStudioVersion());
  _targetPage.zeroConfigurationSupport(info.zeroConfigurationSupport());
}

BEGIN_MESSAGE_MAP(ConfigureWizard,CPropertySheet)
END_MESSAGE_MAP()