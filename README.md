# BlockGenerator
MCS Kollmorgen ProteoPC block image generator for PLC manuals.

# Usage
```
blockgenerator.exe -h
Usage: blockgenerator.exe [options] path
Qt application to read XML files of variables of blocks for the Proteo PC and generate a PNG image 
displaying inputs and outputs of the block. It can be used with a single file or a directory - in 
this case, it will generate blocks for every file inside the directory.

Options:
  -?, -h, --help       Displays this help.
  -v, --version        Displays version information.
  -r, -R, --recursive  Generate PNG image in directories and files recursively
  -c, --call           Generate file with the block calling information

Arguments:
  path                 Path to file or directory to generate block info.
  ```
  
## Example:
```
C:\deployment-blockgen>blockgenerator.exe "C:\Program Files\MCS Kollmorgen\CNC ProteoPC Ativo 2.0\SYSTEM LIBS\System\Blocks" -c -r
[debug] Argument "Path" is a directory.
[debug] "F_Basic.png created!"
[debug] "I_Basic.png created!"
[debug] "sbAxisCam.png created!"
[debug] "sbAxisCamClutch.png created!"
[debug] "sbAxisCamControl.png created!"
[debug] "sbAxisCamDeclutch.png created!"
[debug] "sbAxisGear.png created!"
[debug] "sbAxisGearV2.png created!"
[debug] "sbBlockKeys.png created!"
[debug] "sbCounterDown.png created!"
[debug] "sbCounterTime.png created!"
[debug] "sbCounterUp.png created!"
.
.
.
[debug] "sfLimitTest.png created!"
[debug] "sfModulus.png created!"
[debug] "sfPresetAxis.png created!"
[debug] "sfReadCoordinate.png created!"
[debug] "sfReadParameter.png created!"
[debug] "sfSoftkeyMemoryStatus.png created!"
[debug] "sfWriteModalPgmVelocity.png created!"
[debug] "sfWriteToolRadius.png created!"
```

## Output
The Block Generator stores the output images in the executable directory, under `/path/to/executable_dir/blockimages/` and, in case the -c (or --call) flag is passed, the output calling information under `/path/to/executable_dir/textinfo/`.

### Image example:

![](https://i.imgur.com/CNLAZm1.png)

> sbHoming block image.

### Calling info example:
sbHoming(Enable, Switch, Target, DirectSearch, Axis)
