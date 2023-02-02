# Versioning

Clear versioning is important for the success of an open source project. We follow the common [semantic versioning rule](https://semver.org/), that is `major_version.minor_version.patch`.

We also have a set of rules to keep the versioning across different locations, such as git repository, doc, website, cmake, and source code, consistent with each other. 

The key idea is to use the tag in git repository as the single source of truth for current version, and all other versions should be obtained from git tag and inserted into their corresponding locations.

## version in doc

Since we are using sphinx as the doc generator, we can obtain the tag information in the conf.py file.
```python
import subprocess
release = subprocess.getoutput('git describe --tags --abbrev=0')
```

For projects using doxygen, you can use the following command to insert the git tag information as project number.
```shell
PROJECT_NUMBER=$(git describe --tags --abbrev=0) doxygen
```

## version in cmake 

In cmake, you can use the execute_process command to extract git tag's major, minor, and patch version into the corresponding cmake variables.

```cmake
execute_process(
    COMMAND ${GIT_EXECUTABLE} "--git-dir=${CMAKE_SOURCE_DIR}/.git" describe
            --tags --abbrev=0
    OUTPUT_VARIABLE GIT_TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE)

string(REGEX REPLACE "^([^.]+)\\.([^.]+)\\.(.+)$" "\\1" VERSION_MAJOR
                        "${GIT_TAG}")
string(REGEX REPLACE "^([^.]+)\\.([^.]+)\\.(.+)$" "\\2" VERSION_MINOR
                        "${GIT_TAG}")
string(REGEX REPLACE "^([^.]+)\\.([^.]+)\\.(.+)$" "\\3" VERSION_PATCH
                        "${GIT_TAG}")
```


## version in source code

We do not manually keep the version number in source code. Instead, we use macros which can be defined by cmake to parse the version number during preprocessing.

```cmake
target_compile_definitions(${target} PRIVATE VERSION_MAJOR=${VERSION_MAJOR})
```

```C
int get_major_version(){
    return VERSION_MAJOR;
}
```

## version in website

Most of our website is build with javascript, and if the website is within the current repository, we can use the git-rev-sync package.

```javascript
var git = require('git-rev-sync');

console.log(git.tag());
```

If the website is trying to obtain tag of another repository, I haven't found an proper way to do so without cloning that repository. We may explore more on this if we encounter such use cases.