#include "SocketUtils.h"

SocketUtils::~SocketUtils()
{
}

bool SocketUtils::IsMulticastIp(const Byte * pAddress)
{
    // method result
    bool retVal = false;

    if (strlen(pAddress) > 3)
    {
        // first octate string
        Byte strFirstOctate[4] = { 0 };

        bool allDigitsAreNumeric = true;

        for (Int32 i = 0; i < 3; ++i)
        {
            if ((pAddress[i] >= '0') 
                && (pAddress[i] <= '9'))
            {
                strFirstOctate[i] = pAddress[i];
            }
            else
            {
                allDigitsAreNumeric = false;
            }
        }

        if (allDigitsAreNumeric)
        {
            Int32 iFirstOctate = atoi(strFirstOctate);
            if ((iFirstOctate >= 224) 
                && (iFirstOctate <= 240))
            {
                retVal = true;
            }
        }
    }

    return retVal;
}
