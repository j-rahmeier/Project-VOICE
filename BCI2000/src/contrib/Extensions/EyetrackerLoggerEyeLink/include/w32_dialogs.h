/**********************************************************************************
 * EYELINK W32 compitibility SUPPORT      (c) 1996, 2003 by SR Research           *
 *     8 April 2003 by Suganthan Subramaniam       For non-commercial use only    *
 * Header file for standard functions                                             *
 * This module is for user applications   Use is granted for non-commercial       *
 * applications by Eyelink licencees only                                         *
 *                                                                                *
 *                                                                                *
 ******************************************* WARNING ******************************
 *                                                                                *
 * UNDER NO CIRCUMSTANCES SHOULD PARTS OF THESE FILES BE COPIED OR COMBINED.      *
 * This will make your code impossible to upgrade to new releases in the future,  *
 * and SR Research will not give tech support for reorganized code.               *
 *                                                                                *
 * This file should not be modified. If you must modify it, copy the entire file  *
 * with a new name, and change the the new file.                                  *
 *                                                                                *
 **********************************************************************************/
/*!
	\file w32_dialogs.h
        \brief Declaration of Win32 support dialogs.
 */


#ifndef __SRRESEARCH__W32_DIALOG_H__
#define __SRRESEARCH__W32_DIALOG_H__
#include "eyetypes.h"
#ifdef __cplusplus     /* For C++ compilation */
extern "C" {
#endif
/*! @ingroup datafile
	This receives a data file from the EyeLink tracker PC with graphical progressbar.
	This function only available in Win32 platform.

	@param[in] src Name of eye tracker file (including extension).  If "" (empty string), asks
				tracker for name of last opened data file.
	@param[in] dest Name of local file to write to (including extension).  If "" (empty string),
				prompts for file name.
	@param[in] dest_is_path If nonzero, appends file name to &lt;dest&gt; as a directory path.
	@return \c 0 if file transfer was cancelled.\n
			Size of file if successful.\n
			\c FILE_CANT_OPEN if no such file.\n
			\c FILE_XFER_ABORTED if data error.



	\sa \c receive_data_file()
 */
INT32 ELCALLTYPE receive_data_file_dialog(char *src, char *dest, INT16 dest_is_path);




/*! @ingroup gdi_graphics
	All experiments require the input of information: EDF file name, randomization
	file, and so on.  This function implements a simple text-entry dialog box for
	this purpose.  A title in the window frame and a message can be set, and the
	initial text set.  The length of the text to be entered can also be limited.

	@param hwnd The "parent" window, usually the experiment window or \c NULL if no window exists.
	@param title Text to be displayed in the frame of the dialog box.
	@param msg Instructions to be displayed in the dialog box.
	@param txt The buffer into which text will be entered.  Any text in this buffer
				will be displayed as the initial contents of the edit box.
	@param maxsize The maximum buffer length of txt.
	@return \c 0 if the ENTER key was pressed or "OK" was clicked. \c 1 if 'ESC'
			pressed or "Cancel" clicked. \c -1 if ALT-F4 pressed to destroy the dialog box.
 */
INT16 ELCALLTYPE edit_dialog(HWND hwnd, LPSTR title, LPSTR msg, LPSTR txt, INT16 maxsize);


/*! @ingroup gdi_graphics
	This function accepts path and session name from the user.  

	@param hwnd The "parent" window, usually the experiment window or \c NULL if no window exists.
	@param title Text to be displayed in the frame of the dialog box.
	@param msg Instructions to be displayed in the dialog box.
	@param txt The buffer into which text will be entered.  Any text in this buffer
				will be displayed as the initial contents of the edit box.
	@param maxsize The maximum buffer length of txt.
	@param[in,out] path  The default path and selected path upon return.  If "" used as default path
				the default documents directory will be used as the default path.
	@param pathmax size of the path parameter. 
	@return \c 0 if the ENTER key was pressed or "OK" was clicked. \c 1 if 'ESC'
			pressed or "Cancel" clicked. \c -1 if ALT-F4 pressed to destroy the dialog box.
 */
INT16 ELCALLTYPE ask_session(HWND hw, LPSTR title, LPSTR msg, LPSTR path, INT16 pathmax, LPSTR txt, INT16 maxsize);
/*! @ingroup datafile
	This macro is introduced so that the old eyelink projects compiled against new libraries,
	the file transfer behaves the same as the old receive_data_file in win32 platform
	See \c receive_data_file_dialog()
 */
#define receive_data_file receive_data_file_dialog
#ifdef __cplusplus     /* For C++ compilation */
};
#endif
#endif