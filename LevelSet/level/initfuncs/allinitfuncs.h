#ifndef __ALL2DINITFUNCS_H__
#define __ALL2DINITFUNCS_H__

#include "initfuncs/circle.h"
#include "initfuncs/constant.h"
#include "initfuncs/diamond.h"
#include "initfuncs/heavifunc.h"
#include "initfuncs/ifuncunion.h"
#include "initfuncs/line.h"
#include "initfuncs/linearfunc.h"
#include "initfuncs/oval.h"
#include "initfuncs/sine.h"
#include "initfuncs/square.h"
#include "initfuncs/wavyfunc.h"
#include "initfuncs/cup.h"
#include "initfuncs/dbloval.h"
#include "initfuncs/dblrect.h"
#include "initfuncs/hardsmile.h"
#include "initfuncs/arc.h"
#include "initfuncs/msquares.h"
#include "initfuncs/mdisks.h"
#include "initfuncs/plus.h"
#include "initfuncs/plus2.h"
#include "initfuncs/star.h"
#include "initfuncs/negative.h"
#include "initfuncs/xsine.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "inputparams.h"

namespace levelset {
        
    static InitialFunc* PickInitialFunc(const char* name, InputParams* p)
    {
        char lower[256];
        for (unsigned int i=0; i<strlen(name); ++i)
            lower[i] = tolower(name[i]);
        lower[strlen(name)] = '\0';
        if (!strcmp(lower,"circle")) return new Circle(p);
        if (!strcmp(lower,"mdisks")) return new Mdisks(p);
        if (!strcmp(lower,"line")) return new Line(p);
        if (!strcmp(lower,"constant")) return new Constant(p);
        //      if (!strcmp(lower,"heavifunc")) return new HeaviFunc(p);
        if (!strcmp(lower,"linearfunc")) return new LinearFunc(p);
        if (!strcmp(lower,"oval")) return new Oval(p);
        if (!strcmp(lower,"sine")) return new Sine(p);
        if (!strcmp(lower,"square")) return new Square(p);
        if (!strcmp(lower,"wavy")) return new WavyFunc(p);
        if (!strcmp(lower,"cup")) return new Cup(p);
        if (!strcmp(lower,"dbloval")) return new DblOval(p);
        if (!strcmp(lower,"dblrect")) return new DblRect(p);
        //      if (!strcmp(lower,"hardsmile")) return new HardSmile(p);
        if (!strcmp(lower,"arc")) return new Arc(p);
        if (!strcmp(lower,"msquares")) return new Msquares(p);
        //      if (!strcmp(lower,"plus")) return Plus(p);
        //      if (!strcmp(lower,"plus2")) return Plus2(p);
        //      if (!strcmp(lower,"star")) return Star(p);
        if (!strcmp(lower,"xsine")) return new XSinusoidal(p);
        std::cerr << "Error in PickInitialFunc: no such function: " << name << "\n";
        exit(1);
    }
        
}

#endif
