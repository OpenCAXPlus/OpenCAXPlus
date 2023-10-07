// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#include <stdio.h>
#if defined(__CYGWIN__)
#include <sys/cygwin.h>
#endif
#include "StringUtils.h"
#include "GmshMessage.h"
#include "OS.h"

void SwapBytes(char *array, int size, int n)
{
  char *x = new char[size];
  for(int i = 0; i < n; i++) {
    char *a = &array[i * size];
    memcpy(x, a, size);
    for(int c = 0; c < size; c++)
      a[size - 1 - c] = x[c];
  }
  delete [] x;
}

std::string ExtractDoubleQuotedString(const char *str, int len)
{
  char *c = strstr((char*)str, "\"");
  if(!c) return "";
  std::string ret;
  for(int i = 1; i < len; i++) {
    if(c[i] == '"' || c[i] == EOF || c[i] == '\n' || c[i] == '\r') break;
    ret.push_back(c[i]);
  }
  return ret;
}

std::string SanitizeTeXString(const char *in, int equation)
{
  // if there is a '$' or a '\' in the string, assume the author knows
  // what he's doing:
  if(strstr(in, "$") || strstr(in, "\\"))
    return std::string(in);

  std::string out;

  if(equation) out.push_back('$');

  // otherwise, escape the following special characters:
  char bad[8] = { '%', '^', '#', '%', '&', '_', '{', '}' };
  while(*in){
    for(unsigned int i = 0; i < sizeof(bad); i++){
      if(*in == bad[i]){
        out.push_back('\\');
        break;
      }
    }
    out.push_back(*in++);
  }

  if(equation) out.push_back('$');
  return out;
}

std::string FixWindowsPath(const std::string &in)
{
#if defined(__CYGWIN__)
  char tmp[1024];
  cygwin_conv_to_win32_path(in.c_str(), tmp);
  return std::string(tmp);
#else
  return in;
#endif
}

std::string FixRelativePath(const std::string &reference, const std::string &in)
{
  if(in.empty()) return "";

  if(in[0] == '/' || in[0] == '\\' ||
     (in.size() > 3 && in[1] == ':' && (in[2] == '/' || in[2] == '\\'))){
    // do nothing: 'in' is an absolute path
    return in;
  }
  else{
    // append 'in' to the path of the reference file
    std::vector<std::string> split = SplitFileName(reference);
    return split[0] + in;
  }
}

std::vector<std::string> SplitFileName(const std::string &fileName)
{
  // JFR DO NOT CHANGE TO std::vector<std::string> s(3), it segfaults while
  // destructor si called
  std::vector<std::string> s; s.resize(3);
  if(fileName.size()){
    // returns [path, baseName, extension]
    int idot = (int)fileName.find_last_of('.');
    int islash = (int)fileName.find_last_of("/\\");
    if(idot == (int)std::string::npos) idot = -1;
    if(islash == (int)std::string::npos) islash = -1;
    if(idot > 0)
      s[2] = fileName.substr(idot);
    if(islash > 0)
      s[0] = fileName.substr(0, islash + 1);
    s[1] = fileName.substr(s[0].size(), fileName.size() - s[0].size() - s[2].size());
  }
  return s;
}

std::string GetFileNameWithoutPath(const std::string &fileName)
{
  std::vector<std::string> s = SplitFileName(fileName);
  return s[1] + s[2];
}

std::string ConvertFileToString(const std::string &fileName)
{
  FILE *fp = Fopen(fileName.c_str(), "r");
  if(!fp) return "";
  std::string out;
  char str[256];
  while(!feof(fp) && fgets(str, sizeof(str), fp)) out += str;
  fclose(fp);
  return out;
}

void ReplaceSubStringInPlace(const std::string &olds, const std::string &news,
                             std::string &str)
{
  while(1){
    int pos = (int)str.find(olds.c_str());
    if(pos == (int)std::string::npos) break;
    str.replace(pos, olds.size(), news.c_str());
  }
}

std::string ReplaceSubString(const std::string &olds, const std::string &news,
                             const std::string &str)
{
  std::string copy(str);
  ReplaceSubStringInPlace(olds, news, copy);
  return copy;
}

void ConvertToHTML(std::string &str)
{
  ReplaceSubStringInPlace("<", "&lt;", str);
  ReplaceSubStringInPlace(">", "&gt;", str);
  ReplaceSubStringInPlace("\n\n", "<p>", str);
  ReplaceSubStringInPlace("\n", "<br>", str);
}
