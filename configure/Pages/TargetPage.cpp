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
#include "..\stdafx.h"
#include "..\Shared.h"
#include "TargetPage.h"

IMPLEMENT_DYNCREATE(TargetPage, CPropertyPage)

TargetPage::TargetPage() : CPropertyPage(IDD_TARGET_PAGE)
{
  setVisualStudioVersion();

  _build64bit=FALSE;
  _excludeDeprecated=TRUE;
  _includeIncompatibleLicense=FALSE;
  _installedSupport=FALSE;
  _quantumDepth=Q16;
  _solutionType=DYNAMIC_MT;
  _useHDRI=PathFileExists(L"..\\MagickCore") ? TRUE : FALSE;
  _useOpenCL=TRUE;
  _useOpenMP=TRUE;
  _zeroConfigurationSupport=FALSE;
}

TargetPage::~TargetPage()
{
}

bool TargetPage::excludeDeprecated() const
{
  return(_excludeDeprecated == 1);
}

void TargetPage::excludeDeprecated(bool value)
{
  _excludeDeprecated=value;
}

bool TargetPage::includeIncompatibleLicense() const
{
  return(_includeIncompatibleLicense == 1);
}

void TargetPage::includeIncompatibleLicense(bool value)
{
  _includeIncompatibleLicense=value;
}

bool TargetPage::includeOptional() const
{
  return(_includeOptional == 1);
}

void TargetPage::includeOptional(bool value)
{
  _includeOptional=value;
}

bool TargetPage::installedSupport() const
{
	return(_installedSupport == 1);
}

void TargetPage::installedSupport(bool value)
{
	_installedSupport = value;
}

bool TargetPage::build64bit() const
{
  return(_build64bit == 1);
}

void TargetPage::build64bit(bool value)
{
  _build64bit=value;
}

int TargetPage::quantumDepth() const
{
  return(_quantumDepth);
}

void TargetPage::quantumDepth(int value)
{
  _quantumDepth=value;
}

int TargetPage::solutionType() const
{
  return(_solutionType);
}

void TargetPage::solutionType(int value)
{
  _solutionType=value;
}

bool TargetPage::useHDRI() const
{
  return(_useHDRI == 1);
}

void TargetPage::useHDRI(bool value)
{
  _useHDRI=value;
}

bool TargetPage::useOpenCL() const
{
  return(_useOpenCL == 1);
}

void TargetPage::useOpenCL(bool value)
{
  _useOpenCL=value;
}

bool TargetPage::useOpenMP() const
{
  return(_useOpenMP == 1);
}

void TargetPage::useOpenMP(bool value)
{
  _useOpenMP=value;
}

int TargetPage::visualStudioVersion() const
{
  return(_visualStudioVersion);
}

void TargetPage::visualStudioVersion(int value)
{
  _visualStudioVersion=value;
}

bool TargetPage::zeroConfigurationSupport() const
{
  return(_zeroConfigurationSupport != 0);
}

void TargetPage::zeroConfigurationSupport(bool value)
{
  _zeroConfigurationSupport=value;
}

void TargetPage::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);

  DDX_Check(pDX,IDC_BUILD_64_BIT,_build64bit);
  DDX_CBIndex(pDX,IDC_QUANTUM_DEPTH,_quantumDepth);
  DDX_Radio(pDX,IDC_PROJECT_DYNAMIC_MT,_solutionType);
  DDX_Check(pDX,IDC_HDRI,_useHDRI);
  DDX_Check(pDX,IDC_OPEN_CL,_useOpenCL);
  DDX_Check(pDX,IDC_OPEN_MP,_useOpenMP);
  DDX_CBIndex(pDX,IDC_VISUALSTUDIO,_visualStudioVersion);
  DDX_Check(pDX,IDC_INCLUDE_INCOMPATIBLE,_includeIncompatibleLicense);
  DDX_Check(pDX,IDC_INCLUDE_OPTIONAL,_includeOptional);
  DDX_Check(pDX,IDC_EXCLUDE_DEPRECATED,_excludeDeprecated);
  DDX_Check(pDX,IDC_INSTALLED_SUPPORT,_installedSupport);
  DDX_Check(pDX,IDC_ZERO_CONFIGURATION_SUPPORT,_zeroConfigurationSupport);
}

BOOL TargetPage::OnInitDialog()
{
  CPropertyPage::OnInitDialog();

  UpdateData(FALSE);

  return TRUE;
}

BEGIN_MESSAGE_MAP(TargetPage, CPropertyPage)
END_MESSAGE_MAP()

wstring TargetPage::getEnvironmentVariable(const wchar_t *name)
{
  wchar_t
    *buffer;

  size_t
    length;

  wstring
    value;

  if (_wdupenv_s(&buffer,&length,name) == 0)
  {
    if ((buffer != (wchar_t *) NULL) && (length > 0))
    {
      value=wstring(buffer);
      free(buffer);
      return(value);
    }
  }

  return(value);
}

bool TargetPage::hasVisualStudioFolder(const wchar_t *name)
{
  wstring
    path;

  path=getEnvironmentVariable(L"ProgramW6432") + L"\\Microsoft Visual Studio\\" + name;
  if (PathFileExists(path.c_str()))
    return(true);
  path=getEnvironmentVariable(L"ProgramFiles(x86)") + L"\\Microsoft Visual Studio\\" + name;
  return(PathFileExists(path.c_str()) ? true : false);
}

void TargetPage::setVisualStudioVersion()
{
  if (hasVisualStudioFolder(L"2022"))
    _visualStudioVersion=VS2022;
  else if (hasVisualStudioFolder(L"2019"))
    _visualStudioVersion=VS2019;
  else if (hasVisualStudioFolder(L"2017"))
    _visualStudioVersion=VS2017;
  else if (!getEnvironmentVariable(L"VS140COMNTOOLS").empty())
    _visualStudioVersion=VS2015;
  else if (!getEnvironmentVariable(L"VS120COMNTOOLS").empty())
    _visualStudioVersion=VS2013;
  else if (!getEnvironmentVariable(L"VS110COMNTOOLS").empty())
    _visualStudioVersion=VS2012;
  else
    _visualStudioVersion=VSLATEST;
}

