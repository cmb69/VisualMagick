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
#ifndef __Project__
#define __Project__

#include "ConfigureWizard.h"
#include "ProjectFile.h"

class Project
{
public:

  wstring configDefine() const;

  vector<wstring> &defines();

  vector<wstring> &definesDll();

  vector<wstring> &definesLib();

  vector<wstring> &dependencies();

  vector<wstring> &directories();

  vector<wstring> &excludes();

  vector<wstring> &excludesX64();

  vector<wstring> &excludesX86();

  vector<ProjectFile*> &files();

  vector<wstring> &includes();

  bool isConsole() const;

  bool isCom() const;

  bool isDll() const;

  bool isExe() const;

  bool isFuzz() const;

  bool isLib() const;

  bool isModule() const;

  bool isSupported(const int visualStudioVersion) const;

  vector<wstring> &libraries();

  wstring license() const;

  wstring moduleDefinitionFile() const;

  wstring name() const;

  vector<wstring> &references();

  bool treatWarningAsError() const;

  bool useNasm() const;

  bool useUnicode() const;

  wstring version() const;

  int warningLevel() const;

  void checkFiles(const int visualStudioVersion);

  static Project* create(wstring name);

  bool loadFiles(const ConfigureWizard &wizard);

  void mergeProjectFiles(const ConfigureWizard &wizard);

  bool shouldSkip(const ConfigureWizard &wizard);


private:
  Project(wstring name);

  void addLines(wifstream &config,wstring &value);

  void addLines(wifstream &config,vector<wstring> &container);

  void loadConfig(wifstream &config);

  void loadModules(const ConfigureWizard &wizard);

  void loadVersion();

  wstring              _configDefine;
  vector<wstring>      _defines;
  vector<wstring>      _definesDll;
  vector<wstring>      _definesLib;
  vector<wstring>      _dependencies;
  vector<wstring>      _directories;
  vector<wstring>      _excludes;
  vector<wstring>      _excludesX64;
  vector<wstring>      _excludesX86;
  vector<ProjectFile*> _files;
  bool                 _hasIncompatibleLicense;
  vector<wstring>      _includes;
  bool                 _isEnabled;
  bool                 _isOptional;
  vector<wstring>      _libraries;
  wstring              _license;
  wstring              _moduleDefinitionFile;
  wstring              _modulePrefix;
  wstring              _name;
  vector<wstring>      _references;
  bool                 _treatWarningAsError;
  int                  _type;
  bool                 _useNasm;
  bool                 _useUnicode;
  int                  _minimumVisualStudioVersion;
  wstring              _version;
  int                  _warningLevel;
};

#endif // __Project__