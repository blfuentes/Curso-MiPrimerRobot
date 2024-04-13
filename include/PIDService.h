#include <cstdint>
#ifndef __PIDSERVICE_H__
#define __PIDSERVICE_H__

class PidService {
    float KP = .09f;//.125f;
    float KI = .0;//.03125f;
    float KD = .0;//.0625f;//.0625f;

    int pError;
    int correction;
    int integral;
    int derivative;
public:
    PidService();
    void UpdateCorrection(int16_t p_error);
    int Correction() { return correction; }
};

#endif // __PIDSERVICE_H__