/*
hello.c 
Hello World file of "tiny file dialogs" created [November 9, 2014]
Copyright (c) 2014 - 2015 Guillaume Vareille http://ysengrin.com
http://tinyfiledialogs.sourceforge.net
 
 Let me know here mailto:tinfyfiledialogs@ysengrin.com
	- if you are including tiny file dialogs,
	  I'll be happy to add your link to the list of projects using it.
	- If you are using it on not listed here hardware / OS / compiler.
	- and please, leave a review on Sourceforge. Thanks.
 
tiny file dialogs (cross-platform C C++)
InputBox PasswordBox MessageBox ColorPicker
OpenFileDialog SaveFileDialog SelectFolderDialog
Native dialog library for WINDOWS MAC OSX GTK+ QT CONSOLE & more
v2.3 [December 6, 2015] zlib licence.

A single C file (add it to your C or C++ project) with 6 modal function calls:
- message box & question box
- input box & password box
- save file dialog
- open file dialog & multiple files
- select folder dialog
- color picker.

Complement to OpenGL GLFW GLUT GLUI VTK SDL Ogre3D Unity3D
or any GUI-less program, there is NO INIT & NO MAIN LOOP.
The dialogs can be forced into console mode.

On Windows:
- native code & some vbs create the graphic dialogs
- enhanced console mode can use dialog.exe from
  http://andrear.altervista.org/home/cdialog.php
- basic console input.

On Unix (command line call attempts):
- applescript
- zenity
- kdialog
- Xdialog
- python2 tkinter
- dialog (opens a console if needed)
- whiptail, gdialog, gxmessage
- basic console input.
The same executable can run across desktops & distributions.

tested with C & C++ compilers
on Windows Visual Studio 2013 MinGW Mac OSX LINUX FREEBSD ILLUMOS SOLARIS
using Gnome Kde Enlightenment Mate Cinnamon Unity
Lxde Lxqt Xfce WindowMaker IceWm Cde Jds OpenBox

- License -

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software.  If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/


/*
- Here is the Hello World:
    if a console is missing, it will use graphic dialogs
    if a graphical display is absent, it will use console dialogs
		(on windows the input box may take some time to open the first time)
*/


#include <stdio.h>
#include "tinyfiledialogs.h"
#pragma warning(disable:4996) /* allows usage of strncpy, strcpy, strcat, sprintf, fopen */
int main()
{
	char const * lThePassword;
	char const * lTheSaveFileName;
	char const * lTheOpenFileName;
	FILE * lIn;
	char lBuffer[1024];

  tinyfd_forceConsole = tinyfd_messageBox("Hello World",
    "force dialogs into console mode?\
    \n\t(it's better if dialog is installed)",
    "yesno", "question", 0);

  lThePassword =  tinyfd_inputBox(
    "a password box","your password will be revealed",NULL);

  lTheSaveFileName = tinyfd_saveFileDialog (
	"let's save this password",
    "passwordFile.txt",
    0,
    NULL,
    NULL );

	lIn = fopen(lTheSaveFileName, "w");
	if (!lIn)
	{
		tinyfd_messageBox(
			"Error",
			"Can not open this file in writting mode",
			"ok",
			"error",
			1 );
		return(1);
	}
	fputs(lThePassword, lIn);
	fclose(lIn);

    lTheOpenFileName = tinyfd_openFileDialog (
		"let's read this password",
		"",
		0,
		NULL,
		NULL,
		0);

	lIn = fopen(lTheOpenFileName, "r");
	if (!lIn)
	{
		tinyfd_messageBox(
			"Error",
			"Can not open this file in reading mode",
			"ok",
			"error",
			1 );
		return(1);
	}
	fgets(lBuffer, sizeof(lBuffer), lIn);
	fclose(lIn);

  if ( lBuffer )
    tinyfd_messageBox("your password is", lBuffer, "ok", "info", 1);
}
#pragma warning(default:4996)

/*
OSX :
$ gcc -o hello.app hello.c tinyfiledialogs.c

UNIX :
$ gcc -o hello hello.c tinyfiledialogs.c

MinGW :
> gcc -o hello.exe hello.c tinyfiledialogs.c -LC:/mingw/lib -lcomdlg32

VisualStudio :
  Create a console application project, it links against Comdlg32.lib.
	Right click on your Project, select Properties.
	Configuration Properties/General Character Set to Multi-Byte.
*/