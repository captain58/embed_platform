
#include "public.h"


uint8 CDATA_NOTHING(uint16 id, uint16 tstp, uint8* buffer, const CDATA* cd)
{
    SYS_OK();
}



const CDATAFunc gss_CDATAFuncExt[] =
{
    {
        0xFFFF,                         //Пе
        0,
        CDATA_NOTHING
    },

};

const uint16 gui_CoreFuncExtTblSize = sizeof(gss_CDATAFuncExt)/sizeof(CDATAFunc);
