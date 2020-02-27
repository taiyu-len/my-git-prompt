My simple git prompt in C.

Depends on libgit2

# Usage
```
my-git-prompt [PATH:defaults to .]

+master 123
^  ^    ^^^
|  |    ||+-- Staged added files (in green)
|  |    |+--- Unstaged modified files (in blue)
|  |    +---- Unstaged deleted files (in red)
|  +--------- Branch name
+------------ shows + if there are staged files to be commited
```
