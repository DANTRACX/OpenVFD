#include "SCPI.h"
const char SCPI_INFO[] PROGMEM = {};
/*
const char SCPI_INFO[] PROGMEM = {"                                             \n\
                                   OpenVFD                                      \n\
                     (C) COPYRIGHT 2020 - Christoph Klie                        \n\
                                                                                \n\
Welcome to the OpenVFD SCPI interface.  All register addresses presented in this\n\
printout are also valid for the MODBUS interface.                               \n\
                                                                                \n\
                         [ ASCII SCPI COMMAND FORMAT ]                          \n\
                                                                                \n\
When  issuing  a  SCPI read query, the  ASCII string to be sent needs to use the\n\
Format:                                                                         \n\
                                                                                \n\
            REG:XXXXX?                          e.g. REG:30100?                 \n\
                                                                                \n\
where XXXXX denotes the register address which is intended to be read. The query\n\
must be ended with  a  questionmark in order  to  be identified as a read query.\n\
No further characters are allowed after the ending marker.                      \n\
                                                                                \n\
When issuing  a  SCPI write query, the  ASCII string to be sent needs to use the\n\
Format:                                                                         \n\
            REG:XXXXX YYY                       e.g. REG:40300 600              \n\
                                                                                \n\
where XXXXX denotes the register address which is intended to be written and YYY\n\
the corresponding  value which  should be written  to the register. A seperating\n\
whitespace between X and Y is  mandatory and no  further characters  are allowed\n\
to be sent after Y.                                                             \n\
                                                                                \n\
                    [ SCPI / MODBUS REGISTER OVERVIEW ]                         \n\
                                                                                \n\
    REGISTER # | Description                          | Unit | Range       |    \n\
    ------------------------------------------------------------------------    \n\
      40100    | Enable - must be set periodically    |      |             |    \n\
      40101    | Enable frequency overdrive           |      |             |    \n\
      40102    | Enable current overdrive             |      |             |    \n\
      40103    | Enable progmode                      |      |             |    \n\
      40104    | Target frequency                     |  mHz |  0..65536   |    \n\
      40105    | Target torque                        |   %  | -100..100   |    \n\
      40106    | Reversal                             |      |             |    \n\
                                                                                \0\
"};
*/
