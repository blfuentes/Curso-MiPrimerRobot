#include <cstdint>
#ifndef __PIDSERVICE_H__
#define __PIDSERVICE_H__

class PidService {
    float KP = .125f;//.125f;
    float KI = .0f;//.03125f;
    float KD = .0f;//.0625f;//.0625f;

    int pError;
    int correction;
    int integral;
    int derivative;
public:
    PidService();
    int32_t GetCorrection(int p_error);
};

#endif // __PIDSERVICE_H__