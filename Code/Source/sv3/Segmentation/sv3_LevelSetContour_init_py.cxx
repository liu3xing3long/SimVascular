/* Copyright (c) Stanford University, The Regents of the University of
 *               California, and others.
 *
 * All Rights Reserved.
 *
 * See Copyright-SimVascular.txt for additional details.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "SimVascular.h"
#include "sv_misc_utils.h"
//#include "sv3_LevelSetContour_init_py.h"
#include "sv3_Contour.h"
#include "sv3_LevelSetContour.h"
//#include "sv_adapt_utils.h"
#include "sv_arg.h"

#include <stdio.h>
#include <string.h>
#include "sv_Repository.h"
#include "sv_arg.h"
#include "sv_misc_utils.h"

#include "Python.h"
// The following is needed for Windows
#ifdef GetObject
#undef GetObject
#endif
// Prototypes:
// -----------
//
using sv3::levelSetContour;

levelSetContour* CreatelevelSetContour()
{
	return new levelSetContour(KERNEL_LEVELSET);
}
// Globals:
// --------

#include "sv2_globals.h"

// -----
// Adapt
// -----

PyObject* levelSetContour_AvailableCmd(PyObject* self,PyObject* args);

PyObject* levelSetContour_RegistrarsListCmd(PyObject* self, PyObject* args);

PyMethodDef levelSetContour_methods[] = {
  {"lscontour_available", levelSetContour_AvailableCmd,METH_NOARGS,NULL},
  {"lscontour_registrars", levelSetContour_RegistrarsListCmd,METH_NOARGS,NULL},
  {NULL, NULL}
};

PyMODINIT_FUNC
initpylevelSetContour()
{
  printf("  %-12s %s\n","","levelSetContour Enabled");

  // Associate the adapt registrar with the python interpreter so it can be
  // retrieved by the DLLs.
  cvFactoryRegistrar* contourObjectRegistrar =
    (cvFactoryRegistrar *)PySys_GetObject("ContourObjectRegistrar");

  if (contourObjectRegistrar != NULL) {
          // Register this particular factory method with the main app.
          contourObjectRegistrar->SetFactoryMethodPtr( KERNEL_LEVELSET,
      (FactoryMethodPtr) &CreatelevelSetContour );
  }
  else {
    return;
  }
  PySys_SetObject("ContourObjectRegistrar",(PyObject*)contourObjectRegistrar);

  PyObject* pythonC;
  pythonC = Py_InitModule("pylevelSetContour", levelSetContour_methods);
  if(pythonC==NULL)
  {
    fprintf(stdout,"Error in initializing pylevelSetContour\n");
    return;
  }
}

PyObject*  levelSetContour_AvailableCmd(PyObject* self, PyObject* args)
{
  return Py_BuildValue("s","levelSetContour Available");

}

PyObject* levelSetContour_RegistrarsListCmd(PyObject* self, PyObject* args)
{
  cvFactoryRegistrar *contourObjectRegistrar =
    (cvFactoryRegistrar *) PySys_GetObject("ContourObjectRegistrar");

  char result[255];
  PyObject* pyPtr=PyList_New(6);
  sprintf( result, "Contour object registrar ptr -> %p\n", contourObjectRegistrar );
  PyList_SetItem(pyPtr,0,PyString_FromFormat(result));

  for (int i = 0; i < 5; i++) {
      sprintf( result,"GetFactoryMethodPtr(%i) = %p\n",
      i, (contourObjectRegistrar->GetFactoryMethodPtr(i)));
      fprintf(stdout,result);
      PyList_SetItem(pyPtr,i+1,PyString_FromFormat(result));
  }
  return pyPtr;
}

