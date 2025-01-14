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
#ifndef __ProjectFile__
#define __ProjectFile__

#include "ConfigureWizard.h"

class Project;

class ProjectFile
{
public:
  ProjectFile(const ConfigureWizard *wizard,Project *project,
    const wstring &prefix,const wstring &name);

  ProjectFile(const ConfigureWizard *wizard,Project *project,
    const wstring &prefix,const wstring &name,const wstring &reference);

  vector<wstring> &dependencies();

  wstring fileName() const;

  wstring guid() const;

  wstring name() const;

  vector<wstring> &aliases();

  bool isSupported(const int visualStudioVersion) const;

  void loadConfig();

  void merge(ProjectFile *projectFile);

  void write(const vector<Project*> &allProjects);

private:

  bool isLib() const;

  wstring outputDirectory() const;

  void addFile(const wstring &directory, const wstring &name);

  void addLines(wifstream &config,vector<wstring> &container);

  wstring createGuid();

  wstring getFilter(const wstring &fileName,vector<wstring> &filters);

  wstring getIntermediateDirectoryName(const bool debug);

  wstring getTargetName(const bool debug);

  void initialize(Project* project);

  bool isSrcFile(const wstring &fileName);

  bool isExcluded(const wstring &fileName);

  void loadAliases();

  void loadModule(const wstring &directory);

  void loadSource();

  void loadSource(const wstring &directory);

  void merge(vector<wstring> &input, vector<wstring> &output);

  void setFileName();

  void writeAdditionalDependencies(wofstream &file,const wstring &separator);

  void writeAdditionalIncludeDirectories(wofstream &file,const wstring &separator);

  void writePreprocessorDefinitions(wofstream &file,const bool debug);

  void write(wofstream &file,const vector<Project*> &allProjects);

  void writeItemDefinitionGroup(wofstream &file,const bool debug);

  void writeFiles(wofstream &file,const vector<wstring> &collection);

  void writeFilter(wofstream &file);

  void writeProjectReferences(wofstream &file,const vector<Project*> &allProjects);

  vector<wstring>        _aliases;
  wstring                _fileName;
  wstring                _guid;
  vector<wstring>        _cppFiles;
  vector<wstring>        _dependencies;
  vector<wstring>        _includeFiles;
  vector<wstring>        _includes;
  vector<wstring>        _definesLib;
  wstring                _name;
  wstring                _prefix;
  Project               *_project;
  wstring                _reference;
  vector<wstring>        _resourceFiles;
  vector<wstring>        _srcFiles;
  int                    _minimumVisualStudioVersion;
  const ConfigureWizard *_wizard;
};

#endif // __ProjectFile__