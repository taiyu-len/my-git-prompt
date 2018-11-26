My simple git prompt in C.

# Usage
```
git status -sb --porcelain=v2 2>/dev/null | my-git-prompt

+master 123
^  ^    ^^^
|  |    ||+-- Staged added files (in green)
|  |    |+--- Unstaged modified files (in blue)
|  |    +---- Unstaged deleted files (in red)
|  +--------- Branch name (in bold)
+------------ + if there are staged files to be commited, space otherwise
```





