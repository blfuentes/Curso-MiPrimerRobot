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
    void CalculateCorrection(int32_t p_error);
    int Correction() { return correction; }
};

#endif // __PIDSERVICE_H__